#include "common.h"
#include <mU/Exceptions.h>
#include <mU/Kernel.h>
#include <mU/utils.h>

#ifdef _MSC_VER
#pragma warning(push,1)
#pragma warning(disable:4996)
#endif
#ifdef _WIN32
#include <ShellAPI.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <unistd.h>
#endif

namespace mU {
wstring date() {
    time_t t;
    struct tm *lt;
    time(&t);
    lt = localtime(&t);
    wostringstream oss;
    oss << std::setfill(_W('0'))
        << std::setw(4) << lt->tm_year + 1900 << _W('-')
        << std::setw(2) << lt->tm_mon + 1 << _W('-')
        << std::setw(2) << lt->tm_mday << _W(' ')
        << std::setw(2) << lt->tm_hour << _W(':')
        << std::setw(2) << lt->tm_min << _W(':')
        << std::setw(2) << lt->tm_sec;
    return oss.str();
    /*wchar buf[1024];
    wcsftime(buf, 1024, _W("%04d-%02d-%02d %02d:%02d:%02d"), lt);
    return buf;
    */
}
#ifdef _WIN32
namespace {
#if _MSC_VER >= 1300 // for VC 7.0 
#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif
#endif

HMODULE GetCurrentModule() {
#if _MSC_VER < 1300         //   earlier   than   .NET   compiler   (VC   6.0) 
    //   Here 's   a   trick   that   will   get   you   the   handle   of   the   module
    //   you 're   running   in   without   any   a-priori   knowledge:
    //   http://www.dotnet247.com/247reference/msgs/13/65259.aspx
    MEMORY_BASIC_INFORMATION   mbi;
    static   int   dummy;
    VirtualQuery(   &dummy,   &mbi,   sizeof(mbi)   );
    return   reinterpret_cast <HMODULE> (mbi.AllocationBase);
#else         //   VC   7.0 
    //   from   ATL   7.0   sources
    return   reinterpret_cast <HMODULE> (&__ImageBase);
#endif
}
}
#endif
wstring path() {
	return mbs2wcs(mU_Home() + "/lib/mU/");
}
bool shell(wcs x) {
#ifdef _WIN32
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    // Start the child process.
    if (CreateProcessW(NULL,	// No module name (use command line)
                       const_cast<wchar*>(x),	// Command line
                       NULL,           // Process handle not inheritable
                       NULL,           // Thread handle not inheritable
                       FALSE,          // Set handle inheritance to FALSE
                       0,              // No creation flags
                       NULL,           // Use parent's environment block
                       NULL,           // Use parent's starting directory
                       &si,            // Pointer to STARTUPINFO structure
                       &pi )           // Pointer to PROCESS_INFORMATION structure
       ) {
        // Wait until child process exits.
        WaitForSingleObject( pi.hProcess, INFINITE );
        // Close process and thread handles.
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
        return true;
    }
    return false;
#else
    return system(string(x, x + wcslen(x)).c_str()) == 0;
#endif
}
wstring mbs2wcs(const char* x) {
    return to_wstring(x, strlen(x));
}
string wcs2mbs(wcs x) {
    return to_string(x, wcslen(x));
}
void _fdebugf(FILE * fp, const char * funcname, const char * filename, const int line, char *fmt, ...) {
    char buf[1024];
    time_t t;
    struct tm * now;
    va_list ap;
    time(&t);
    now = localtime(&t);
    va_start(ap, fmt);
    fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] -- %s - %s(%d): DEBUG:@\"",
            now -> tm_year + 1900, now -> tm_mon + 1, now -> tm_mday, now -> tm_hour,
            now -> tm_min, now -> tm_sec, funcname, filename, line);
    // gmp_vsprintf(buf, fmt, ap);//commented for compile error in debian
    fprintf(fp, "%s\"@\n", buf);
    va_end(ap);
}
wstring current_path() {
#ifdef _WIN32
    wchar buf[0x100];
    memset(buf, 0, 0x100);
	GetCurrentDirectory(0x100, buf);
    return buf;
#else
    char buf[0x100];
    memset(buf, 0, 0x100);
    getcwd(buf, 0x100);
    return wstring(buf, buf + strlen(buf));
#endif
}
string dump_path() {
	wstring wdump_path = current_path() + _W("\\dump");
	string dump_path = wcs2mbs(wdump_path);
	fstream f(dump_path.c_str(), ios::in);
	
	if (!f){
#ifdef _WIN32
		CreateDirectory(wdump_path.c_str(), NULL);
#else
		mkdir(dump_path.c_str(), 0);
#endif
	}
	f.close();
	return dump_path + "\\";
}
wstring to_wstring(const char *src, size_t len)
{
	using namespace std;
	using namespace boost;

	return wstring(src, src + len); // FIXME: placebo implementation

	scoped_array<wchar_t> buffer(new wchar_t[len]);
	int count;
#ifdef _WIN32
	count = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_USEGLYPHCHARS | MB_ERR_INVALID_CHARS,
			src, len, buffer.get(), len);

	if (!count)
	{
		switch (GetLastError())
		{
		case ERROR_NO_UNICODE_TRANSLATION:
			throw InvalidCodePointException();
		default:
			throw RuntimeException();	// FIXME: 确切的错误类型
		}
	}
#else
	locale def("");
	typedef codecvt<wchar_t, char, mbstate_t> codecvt_t;
	const codecvt_t &cvt = use_facet<codecvt_t>(def);
	mbstate_t state;
	mbsinit(&state);
	const char *in_next = src;
	wchar_t *out_next = buffer.get();

	codecvt_base::result r = cvt.in(state,
									src, src+len, in_next,
									buffer.get(), buffer.get()+len,
									out_next);

	switch (r)
	{
	case codecvt_base::ok:
		break;
	case codecvt_base::noconv:
		throw InvalidCodePointException();
	default:
		throw RuntimeException();	// FIXME: 确切的错误类型
	}
	count = out_next - buffer.get();
#endif
	return wstring(buffer.get(), count);
}

string to_string(const mU::wchar *src, size_t len)
{
	using namespace std;
	using namespace boost;

	return string(src, src + len); // FIXME: placebo implementation

	scoped_array<char> buffer(new char[len*8]);
	int count;
#ifdef _WIN32
	const char def_char = '?';
	BOOL used_def_char = false;
	count = WideCharToMultiByte(CP_ACP,
#ifdef __MINGW32__
			// FIXME: 应该是WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS
			//        但mingw不支持
			0
#else
			WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS
#endif
			,
			src, len, buffer.get(), len*8,
			&def_char, &used_def_char);

	if (used_def_char)
	{
		throw InvalidCodePointException();
	}

	if (!count)
	{
		switch (GetLastError())
		{
		case ERROR_NO_UNICODE_TRANSLATION:
			throw InvalidCodePointException();
		default:
			throw RuntimeException();	// FIXME: ??е????????
		}
	}
#else
	locale def("");
	typedef codecvt<wchar_t, char, mbstate_t> codecvt_t;
	const codecvt_t &cvt = use_facet<codecvt_t>(def);
	mbstate_t state;
	mbsinit(&state);
	const wchar_t *in_next = src;
	char *out_next = buffer.get();

	codecvt_base::result r = cvt.out(state,
			src, src+len, in_next,
			buffer.get(), buffer.get()+len*8,
			out_next);

	switch (r)
	{
	case codecvt_base::ok:
		break;
	case codecvt_base::noconv:
		throw InvalidCodePointException();
	default:
		throw RuntimeException();	// FIXME: ??е????????
	}
	count = out_next - buffer.get();
#endif
	return string(buffer.get(), count);
}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
