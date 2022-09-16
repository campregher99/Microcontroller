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

#serial communication initialization
port=fn.chose_COMPORT()
micro = serial.Serial(port=port, baudrate=115200, timeout=.1)
micro.write(bytes("msg", 'utf-8'))
data=fn.read_COM(micro)
if(not data.split("!")[0].split("?")[1] =="OK"):
    print("there was something wrong. Retry")
    exit()

perc=fn.perc_question("What is the input amplitude for the tuning procedure (usually around 8%)?")
msg=starter+separator+str(perc)+ender
micro.write(bytes(msg, 'utf-8'))

data=fn.read_COM(micro)
if(not data.split("!")[0].split("?")[1] =="OK"):
    print("there was something wrong. Retry")
    exit()

print("Now you have to wait while micro makes some tests to estimate the process")
data=fn.read_COM(micro)
list=data.split("!")[0].split("?")[1].split(separator)[1:]
msg="K:\t"+list[0]+"\nL:\t"+list[1]+"\nT:\t"+list[2]+"\nfinal std (68%):\t"+list[3]+"\nnoise std (68%):\t"+list[4]
print(msg)
if fn.digital_question("Do you want to save these datas?"):
    list=[float(el) for el in list]
    name=fn.str_question("file name:")
    fn.write_data(name, list)