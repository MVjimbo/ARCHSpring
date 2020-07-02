from numba import cuda
import numpy as np
import random
import time

N=1000
griddim=1
blockdim=1

@cuda.jit("void(float64[:],float64[:])")
def Sum(a,b):
    for i in range(N):
        a[i]+=b[i]


if __name__=="__main__":
    print(griddim,blockdim)

    a=np.array([random.random()*10 for i in range(N)],dtype=np.float64)
    b=np.array([random.random()*10 for i in range(N)],dtype=np.float64)
    resultIt=np.zeros(N,dtype=np.float64)

    start=time.time()
    result=a+b
    print("Time without CUDA(in Numpy) ",time.time()-start)

    start1=time.time()
    for i in range(N):
        resultIt[i]=a[i]+b[i]
    print("Time without CUDA(in loop) ",time.time()-start1)

    start2 = time.time()
    da=cuda.to_device(a)
    db=cuda.to_device(b)
    Sum[griddim,blockdim](da,db)

    cuda.synchronize()
    resultC=da.copy_to_host()
    print("Time with CUDA ",time.time()-start2)

    is_Equal=True
    for i in range(N):
        if abs(result[i]-resultC[i])>1/100:
            print("Error ",i)
            print(result[i])
            print(resultC[i])
            is_Equal=False
            break
    if is_Equal:
        print("Same results")