#!/usr/bin/python3

import os
import pickle
import math
import json
from json import encoder
from sys import platform

import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import filedialog
from datetime import datetime
import numpy as np
from sympy import *
from sympy.solvers import solve
import serial.tools.list_ports
import serial


s,z=symbols("s,z")
init_printing()


def read_COM(port):
    while(True):
        data = port.readline()
        if(data):
            break
    return str(data)


def write_COM(port,msg):
    port.write(bytes(msg, 'utf-8'))

def chose_COMPORT():
    while(True):
        ports = serial.tools.list_ports.comports()
        i = 0
        for port, desc, hwid in ports:
            print(str(i)+"\t{}: {}".format(port, desc))
            i=i+1
        port=integer_question("What is the port number?\nIf there isn't the desired port write: -1")
        if(port<=i and port>=0):
            break
        print("Port number doesn't exist")
    return ports[port][0]


def c2d(expr,Tc):
    new=cancel(expr)
    new=new.subs(s,2/Tc*(z-1)/(z+1))
    new=simp_expr(new)
    return new

def d2c(expr,Tc):
    new = cancel(expr)
    new = new.subs(z, (1+Tc/2*s)/(1-Tc/2*s))
    new = simp_expr(new)
    return new

def simp_expr(expr):
    if expr.is_Pow:
        if expr.args[0].is_Mul:
            mul=1
            for arg in expr.args[0].args:
                s=simp_expr(arg)
                for i in range(int(expr.args[1])-1):
                    s=s*s
                mul=mul*s
            mul = cancel(mul)
            return mul
        s=simp_expr(expr.args[0])
        return (Pow(s,expr.args[1]))
    elif expr.is_Add:
        add=0
        for arg in expr.args:
            add=add+simp_expr(arg)
        add=cancel(add)
        return add
    elif expr.is_Mul:
        mul=1
        for arg in expr.args:
            mul = mul * simp_expr(arg)
        mul =cancel(mul)
        return mul
    else:
        return expr


def digital_question(question):
    while True:
        print(question + "(y/n)")
        ans = str(input())
        if len(ans) == 1 and ("y" in ans or "n" in ans):
            break
    if ans == "y":
        return True
    else:
        return False

def integer_question(question):
    while True:
        print(question)
        isExept = False
        try:
            number = int(input())
        except:
            isExept = True
        if not isExept:
            return number

def float_question(question):
    while True:
        print(question)
        isExept=False
        try:
            number = float(input())
        except:
            isExept=True
        if not isExept:
            return number

def perc_question(question):
    while True:
        print(question)
        isExept=False
        try:
            number = float(input())
        except:
            isExept=True
        if not (isExept or number>100 or number<0):
            return number

def str_question(question):
    while True:
        print(question)
        str = input()
        if not str=="":
            return str

def multiple_choice(question, choices):
    while True:
        for i, choice in enumerate(choices):
            print(str(i)+"-\t"+str(choice))
        ch=integer_question(question)
        if ch >= 0 and ch < len(choices):
            return ch
            break

def insert_sys(string):
    print(string)
    while True:
        is_ok=True
        sys=input()
        try:
            sys=parse_expr(sys)
        except:
            is_ok=False
        if(is_ok):
            return sys
        print("There is an error in your input system:\n"+sys)

def insert_PID(string):
    print(string)
    while True:
        is_ok = True
        print("Kp:\t")
        Kp=input()
        print("Ti:\t")
        Ti = input()
        print("Td:\t")
        Td = input()
        print("N:\t")
        N = input()
        sys=Kp+"*(1+1/(s*"+Ti+")+"+Td+"*s/("+Td+"/"+N+"*s+1))"
        try:
            sys = parse_expr(sys)
        except:
            is_ok = False
        if (is_ok):
            return sys
        print("There is an error in your input system:\n" + sys)

def zpk_data(expr,var):
    num,den=fraction(expr)
    poles=solve(den)
    zeros=solve(num)
    coef_n=Poly(num,var).all_coeffs()
    coef_d=Poly(den,var).all_coeffs()
    if(var==s):
        K=coef_n[-1]/coef_d[-1]
    elif(var==z):
        K=num.subs(z,1)/den.subs(z,1)
    return zeros,poles,K

