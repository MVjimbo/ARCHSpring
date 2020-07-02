from numba import cuda
import numpy as np
import random
import time

N=5000000
blockdim=1024
griddim=int((N+(blockdim-1))/blockdim)
N_new=blockdim*griddim

@cuda.jit("void(float64[:],float64[:])")
def Sum(a,b):
    i=cuda.grid(1)
    a[i]+=b[i]


if __name__=="__main__":
    print(griddim,blockdim)

    a=np.array([random.random()*10 for i in range(N)],dtype=np.float64)
    b=np.array([random.random()*10 for i in range(N)],dtype=np.float64)
    resultIt=np.zeros(N,dtype=np.float64)

    start=time.time()
    result=a+b
    print("Time without CUDA(in Numpy) ",time.time()-start)

    start2 = time.time()
    for i in range(N):
        resultIt[i] = a[i] + b[i]
    print("Time without CUDA(in loop) ", time.time() - start2)

    start1 = time.time()
    a_new=a.copy()
    a_new.resize(N_new)
    b_new = b.copy()
    b_new.resize(N_new)
    da=cuda.to_device(a_new)
    db=cuda.to_device(b_new)
    Sum[griddim,blockdim](da,db)

    cuda.synchronize()
    resultC=da.copy_to_host()
    print("Time with CUDA ",time.time()-start1)

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