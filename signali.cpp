//
// Created by vilimsivec on 3/16/23.
//

#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;
//globalne varijable
int zastavice[3] = {0};
int T_P = 0;
string *stog = new string[3];
int stog_vrh = -1;   //-1 znaci prazan
bool obradaZastavice = false; //javljat ce ulazimo li u prekidni potprogram
//zbog obrade signala koji je bio u zastavicama
//ili jer je tek došao (ispis ovisi o tome)


struct timespec t0; /* vrijeme pocetka programa */

/* postavlja trenutno vrijeme u t0 */
void postavi_pocetno_vrijeme()
{
    clock_gettime(CLOCK_REALTIME, &t0);
}

/* dohvaca vrijeme proteklo od pokretanja programa */
void vrijeme(void)
{
    struct timespec t;

    clock_gettime(CLOCK_REALTIME, &t);

    t.tv_sec -= t0.tv_sec;
    t.tv_nsec -= t0.tv_nsec;
    if (t.tv_nsec < 0) {
        t.tv_nsec += 1000000000;
        t.tv_sec--;
    }

    printf("%03ld.%03ld:\t", t.tv_sec, t.tv_nsec/1000000);
}

/* ispis kao i printf uz dodatak trenutnog vremena na pocetku */
#define PRINTF(format, ...)       \
do {                              \
  vrijeme();                      \
  printf(format, ##__VA_ARGS__);  \
}                                 \
while(0)



void ispisStanja()
{
    PRINTF("K_Z = %d %d %d, T_P = %d, stog:", zastavice[0],
           zastavice[1], zastavice[2], T_P);

    if(stog_vrh < 0){
        cout<<" - ";
    }else{
        for(int i= stog_vrh; i>=0; i--){
            cout<<stog[i];
        }
    }

    cout<<endl<<endl;
}

int ispitZastavica() //vraca 0 ako ni jedna zastavica nije dignuta ili
                    // broj prekida koja je zastavica dignuta
{
    int zaReturn = 0;

    if(zastavice[1] > 0) return 10;
    if(zastavice[0] > 0) return 2;



    return zaReturn;
}

void spavanje()
{
    for(int i=0; i<5;i++){
        sleep(1);
    }
}


void obradi_signal(int sig)
{
    switch(sig){
        case 15:
            PRINTF("SKLOP: Dogodio se "
                   "prekid razine 3 i prosljeđuje se procesoru\n");
            zastavice[2] = 1;
            ispisStanja();

            PRINTF("Počela obrada razine 3\n");
            char stogEl[30];
            sprintf(stogEl, "%d, reg[%d] ", T_P, T_P );
            ++stog_vrh;
            stog[stog_vrh] = stogEl;

            zastavice[2] = 0;
            T_P = 3;

            ispisStanja();

            spavanje();


            PRINTF("Završila obrada prekida razine 3\n");

            if(stog_vrh > 0){
                //onda moramo ispisat da se nastavlja ono sto je zadnje
                // na stogu
                T_P = stoi(stog[stog_vrh--]);


                PRINTF("Nastavlja se obrada prekida razine %d\n", T_P);
                ispisStanja();

                if(ispitZastavica() > 2){  //po logici funkcije ispitZastavica() provjerava jel
                                        //postoji prekid razine vece od 1 medu zastavicama
                    obradaZastavice = true;
                    obradi_signal(ispitZastavica());
                }


            }else if(stog_vrh <=0){
                //nastavlja se izvodenje glavnog programa i ispis stanja
                T_P = 0;
                --stog_vrh;
                PRINTF("Nastavlja se izvođenje glavnog programa\n");
                ispisStanja();
                //ispitzastavica i ovisno o tome pokrenut neki prekidni
                // potprogram koji ceka
                if(ispitZastavica() > 0){
                    obradaZastavice = true;
                    obradi_signal(ispitZastavica());
                }
            }

            break;



        case 10:
            zastavice[1] = 1;

            if(2 > T_P){
                //obradujemo ovaj
                if(obradaZastavice == false){
                    PRINTF("SKLOP: Dogodio se "
                           "prekid razine 2 i prosljeđuje se procesoru\n");
                    ispisStanja();
                }
                if(obradaZastavice == true){
                    PRINTF("SKLOP: promijenio se T_P, prosljeđuje prekid razine 2 procesoru\n");
                }


                PRINTF("Počela obrada razine 2\n");
                char ispis[30];
                sprintf(ispis, "%d, reg[%d] ", T_P, T_P );
                stog[++stog_vrh] = ispis;

                zastavice[1] = 0;
                T_P = 2;

                ispisStanja();
                obradaZastavice = false;
                spavanje();


                PRINTF("Završila obrada prekida razine 2\n");

                if(stog_vrh > 0){
                    //onda moramo ispisat da se nastavlja ono sto je zadnje na stogu
                    T_P = stoi(stog[stog_vrh--]);


                    PRINTF("Nastavlja se obrada prekida razine %d\n", T_P);
                    ispisStanja();


                }else if(stog_vrh <=0){
                    //nastavlja se izvodenje glavnog programa i ispis stanja
                    T_P = 0;
                    --stog_vrh;
                    PRINTF("Nastavlja se izvođenje glavnog programa\n");
                    ispisStanja();
                    //ispitzastavica i ovisno o tome pokrenut neki prekidni potprogram koji ceka
                    if(ispitZastavica() > 0){
                        obradaZastavice = true;
                        obradi_signal(ispitZastavica());
                    }
                }


            }else{
                //ne obradujemo ga sad
                PRINTF("SKLOP: Dogodio se prekid razine 2 ali "
                       "se on pamti i ne prosljeđuje procesoru\n");
                ispisStanja();
            }


            break;

        case 2:
            zastavice[0] = 1;

            if(1 > T_P){
                //obradujemo ovaj
                if(obradaZastavice == false){
                    PRINTF("SKLOP: Dogodio se "
                           "prekid razine 1 i prosljeđuje se procesoru\n");
                    ispisStanja();
                }
                if(obradaZastavice == true){
                    PRINTF("SKLOP: promijenio se T_P, prosljeđuje prekid razine 1 procesoru\n");
                }


                PRINTF("Počela obrada razine 1\n");
                char ispis[30];
                sprintf(ispis, "%d, reg[%d] ", T_P, T_P );
                stog[++stog_vrh] = ispis;

                zastavice[0] = 0;
                T_P = 1;

                ispisStanja();
                obradaZastavice = false;
                spavanje();


                PRINTF("Završila obrada prekida razine 1\n");

                if(stog_vrh > 0){
                    //onda moramo ispisat da se nastavlja ono sto je zadnje na stogu
                    T_P = stoi(stog[stog_vrh--]);


                    PRINTF("Nastavlja se obrada prekida razine %d\n", T_P);
                    ispisStanja();


                }else if(stog_vrh <=0){
                    //nastavlja se izvodenje glavnog programa i ispis stanja
                    T_P = 0;
                    --stog_vrh;
                    PRINTF("Nastavlja se izvođenje glavnog programa\n");
                    ispisStanja();
                    //ispitzastavica i ovisno o tome pokrenut neki prekidni potprogram koji ceka
                    if(ispitZastavica() > 0){
                        obradaZastavice = true;
                        obradi_signal(ispitZastavica());
                    }
                }



            }else{
                //ne obradujemo ga sad
                PRINTF("SKLOP: Dogodio se prekid razine 1 ali "
                       "se on pamti i ne prosljeđuje procesoru\n");
                ispisStanja();
            }

            break;

        default:break;

    }


}
//ovdje definirat kako obradit pokoji od 3 potrebna prekida
//SIGINT -- 2          SIGUSR1 -- 10        SIGTERM -- 15 najjaci
void inicijalizacija()
{
    struct sigaction act;

    act.sa_handler = obradi_signal;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = obradi_signal;
    sigaction(SIGINT, &act, NULL);

    act.sa_handler = obradi_signal;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = obradi_signal;
    sigaction(SIGTERM, &act, NULL);

    act.sa_handler = obradi_signal;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = obradi_signal;
    sigaction(SIGUSR1, &act, NULL);

    postavi_pocetno_vrijeme();
}


int main()
{
    inicijalizacija();

    PRINTF("Program s PID=%ld krenuo s radom\n", (long) getpid());
    PRINTF("K_Z = %d %d %d, T_P = %d, stog: - \n", zastavice[0],
           zastavice[1], zastavice[2], T_P);

    while(true){
        sleep(1);
    }

    return 0;
}