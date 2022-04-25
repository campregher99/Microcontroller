#!/usr/bin/python3

import os
import pickle
import math
import json
from json import encoder
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import filedialog
from datetime import datetime
import numpy as np
from sympy import *
from sympy.solvers import solve

s,z=symbols("s,z")
init_printing()

def c2d(expr,Tc):
    new=cancel(expr)
    new=new.subs(s,2/Tc*(z-1)/(z+1))
    new=simp_expr(new)
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
    coef_n=[coeff/coef_d[0] for coeff in coef_n]
    coef_d=[-coeff/coef_d[0] for coeff in coef_d[1:]]
    return coef_n,coef_d


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
    path=get_path()+"/"+name+".txt"
    file = open(path, "wb")
    pickle.dump(data,file)
    file.close()


def get_path():
    file_path = os.path.realpath(__file__)
    file_path=file_path.split('/')
    file_path.pop()
    file_path.pop(0)
    path=''
    for el in file_path:
        path+="/"+el
    return path


def open_data_user():
    root = tk.Tk()
    root.withdraw()
    file_path = filedialog.askopenfilename()
    file = open(file_path, "rb")
    data=pickle.load(file)
    file.close()
    return data


def plot_data(data_x,data_y,label_x="x",label_y="y",fig=1):
    plt.figure(fig)
    plt.plot(data_x, data_y)
    plt.xlabel(label_x)
    plt.ylabel(label_y)
    plt.show()
