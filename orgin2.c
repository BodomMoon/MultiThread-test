#include <string.h>
#include <time.h>
#include <stdio.h>

//#define size 10

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

void F(int* A,int* B,int N){
	int temp = 1,zero_count=0;
	for(int i=0;i<N;i++){
		B[i] = 0;
		//zero_count+= !A[i];
		if(A[i])
			temp *= A[i];
		else
			zero_count++;
	}

	switch(zero_count) {
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
	return ;
}
	int testNumber[7]={10,100,1000,10000,100000,1000000,10000000};
	int A[10000000]={0},B[10000000]={0};

int main(int argc, char const *argv[])
{
	FILE *fp = fopen("time3.txt","w");

	struct timespec start, end;
	for(int counter = 0;counter < 7;counter++){
#if defined(__GNUC__)
 __builtin___clear_cache((char *) A, (char *) A + 8*testNumber[counter]*4);
#endif
		for(int i = 0;i<testNumber[counter];i++){
			A[i]=(i&3)+1;
		}
		clock_gettime(CLOCK_REALTIME, &start);
		F(A,B,testNumber[counter]);
	    clock_gettime(CLOCK_REALTIME, &end);
	    fprintf(fp,"%d %ld\n",testNumber[counter], diff_in_us(start, end));
	}
	fclose(fp);
	return 0;
}


