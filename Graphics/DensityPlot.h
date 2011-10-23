#pragma once
#include "GraphicsMaker2D.h"
#include "F2P.h"
namespace nV {
namespace Graphics {
	#define MIN_Z_SCALE  0.005
	#define MAX_DERIVATIVE 20
#define STD_E 160
#define STD_S 240
#define MAX_DEPTH_SPLIT 2
#define MAX_DEPTH_ADD 3
	class DensityPlot:GraphicsMaker2D {
	private:
		struct two_dev
		{
			double dev_x,dev_y;
		};
		struct v_xyz
		{
		    double x,y,z;
		};
		struct Triangle{
           int p11, p12, p13;
		   bool flag;// 标志三角形是否有NaN或导数非常大的点，若有，为true，不画
           Triangle(int a, int b, int c,bool flag) : p11(a), p12(b), p13(c),flag(flag) {}

        };
		struct Quad{
           int p11, p12, p21,p22;
		  
           Quad(int a, int b, int c,int d) : p11(a), p12(b), p21(c),p22(d) {}
        };
		struct Quads{
           int p11, p12, p21,p22;
		   bool flag;
           Quads(int a, int b, int c,int d,bool flag) : p11(a), p12(b), p21(c),p22(d),flag(flag) {}
        };
		int nx,ny;
		long vert_sum;
		int polynum;
		double zmin,zmax;
		F2P *fz;
		//kernel
	    Kernel &k;
		ArrayList<Point2d*> verts;
		ArrayList<Triangle*> Tris;
		ArrayList<Quads*>quads;
		ArrayList<double >z_value;
		//std::vector<> polygons;
	public:
   
		DensityPlot(Kernel& k, const Tuple& expr);
       ~DensityPlot();
	   void constructTriangle(ArrayList<Point2d*> &verts,std::vector<struct two_dev >&poly_dev,int a,int b,int c,int d,int depth);
	    var exportGraphics();
		int getBaseNum();
		bool fillIn(std::vector<struct two_dev> &dev,int i);
		Color * getRainbowColor(double x);
		void splitPoly(std::vector<struct v_xyz>& v,std::vector<struct two_dev >& dev_z,int a,int b,int c,int d,int depth,
			           ArrayList<Quad *> & quads);
		Hue*getPolygonHue(int i);
		double Z_inPlotRange(double z);
		int findInVerts(Point2d *);
	protected:
        void setRule(var title, var rule);
	};
}
}