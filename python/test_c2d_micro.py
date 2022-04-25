#!/usr/bin/python3

from __future__ import division
import prova_sympy_simplify as fn
from sympy.solvers import solve
from sympy import *
import Function as fn
x, y, z, t = symbols('x y z t')
k, m, n = symbols('k m n', integer=True)
f, g, h = symbols('f g h', cls=Function)

init_printing() # doctest: +SKIP
s, z = symbols("s, z")

expr = fn.insert_sys("inserisci il tuo sistema:")
expr=cancel(expr)

pprint(expr)
print("Z P K:\t"+str(fn.zpk_data(expr,s)))
zexpr=fn.c2d(expr,0.001)
pprint(zexpr)
zn,zd=fraction(zexpr)
div=Poly(zd).all_coeffs()[0]
zn=cancel(zn/div)
zd=cancel(zd/div)
pprint(zn/zd)
print("Z P K:\t"+str(fn.zpk_data(zexpr,z)))
coeff_u,coeff_y=fn.coeff4micro(zexpr)
fn.step(zexpr,5,0.001)
