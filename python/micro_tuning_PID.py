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


perc=fn.perc_question("What is the input amplitude for the tuning procedure (usually around 8%)?")
msg=starter+separator+str(perc)+ender
