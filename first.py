print('for calculateing the function"f(x)=1+x+x^2+x^3...x^n,"')
x=int(input("enter the value of x: "))
n=int(input("enter the range for the exponent: "))
s=0
for i in range(n+1):
    s=s+x**i
print(f'the value of f({x}) upto exponent {x}^{n} is,', s)
