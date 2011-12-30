#include <nV/Parser.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVKernel")
#endif

namespace nV {
Parser::Parser(Kernel& k) : kernel(k), parsing(0), genning(0), loading(0) {
    Grammar::Init();
}
}
