import sys

skup_stanja = input().split(',')
ulazi_abeceda = input().split(',')
prihvatljiva_stanja = input().split(',')
pocetno_prihvatljiva_duljina = len(prihvatljiva_stanja)
poc_stanje = input()

prijelazi = []

while True:
    try:
        line = input()
    except EOFError:
        break
    prijelazi.append(line)

# print(skup_stanja)
# print(ulazi_abeceda)
# print(prihvatljiva_stanja)
# print(poc_stanje)
# print(prijelazi)

stanje_ulaz_tmp = []
novostanje = []
for prijelaz in prijelazi:
    stanje_ulaz_tmp.append(prijelaz.split('->')[0])
    novostanje.append(prijelaz.split('->')[1])

stanje_ulaz = []

for st_ul in stanje_ulaz_tmp:
    parts = st_ul.split(',')
    zaAppend = [parts[0], parts[1]]
    stanje_ulaz.append(zaAppend)


# stanje_ulaz je: [0] stanje, [1] ulaz
# a novostanje je korespondentna lista, za stanjeulaz[0] ima
# odgovarajuce sljedece stanje

def dohvati_indexe(arr, elem):  # vraca listu indeksa na kojima se u arr[] pojavljuje elem
    indeksi = []
    for index, value in enumerate(arr):
        if value == elem:
            indeksi.append(index)
    return indeksi


stanje_novastanja = {}  # mapa koja ima: kljuc je stanje, vrijednost su stanja u koja se moze doc iz kljuca
for i in range(0, len(stanje_ulaz), len(ulazi_abeceda)):
    kljuc = stanje_ulaz[i][0]
    vrijednosti = novostanje[i:i + len(ulazi_abeceda)]
    stanje_novastanja[kljuc] = vrijednosti

# nađi sva dohvatljiva stanja
dohvatljiva = []
dohvatljiva.append(poc_stanje)


def print_matrica(zaPrint):
    for i in range(len(zaPrint)):
        for j in range(len(zaPrint[i])):
            if(j<=i - 1):
                print(zaPrint[i][j], end=' ')
        print()


def nadi_dohvatljiva():
    for doh in dohvatljiva:
        for novodoh in stanje_novastanja[doh]:
            if novodoh not in dohvatljiva:
                dohvatljiva.append(novodoh)
                nadi_dohvatljiva()

    return


def lista_prihvatljivosti(list):
    zaReturn = []
    for l in list:
        if (l in dohvatljiva and l in prihvatljiva_stanja):
            zaReturn.append(1)
        else:
            zaReturn.append(0)
    return zaReturn


nadi_dohvatljiva()

for kljuc in list(stanje_novastanja.keys()):
    if kljuc not in dohvatljiva:
        stanje_novastanja.pop(kljuc)

# maknuta su nedohvatljiva stanja

dohvatljiva.sort()

#print(dohvatljiva)
#print(stanje_novastanja)

dulj = len(dohvatljiva)

tablica = [[0 for j in range(dulj)] for i in range(dulj)]

for i in range(0, dulj):
    for j in range(0, dulj):
        if (j > i): tablica[i][j] = 1

#print_matrica(tablica)
#print()
# prvi test (ako stanja nisu iste prihvatljivosti, stavlja im se 1)
for i in range(0, dulj):
    for j in range(0, dulj):
        if (((dohvatljiva[i] in prihvatljiva_stanja) and (dohvatljiva[j] not in prihvatljiva_stanja))
                or ((dohvatljiva[j] in prihvatljiva_stanja) and (dohvatljiva[i] not in prihvatljiva_stanja))):
            tablica[i][j] = 1

#print_matrica(tablica)
#print()

# drugi test (ako stanja u koja idu redom nisu iste prihvatljivosti, stavlja im se 1)

for i in range(0, dulj):
    for j in range(0, dulj):
        if (lista_prihvatljivosti(stanje_novastanja[dohvatljiva[i]]) !=
                lista_prihvatljivosti(stanje_novastanja[dohvatljiva[j]])):
            tablica[i][j] = 1

# 3 test: ovi koji imaju 0 jos uvijek;   provjera jel ti parovi tipa 1 2,
# za svaki od njihovih praova novih staanja provjeravamo jel im je u
# matrici vec x ili ne. Ako ni za jedan nije x, ne stavljamo ni za provjeravani
# a ako za bilo koji je x u tablici vec, stavljamo i provjeravanima x


