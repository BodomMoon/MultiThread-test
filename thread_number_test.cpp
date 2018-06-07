#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

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
    int getProduct(){
    	int product = 1;
    	for(int i=0;i < psize ;i++){
    		//product_pool[i] == 0 case will be count in thread
			product *= product_pool[i];
    	}
       return product;
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

void multiFor(int* A,int *B,int N,int ID,AnswerPool *mypool){
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

void F(int* A,int* B,int N,int thread_Count){
	int temp = 0;

	AnswerPool *mypool = new AnswerPool(thread_Count);
	std::thread mThread[thread_Count];

	for(int i=0;i<thread_Count;i++){
		 mThread[i] = std::thread( multiFor ,A+(N/thread_Count*i),B+(N/thread_Count*i),N/thread_Count,i,mypool);
	}
	for(int i=0;i<thread_Count;i++){
		 mThread[i].join();
	}


	temp = mypool->getProduct();
	switch(mypool->zero_count) {
	case 0:
		for(int i = 0;i<N;i++)
			B[i] = temp/A[i];
		break;
	case 1:
		for(int i = 0;i<N;i++){
			if(A[i] == 0){
				B[i]=temp;
				break;
			}
		}
		break;
	default:
		break;
	}

	delete(mypool);
	return ;
}

	int testNumber[7]={10000000,10000000,10000000,10000000,10000000,10000000,10000000};
	int A[10000000]={0},B[10000000]={0};

int main( int argc, char** argv )
{
	FILE *fp = fopen("time2.txt","w");
	unsigned int n = std::thread::hardware_concurrency();
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
		A[5]=0;
		clock_gettime(CLOCK_REALTIME, &start);
		F(A,B,testNumber[counter],1);
	    clock_gettime(CLOCK_REALTIME, &end);
	    fprintf(fp,"%d %ld\n",testNumber[counter], diff_in_us(start, end));
	}
	fclose(fp);
	return 0;
}


