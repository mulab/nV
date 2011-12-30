#!/usr/bin/python
import random
def nV_UP(p):
    return "{" + ",".join(str(x) for x in p) + "}"
def M_UP(p):
    return "+".join(str(p[i]) + "*x^" + str(i) for i in range(len(p)))
def rand_poly(degree,max):
    return [random.randint(-max,max) for x in range(1,degree+1)]

def print_nV_Timing(c):
    return "Print[N[Timing["+c+"]#1/1000000]]"
def print_M_Timing(c):
    return "Print[Part[Timing["+c+"],1]]"
def Timing(c):
    return "Timing["+c+"]"

def nV_Mul(p1,p2):
    return "NTL`Mul[" + p1 + "," + p2 + "]"
def nV_Add(p1,p2):
    return print_nV_Timing("NTL`Add[" + nV_UP(p1) + "," + nV_UP(p2) + "]")
def nV_Div(p1,p2):
    return "NTL`Div[" + p1 + "," + p2 + "]"

def M_Add(p1,p2):    
    return print_M_Timing( M_UP(p1)+"+"+M_UP(p2) )
def M_Mul(p1,p2):
    return "Expand[(" + p1+ ")*(" + p2 + ")]"
def M_Div(p1,p2):
    return "PolynomialQuotient["+p1+","+p2+","+"x]"

def Add_Test(n,degree,max):
    x=""
    y=""
    for m in range(n):
        p1=rand_poly(degree,max);
        p2=rand_poly(degree,max);
        x=x+nV_Add(p1,p2)+"\n"
        y=y+M_Add(p1,p2)+"\n"
    f1=open("Add.u",'w')
    f1.writelines(x)
    f2=open("Add.m",'w')
    f2.writelines(y)
def Mul_Test(n,degree,max):
    x=""
    y=""
    for m in range(n):
        p1=rand_poly(degree,max)
        p2=rand_poly(degree,max)
        x=x+"p1="+nV_UP(p1)+";\n"
        x=x+"p2="+nV_UP(p2)+";\n"
        x=x+"R="+Timing(nV_Mul("p1","p2"))+";\n"
        x=x+"Print[N[R#1/1000000]]\n"
        #x=x+"Print[R#2]\n"
        
        y=y+"p1="+M_UP(p1)+";\n"
        y=y+"p2="+M_UP(p2)+";\n"
        y=y+"R="+Timing(M_Mul("p1","p2"))+";\n"
        y=y+"Print[Part[R,1]]\n"
        #y=y+"Print[CoefficientList[Part[R, 2], x]]\n"
    f1=open("Mul.u",'w')
    f1.writelines(x)
    f2=open("Mul.m",'w')
    f2.writelines(y)
def Div_Test(n,degree,max):
    x=""
    y=""
    for m in range(n):
        pp1=rand_poly(degree,max)
        p2=rand_poly(degree,max)
        r=rand_poly(int(degree/2),max)
        x=x+"pp1="+nV_UP(pp1)+";\n"
        x=x+"p2="+nV_UP(p2)+";\n"
        x=x+"r="+nV_UP(r)+";\n"
        x=x+"p1=NTL`Add[(" + nV_Mul("pp1","p2")+ ")," + "r"+ "];\n"
        x=x+"R="+Timing(nV_Div("p1","p2"))+";\n"
        x=x+"Print[N[R#1/1000000]]\n"
        #x=x+"Print[R#2]\n"
        
        y=y+"pp1="+M_UP(pp1)+";\n"
        y=y+"p2="+M_UP(p2)+";\n"
        y=y+"r="+M_UP(r)+";\n"
        y=y+"p1="+M_Mul("pp1","p2")+"+r;\n"
        y=y+"R="+Timing(M_Div("p1","p2"))+";\n"
        y=y+"Print[Part[R,1]]\n"
        #y=y+"Print[CoefficientList[Part[R, 2], x]]\n"
    f1=open("Div",'w')
    f1.writelines(x)
    f2=open("Div.m",'w')
    f2.writelines(y)
def Factor_Test(n):
    x=""
    y=""
    for i in range(n):
        ps=[rand_poly(int(random.gauss(10,2)),int(random.gauss(100,20))) for j in range(20)]
        nV_ps=map(nV_UP,ps)
        M_ps=map(M_UP,ps)
        x=x+"p=NTL`Mul["+ ",".join(nV_ps)+"]"+ ";"+"\n" +print_nV_Timing("NTL`Factor[p]")+"\n"
        y=y+"p=Expand[("+")*(".join(M_ps)+")]"+";"+"\n" +print_M_Timing("Factor[p]")+"\n"
    open("Factor.u",'w').writelines(x)
    open("Factor.m",'w').writelines(y)

n=100;degree=1000;max=2**100;
#Add_Test(n,20000,max)
Mul_Test(n,10000,max)
#Div_Test(n,degree,max)
#Factor_Test(n)
