#pragma once

#include "IndexedQuadArray3D.h"
#include "F2P.h"

namespace nV {
namespace Graphics {
#define MAX_DERIVATIVE 20
//#define MAX_DEPTH_SPLIT 1
class ParametricSurface : IndexedQuadArray3D {

public:
	struct dev
		{
			double dev_xu,dev_xv,dev_yu,dev_yv,dev_zu,dev_zv;
		};
	struct v_xyz
		{
		    double x,y,z;
		};
	struct uv
	{
		double u,v;
	};
    double umin, umax, spu, vmin, vmax, spv;
    int nu, nv, numesh, nvmesh, wumesh, wvmesh;
    F2P *fx, *fy, *fz;
	bool fillIn(std::vector< struct dev >dev1,int i);
	bool inRange(std::vector <struct v_xyz> vert, int i );
	/*void splitSurface(std::vector <struct uv> & uv,std::vector<struct dev>& dev_vec,std::vector <struct v_xyz>&vert,
		              int a,int b,int c,int d, int depth,ArrayList<Quad*>& fillQuads);*/
	void changeRange(std::vector <struct dev>& dev_vec,std::vector <struct v_xyz>& vert,int i);
	int findInVerts( Point3d * p);
	bool inSurfaceRange(std::vector<struct v_xyz> vert,int i);
public:
    ParametricSurface(F2P *fx, F2P *fy, F2P *fz, double umin, double umax, double vmin, double vmax);
    ~ParametricSurface();

private:
    int getUMeshNum();
    int getVMeshNum();
    int getUMeshWidth();
    int getVMeshWidth();
};
}
}