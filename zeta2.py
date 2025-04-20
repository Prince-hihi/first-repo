# for checking at what range does ζ(2) is approx equal to π^2/6 (to 16 decimal places)



π=3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067
n=float(input("enter the value to calculate the ζ function: "))
y=int(0)
r=(int(input("enter the range: ")))
for i in range(1,r):
    y=y+(1/(i**n))
    if y>(π**2)/6:
        break        
if y>(π**2)/6:
    print(f"the minimum range for ζ({n}) req to exceed π^2/6 is: {i}")
else:
    print(f"the value of ζ({n}) could not exceed π^2/6 with the range {i}")

print(f"val obtained with range {i} is equal to {y}")
print("π^2/6=",π**2/6,sep="")
diff=(π**2/6)-y
print("the difference between val obtained, and π^2/6=",diff,sep="")