for i in range(0, dulj):
    for j in range(0, dulj):
        if tablica[i][j] == 0:
            for k in range(0, len(ulazi_abeceda)):
                if tablica[dohvatljiva.index(stanje_novastanja[dohvatljiva[i]][k])][dohvatljiva.index(stanje_novastanja[dohvatljiva[j]][k])] == 1:
                    tablica[i][j] = 1



#print_matrica(tablica)
#print()

#print(dohvatljiva)
za_izbacit = []
ostavljeni = []


#sad moras iz dohvatljivih izbacit istovjetna

for i in list(range(len(dohvatljiva))):
    for j in list(range(len(dohvatljiva))):
        if (tablica[i][j] == 0 and j<i):
            if(dohvatljiva[i] < dohvatljiva[j]):
                if(dohvatljiva[i] not in za_izbacit):
                    za_izbacit.append(dohvatljiva[j])
                    ostavljeni.append(dohvatljiva[i])

            else:
                if (dohvatljiva[j] not in za_izbacit):
                    za_izbacit.append(dohvatljiva[i])
                    ostavljeni.append(dohvatljiva[j])

#print(za_izbacit)
#print(ostavljeni)

for st in za_izbacit:
    if(st in dohvatljiva):
        dohvatljiva.remove(st)

#print(dohvatljiva)


#iz dictionarya izbacit one kljuceve koji nisu medu dohvatljiva (dohvatljiva
# su sad zapravo sva stanja koja mogzu postojat jer smo maknuli nedohvatljiva
# i istovjetna)  i   iz valuesa svakog zamijenit ovako, ako je neko medu onima u  zaizbacit, zamijenit
#ga onim iz ostavljeni (ostavljeni na [i] ima onog koji je ostao nakon kaj je onaj sa
# zaizbacit[i] izbacen)

for st in za_izbacit:
    if(st in stanje_novastanja.keys()):
        stanje_novastanja.pop(st)

for key in stanje_novastanja.keys():
    for v in stanje_novastanja[key]:
        if(v in za_izbacit):
            stanje_novastanja[key][stanje_novastanja[key].index(v)] = ostavljeni[za_izbacit.index(v)]

#print(stanje_novastanja)

#iz prihvatljivih izbaci ona koja nisu medu dohvatljiva
for st in za_izbacit:
    if (st in prihvatljiva_stanja):
        prihvatljiva_stanja.remove(st)

#iz prihvatljivih izbacit koja nisu u dohvatljiva
prihvatljiva_stanja = list(filter(lambda x: x in dohvatljiva, prihvatljiva_stanja))


#pocetno potencijalno zamjenit
if poc_stanje in za_izbacit:
    poc_stanje = ostavljeni[za_izbacit.index(poc_stanje)]


abeceda_zaprint = ",".join(elem for elem in ulazi_abeceda)
########################
if(len(prihvatljiva_stanja) == 0 or prihvatljiva_stanja[0] == ""):
    print(poc_stanje)
    print(abeceda_zaprint)
    print()
    print(poc_stanje)
    for i in range(0, len(ulazi_abeceda)):
        print(f"{poc_stanje},{ulazi_abeceda[i]}->{poc_stanje}")
elif (len(skup_stanja) == pocetno_prihvatljiva_duljina):
    print(poc_stanje)
    print(abeceda_zaprint)
    print(poc_stanje)
    print(poc_stanje)
    for i in range(0, len(ulazi_abeceda)):
        print(f"{poc_stanje},{ulazi_abeceda[i]}->{poc_stanje}")


else:
    # printat dohvatljiva
    dohvatljiva_zaprint = ",".join(elem for elem in dohvatljiva)
    print(dohvatljiva_zaprint)

    # printat abecedu

    print(abeceda_zaprint)

    # printat prihvatljiva
    prihvatljiva_zaprint = ",".join(elem for elem in prihvatljiva_stanja)
    print(prihvatljiva_zaprint)

    # printat pocetno
    print(poc_stanje)

    # printat prijelaze
    for key in stanje_novastanja.keys():
        for i in range(0, len(ulazi_abeceda)):
            print(f"{key},{ulazi_abeceda[i]}->{stanje_novastanja[key][i]}")
