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
void find_result(parser *p, var *r)
{
	try
	{
		(*r) = SafeEval(Optimi(p->result()));
	}
	catch (boost::thread_interrupted &)
	{
		(*r) = mU::Aborted;
	}
}
}
using namespace mU;

extern "C"
//#ifdef _WIN32
#ifdef _MSC_VER
__declspec(dllexport)
#endif
int mU_main(int argc,char *argv[]) {
//	setlocale(LC_ALL,"");
//	std::locale::global(std::locale(""));
	//wcerr.rdbuf(0);
	Initialize();
	Get(path() + _W("mU.ini"));
	//Get(_W("mU.ini"));
	if(argc > 1) {
		for(size_t i = 1; i < argc; ++i)
			Get(to_wstring(argv[i], strlen(argv[i])));
		return 0;
	}
//    wcin.imbue(std::locale(""));
//    wcout.imbue(std::locale(""));
    if (!backend_mode)
      {
	wcout<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *   *\n"<<
		std::setw(45)<<L"* *     *  * * \n"<<
		std::setw(45)<<L"*   * *        \n"<<
		std::setw(45)<<L"*              \n";
      }
	prompt();
	
	wstring buf, line;

	while(std::getline(wcin,line))
	{
		parser p;
		if(line.empty())
		{
			buf.clear();
			prompt();
			continue;
		}
		buf += line;
		wistringstream stream(buf);
		var r;
		try {
			p.start(stream);
			p.parse();
		}
		catch (UnexpectedTokenException &)
		{
			if (backend_mode)
			{
				wcerr << L"Syntax::etoken : unexpected token encountered\n" << std::flush;
			}
			else
			{
				wcerr << std::setw(p.column + 4) << L"^\n" << std::flush;
			}
			buf.clear();
			prompt();
			continue;
		}
		catch (MoreInputNeededException &)
		{

			if (backend_mode)
			{
				wcerr << L"Syntax::sntxi : Incomplete expression; more input is needed\n" << std::flush;
			}
			else
			{
				newline();
				buf += L'\n';
				continue;
			}
		}
		++InputLineCounter;
		boost::thread worker(find_result, &p, &r);
		worker.join();
		if(r && r != Null) Println(r);
		buf.clear();
		prompt();
	}

	return 0;
}
