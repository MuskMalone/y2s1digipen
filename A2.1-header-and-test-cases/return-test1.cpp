#include <cstdio>
#include <cstdlib>
#include "new-coro-lib.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

void *get_return_value(void *param )
{
    static long count = 5;
    count--;
    return (void *)count;
}


int main()
{
    printf("nosegfault1\n");
    CORO::ThreadID thd_ids[5];
    int i;
    printf("nosegfault2\n");

    CORO::thd_init();
    printf("nosegfault3\n");

    for (i=0; i<5; i++)
        thd_ids[i]=CORO::new_thd(get_return_value, NULL);
    printf("nosegfault4\n");

    for (i=0; i<5; i++)
    {
        long long thd_ret_value;
        CORO::wait_thread(thd_ids[i], (void**)&thd_ret_value);
        printf("Thread %u returns %lld\n", thd_ids[i], thd_ret_value);
    }
    printf("nosegfault5\n");

}
