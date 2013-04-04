#include "common.h"
#include <mU/Exceptions.h>
#include <mU/Kernel.h>
#include <mU/Parser.h>
#include <mU/Interface.h>
#include <mU/UnicodeDevice.h>
#include <mU/utils.h>

namespace mU {
bool backend_mode = getenv("MU_BACKEND") != 0;
inline void prompt() { wcout<<L"mU> "; if (backend_mode) wcout << L"\n" << std::flush; }
inline void newline() { wcout<<L"  > "; }
}
using namespace mU;

extern "C"
//#ifdef _WIN32
#ifdef _MSC_VER
__declspec(dllexport)
#endif

////////////////////////////////////////////////////////////////////////////////////////
var Det(Var A)
{
	size_t n=Size(A);
	size_t m=Size(At(A,0));
	if (n==1) return At(At(A,0),0);
	var tmp=Vec(n-1);
	for (int i=0;i<n-1;i++)
		At(tmp,i)=Vec(m-1);

	var res=Z::Zero;
	for (int i=0;i<m;i++)
	{
		for (int a=1;a<n;a++)
		for (int b=0;b<m;b++)
		if (b!=i)
			At(At(tmp,a-1),b-(b>i))=At(At(A,a),b);
		
		var m=Det(tmp);

		if (i&1) m=Eval(Ex(TAG(Times),Vec(m,Z::NOne)));
		m=Eval(Ex(TAG(Times),Vec(m,At(At(A,0),i))));
		res=Eval(Ex(TAG(Plus),Vec(res,m)));
	}
	return res;
}

//////////////////////////////////////////////////////////////////////////////////

void Next(std::vector<int> &t,int n)
{
	for (int i=t.size()-1,j=1;i>=0;i--,j++)
	if (t[i]!=n-j)
	{
		for (int k=t.size()-1;k>=i;k--)
			t[k]=t[i]+k-i+1;
		return;
	}
}

var Minors(Var A,int k=-1)
{
	size_t n=Size(A);
	size_t m=Size(At(A,0));
	//Î´ÅÐ¶Ïk>min(n,m)µÄÇé¿ö
	if (k==-1) k=std::min(n,m)-1;//Î´ÅÐ¶Ïk=0
	size_t cn=1,cm=1;
	for (int i=0;i<k;i++) cn=cn*(n-i)/(i+1);
	for (int i=0;i<k;i++) cm=cm*(m-i)/(i+1);

	var res;
	res=Vec(cn);
	for (int i=0;i<cn;i++) At(res,i)=Vec(cm);
	std::vector<int> t1(k,0),t2(k,0);
	var tmp=Vec(k);
	for (int i=0;i<k;i++) At(tmp,i)=Vec(k);

	for (int i=0;i<k;i++) t1[i]=i;
	for (int i=0;i<cn;i++)
	{
		for (int j=0;j<k;j++) t2[j]=j;
		for (int j=0;j<cm;j++)
		{
			for (int a=0;a<k;a++)
			for (int b=0;b<k;b++)
				At(At(tmp,a),b)=At(At(A,t1[a]),t2[b]);
			At(At(res,i),j)=Det(tmp);
			Next(t2,m);
		}
		Next(t1,n);
	}
	return res;
}

int main(int argc,char *argv[]) {

	Initialize();
#ifdef _WIN32
	Get(mbs2wcs(mU_Home() + "/conf/mU.ini"));
#else
	Get(mbs2wcs(mU_Home() + "/etc/mU.ini"));
#endif
	//Get(_W("mU.ini"));
	if(argc > 1) {
		for(int i = 1; i < argc; ++i)
			Get(to_wstring(argv[i], strlen(argv[i])));
		return 0;
	}
	var tmp=Rat(-1);
	Println(tmp);
	for (;;)
	{
		wstring buf;
		std::getline(wcin,buf);
		wistringstream stream(buf);
		var xxx=(Parse(stream));
		Println(Det(xxx));
		Println(Minors(xxx,2));
	}
	return 0;
}