def combinations(lst,k):
    result=[]
    for i in range (len(lst)):
        for j in range (i+1):
            for k in range(j+1):
                if i!=j!=k:
                    result.append([str(lst[i]),str(lst[j]),str(lst[k])])
    return result

lst=[1,2,3,4]
print(combinations(lst,3))