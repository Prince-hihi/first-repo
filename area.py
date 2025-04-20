import math
def area(x,y,z):
    s=(x+y+z)/2
    area=math.sqrt(s*(s-x)*(s-y)*(s-z))
    return area
a=float(input("enter lenght 1: "))
b=float(input("enter lenght 2: "))
c=float(input("enter lenght 3: "))

print(f"the area of the triangle is :{area(a,b,c)} unit^2")