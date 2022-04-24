#!/usr/bin/python3

from __future__ import division
import prova_sympy_simplify as fn
import prova_ricorsione as ric
from sympy.solvers import solve
from sympy import *
x, y, z, t = symbols('x y z t')
k, m, n = symbols('k m n', integer=True)
f, g, h = symbols('f g h', cls=Function)

init_printing() # doctest: +SKIP
T=0.01
s, z = symbols("s, z")
expr = (s+10)/(s**2+s+25)
expr=cancel(expr)
pprint(expr)
zexpr=ric.c2d(expr,0.1)
pprint(zexpr)
