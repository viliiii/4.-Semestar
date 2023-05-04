//
// Created by vilimsivec on 5/4/23.
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
#include <semaphore>
#include <semaphore.h>

using namespace std;

int Id; /* identifikacijski broj segmenta */




typedef struct {
    int otvoreno = 0;
    int brMjesta = 5;  //slobodnih
    int klijent = 0;
    int spavam = 1;
    sem_t otvoren;
    sem_t provjeraMjesta;
    sem_t trKlijentSemafor;
    sem_t imaNekogSemafor;
    sem_t klijentSpremanSemafor;
    sem_t radimSemafor;
    sem_t spavamSemafor;

}spremnik;

void frizerka(){
    spremnik *sprem= (spremnik *) shmat(Id, NULL, 0);


    sprem->otvoreno = 1;

    cout<<"Frizerka: " << "Otvaram salon"<<endl;
    cout<<"Frizerka: " << "Postavljam znak OTVORENO"<<endl;
    cout<<"Frizerka: " << "Spavam dok klijenti ne dođu"<<endl;
    sem_wait(&sprem->spavamSemafor);
    sprem->spavam = 1;
    sem_post(&sprem->spavamSemafor);

    sem_post(&sprem->otvoren); //inicijalno zatvoren

    while(true){
        sem_wait(&sprem->otvoren);
        if(sprem->otvoreno == 0){
            sem_post(&sprem->otvoren);
            cout<<"Frizerka: " << "Postavljam znak ZATVORENO"<<endl;
            cout<<"Frizerka: " << "Zatvaram salon"<<endl;
            break;
        }
        sem_post(&sprem->otvoren);

        sem_wait(&sprem->provjeraMjesta);
        if(sprem->brMjesta < 5){  //postavi inicijalni brMjesta i ovaj broj na isto
            //postaviOpci, onaj koji cekaju
            sprem->brMjesta = sprem->brMjesta + 1;
            sem_post(&sprem->provjeraMjesta);

            sem_wait(&sprem->klijentSpremanSemafor);

            cout<<"Frizerka: " << "Idem raditi na klijentu " << sprem->klijent<<endl;
            sem_post(&sprem->radimSemafor);

            ///////

            for(int i=0; i<1; i++){
                sleep(1);
            }
            cout<<"Frizerka: " << "Klijent "<< sprem->klijent << " gotov"<<endl;
            sem_post(&sprem->trKlijentSemafor);

        } else{
            sem_post(&sprem->provjeraMjesta);
            sem_wait(&sprem->otvoren);

            if(sprem->otvoreno == 1){
                sem_post(&sprem->otvoren);
                sem_wait(&sprem->spavamSemafor);
                if(sprem->spavam == 0){
                    cout<<"Frizerka: " << "Spavam dok klijenti ne dođu"<<endl;
                    sprem->spavam = 1;
                }

                sem_post(&sprem->spavamSemafor);

                sem_wait(&sprem->imaNekogSemafor);

                sem_wait(&sprem->spavamSemafor);
                sprem->spavam = 0;
                sem_post(&sprem->spavamSemafor);
            } else{
                sem_post(&sprem->otvoren);
                cout<<"Frizerka: " << "Postavljam znak ZATVORENO"<<endl;
                cout<<"Frizerka: " << "Zatvaram salon"<<endl;
                break;
            }
        }


    }

    (void) shmdt(sprem);
}

void klijent(int x){
    spremnik *sprem= (spremnik *) shmat(Id, NULL, 0);
    sem_wait(&sprem->otvoren);
    cout<<"        "<<"Klijent("<<x<<"): Zelim na frizuru"<<endl;


    sem_wait(&sprem->provjeraMjesta);

    if(sprem->otvoreno == 1 and sprem->brMjesta > 0){
        sem_post(&sprem->otvoren);


        sprem->brMjesta = sprem->brMjesta - 1;

        cout<<"        "<<"Klijent("<<x<<"): Ulazim u cekaonicu, "<< 5 - sprem->brMjesta << endl;
        sem_post(&sprem->provjeraMjesta);

        //
        sem_wait(&sprem->spavamSemafor);
        if(sprem->spavam == 1){
            sem_post(&sprem->imaNekogSemafor);
        }
        sem_post(&sprem->spavamSemafor);
        //
        sem_wait(&sprem->trKlijentSemafor);
        sprem->klijent = x;
        sem_post(&sprem->klijentSpremanSemafor);
        sem_wait(&sprem->radimSemafor);
        cout<<"        "<<"Klijent("<<x<<"): Frizerka mi radi frizuru"<<endl;

    }else{
        sem_post(&sprem->otvoren);
        sem_post(&sprem->provjeraMjesta);
        cout<<"        "<<"Klijent("<<x<<"): Nema mjesta u čekaoni, vratit ću se sutra"<<endl;
    }




    (void) shmdt(sprem);
}


int main()
{

    /* zauzimanje zajedničke memorije */
    Id = shmget(IPC_PRIVATE, sizeof(spremnik) , 0600);

    if (Id == -1)
        exit(1);  /* greška - nema zajedničke memorije */

    spremnik *sprem= (spremnik *) shmat(Id, NULL, 0);
    sprem->brMjesta = 5;
    sprem->otvoreno = 0;
    sprem->klijent = 0;
    sprem->spavam = 1;

    sem_init(&sprem->otvoren, 1, 0);
    sem_init(&sprem->provjeraMjesta, 1, 1);
    sem_init(&sprem->trKlijentSemafor, 1, 1);
    sem_init(&sprem->imaNekogSemafor, 1, 0);
    sem_init(&sprem->klijentSpremanSemafor, 1, 0);
    sem_init(&sprem->radimSemafor, 1, 0);
    sem_init(&sprem->spavamSemafor, 1, 1);



    /* pokretanje paralelnih procesa */

    if (fork() == 0) {
        frizerka();
        exit(0);
    }

    for(int i=1; i<=7; i++){

        if (fork() == 0) {
            klijent(i);
            exit(0);
        }
    }

    for(int i=0; i<10; i++){
        sleep(1);
    }
    sem_wait(&sprem->otvoren);
    sprem->otvoreno = 0;
    sem_post(&sprem->otvoren);
    sem_post(&sprem->imaNekogSemafor);



    for(int i=0; i<8; i++){
        (void) wait(NULL);
    }

    (void) shmdt(sprem);

    return 0;
}