def step(sys,Tc,T=-1):
    coeff_u,coeff_y=coeff4micro(sys)
    k=zpk_data(sys,z)
    U=np.zeros(len(coeff_u)-1).tolist()
    Y=np.zeros(len(coeff_y)).tolist()
    if(T==-1):
        time=[0]
        i=0
        while True:
            time.append(time[-1]+Tc)
            U.append(1)
            Y.append(sys_out(coeff_u,coeff_y,U,Y))
            if(Y[-1]<1.01*k[2] and Y[-1]>0.99*k[2]):
                i=i+1
            else:
                i=0
            if(i==100):
                break
        time.pop()
    else:
        time=np.linspace(0,T,T/Tc).tolist()
        for t in time:
            U.append(1)
            Y.append(sys_out(coeff_u,coeff_y,U,Y))
    plot_data(time,Y[len(coeff_y):])
    time.insert(0,-Tc)
    plot_data(time,U[len(coeff_u)-2:])
    return Y,U,time


def feedback(L,H):
    sys=L/(1+L*H)
    sys=cancel(sys)
    return sys

def sys_out(coeff_u,coeff_y,U,Y):
    y=0.0
    for i in range(len(coeff_u)):
        y=y+U[-i-1]*coeff_u[i]
    for i in range(len(coeff_y)):
        y=y+Y[-i-1]*coeff_y[i]
    return y


def coeff4micro(expr):
    num,den=fraction(expr)
    coef_n=Poly(num,z).all_coeffs()
    coef_d=Poly(den,z).all_coeffs()
    coef_n=np.array([coeff/coef_d[0] for coeff in coef_n])
    coef_d=np.array([-coeff/coef_d[0] for coeff in coef_d[1:]])
    return coef_n.tolist(),coef_d.tolist()


def open_data(name):
    path=get_path()+"/"+name+".txt"
    try:
        file = open(path, "rb")
    except:
        return "not_in"
    try:
        data=pickle.load(file)
    except Exception as e:
        return ["empty file", e]
    file.close()
    return data


def write_data(name,data):
    sep = detect_sep()
    path=get_path()+sep+name+".txt"
    file = open(path, "wb")
    pickle.dump(data,file)
    file.close()


def get_path():
    sep=detect_sep()
    file_path = os.path.realpath(__file__)
    file_path=file_path.split(sep)
    file_path.pop()
    file_path.pop(0)
    path=''
    for el in file_path:
        path+=sep+el
    return path

def detect_sep():
    if platform == "win32":
        return "\\"
    elif os == "Ubuntu":
        return "/"
    else:
        return "/"

def open_data_user():
    root = tk.Tk()
    root.withdraw()
    file_path = filedialog.askopenfilename()
    file = open(file_path, "rb")
    data=pickle.load(file)
    file.close()
    return data

def save_data_user(data):
    root = tk.Tk()
    root.withdraw()
    file_path = filedialog.asksaveasfilename()
    file = open(file_path, "wb")
    pickle.dump(data, file)
    file.close()

def plot_data(data_x,data_y,label_x="x",label_y="y",fig=1):
    plt.figure(fig)
    plt.plot(data_x, data_y)
    plt.xlabel(label_x)
    plt.ylabel(label_y)
    plt.show()

