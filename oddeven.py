data=[]
value=input("enter value: ")
while value:
    data.append(value)
    value=input("enter value: ")
    
def even(x):
    even=[]
    for i in x:
        if int(i)%2==0:
            even.append(int(i))
    return even
def odd(y):
    odd=[]
    for i in y:
        if int(i)%2==1:
            odd.append(int(i))
    return odd

def sum(list):
    sum=0
    for i in list:
        sum=sum+i
    return sum

print('the sum of all even numbers is:', sum(even(data)))
print('the sum of all odd numbers is:', sum(odd(data)))