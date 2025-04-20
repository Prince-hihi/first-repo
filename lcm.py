a = int(input('enter num-1: '))
b = int(input('enter num-2: '))

def lcm(x,y):
   if x > y:
      greater = x
   else:
      greater = y
   i=2
   if x==y:
      lcm=x
   else:
      while(True):
         if((greater % x == 0) and (greater % y == 0)):
            lcm = greater
            break
         greater = (greater*i)/(i-1)
         i+=1
   return lcm

print(f'the LCM of {a}, {b} is: ', lcm(a,b))