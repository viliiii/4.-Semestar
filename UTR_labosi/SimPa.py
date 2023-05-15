ulazni_nizovi_line = input().split('|')
ulazni_nizovi = []
for niz in ulazni_nizovi_line:
    ulazni_nizovi.append(niz.split(','))

skup_stanja = input().split(',')
skup_ul_znakova = input().split(',')
skup_stog_znakova = input().split(',')
skup_prihvatljivih_stanja = input().split(',')
pocetno_stanje = input()
pocetni_znak_stoga = input()

funkcije_prijelaza_lines = []
while True:
    try:
        line = input()
    except EOFError:
        break
    funkcije_prijelaza_lines.append(line)

tr_tmp = []
nov_tmp = []

for prijelaz in funkcije_prijelaza_lines:
    tr_tmp.append(prijelaz.split('->')[0])
    nov_tmp.append(prijelaz.split('->')[1])

trSt_ulZn_znSt = []  # Trenutno stanje, ulazni znak, trenutni vrh stoga, korespondentno sa
# novSt_znSt.

novSt_znSt = []  # Novo stanje, novi vrh stoga. Ako je novi vrh stoga dulji od 1, onda
# znaci da se nes dodalo na stog, inace je ostao isti. Ako je novi vrh
# stoga $, znaci obrise se znak sa vrha stoga.

for strn in tr_tmp:
    trSt_ulZn_znSt.append(strn.split(','))

for strn in nov_tmp:
    novSt_znSt.append(strn.split(','))

# print(trSt_ulZn_znSt, len(trSt_ulZn_znSt))
# print(novSt_znSt, len(novSt_znSt))

# slucaj: dode epsilon prijelaz (ulazni znak je epsilon). Onda se mora proc kroz sve
# do kud se moze doc s tim epsilonom

# slucaj: nema ni jednog prijelaza za primjenit (u trst_ulzn_znSt ne postoji
# kombinacija koja je ista trenutnoj zadnjoj kombinaciji da bi se islo dalje). Onda pises fail
# i gotov je taj test.

# slucaj: kad dode do kraja mora provjerit se moze kam dalje s $ odnosno praznim nizom

for i in range(0, len(ulazni_nizovi)):
    j = 0  # iterator za svaki while, za stanje_stog
    u = 0  # iterator za ulazne znakove
    stanje_stog = [[pocetno_stanje, pocetni_znak_stoga]]
    zadnjaprovjera = False
    prviEpsilon = False
    while True:

        if not zadnjaprovjera and not prviEpsilon:
            tmp_stanje_ulaz_stog = [stanje_stog[j][0], ulazni_nizovi[i][u], stanje_stog[j][1][0]]
            u = u + 1
        # stanje_stog[j][1][0] je zadnji clan stoga (zapravo prvi clan stringa),
        # inace je stanje_stog[j][1] cijeli stog, string

        if tmp_stanje_ulaz_stog in trSt_ulZn_znSt:
            # ima prijelaz, napravi ga
            if prviEpsilon:
                prviEpsilon = False
                u = u - 1

            zaAppend = [None, None]
            zaAppend[0] = novSt_znSt[trSt_ulZn_znSt.index(tmp_stanje_ulaz_stog)][0]

            if len(novSt_znSt[trSt_ulZn_znSt.index(tmp_stanje_ulaz_stog)][1]) > 1:
                # dodaje se na stog, odnosno novi znak + stari stog
                # zaAppend[1] = novSt_znSt[trSt_ulZn_znSt.index(tmp_stanje_ulaz_stog)][1][:-1] + stanje_stog[j][1]
                zaAppend[1] = novSt_znSt[trSt_ulZn_znSt.index(tmp_stanje_ulaz_stog)][1] + stanje_stog[j][1][1:]
            elif novSt_znSt[trSt_ulZn_znSt.index(tmp_stanje_ulaz_stog)][1] == '$':

                # brise se vrh stoga
                if stanje_stog[j][1] != '$':
                    subStog = stanje_stog[j][1][1:]  # subStog je stog bez prvog elementa odnosno obrisan je vrh
                    if len(subStog) == 0:
                        subStog = '$'
                    zaAppend[1] = subStog
                else:
                    zaAppend[1] = stanje_stog[j][1]

            elif len(novSt_znSt[trSt_ulZn_znSt.index(tmp_stanje_ulaz_stog)][1]) == 1 and \
                    novSt_znSt[trSt_ulZn_znSt.index(tmp_stanje_ulaz_stog)][1] != '$':
                # vrh stoga ostaje isti, ne mijenja se stog, samo se appenda isti kakav je bio
                zaAppend[1] = stanje_stog[j][1]





            stanje_stog.append(zaAppend)
            j = j + 1




            if zadnjaprovjera:
                if stanje_stog[j][0] in skup_prihvatljivih_stanja:
                    stanje_stog.append('1')
                    break
                # else: !!!!!!!!!!!!!!!!!!!!!!!!!!!!
                #     # stanje_stog.append('0')
                #
                #     break

            if zaAppend[0] in skup_prihvatljivih_stanja and u == len(ulazni_nizovi[i]):
                # dosli do prihvatljivog
                # prihvacamo i zavrsavamo jedino ako su svi znakovi procitani

                stanje_stog.append('1')
                break

            if u == len(ulazni_nizovi[i]):
                # dosli do kraja ulaznih znakova a nismo u prihvatljivom
                # provjerit jel ide kam s $ odnosno praznim nizom, ak da, odi tam
                # inace 0
                tmp_stanje_ulaz_stog = [stanje_stog[j][0], '$', stanje_stog[j][1][0]]

                if tmp_stanje_ulaz_stog in trSt_ulZn_znSt:
                    # print('je')

                    zadnjaprovjera = True


                else:
                    stanje_stog.append('0')
                    break
                # stanje_stog.append('0')
                # break

        else:

            tmp_stanje_ulaz_stog = [stanje_stog[j][0], '$', stanje_stog[j][1][0]]
            if tmp_stanje_ulaz_stog in trSt_ulZn_znSt:
                prviEpsilon = True

            else:
                # nema prijelaz, fail
                stanje_stog.append('fail')
                stanje_stog.append('0')
                break

        # povecaj j
        # ispisi kaj treba prije nove iteracije
        # isprazni polje koje si ispiso

        # if not prviEpsilon:  !!!!!!!!!
        #     j = j + 1







    # print print print

    if (stanje_stog[len(stanje_stog) - 1] == '1' or
        stanje_stog[len(stanje_stog) - 1] == '0') and \
            stanje_stog[len(stanje_stog) - 2] != 'fail':
        for k in range(0, len(stanje_stog)):
            if k == len(stanje_stog) - 1:
                print(stanje_stog[k])
            else:
                print(stanje_stog[k][0], end='#')
                print(stanje_stog[k][1], end='|')
    elif stanje_stog[len(stanje_stog) - 1] == '0' and \
            stanje_stog[len(stanje_stog) - 2] == 'fail':
        for k in range(0, len(stanje_stog)):
            if k == len(stanje_stog) - 2:
                print(stanje_stog[k], end='|')
                print(stanje_stog[k+1])
                break

            else:
                print(stanje_stog[k][0], end='#')
                print(stanje_stog[k][1], end='|')

    # print print print
    stanje_stog.clear()