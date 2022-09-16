#!/usr/bin/python3

import serial
import Function as fn
from sympy import *

#declaration communication standrd
starter="?"
ender="!"
separator="/"

#sympy library functions initialization
init_printing() # doctest: +SKIP
s, z = symbols("s, z")

#sure that the micro was erased
if not fn.digital_question("Have you erased microcontroller?"):
    print("Erase microcontroller by flashing eraser.ino")
    exit()

#serial communication initialization
port=fn.chose_COMPORT()
micro = serial.Serial(port=port, baudrate=115200, timeout=.1)

#check if the micro is reading
micro.write(bytes("msg", 'utf-8'))
msg=fn.read_COM(micro)
if(msg.split("!")[0].split("?")[1]=="IE"):
    print("you have to erase the micro before setting up it!!!")
    while(True):
        c=0
elif(msg.split("!")[0].split("?")[1]!="NE"):
    print("somthing wrong with the micro!!!")
    while (True):
        c = 0

#system acquisition
expr = fn.insert_PID("insert the system:")
expr=cancel(expr)
pprint(expr)

#Z transform
print("Z P K:\t"+str(fn.zpk_data(expr,s)))
zexpr=fn.c2d(expr,0.001)
pprint(zexpr)
print("Z P K:\t"+str(fn.zpk_data(zexpr,z)))

#preparing for microcontroller communication
coeff_u,coeff_y=fn.coeff4micro(zexpr)
pprint(coeff_u)
pprint(coeff_y)
msg=starter
msg=msg+separator+str(1+len(coeff_y)+len(coeff_u))
for el in coeff_u:
    msg=msg+separator+str(el)
for el in coeff_y:
    msg=msg+separator+str(el)
msg=msg+ender
print(msg)

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
print(fs)
Ts=1/fs
zexpr=fn.c2d(expr,Ts)
pprint(zexpr)
coeff_u,coeff_y=fn.coeff4micro(zexpr)
msg=starter
msg=msg+separator+str(3+len(coeff_y)+len(coeff_u))
for el in coeff_u:
    msg=msg+separator+str(el)
for el in coeff_y:
    msg=msg+separator+str(el)
msg=msg+ender
micro.write(bytes(msg, 'utf-8'))
msg=starter+separator+str(Ts*1000000)+ender
micro.write(bytes(msg,'utf-8'))
data=fn.read_COM(micro)
print(data)