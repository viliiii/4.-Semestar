//
// Created by vilimsivec on 5/6/23.
//

#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <atomic>
#include <algorithm>
#include <csignal>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#define brStvaranja 20

using namespace std;

pthread_mutex_t monitor;
pthread_cond_t misionariD;
pthread_cond_t misionariL;
pthread_cond_t kanibaliD;
pthread_cond_t kanibaliL;
pthread_cond_t camacCeka;

int brMuC = 0; // misionara u čamcu
int brKuC = 0;
int brUKuC = 0;     // ukupno u čamcu
int camacObala = 1; // trenutna obala camca, 1 je desna
vector<string> desnaObala;
// int size_desnaObala = 0;
vector<string> lijevaObala;
// int size_lijevaObala = 0;
vector<string> camac;

void ispisVektor(const vector<string> &v)
{
    cout << "{ ";
    for (const auto &i : v)
    {
        cout << i << " ";
    }
    cout << "}";
}

void ispis()
{
    if (camacObala == 1)
    {
        cout << "C[D] = ";
        ispisVektor(camac);
        cout << " LO =";
        ispisVektor(lijevaObala);
        cout << " DO =";
        ispisVektor(desnaObala);
    }
    else
    {
        cout << "C[L] = ";
        ispisVektor(camac);
        cout << " LO =";
        ispisVektor(lijevaObala);
        cout << " DO =";
        ispisVektor(desnaObala);
    }
    cout << endl;
    cout << endl;
}

// provjerava jel camac na strani na kojoj je doso, ak nije onda ceka
// ili je camac pun
// dolazi i provjerava jel nutra 2 kabibala i ak jesu, ceka.
// ak smije uc, onda signalizira da smije i jos jedan kanibal uc
void *Misionar(void *x)
{
    int i = *((int *)x) / 2;
    int obala = std::rand() % 2; // 0 lijeva, 1 desna

    pthread_mutex_lock(&monitor);
    if (obala == 1)
    {
        // desna
        cout << "M" << i << ": dosao na desnu obalu" << endl;
        string Mi = "M" + to_string(i);
        desnaObala.push_back(Mi);
        ispis();

        while (camacObala != 1 || camac.size() == 7 || (brKuC - brMuC >= 2))
        {
            pthread_cond_wait(&misionariD, &monitor);
        }

        camac.push_back(Mi);
        //desnaObala.pop_back();
        
        auto it = find(desnaObala.begin(), desnaObala.end(), Mi);

        if (it != desnaObala.end()) {
        desnaObala.erase(it);
        }


        brMuC++;

        cout << Mi << ": usao u camac" << endl;
        ispis();

        if (camac.size() < 7)
        {
            pthread_cond_signal(&kanibaliD);
        }

        pthread_cond_signal(&camacCeka);
    }
    else
    {
        // lijeva
        cout << "M" << i << ": dosao na lijevu obalu" << endl;
        string Mi = "M" + to_string(i);
        lijevaObala.push_back(Mi);
        ispis();

        while (camacObala != 0 || camac.size() == 7 || (brKuC - brMuC >= 2))
        {
            pthread_cond_wait(&misionariL, &monitor);
        }

        camac.push_back(Mi);
        //lijevaObala.pop_back();
        auto it = find(lijevaObala.begin(), lijevaObala.end(), Mi);

        if (it != lijevaObala.end()) {
        lijevaObala.erase(it);
        }

        brMuC++;

        cout << Mi << ": usao u camac" << endl;
        ispis();

        if (camac.size() < 7)
        {
            pthread_cond_signal(&kanibaliL);
        }

        pthread_cond_signal(&camacCeka);
    }

    pthread_mutex_unlock(&monitor);

    return NULL;
}




// provjerava jel camac na strani na kojoj je doso, ak nije onda ceka
// ili je camac pun
//
void *Kanibal(void *x)
{
    int i = *((int *)x);
    int obala = std::rand() % 2; // 0 lijeva, 1 desna

    pthread_mutex_lock(&monitor);
    if (obala == 1)
    {
        // desna
        cout << "K" << i << ": dosao na desnu obalu" << endl;
        string Ki = "K" + to_string(i);
        desnaObala.push_back(Ki);
        ispis();

        while (camacObala != 1 || camac.size() == 7 || (brMuC == brKuC && brMuC != 0))
        {
            pthread_cond_wait(&kanibaliD, &monitor);
        }

        camac.push_back(Ki);
        // desnaObala.pop_back();
        //auto first = desnaObala.begin();
        //desnaObala.erase(first);

        auto it = find(desnaObala.begin(), desnaObala.end(), Ki);

        if (it != desnaObala.end()) {
        desnaObala.erase(it);
        }


        brKuC++;

        cout << Ki << ": usao u camac" << endl;
        ispis();

        pthread_cond_signal(&camacCeka);
    }
    else
    {
        // lijeva
        cout << "K" << i << ": dosao na lijevu obalu" << endl;
        string Ki = "K" + to_string(i);
        lijevaObala.push_back(Ki);
        ispis();

        while (camacObala != 0 || camac.size() == 7 || (brMuC == brKuC && brMuC != 0))
        {
            pthread_cond_wait(&kanibaliL, &monitor);
        }

        camac.push_back(Ki);
        // lijevaObala.pop_back();
        //auto first = lijevaObala.begin();
        //lijevaObala.erase(first);

        auto it = find(lijevaObala.begin(), lijevaObala.end(), Ki);

        if (it != lijevaObala.end()) {
        lijevaObala.erase(it);
        }
        brKuC++;

        cout << Ki << ": usao u camac" << endl;
        ispis();

        pthread_cond_signal(&camacCeka);
    }

    pthread_mutex_unlock(&monitor);

    return NULL;
}







