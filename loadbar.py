#computes the series 1/2^n but with a loading bar telling how much is done

import math
result=0
bar=''
barlist=['|','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.','|']
percent=[0]
rng=60000
for i in range(rng):
    result=result+(1/2**i)

    percent.append(math.floor(i*10/(rng-5)))

    if percent[-1]!=percent[-2]:
        tempbar=bar
        for k in range(percent[-1]):
            barlist[2*k+1]="#"
        for s in barlist:
            tempbar+=s
        print(tempbar)
        print(f"{percent[-1]*10}% done\n")
print(f"the sum is {result}\n")