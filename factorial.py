#for x!  (f(x)=x(x-1)(x-2)...3*2*1)
x=int(input("enter the number to calculate the factorial- "))
f=1
for i in range(x):
    f=f*(x-i)
print(f"the factorial of {x} is:",f)