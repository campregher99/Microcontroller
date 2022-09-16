#!/usr/bin/python3

import serial
import Function as fn
from sympy import *
import time

#declaration communication standrd
starter="?"
ender="!"
separator="/"

#sympy library functions initialization
init_printing() # doctest: +SKIP
s, z = symbols("s, z")

#load identification data
data=fn.open_data_user()
process=data[0]/(data[2]*s+1)
pprint(process)

#built controller
set_time=fn.float_question("Settling time desired[s]:")
wt=5/set_time
controller = wt/process/s/(s/wt/10 +1)
print("Controller(S):")
pprint(controller)

#praparing micro
zcontroller=fn.c2d(controller,1)
coeff_u,coeff_y=fn.coeff4micro(zcontroller)
print(coeff_u)
print(coeff_y)

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
msg=starter
msg=msg+separator+str(1+len(coeff_y)+len(coeff_u))
msg=msg+separator+str(len(coeff_u))
msg=msg+separator+str(len(coeff_y))
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
zexpr=fn.c2d(controller,Ts)
pprint(zexpr)
zpk=fn.zpk_data(zexpr,z)
print(zpk)
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
msg=starter+separator+str(Ts*1000000)+ender
micro.write(bytes(msg,'utf-8'))
data=fn.read_COM(micro)
print(data)