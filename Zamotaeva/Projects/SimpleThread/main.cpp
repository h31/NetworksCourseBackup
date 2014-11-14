

using namespace std;
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>


void * thread_entry(void *argument) //bez arg
{
    printf("run!\n");
    printf("sleep!\n");
}

void * thread_entryP(void* arg) // s arg
{
    int nc = *(reinterpret_cast<int*>(arg));//preobr
    for(int i=0;i<nc;i++)
    {
        printf("runP!\n");
        sleep(2);
    }

}

int main()
{
    int nc = 3;
    pthread_t t1, t2 ;
    pthread_create(&t2,NULL,thread_entryP,reinterpret_cast<void*>(&nc));
    pthread_create(&t1,NULL,thread_entry,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    return 0;
}

