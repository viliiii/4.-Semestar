import sys

redak1 = input().split('|')
ulazi = []
for i in range(len(redak1)):
    ulazi.append(redak1[i].split(','))  # ulazi je sada lista listica koje su ulazi npr.
    # [[' a', 'pnp', 'a'], ['pnp', 'lab2'],
    # ['pnp', 'a'], ['pnp', 'lab2', 'utr', 'utr']]

stanja = input().split(',')

abeceda = input().split(',')

prihvatljiva_stanja = input().split(',')

poc_stanje = input()

prijelazi = []

while True:
    try:
        line = input()
    except EOFError:
        break
    prijelazi.append(line)

prijelazi_split = []
for prijelaz in prijelazi:
    prijelazi_split.append(prijelaz.split("->"))

stanje_ulaz = [item[0].split(",") for item in prijelazi_split]
novastanja = [item[1].split(",") for item in prijelazi_split]


# stanje_ulaz je lista tipa [[stanje, ulaz], [stanje, ulaz]]

# novastanja je lista tipa [[stanjeNeko], [stanjeNeko, stanjeDrugo]]
# na korespondentnim indeksima su povezane informacije, novastanja[0] sadrze
# nova stanja za stanje_ulaz[0]

# tr_st = [] #trenutna_stanja ce bit lista tr_st

def provj_epsilone():  # dodat da ne gleda nista ak ima hashtagove
    for stanje in trenutna_stanja[zadnja_tr_stanja]:
        for st_ul1 in stanje_ulaz:
            if st_ul1[1] == "$" and st_ul1[0] == stanje:
                # onda dodajemo novastanja
                for novostanje in novastanja[stanje_ulaz.index(st_ul1)]:
                    if novostanje not in trenutna_stanja[zadnja_tr_stanja]:
                        trenutna_stanja[zadnja_tr_stanja] += [novostanje]
                        provj_epsilone()
                        return
    return


def uredi_printaj():
    # dodat hestegove

    for st in trenutna_stanja:
        if("#" in st):
            st.remove("#")

    for listica in trenutna_stanja:
        if len(listica) == 0:
            listica += "#"

    # poredat kak treba
    for listica in trenutna_stanja:
        listica.sort()
        #listica = list(dict.fromkeys(listica))

    trenutna_st = [list(set(x)) for x in trenutna_stanja]


    for st in trenutna_st:
        st.sort()



    # printat sa stapovima
    for i in range(len(trenutna_st)):
        for j in range(len(trenutna_st[i])):
            if (j != len(trenutna_st[i]) - 1):
                print(trenutna_st[i][j] + ",", end="")
            else:
                print(trenutna_st[i][j], end="")

        if (i != len(trenutna_st) - 1):
            print("|", end="")
        else:
            print("")



for ulazi_br in ulazi:
    zadnja_tr_stanja = 0
    trenutna_stanja = [[] for _ in range(len(ulazi_br) + 1)]
    trenutna_stanja[0] = [poc_stanje]
    for ulaz in ulazi_br:
        # ovdje skupit sve epsilon prijelaze
        # za sva trenutna stanja medu njih jos dodat ona u koja se moze doc epsilon prijelazima
        # i onda tek pustit dalje da se provjerava za neki konkretni ulaz u kaj ce se otic
        # prakticki iz svih tih stanja
        provj_epsilone()
        for st_ul in stanje_ulaz:
            if st_ul[1] == ulaz:
                if st_ul[0] in trenutna_stanja[zadnja_tr_stanja]:

                    if "#" in novastanja[stanje_ulaz.index(st_ul)]: novastanja[stanje_ulaz.index(st_ul)].remove("#")

                    trenutna_stanja[zadnja_tr_stanja + 1] += novastanja[stanje_ulaz.index(st_ul)]  # dodaj stanja u koja se dode s tim ulazom

        zadnja_tr_stanja = zadnja_tr_stanja + 1
        provj_epsilone()

    uredi_printaj()