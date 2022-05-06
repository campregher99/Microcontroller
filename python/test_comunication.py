#!/usr/bin/python3

import serial
import time
import serial.tools.list_ports
import Function as fn
port=fn.chose_COMPORT()
micro = serial.Serial(port=port, baudrate=115200, timeout=.1)
def write_read(x):
    micro.write(bytes(x, 'utf-8'))
    data=fn.read_COM(micro)
    return data

while True:
    num = input("Enter a number: ") # Taking input from user
    value = write_read(str(num))
    print(value) # printing the value