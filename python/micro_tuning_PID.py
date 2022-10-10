#!/usr/bin/python3

import serial
import Function as fn
from sympy import *
import numpy as np

methods=["areas method","data streaming","relay method"]
messages=["AM","ST","RM"]
sampling_div=10000

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
choice=fn.multiple_choice("what type of test do you want do?",methods)
msg=starter+separator+messages[choice]+ender
micro.write(bytes(msg, 'utf-8'))
if choice == 0:
    print("Now you have to wait while micro makes some tests to estimate the process")
    data=fn.read_COM(micro)
    list=data.split("!")[0].split("?")[1].split(separator)[1:]
    msg="K:\t"+list[0]+"\nL:\t"+list[1]+"\nT:\t"+list[2]+"\nfinal std (68%):\t"+list[3]+"\nnoise std (68%):\t"+list[4]
    print(msg)
    if fn.digital_question("Do you want to save these datas?"):
        list=[float(el) for el in list]
        fn.save_data_user(list)
elif choice == 1:
    samples=np.empty(sampling_div*2)
    print("testing...")
    sampling_time = float(fn.read_COM(micro).split(starter)[1].split(ender)[0])
    time = np.empty(sampling_div * 2)
    for index in range(len(time)):
        time[index] = index * sampling_time / 1000000.0;
    print(str(sampling_time*sampling_div*2/1000000.0)+"s\t seconds left")
    msg=starter+separator+"OK"+ender
    micro.write(bytes(msg, 'utf-8'))

    for index in range(len(samples)):
        samples[index]=float(fn.read_COM(micro).split("\\")[0].split("\'")[1])
    fn.plot_data(time,samples,"s","y")
    data=[time.tolist(),samples.tolist(),sampling_time,perc]
    fn.save_data_user(data)
elif choice == 2:
    data = fn.read_COM(micro)
    if (not data.split("!")[0].split("?")[1] == "OK"):
        print("there was something wrong. Retry")
        exit()
    max_deriv=fn.float_question("Set the max derivative of the conroller output to bring the system to steady state")
    msg = starter + separator + str(max_deriv) + ender
    micro.write(bytes(msg, 'utf-8'))
    data = fn.read_COM(micro)
    if (not data.split("!")[0].split("?")[1] == "OK"):
        print("there was something wrong. Retry")
        exit()
    print("now you have to wait until the microcontroller testing the system.")
    data = fn.read_COM(micro)
    list = data.split("!")[0].split("?")[1].split(separator)[1:]
    print("There was an error during the procedure. Check if it is all right, then repeat the test. "
          "\nIt could be related to the signal noise")
    if list[0] == "ER":
        print("There was an error during the procedure. Check if it is all right, then repeat the test. "
              "\nIt could be related to the signal noise")
        exit()
    msg = "K:\t" + list[0] + "\nL:\t" + list[1] + "\nT:\t" + list[2] + "\nnoise std (68%):\t" + list[3]
    print(msg)
    if fn.digital_question("Do you want to save these datas?"):
        list = [float(el) for el in list]
        fn.save_data_user(list)