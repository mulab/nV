// @call "C:\Program Files\Microsoft Visual Studio 10.0\Common7\Tools\vsvars32.bat"
// cl /O2 Kernel\mU.cpp /link Lib\mUKernel.lib

#ifdef _MSC_VER
#pragma comment(lib,"mUKernel")
#endif

extern "C"
// #ifdef _WIN32
#ifdef _MSC_VER
__declspec(dllimport)
#endif
int mU_main(int argc, char *argv[]);
int main(int argc,char *argv[]) {
	return mU_main(argc, argv);
}