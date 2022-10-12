#!/usr/bin/python3

import serial
import Function as fn
from sympy import *
import control
import time

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
s=control.tf("s")
z=control.tf("z")

#serial communication initialization
port=fn.chose_COMPORT()
micro = serial.Serial(port=port, baudrate=115200, timeout=.1)
time.sleep(0.1)
micro.write(bytes("msg", 'utf-8'))
data=fn.read_COM(micro)
if(not data.split("!")[0].split("?")[1] =="NE"):
    print("The controller is already setted up. Erase it and retry!")
    exit()

#open control law
ctrl_law=fn.open_data_user()
ctrl_law=ctrl_law
print(ctrl_law)

#Z transform
zexpr=control.c2d(ctrl_law,1,'tustin')
print(zexpr)

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
time.sleep(0.1)
data=fn.read_COM(micro)
Ts_max=int(data.split("!")[0].split("?")[1])
fmax=floor(1000000/Ts_max*0.9)
fs=0
while(True):
    fs=fn.float_question("The maximum sampling frequency is "+str(fmax)+"\nAt what frequency do you want sampling?")
    if(fs<=fmax):
        break
Ts=1/fs
zexpr=control.c2d(ctrl_law,Ts,"tustin")
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
time.sleep(0.1)
msg = starter + separator + str(Ts*1000000) + ender
micro.write(bytes(msg, 'utf-8'))
data=fn.read_COM(micro)
print(data)