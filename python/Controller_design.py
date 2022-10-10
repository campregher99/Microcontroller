#!/usr/bin/python3
import time

import matplotlib.pyplot as plt
import serial
import Function as fn
from sympy import *
import control
import numpy as np

#declaration communication standrd
starter="?"
ender="!"
separator="/"

#sympy library functions initialization
init_printing() # doctest: +SKIP
s= control.tf("s")
z=control.tf("z")

#loading identification data
data=fn.open_data_user()
k=data[0]
L=data[1]
T=data[2]
std=data[3]

print("Standard deviation test(68%):\t"+str(std))

Controller = [["Zieggler Nichols","Chien-Hrones-Reswick","Haalman","Cancellation controller"],
              ["ZN","CHR","Hal","ConDel"]]
task=[["set point following","disturbance rejection"],
      ["-SP","-DR"]]
type=["P","PI","PID"]
choice=fn.multiple_choice("Chose the designe procedure:",Controller[0])
ctrl_mtd=""
ctrl_law=0
ctrl_type=0
if choice in range(len(Controller[0])-1):
    if Controller[1][choice] == "CHR":
        ovs=[0,20]
        ctrl_mtd=Controller[1][choice]+task[1][fn.multiple_choice("What is the task you want perform?",task[0])]\
                 +"-"+str(ovs[fn.multiple_choice("how many overshot do you want?",ovs)])
        ctrl_type=fn.multiple_choice("What type of PID law you want?",type)
    elif Controller[1][choice] == "ZN":
        ctrl_mtd=Controller[1][choice]
        ctrl_type = fn.multiple_choice("What type of PID law you want?", type)
    elif Controller[1][choice] == "Hal":
        ctrl_mtd=Controller[1][choice]
        ctrl_type=1
    ctrl_law=fn.PID(K=k,L=L,T=T,method=ctrl_mtd,type=ctrl_type)[0]
else:
    process=k/(T*s+1)
    set_time = fn.float_question("Settling time desired[s]:")
    wt = 5 / (set_time-L)
    ctrl_law = wt / process / s / (s / wt / 10 + 1)
    w = np.logspace(np.log10(wt/10), np.log10(wt*100), 200)
    control.bode(wt / s / (s / wt / 10 + 1),w)
    plt.show()
    dist = 1 / (1 + wt / s / (s / wt / 10 + 1))
    control.bode(dist,w)
    plt.show()

print("\n\nControl law:")
print(ctrl_law)

fn.save_data_user(ctrl_law)