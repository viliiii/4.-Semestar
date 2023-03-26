//
// Created by vilimsivec on 3/26/23.
//

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <atomic>
#include <algorithm>
#include <csignal>

int N; /*broj dretvi*/
int M; /*broj ponavljanja za svaku dretvu*/
int A; /*varijabla koju ce svaka dretva povecavati*/
std::atomic<int> *ulaz;
std::atomic<int> *broj;


void *lamport(void *x) {
    int i = *((int *) x);


    for (int k = 0; k < M; k++) {
        ulaz[i] = 1;
        broj[i] = ((int) *std::max_element(broj, broj + N)) + 1;
        ulaz[i] = 0;

        for (int j = 0; j < N; j++) {
            while (ulaz[j] != 0);
            while (broj[j] != 0 && (broj[j] < broj[i] || (broj[j] == broj[i] && j < i)));
        }
        A = A + 1;
        broj[i] = 0;
    }

    return NULL;
}

int main(int argc, char*argv[], char*env[])
{
    N = std::stoi(argv[1]);
    M = std::stoi(argv[2]);
    ulaz = new std::atomic<int>[N];
    broj = new std::atomic<int>[N];
    std::fill(ulaz, ulaz + N, 0);
    std::fill(broj, broj + N, 0);

    std::cout<<"N = "<<N<<", M = "<<M<<std::endl;

    pthread_t thr_id[N];

    A = 0;
    int br[N];
    for(int i=0; i<N; i++){
        /* pokretanje dretvi */
        //br[i] = i;
        if (pthread_create(&thr_id[i], NULL, lamport, &i) != 0) {
            printf("Greska pri stvaranju dretve!\n");
            exit(1);
        }
    }

    for(int j=0; j<N; j++){
        /*hvatanje dretvi*/
        pthread_join(thr_id[j], NULL);
    }

    std::cout<<"A = "<<A<<std::endl;

    return 0;
}