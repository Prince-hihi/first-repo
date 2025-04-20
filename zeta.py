# ζ(n)=1+(1/2^n)+(1/3^n)+...(1/r^n)
print('for calculating the ζ function of a number"n",(ζ(n)=1+(1/2^n)+(1/3^n)+...(1/r^n))')
n=float(input("enter the value to calculate the ζ function: "))
r=int(input("enter the range: "))
y=0
for i in range(r):
    y=y+(1/((i+1)**n))
print(f"the ζ fuction of {n} to the range 1/{r}^{n} is: {y}")