// mora signalizirat da je doso na odredenu stranu
void *Camac(void *x)
{
    int i = *((int *)x);

    cout << "C: prazan na desnoj obali" << endl;
    ispis();

    while (true)
    {
        pthread_mutex_lock(&monitor);
        if (camacObala == 1)
        {
            //cout << "C: prazan na desnoj obali" << endl;
            //ispis();

            while (camac.size() < 3)
            {
                
                pthread_cond_wait(&camacCeka, &monitor); // stavi da ga putnici signal kad udu
            }

            cout<<"C: tri putnika ukrcana, polazim za jednu sekundu"<<endl;
            pthread_mutex_unlock(&monitor);
            sleep(1);
            pthread_mutex_lock(&monitor);

            cout << "C: prevozim s desne na lijevu obalu:";
            ispisVektor(camac);
            cout << endl;
            camacObala = -1;
            pthread_mutex_unlock(&monitor);
            sleep(2);
            pthread_mutex_lock(&monitor);
            cout << "C: preveo s desne na lijevu obalu:";
            ispisVektor(camac);
            cout << endl;
            camac.clear();
            brKuC = 0;
            brMuC = 0;
            brUKuC = 0;
            camacObala = 0;
            //broadast i promjenit obalu
            pthread_cond_broadcast(&misionariL);
            pthread_cond_broadcast(&kanibaliL);

            cout << "C: prazan na lijevoj obali" << endl;
            ispis();
            
            pthread_mutex_unlock(&monitor);

        }
        else if(camacObala == 0)
        {
            //cout << "C: prazan na lijevoj obali" << endl;
            //ispis();
            
            while (camac.size() < 3)
            {
                pthread_cond_wait(&camacCeka, &monitor); // stavi da ga putnici signal kad udu
            }

            cout<<"C: tri putnika ukrcana, polazim za jednu sekundu"<<endl;
            pthread_mutex_unlock(&monitor);
            sleep(1);
            pthread_mutex_lock(&monitor);

            cout << "C: prevozim s lijeve na desnu obalu:";
            ispisVektor(camac);
            cout << endl;
            camacObala = -1;
            pthread_mutex_unlock(&monitor);
            sleep(2);
            pthread_mutex_lock(&monitor);
            cout << "C: preveo s lijeve na desnu obalu:";
            ispisVektor(camac);
            cout << endl;
            camac.clear();
            brKuC = 0;
            brMuC = 0;
            brUKuC = 0;
            camacObala = 1;
            pthread_cond_broadcast(&misionariD);
            pthread_cond_broadcast(&kanibaliD);
            
            cout << "C: prazan na desnoj obali" << endl;
            ispis();

            pthread_mutex_unlock(&monitor);
        }
    }

    pthread_mutex_unlock(&monitor);
    return NULL;
}





int main()
{

    pthread_t thr_id_misionari[brStvaranja * 2];
    pthread_t thr_id_kanibali[brStvaranja * 2];
    pthread_t thr_id_camac;
    pthread_mutex_init(&monitor, NULL);

    int *br = new int[brStvaranja]();

    for (int i = 1; i <= brStvaranja; i++)
    {
        br[i - 1] = i;
    }
    std::srand(std::time(nullptr));

    if (pthread_create(&thr_id_camac, NULL, Camac, &br[0]) != 0)
    {
        printf("Greska pri stvaranju dretve!\n");
        exit(1);
    }

    int idx_kanibal = 0;
    int idx_misionar = 0;
    for (int i = 0; i < brStvaranja; i++)
    {
        /* pokretanje dretvi */

        sleep(1);

        if (pthread_create(&thr_id_kanibali[i], NULL, Kanibal, &br[i]) != 0)
        {
            printf("Greska pri stvaranju dretve!\n");
            exit(1);
        }
        idx_kanibal++;

        if (i % 2 != 0)
        {
            if (pthread_create(&thr_id_misionari[i], NULL, Misionar, &br[i]) != 0)
            {
                printf("Greska pri stvaranju dretve!\n");
                exit(1);
            }
            idx_misionar++;
        }
    }

    for (int j = 0; j < idx_kanibal; j++)
    {
        /*hvatanje dretvi*/
        pthread_join(thr_id_kanibali[j], NULL);
    }

    for (int j = 0; j < idx_misionar; j++)
    {
        /*hvatanje dretvi*/
        pthread_join(thr_id_misionari[j], NULL);
    }

    pthread_join(thr_id_camac, NULL);

    return 0;
}