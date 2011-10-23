#ifdef _MSC_VER
#pragma comment(lib,"nVParser")
#endif

namespace nV {
class Kernel;
}

extern "C"
// #ifdef _WIN32
#ifdef _MSC_VER
__declspec(dllimport)
#endif
int nV_main(int argc, char *argv[], nV::Kernel*);
int main(int argc,char *argv[]) {
	return nV_main(argc, argv, 0);
}