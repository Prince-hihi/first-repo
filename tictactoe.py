import random
import itertools

data = {"A":["       |     |     ","A _____|_____|_____"],"B":["       |     |     ","B _____|_____|_____"],"C":["       |     |     ","C      |     |     "]}

def refresh():

    print((data["A"])[0])
    print((data["A"])[1])
    print((data["B"])[0])
    print((data["B"])[1])
    print((data["C"])[0])
    print((data["C"])[1])
    print("    1    2    3")

def cross(A,n):

    a= list((data[A])[0])
    a[4+5*(n-1)]="\\"
    a[5+5*(n-1)]="/"
    (data[A])[0]="".join(str(x) for x in a)
    b= list((data[A])[1])
    b[4+5*(n-1)]="/"
    b[5+5*(n-1)]="\\"
    (data[A])[1]="".join(str(x) for x in b)

def circle(A,n):

    a= list((data[A])[0])
    a[4+5*(n-1)]="/"
    a[5+5*(n-1)]="\\"
    (data[A])[0]="".join(str(x) for x in a)
    b= list((data[A])[1])
    b[4+5*(n-1)]="\\"
    b[5+5*(n-1)]="/"
    (data[A])[1]="".join(str(x) for x in b)

combos=["A1","A2","A3","B1","B2","B3","C1","C2","C3"]

Win_combos = [
    ['A1', 'A2', 'A3'], ['A1', 'B2', 'C3'], ['A3', 'B2', 'C1'],
    ['A1', 'B1', 'C1'], ['A2', 'B2', 'C2'], ['A3', 'B3', 'C3'],
    ['B1', 'B2', 'B3'], ['C1', 'C2', 'C3']
]

print("instructions: enter the coordinate of the square you want to fill for example: A2 or B3")
refresh()

moves=[]
userlist=[]
autolist=[]

i=1
while i==1:
    z=1
    while z==1:
        usermove=input("your move: ").capitalize()
        if usermove not in combos:
            print("enter valid move")
        elif usermove in moves:
            print("square occupied!")
        else:
            cross((usermove[0]),int(usermove[1]))
            moves.append(usermove)
            userlist.append(usermove)
            z=z+1

    val_avail=[]
    for j in combos:
        if j not in moves:
            val_avail.append(j)

    if len(val_avail)==0:
        i+=1
        print("Game over: Draw")
    
    if len(userlist)>2:
        for k in itertools.combinations(userlist,3):
            if sorted(k) in Win_combos:
                i+=1
                refresh()
                print("Game over!: you win")
    
    if i!=1:
        break

    if len(val_avail)>0:
        automove=random.choice(val_avail)
        circle(automove[0],int(automove[1]))
        autolist.append(automove)
        moves.append(automove)
        print(f"the computer has chosen {automove}")

    refresh()

    if len(autolist)>2:
        for l in itertools.combinations(autolist,3):
            if sorted(l) in Win_combos:
                i=i+1
                print("Game over!: you lose")