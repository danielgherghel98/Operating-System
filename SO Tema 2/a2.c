#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#define M 45
#define N 6
sem_t Sem,Sem1,Sem2,Sem3,Sem4;
sem_t* Sem5;
sem_t* Sem6;
typedef struct {
    int value;
} TH_STRUCT;
void* thread_function(void *argv)
{
    TH_STRUCT *s=(TH_STRUCT*)argv;
    int arg=s->value;
    if(arg!=2)
    {
        sem_wait(&Sem);
    }
    if(arg==4)
    {
        sem_wait(Sem5);
    }
    info(BEGIN, 6, arg);
    for(int j=1; j<=arg;j++)
    {
        sem_post(&Sem);
    }
    if(arg!=2 && arg!=5)
    {
        
        info(END,6,arg);
        if(arg==4)
        {
            sem_post(Sem6);
        }
    }
    else if (arg==5){
        info(END,6,5);
        info(END,6,2);
    }
    
    return NULL;
}
void* thread_function1(void *argv)
{
    TH_STRUCT *s=(TH_STRUCT*)argv;
    int arg=s->value;
    if(arg!=11)
    {
        sem_wait(&Sem2);
    }
    sem_wait(&Sem1);
    
    info(BEGIN, 2, arg);
    if(arg==11)
    {
        for(int i=1; i<M; i++)
        {
            sem_post(&Sem2);
        }
    }
    if(arg==11)
    {
        for(int i=1; i<N; i++)
        {
            sem_wait(&Sem4);
        }
    }
    if(arg!=11)
    {
        sem_post(&Sem4);
    }
    if(arg!=11)
    {
        sem_wait(&Sem3);
    }
    info(END,2,arg);
    if(arg==11)
    {
        for(int i=1; i<M; i++)
        {
            sem_post(&Sem3);
        }
    }
    sem_post(&Sem1);
    
    return NULL;
}
void* thread_function2(void *argv)
{
    TH_STRUCT *s=(TH_STRUCT*)argv;
    int arg=s->value;
    if(arg==5)
    {
        sem_wait(Sem6);
    }
    info(BEGIN, 5, arg);
    info(END,5,arg);
    if(arg==1)
    {
        sem_post(Sem5);
    }
    
    
    return NULL;
}
int main(){
    sem_unlink("semaphore5");
    sem_unlink("semaphore6");

    Sem5=sem_open("semaphore5", O_CREAT, 0644, 0);
    Sem6=sem_open("semaphore6", O_CREAT, 0644, 0);
    init();
    int pid2, pid3, pid4, pid5,pid6,pid7;
    pthread_t th[6];
    pthread_t th2[6];
    pthread_t th1[46];
    TH_STRUCT params[5];
    TH_STRUCT params2[5];
    TH_STRUCT params1[45];

    info(BEGIN, 1, 0);

        pid2=fork();
        if (pid2==-1)
        {
            perror("nacaz");
            return -1;
        }
        else if (pid2==0)
        { // SUNT IN P2
            info(BEGIN,2,0);
                sem_init(&Sem1,0,N);
                sem_init(&Sem2,0,0);
                sem_init(&Sem3,0,0);
                sem_init(&Sem4,0,0);
                for(int i=1;i<=45;i++)
                {
                    params1[i].value =i;
                    pthread_create(&th1[i],NULL,thread_function1,&params1[i]);
                            
                }
                for(int i=1;i<=45;i++)
                {
                    pthread_join(th1[i],NULL);
                            
                }
                pid4=fork();
                
                    if (pid4==-1)
                        {perror("necaz");
                        return -1;
                    }
                    else if (pid4==0)
                    {// SUNT IN P4
                        info(BEGIN,4,0);

                        info(END, 4, 0);
                        exit(0);
                    }
                    else 
                    {
                        //P2
                        pid5=fork();
                        if (pid5==-1)
                        {
                            perror("necaz");
                            return -1;
                        } 
                        else if (pid5 == 0)
                        {
                            //sunt in P5
                            info(BEGIN,5,0);
                                //sem_init(&Sem5,0,0);
                                //sem_init(&Sem6,0,0);
                                for(int i=1;i<=5;i++)
                                {
                                    params2[i].value =i;
                                    pthread_create(&th2[i],NULL,thread_function2,&params2[i]);
                                    
                                }
                                for(int i=1;i<=5;i++)
                                {
                                    pthread_join(th2[i],NULL);
                                    
                                }
                                pid7=fork();
                                if (pid7==-1)
                                {
                                    perror("necaz");
                                    return -1;
                                } 
                                else if (pid7 == 0)
                                {
                                    // P7
                                    info(BEGIN,7,0);

                                    info(END, 7, 0);
                                    exit(0);
                                }
                                waitpid(pid7,NULL,0);
                            info(END, 5, 0);
                            exit(0);
                        }
                    }
                
                waitpid(pid5,NULL,0);
                waitpid(pid4,NULL,0);
            info(END, 2, 0);
            exit(0);
        }   
        else 
        { // P1 
            pid3 = fork();
            if (pid3==-1)
            {
                perror("necaz");
                return -1;
            } else if (pid3 == 0)
            {
                // P3
                info(BEGIN,3,0);
                    pid6=fork();
                    if (pid6==-1)
                    {
                        perror("necaz");
                        return -1;
                    } 
                    else if (pid6 == 0)
                    {
                        // P6
                        info(BEGIN,6,0);
                        sem_init(&Sem,0,0);
                        for(int i=1;i<=5;i++)
                        {
                            params[i].value =i;
                            pthread_create(&th[i],NULL,thread_function,&params[i]);
                            
                        }
                        for(int i=1;i<=5;i++)
                        {
                            pthread_join(th[i],NULL);
                            
                        }
                        info(END, 6, 0);
                        exit(0);
                    }
                    waitpid(pid6,NULL,0);
                info(END, 3, 0);
                exit(0);
            }
            
        }
        waitpid(pid2,NULL,0);
        waitpid(pid3,NULL,0);
    
    info(END, 1, 0);
    exit(0);
    sem_destroy(&Sem);
    sem_destroy(&Sem1);
    sem_destroy(&Sem2);
    sem_destroy(&Sem3);
    sem_destroy(&Sem4);
    sem_close(Sem5);
    sem_close(Sem6);
    // sem_destroy(&Sem5);
    // sem_destroy(&Sem6);
    return 0;
}