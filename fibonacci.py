import math
fib=[]
n=input("enter range:")
a=1
for i in range(1,int(n)+1):
    if len(fib)<2:
        fib.append(1)
    else:
        fib.append(fib[-1]+fib[-2])

while a==1:
    choice=int(input("1 for full sequence, 2 for last term, 3 for the no.of digits in last term: "))
    if choice==2:
        print(fib[-1])
        a=a+1
    elif choice==1:
        for index, i in enumerate(fib):
            if index<100:
                print(i, end="  ")
            else:
                print(i)
        a=a+1
    elif choice==3:
        print(math.floor(math.log10(fib[-1])+1))
        a=a+1
    else:
        print("enter either 1, 2, or 3")