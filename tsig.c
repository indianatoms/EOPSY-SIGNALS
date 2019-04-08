#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define WITH_SIGNALS 1                          //set wheteher perform with signals or without
#define NUM_CHILD 5                             //number of childs

char interupt = 0;                              //interupt indicator set to 0

#if WITH_SIGNALS == 1
//my own interput log
void myinterrupt(){
   printf("Parent[%i]: Interupt =====================================================\n", getpid());
   interupt = 1;//set interupt indicator to 1
}

//my own handler for child termiation
void myhandler()
{
    printf("child[%i]: has been terminated \n",getpid());
}
#endif // WITH_SIGNALS


int main()
{
    int wpid;
    int i,j,child_status;
    int status;
    pid_t pid[NUM_CHILD];



#if WITH_SIGNALS == 0

//for each NUM_CHILD perform a fork - create 5 childs
        for (i = 0; i < NUM_CHILD; i++) {
        if (( pid[i] = fork()) == 0) {
            printf("Parent[%i] succesfully created a child[%i] \n",getppid(),getpid()); //Print message of creation of all child processes
            sleep(10);
            printf("child[%i] Execution completed \n",getpid()); //Print message of creation of all child processes
            exit(0);
        } else if (pid[i] == -1){ //chack for errors
            printf("child[%i] has been created incorrect \n",getpid());
              for(j=0; j<i; j++)
            {
                kill(pid[j],SIGTERM);   //kill all childs
                printf("child[%i] has been termianted \n",getpid());
            }
            return 1;
        }
        sleep(1);
    }
    //wait untill all childs end its process
      while ((wpid = wait(&status)) > 0){
        printf("Parent[%i]: Child exit \n",getpid());
    }
#endif // WITH_SIGNALS

#if WITH_SIGNALS == 1

    //assign my own ignore ignals
    struct sigaction sigactionIgnore;
    sigactionIgnore.sa_handler = SIG_IGN;

    for(i=0; i < NSIG; i++)
    {
        sigaction(i, &sigactionIgnore, NULL); //ignore all signals
    }

    //Assign my own child restore
    struct sigaction sigactionRestore;
    sigactionRestore.sa_handler = SIG_DFL;
    sigaction(SIGCHLD, &sigactionRestore, NULL);

    //assign my own child interuption
    struct sigaction sigactionInterrupt;
    sigactionInterrupt.sa_handler = myinterrupt;
    sigaction(SIGINT, &sigactionInterrupt, NULL);

    //for each NUM_CHILD perform a fork - create 5 childs
    for (i = 0; i < NUM_CHILD; i++) {
        if (( pid[i] = fork()) == 0) {
            sigaction(SIGINT, &sigactionIgnore, NULL);    //ignore signal interupt
            //assign my handler to sigterm in child
            struct sigaction sigactionTerm;
            sigactionTerm.sa_handler = myhandler;
            sigaction(SIGTERM, &sigactionTerm, NULL);

            printf("Parent[%i] succesfully created a child[%i] \n",getppid(),getpid()); //Print message of creation of all child processes
            sleep(10);
            printf("child[%i] Execution completed \n",getpid()); //Print message of creation of all child processes
            exit(0);
        } else if (pid[i] == -1){
        //indicate if child has been created incorectly
            printf("child[%i] has been created incorrect \n",getpid());
              for(j=0; j<i; j++)
            {
                kill(pid[j],SIGTERM);
                printf("child[%i] has been termianted \n",getpid());
            }
            return 1;
        }
    //check if interupt occured
        if (interupt)
        {
            interupt = 0;
            printf("parent[%i] Interrupt of process \n", getpid());
            for(j=0; j<i; j++)
            {
                kill(pid[j], SIGTERM);
            }
        }
        sleep(1);
    }


    int count = 0;

    //wait for all childs and count them
    while ((wpid = wait(&status)) > 0){
        printf("Parent[%i]: Child exit \n",getpid());
        count++;
    }

    //restore all signals
    for (i=0; i<NSIG; i++)
    {
        sigaction(i, &sigactionRestore,NULL); //restore signal one by one
    }

    printf("parent[%i] Exited %i childs \n", getpid(),count);


#endif
    return 0;
}