#include "IndexedQuadArray.h"

namespace nV {
namespace Graphics {
IndexedQuadArray::IndexedQuadArray() {
    verts.clear();
    quads.clear();
    edges.clear();
}

IndexedQuadArray::~IndexedQuadArray() {
    for (unsigned int i = 0; i < verts.size(); i++) {
        delete verts[i];
    }
    verts.clear();
    for (unsigned int i = 0; i < quads.size(); i++) {
        delete quads[i];
    }
    quads.clear();
}
}
}