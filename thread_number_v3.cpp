#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


// sorry to use global value QAQ

class AnswerPool{
    // Access specifier
    public:
    // Data Members
    int *product_pool;
    int zero_count;
    int psize;
    std::mutex mu;

    // Member Functions()
    AnswerPool(){
    	psize = 0;
    	product_pool =new int[1];
    	zero_count = 0;
  	}
    AnswerPool(int size){
    	psize = size;
    	product_pool =new int[psize];
    	zero_count = 0;
  	}
    void zero_countAdd(){
    	lock_guard<std::mutex> lockGuard(mu);
    	zero_count++;
    }
};



static long int diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000000 + diff.tv_nsec  );
}

void multiFor(int *A,int *B,int N,int ID,AnswerPool *mypool){
	mypool->product_pool[ID]=1;
	for(int i=0;i<N;i++){
		//zero_count+= !A[i];
		if(A[i])
			mypool->product_pool[ID] *= A[i];
		else
			mypool ->zero_countAdd();
	}
}

void testf(int A,int B){
	printf("%d\n", A);
	return ;
}

void getProductUnit(int *array,int ID,int size,condition_variable *gCV,volatile int *FlagArray){
	size>>=1;
	while(ID < size){
		mutex gMutex;
  		unique_lock<mutex> mLock(gMutex);
  		while((FlagArray[ID+size]!=FlagArray[ID])){
			gCV[ID].wait_for(mLock, chrono::microseconds(50));
		}
		++FlagArray[ID];
		array[ID] *= array[ID+size];
		size>>=1;

	}
	if(ID){
		gCV[(ID-size)].notify_all();
	}
	return ;
}

void F(int* A,int* B,int N,int thread_Count){
	int temp = 0;
	condition_variable *gCV = new condition_variable[(thread_Count>>1)];
	volatile int *FlagArray = new volatile int[thread_Count];
	for(int i = 0;i<thread_Count;i++){
		FlagArray[i] = 0;
	}
	AnswerPool *mypool = new AnswerPool(thread_Count);
	std::thread mThread[thread_Count];

	for(int i=0;i<thread_Count;i++){
		mThread[i] = std::thread( multiFor ,A+(N/thread_Count*i),B+(N/thread_Count*i),N/thread_Count,i,mypool);
	}
	for(int i=0;i<thread_Count;i++){
		mThread[i].join();
	}


	//temp = mypool->getProduct();
	switch(mypool->zero_count) {
	case 0:
		//int i = 0;i<(thread_Count>>1);i++  this really faster
		//int i=(thread_Count>>1)-1;i>=0;i--
		for(int i = 0;i<(thread_Count>>1);i++){
			mThread[i] = std::thread( getProductUnit ,mypool->product_pool,i,thread_Count,gCV,FlagArray);
		}
		for(int i=(thread_Count>>1)-1;i>=0;i--){
		 	mThread[i].join();
		}
		for(int i = 0;i<N;i++)
			B[i] = mypool->product_pool[0]/A[i];
		break;
	case 1:
		for(int i = 0;i<(thread_Count>>1);i++){
			mThread[i] = std::thread( getProductUnit ,mypool->product_pool,i,thread_Count,gCV,FlagArray);
		}
		for(int i=(thread_Count>>1)-1;i>=0;i--){
		 	mThread[i].join();
		}
		for(int i = 0;i<N;i++){
			if(A[i] != 0){
				//pass
			}
			else{
				B[i]=mypool->product_pool[0];
				break;
			}
		}
		break;
	default:
		break;
	}
	//delete(gCV);
	delete(FlagArray);
	delete(mypool);
	return ;
}

	int testNumber[7]={2048,8192,32768,131072,524288,2097152,8388608};
	int A[8388608]={0},B[8388608]={0};

int main( int argc, char** argv )
{
	FILE *fp = fopen("time2_v3.txt","w");
	struct timespec start, end;
	for(int counter = 0;counter < 7;counter++){
#if defined(__GNUC__)
 __builtin___clear_cache((char *) A, (char *) A + 8*testNumber[counter]*4);
#endif
		for(int i = 0;i<testNumber[counter];i++){
			A[i]=1;
		}
		for(int i = 0;i<9;i++){
			A[i]=2;
		}
		clock_gettime(CLOCK_REALTIME, &start);
		F(A,B,testNumber[counter],8);
	    clock_gettime(CLOCK_REALTIME, &end);
	    fprintf(fp,"%d %ld\n",testNumber[counter], diff_in_us(start, end));
	}
	fclose(fp);
	return 0;
}


