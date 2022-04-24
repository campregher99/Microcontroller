#!/usr/bin/python3

from sympy import *

s,z=symbols("s,z")
def c2d(expr,T):
	new=cancel(expr)
	new=new.subs(s,2/T*(z-1)/(z+1))
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

def coefficient(expr,var):
	coeffs=[]
	i=0
	a=0
	while(True):
		coeffs.append(expr.coeff(var**i))
		print(var**i)
		i=i+1
		if(coeffs[-1]==0):
			a=a+1
		else:
			a=0
		if(a==10):
			return coeffs[:-10]