def factorial(a):
    f=1
    for j in range(i):
        f=f*(a-j)
    return f
e=0
for i in range(1000):
    e+=(1/factorial(i))
print("e=",e)