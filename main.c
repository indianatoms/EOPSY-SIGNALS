#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define WITH_SIGNALS 1
#define NUM_CHILD 5

char interupt = 0;

void myinterrupt(){
   printf("Parent[%I]: Interupt", getpid());
   interupt = 1;
}

void myhandler()
{
    printf("child[%i]: has been terminated");
}



int main()
{
    int wpid;
    int i,j,child_status;
    int status;
    pid_t pid[NUM_CHILD];



#if WITH_SIGNALS == 0

        for (i = 0; i < NUM_CHILD; i++) {
        if (( pid[i] = fork()) == 0) {
            printf("Parent[%i] parent id \n", getppid());
            printf("child[%i] succesfully created\n",getpid()); //Print message of creation of all child processes
            sleep(10);
            printf("child[%i] Execution completed \n",getpid()); //Print message of creation of all child processes

            exit(0);
        } else if (pid[i] == -1){
            printf("child[%i] has been created incorrect \n",getpid());
           // for(int j=i; j>0; j--) czemu wywala ???
              for(j=0; j<i; j++)
            {
                kill(pid[j],SIGTERM);
                printf("child[%i] has been termianted \n",getpid());
            }
            return 1;
        }
        sleep(1);
    }





    for (i=0; i<NUM_CHILD; ++i)
    {
        printf("Parent[%i] parent id \n", getpid());
        pid[i] = wait(&status);
        printf("chlid[%i] Succesfully waited for child number %i \n",pid[i]);
    }
#endif // WITH_SIGNALS

#if WITH_SIGNALS == 1

    for(i =0; i < NSIG; i++)
    {
        sigignore(j);
    }

    signal(SIGCHLD,SIG_DFL); //restore default SIGCHLD handelr
    signal (SIGINT,myinterrupt); // set my interraput

    for (i = 0; i < NUM_CHILD; i++) {
        if (( pid[i] = fork()) == 0) {
            printf("Parent[%i] parent id \n", getppid());
            printf("child[%i] succesfully created\n",getpid()); //Print message of creation of all child processes
            sleep(10);
            printf("child[%i] Execution completed \n",getpid()); //Print message of creation of all child processes
            exit(0);
        } else if (pid[i] == -1){
            printf("child[%i] has been created incorrect \n",getpid());
           // for(int j=i; j>0; j--) czemu wywala ???
              for(j=0; j<i; j++)
            {
                kill(pid[j],SIGTERM);
                printf("child[%i] has been termianted \n",getpid());
            }
            return 1;
        }
        sleep(1);
        if (interupt)
        {
            interupt = 0;
            printf("parent[%i] Interrupt of process \n", getpid());
            kill(-2,SIGTERM);
        }
    }


    int count = 0;

    while ((wpid = wait(&status)) > 0){
        printf("Parent[%i]: Child exit \n",getpid());
        count++;
    }

    //restore all signals
    for (i=0; i<NSIG; i++)
    {
        signal(i,SIG_DFL);
    }



    printf("parent[%i] Exited %i childs \n", count);


#endif // WITH_SIGNALS

  //while((wpid = wait(&status)) > 0)
  //  {
  //      printf("Succesfully waited for child number %i \n",wpid);
  //  }
    // how to recieve info that there is no more processes to be syncronized with parent one

    return 0;
}
