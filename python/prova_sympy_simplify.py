#!/usr/bin/python3

import sympy as s


def recurse_replace(expr,pred,func):
    if len(expr.args) == 0:
        return expr
    else: 
        new_args = tuple(recurse_replace(a,pred,func) for a in expr.args)
        if pred(expr):
            return func(expr,new_args)
        else: 
            return type(expr)(*new_args)

def rewrite(expr,new_args):
    new_args = list(new_args)
    pow_val = new_args[1]
    pow_val_int = int(new_args[1])
    if pow_val.epsilon_eq(pow_val_int):
        new_args[1] = s.Integer(pow_val_int)
    new_node = type(expr)(*new_args)
    return new_node

def isfloatpow(expr):
    out = expr.is_Pow and expr.args[1].is_Float
    return out

def clean_exponents(expr):
    return recurse_replace(expr,isfloatpow,rewrite)

if __name__ == "__main__":
    x=s.symbols('x')
    expr = (1+x) ** 1.0
    s.pprint(expr)
    expr2 = recurse_replace(expr,isfloatpow,rewrite)
    s.pprint(expr2)