def PID(K=0,Kstar=0,T=0,Tstar=0,L=0,N=10,method="CHR-SP-0",type=1):
    if not T == 0:
        a = L * K / T
    else:
        a=0
    if method == 'ZN':
        if type == 0:
            kp = 1 / a
            Cc = kp
            Cf = kp
        elif type == 1:
            kp = 0.9 / a
            Ti = 3 * L
            Cc = kp * (1 + 1 / Ti / s)
            Cf = kp * (1 + 1 / Ti / s)
        elif type == 2:
            kp = 1.2 / a
            Ti = 2 * L
            Td = L / 2
            Cc = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
            Cf = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
    elif method == 'CHR-SP-0':
        if type == 0:
            kp = 0.3 / a
            Cc = kp
            Cf = kp
        elif type == 1:
            kp = 0.35 / a
            Ti = 1.2 * L
            Cc = kp * (1 + 1 / Ti / s)
            Cf = kp * (1 + 1 / Ti / s)
        elif type == 2:
            kp = 0.6 / a
            Ti = 1 * L
            Td = L / 2
            Cc = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
            Cf = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
    elif method == 'CHR-SP-20':
        if type == 0:
            kp = 0.7 / a
            Cc = kp
            Cf = kp
        elif type == 1:
            kp = 0.6 / a
            Ti = 1 * L
            Cc = kp * (1 + 1 / Ti / s)
            Cf = kp * (1 + 1 / Ti / s)
        elif type == 2:
            kp = 0.95 / a
            Ti = 1.4 * L
            Td = L * 0.47
            Cc = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
            Cf = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
    elif method == 'Hal':
        if type == 0:
            kp = 0.7 / a;
            Cc = kp;
            Cf = kp;
            print("not used")
        elif type == 1:
            kp = 2 * T / (3 * L * K)
            Ti = T
            Cc = kp * (1 + 1 / Ti / s)
            Cf = kp * (1 + 1 / Ti / s)
        elif type == 2:
            kp = 0.95 / a
            Ti = 1.4 * L
            Td = L * 0.47
            Cc = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
            Cf = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
            print("not used")
    elif method == 'KT-SR':
        if type == 0:
            kp = 0.7 / a
            Cc = kp
            Cf = kp
            print("not used")
        elif type == 1:
            tau = L / (L + T)
            kp = 0.29 / a * exp(-2.7 * tau + 3.7 * tau ^ 2)
            Ti = T * 0.79 * exp(-1.4 * tau + 2.4 * tau ^ 2)
            b = 0.81 * exp(0.73 * tau + 1.9 * tau ^ 2)
            Cc = kp * (1 + 1 / Ti / s)
            Cf = kp * (b + 1 / Ti / s)
        elif type == 2:
            tau = L / (L + T)
            kp = 3.8 / a * exp(-8.4 * tau + 7.3 * tau ^ 2)
            Ti = T * 0.46 * exp(2.8 * tau + 2.1 * tau ^ 2)
            Td = T * 0.077 * exp(5 * tau + -4.8 * tau ^ 2)
            b = 0.4 * exp(0.18 * tau + 2.8 * tau ^ 2)
            Cc = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
            Cf = kp * (b + 1 / Ti / s + Td * s / (Td / N * s + 1))
    elif method == 'KT-CL':
        if type == 0:
            kp = 0.7 / a
            Cc = kp
            Cf = kp
            print("not used")
        elif type == 1:
            tau = L / (L + T)
            kp = 0.053 * Kstar * exp(2.9 * tau - 2.6 * tau ^ 2)
            Ti = Tstar * 0.9 * exp(-4.4 * tau + 2.7 * tau ^ 2)
            b = 1.1 * exp(-0.0061 * tau + 1.8 * tau ^ 2)
            Cc = kp * (1 + 1 / Ti / s)
            Cf = kp * (b + 1 / Ti / s)
        elif type == 2:
            tau = L / (L + T);
            kp = 0.33 * Kstar * exp(-0.31 * tau - 1 * tau ^ 2)
            Ti = Tstar * 0.76 * exp(-1.6 * tau - 0.36 * tau ^ 2)
            Td = Tstar * 0.17 * exp(-0.46 * tau - 2.1 * tau ^ 2)
            b = 0.4 * exp(0.18 * tau + 2.8 * tau ^ 2)
            Cc = kp * (1 + 1 / Ti / s + Td * s / (Td / N * s + 1))
            Cf = kp * (b + 1 / Ti / s + Td * s / (Td / N * s + 1))
    return [Cc, Cf]

def array2onezero(array):
    new=np.empty(array.size)
    for i,el in enumerate(array):
        if abs(el) > 0:
            new[i]=1
        else:
            new[i]=0
    return new