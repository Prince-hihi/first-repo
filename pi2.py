import time
i=0
pi=0
t_end=time.time()+5
while time.time()<t_end:
    d=((2*i+1)*(-1)**i)/4
    pi=pi+1/d
    i+=1
print(pi)
print("4/",4*d)
print(i)
pi2=0
for j in range (1,100000):
    pi2+=(6/(j**2))**(1/2)
print(pi2)