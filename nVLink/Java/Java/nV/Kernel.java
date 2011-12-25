package nV;

// import utils.Constants;
// import java.lang.reflect.*;

public class Kernel {

    static {
    	// System.load(Constants.MU_JAVABIND_DLL_PATH);
    	// System.load("nvbindjava.dll");
    	System.loadLibrary("nvbindjava");
    }
    
    public long cptr;
    
    private static native long _constructor();
    public Kernel() {
        cptr = _constructor();
    }
    
    private static native void _destructor(long $cptr);
    private void destructor() {
		_destructor(cptr);
	}
    protected void finalize() {
        destructor();
    }
    
    private static native long _eval(long $cptr, long x);
    public VarPtr eval(VarPtr x) {
    	return new VarPtr(_eval(cptr, x.ptr));
    }
    public Var eval(Var x) {
    	return eval(x.toVarPtr()).toVar();
    }
    public Var eval(Var[] x) {
    	return eval(new Var(x));
    }
}
