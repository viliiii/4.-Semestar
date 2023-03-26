//
// Created by vilimsivec on 3/25/23.
//

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <iostream>
#include <atomic>

int M;
int Id; /* identifikacijski broj segmenta */


typedef struct {
    int pravo = 0;  /*ako je 1, dijete 1 ima pravo, ako je 2 dijete 2 ima pravo*/
    std::atomic<int> zastavice[2];
    int A = 0;
}spremnik;

void dijete1()
{
    spremnik *sprem= (spremnik *) shmat(Id, NULL, 0);

    for(int i=0; i<M; i++){
        sprem->zastavice[0] = 1;
       while (sprem->zastavice[1] == 1){
            if(sprem->pravo != 0){
                sprem->zastavice[0] = 0;
                while (sprem->pravo != 0){

                }
                sprem->zastavice[0] = 1;
            }
       }

        sprem->A = sprem->A + 1;

        sprem->pravo = 1;
        sprem->zastavice[0] = 0;
    }

    (void) shmdt(sprem);
}



void dijete2()
{
    spremnik *sprem= (spremnik *) shmat(Id, NULL, 0);

    for(int i=0; i<M; i++){
        sprem->zastavice[1] = 1;
        while (sprem->zastavice[0] == 1){
            if(sprem->pravo != 1){
                sprem->zastavice[1] = 0;
                while (sprem->pravo != 1){

                }
                sprem->zastavice[1] = 1;
            }
        }
        sprem->A = sprem->A + 1;
        sprem->pravo = 0;
        sprem->zastavice[1] = 0;
    }

    (void) shmdt(sprem);
}




void brisi(int sig)
{
    /* oslobađanje zajedničke memorije */
    (void) shmctl(Id, IPC_RMID, NULL);
    exit(0);
}
int main(int argc, char*argv[], char*env[])
{
    M = std::stoi(argv[1]);
    //printf("M = %d", M);
    /* zauzimanje zajedničke memorije */
    Id = shmget(IPC_PRIVATE, sizeof(spremnik) , 0600);

    if (Id == -1)
        exit(1);  /* greška - nema zajedničke memorije */


    /*spremnik je dio memorije kojemu mmogu pristupati svi procesi kojima se to
     * dopusti zadavanjem id-a Id*/


    /* pokretanje paralelnih procesa */

    if (fork() == 0) {
        dijete1();
        exit(0);
    }
    if (fork() == 0) {
        dijete2();
        exit(0);
    }
    (void) wait(NULL);
    (void) wait(NULL);

    spremnik *sprem= (spremnik *) shmat(Id, NULL, 0);
    printf("A = %d\n", sprem->A );
    (void) shmdt(sprem);
    brisi(0);



    return 0;
}