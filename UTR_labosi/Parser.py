ulaz = input()
zavrseno = False


def C():
    print('C', end="")
    A()
    A()


def B():
    global ulaz
    global zavrseno
    if zavrseno: return
    print('B', end="")

    if len(ulaz) == 0 and not zavrseno:
        zavrseno = True
        print()
        print('DA')
        return

    if ulaz[0] == 'c':
        ulaz = ulaz[1:]
        if ulaz[0] != 'c':
            print()
            print('NE')
            zavrseno = True
        ulaz = ulaz[1:]
        S()
        if zavrseno: return
        if len(ulaz) == 0 and not zavrseno:
            zavrseno = True
            print()
            print('DA')
            return

        if ulaz[0] != 'b':
            print()
            print('NE')
            zavrseno = True
        ulaz = ulaz[1:]
        if ulaz[0] != 'c':
            print()
            print('NE')
            zavrseno = True
        ulaz = ulaz[1:]


def A():
    global ulaz
    global zavrseno
    if zavrseno: return
    print('A', end="")

    if len(ulaz) == 0 and not zavrseno:
        zavrseno = True
        print()
        print('NE')
        zavrseno = True
        return

    if ulaz[0] == 'b':
        ulaz = ulaz[1:]
        C()

    elif ulaz[0] == 'a':
        ulaz = ulaz[1:]
    else:
        print()
        print('NE')
        zavrseno = True


def S():
    global ulaz
    global zavrseno
    if zavrseno: return
    print('S', end="")

    if len(ulaz) == 0 and not zavrseno:
        zavrseno = True
        print()
        print('NE')
        zavrseno = True
        return

    if ulaz[0] == 'a':
        ulaz = ulaz[1:]
        A()
        B()
    elif ulaz[0] == 'b':
        ulaz = ulaz[1:]
        B()
        A()
    else:
        print()
        print('NE')
        zavrseno = True


S()

if len(ulaz) != 0 and not zavrseno:
    print()
    print('NE')
elif not zavrseno:
    print()
    print('DA')
