#!/usr/bin/python3
import sympy

import Function as fn
from sympy import *
import numpy as np
import scipy.optimize as op
import System







#sympy library functions initialization
init_printing() # doctest: +SKIP
s, z, t, k = symbols("s, z, t, k")








sys=1/(s+1)

coeff_u,coeff_y=fn.coeff4micro(fn.c2d(sys,1))
pprint(coeff_u)
pprint(coeff_y)
cu=fn.array2onezero(np.array(coeff_u))
cy=fn.array2onezero(np.array(coeff_y))
data=fn.open_data_user()
data_u=np.array(data[0])
data_y=np.array(data[1])
time=np.array(data[0])

for index in range(len(data_u)):
    if index < len(data_u) * 0.25:
        data_u[index] = 0
    else:
        data_u[index] = 80
data_u=np.array(data_u[int(len(data_u) * 0.25)+200:])
data_y=np.array(data_y[int(len(data_y) * 0.25)+200:])
time=np.array(time[int(len(time) * 0.25)+200:])

n_poles=fn.integer_question("How many poles has the system?")
n_zeros=fn.integer_question("How many zeros has the system?")
phi=data_u
integral=np.empty(data_y.size)
for i in range(n_zeros):
    integral=np.array([np.trapz(data_u[:i],time[:i]) for i in range(data_u.shape[0])])
    phi=np.array([phi.tolist(),integral.tolist()])
for i in range(n_poles):
    integral=np.array([np.trapz(-data_y[:i],time[:i]) for i in range(data_y.shape[0])])
    phi=np.array([phi.tolist(),integral.tolist()])
print(phi)
Q,R=np.linalg.qr(np.transpose(phi))
Q=Q[:1+n_poles+n_zeros,:1+n_poles+n_zeros]
result=
result=np.dot(np.transpose(phi),phi)
print(np.linalg.det(result))
result=np.linalg.inv(result)
result=np.dot(np.dot(result,np.transpose(phi)),Y)

norm_err_old=0
norm_err=0;
shift=0
is_first=True
while True:
    norm_err_old=norm_err
    for index in range(len(data_u)):
        if index < len(data_u)*0.25+shift:
            data_u[index]=0
        else:
            data_u[index]=80

    u_size=len(coeff_u)
    y_size=len(coeff_y)
    l=max([u_size,y_size])
    Y=np.array(data[1][l:])
    print(Y.shape)
    phi=np.empty([Y.size,u_size+y_size])
    print(phi.shape)
    for index in range(Y.size):
        for i,el in enumerate(data_u[index:index+u_size]):
            phi[index][i]=el
        for i, el in enumerate(data[1][index:index+y_size]):
            phi[index][i+u_size] = el
    result=np.dot(np.dot(np.linalg.inv(np.dot(np.transpose(phi),phi)),np.transpose(phi)),Y)
    print(result)
    err=np.add(np.dot(phi,result),-Y)
    norm_err=np.linalg.norm(err)
    print(norm_err)
    print(norm_err_old)
    if not is_first:
        if norm_err_old<norm_err:
            break
    is_first=False
    shift=shift+1
print("finito")
print(shift)
num=0
for i,el in enumerate(result.tolist()[0:u_size]):
    num=num + z**(i-u_size+1)*el
den=1
for i,el in enumerate(result.tolist()[u_size:]):
    den=den-z**(-y_size+i)*el
zexpr=cancel(num/den)
pprint(zexpr)
sexpr=fn.d2c(zexpr,data[2]/1000000.0)
pprint(sexpr)
print(fn.zpk_data(sexpr,s))