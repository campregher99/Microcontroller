from sympy import *
import numpy as np
class System:
    zeros_s=[]
    poles_s=[]
    poles_z=[]
    zeros_z=[]
    Tc=1

    def __init__(self,s,z,t):
        self.s=s
        self.z=z
        self.t=t
        self.s_expr=self.s
        self.c2d()


    def set_ampling_time(self,_Tc):
        self.Tc=_Tc

    def set_sys_S(self,_s_expr):
        self.s_expr=_s_expr
        self.c2d()
        self.zpk_s()
        self.zpk_z()

    def inv_transf(self,expr):
        expr=expr.apart()
        return inverse_laplace_transform(expr, self.s, self.t)

    def curve_fit_func(self,t,k,p1,p2,p3,p4,p5,z1,z2,z3,z4,z5):
        P=np.array([p1,p2,p3,p4,p5])
        Z=np.array([z1,z2,z3,z4,z5])
        num = 1
        den = 1
        for i in range(5):
            if i < len(self.poles_s)-1:
                num=num*(P[i]+self.s)
            if i < len(self.zeros_s)-1:
                den=den*(Z[i]+self.s)
        expr=num/den*k/self.s
        inv=self.inv_transf(expr)
        return inv.subs(self.t,t).evalf()

    def c2d(self):
        self.z_expr = cancel(self.s_expr)
        self.z_expr = self.s_expr.subs(self.s, 2 / self.Tc * (self.z - 1) / (self.z + 1))
        self.z_expr = self.simp_expr( self.z_expr)

    def simp_expr(self,expr):
        if expr.is_Pow:
            if expr.args[0].is_Mul:
                mul = 1
                for arg in expr.args[0].args:
                    s = self.simp_expr(arg)
                    for i in range(int(expr.args[1]) - 1):
                        s = s * s
                    mul = mul * s
                mul = cancel(mul)
                return mul
            s = self.simp_expr(expr.args[0])
            return (Pow(s, expr.args[1]))
        elif expr.is_Add:
            add = 0
            for arg in expr.args:
                add = add + self.simp_expr(arg)
            add = cancel(add)
            return add
        elif expr.is_Mul:
            mul = 1
            for arg in expr.args:
                mul = mul * self.simp_expr(arg)
            mul = cancel(mul)
            return mul
        else:
            return expr

    def zpk_s(self):
        num, den = fraction(self.s_expr)
        self.poles_s = solve(den)
        self.zeros_s = solve(num)
        coef_n = Poly(num, self.s).all_coeffs()
        coef_d = Poly(den, self.s).all_coeffs()
        K = coef_n[-1] / coef_d[-1]

    def zpk_z(self):
        num, den = fraction(self.z_expr)
        self.poles_z = solve(den)
        self.zeros_z = solve(num)
        K = num.subs(self.z, 1) / den.subs(self.z, 1)