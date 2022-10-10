#!/usr/bin/python3

import serial
import Function as fn
from sympy import *
import control

def write_read(x):
    micro.write(bytes(x, 'utf-8'))
    data=fn.read_COM(micro)
    return data

#declaration communication standrd
starter="?"
ender="!"
separator="/"

#sympy library functions initialization 
init_printing() # doctest: +SKIP
s, z = symbols("s, z")

#serial communication initialization
port=fn.chose_COMPORT()
micro = serial.Serial(port=port, baudrate=115200, timeout=.1)

#open control law
ctrl_law=fn.open_data_user()
ctrl_law=cancel(ctrl_law)
pprint(ctrl_law)

#Z transform
zexpr=control.c2d(ctrl_law,1,'tustin')
pprint(zexpr)

#preparing for microcontroller communication
coeff_u,coeff_y=fn.coeff4micro(zexpr)
msg=starter
msg=msg+separator+str(3+len(coeff_y)+len(coeff_u))
msg=msg+separator+str(len(coeff_u))
msg=msg+separator+str(len(coeff_y))
for el in coeff_u:
    msg=msg+separator+str(el)
for el in coeff_y:
    msg=msg+separator+str(el)
msg=msg+ender

#communication
micro.write(bytes(msg, 'utf-8'))
data=fn.read_COM(micro)
Ts_max=int(data.split("!")[0].split("?")[1])
fmax=floor(1000000/Ts_max*0.9)
fs=0
while(True):
    fs=fn.float_question("The maximum sampling frequency is "+str(fmax)+"\nAt what frequency do you want sampling?")
    if(fs<=fmax):
        break
Ts=1/fs
zexpr=fn.c2d(ctrl_law,Ts)
coeff_u,coeff_y=fn.coeff4micro(zexpr)
msg=starter
msg=msg+separator+str(3+len(coeff_y)+len(coeff_u))
msg=msg+separator+str(len(coeff_u))
msg=msg+separator+str(len(coeff_y))
for el in coeff_u:
    msg=msg+separator+str(el)
for el in coeff_y:
    msg=msg+separator+str(el)
msg=msg+ender
micro.write(bytes(msg, 'utf-8'))
data=fn.read_COM(micro)
print(data)