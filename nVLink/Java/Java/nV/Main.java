package nV;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.math.BigInteger;

import nV.Kernel;
import nV.Parser;
import nV.Symbol;
import nV.Var;

public class Main {
	
	public static Kernel kernel;
	public static Parser parser;
	
	public static final Var A = new Var(Symbol.global("a"));
	public static final Var B = new Var(Symbol.global("b"));
	public static final Var C = new Var(Symbol.global("c"));
	
	public static final Var X = new Var(Symbol.global("x"));
	public static final Var Y = new Var(Symbol.global("y"));
	public static final Var Z = new Var(Symbol.global("z"));
	
	public static final Var FACTORIAL = new Var(Symbol.system("Factorial"));
	public static Var factorial(Var x) {
		Var[] expr = new Var[2];
		expr[0] = FACTORIAL;
		expr[1] = x;
		return kernel.eval(expr);
	}
	
	public static final Var N = new Var(Symbol.system("N"));
	public static Var n(Var x, Var y) {
		Var[] expr = new Var[3];
		expr[0] = N;
		expr[1] = x;
		expr[2] = y;
		return kernel.eval(expr);
	}
	
	public static final Var PLUS = new Var(Symbol.system("Plus"));
	public static Var plus(Var x, Var y) {
		Var[] expr = new Var[3];
		expr[0] = PLUS;
		expr[1] = x;
		expr[2] = y;
		return kernel.eval(expr);
	}
	
	public static final Var TIMES = new Var(Symbol.system("Times"));
	public static Var times(Var x, Var y) {
		Var[] expr = new Var[3];
		expr[0] = TIMES;
		expr[1] = x;
		expr[2] = y;
		return kernel.eval(expr);
	}
	
	public static final Var POWER = new Var(Symbol.system("Power"));
	public static Var power(Var x, Var y) {
		Var[] expr = new Var[3];
		expr[0] = POWER;
		expr[1] = x;
		expr[2] = y;
		return kernel.eval(expr);
	}
	
	public static final Var MINUS = new Var(Symbol.system("Minus"));
	public static Var minus(Var x, Var y) {
		Var[] expr = new Var[2];
		expr[0] = MINUS;
		expr[1] = x;
		return kernel.eval(expr);
	}
	
	public static final Var SIN = new Var(Symbol.system("Sin"));
	public static Var sin(Var x) {
		Var[] expr = new Var[2];
		expr[0] = SIN;
		expr[1] = x;
		return kernel.eval(expr);
	}
	
	public static final Var COS = new Var(Symbol.system("Cos"));
	public static Var cos(Var x) {
		Var[] expr = new Var[2];
		expr[0] = COS;
		expr[1] = x;
		return kernel.eval(expr);
	}
	
	public static final Var TAN = new Var(Symbol.system("Tan"));
	public static Var tan(Var x) {
		Var[] expr = new Var[2];
		expr[0] = TAN;
		expr[1] = x;
		return kernel.eval(expr);
	}
	
	public static final Var D = new Var(Symbol.system("D"));
	public static Var d(Var x, Var y) {
		Var[] expr = new Var[3];
		expr[0] = D;
		expr[1] = x;
		expr[2] = y;
		return kernel.eval(expr);
	}
	
	public static final Var INTEGRATE = new Var(Symbol.system("Integrate"));
	public static Var integrate(Var x, Var y) {
		Var[] expr = new Var[3];
		expr[0] = INTEGRATE;
		expr[1] = x;
		expr[2] = y;
		return kernel.eval(expr);
	}
	
	public static void main(String[] args){ 
		try{
			kernel = new Kernel();
			parser = new Parser(kernel);
			
			System.out.println(factorial(new Var(Var.INTEGER, "100")));
			System.out.println(d(X, X));
			System.out.println(integrate(X, X));
			System.out.println(d(integrate(X, X),X));
			Var x = new Var(new BigInteger("3"));
			Var prec = new Var(Var.INTEGER, "1000");
			Var result = n(sin(x), prec);
			System.out.println(result);
			
			BufferedReader fin;
			String line, buf = "";
			if(args.length >= 1){
				fin = new BufferedReader(new FileReader(args[0]));
				while ((line = fin.readLine()) != null)
					buf += line;
				Var expr = parser.parse(buf).toVar();
				if (args.length >= 2) {
					BufferedWriter fout = new BufferedWriter(new FileWriter(args[1]));
					fout.write(expr.toString());
					fout.close();
				} else {
					System.out.println(expr);
				}
			} else {
				fin = new BufferedReader( new InputStreamReader(System.in));
				while ((line = fin.readLine()) != null) {
					Var expr = parser.parse(line).toVar();
					System.out.println(expr);
				}
			}
			System.exit(0);			
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}
}
