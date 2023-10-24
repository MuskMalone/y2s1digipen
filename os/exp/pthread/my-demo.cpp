#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#define PTHREAD_JOIN
static int counter = 0;
void *whatever(void *param)
{
	printf("thread %d run\n", counter);
	int *my_haha=(int *)param;
	int *what = (int *) malloc(sizeof(int)*4);
	printf("%d\n", *my_haha);
	what[2]=7458;
	sleep(5);
	
	return ((void *)what);
}

int main()
{	
	int haha;
	int *heehee;
	haha = 5;
	std::vector<pthread_t> ids{};

	int loop{10};
	while(loop-- >= 0){
		ids.push_back({});

		
		pthread_create(&ids.back(), 0, whatever, (void*)&haha);
	}

#ifdef PTHREAD_JOIN	
	for (auto const& i : ids){
		pthread_join(i, (void **)(&heehee));
		printf("Heehee[2] is %d\n", heehee[2]);

	}
#else	
	//To allow other threads to continue execution, 
	//the main thread should terminate by calling 
	//pthread_exit() rather than exit()
	pthread_exit(0);
	//exit(1);
	//if call pthread_exit(); the main thread will stop executing and 
	//will remain in zombie(defunct) status until all other threads exit
	//cannot get return status of other threads 
	//and cannot do clean-up for other threads (unless use pthread_join()	
#endif
}
