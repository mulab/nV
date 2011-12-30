#include <nV/Graphics.h>
#include "ParametricRegion.h"

namespace nV {
namespace Graphics {

//fix me
	bool fillIn(double *dev_xu,double *dev_xv,double *dev_yu,double *dev_yv,int i)
	{
		return ((!isNaN(dev_xu[i]))&&dev_xu[i]<=MAX_DERIVATIVE
			&&(!isNaN(dev_yu[i]))&&dev_yu[i]<=MAX_DERIVATIVE&&
			(!isNaN(dev_xv[i]))&&dev_xv[i]<=MAX_DERIVATIVE&&
			(!isNaN(dev_yv[i]))&&dev_yv[i]<=MAX_DERIVATIVE);
	}
ParametricRegion::ParametricRegion(F2P *ffx, F2P *ffy, double umin, double umax, double vmin, double vmax) {
    fx = ffx;
    fy = ffy;
    this->umin = umin;
    this->umax = umax;
    this->vmin = vmin;
    this->vmax = vmax;
    spu = umax - umin;
    spv = vmax - vmin;
    nu = getBaseUNum();
    nv = getBaseVNum();
    double *u = new double[nu*nv];
    double *v = new double[nu*nv];
    double *x = new double[nu*nv];
    double *y = new double[nu*nv];
    for (int i = 0; i < nv; i++) {
        for (int j = 0; j < nu; j++) {
            u[i*nu+j] = umin + (umax - umin) * (double)j / (double)(nu - 1);
        }
    }
    for (int i = 0; i < nv; i++) {
        for (int j = 0; j < nu; j++) {
            v[i*nu+j] = vmin + (vmax - vmin) * (double)i / (double)(nv - 1);
        }
    }
    fx->getArrayData(u, v, x, nu*nv);
    fy->getArrayData(u, v, y, nu*nv);
	xmin=xmax=ymin=ymax=0L;
    int i=0;
	for (; i < nu*nv; i++) {
        if ((!isNaN(x[i]))&&(!isNaN(y[i])))
        {
			xmax = x[i];
			xmin=xmax;
			ymin=ymax=y[i];
			break;
		}	
	}
    for (; i < nu*nv; i++) {
		if((!isNaN(x[i]))&&(!isNaN(y[i])))
		{
            if (x[i] > xmax) xmax = x[i];
            if (x[i] < xmin) xmin = x[i];
            if (y[i] > ymax) ymax = y[i];
            if (y[i] < ymin) ymin = y[i];
		}
    }
	double graphscale_xu=(xmax-xmin)/(umax-umin);
	double graphscale_xv=(xmax-xmin)/(vmax-vmin);
	double graphscale_yu=(ymax-ymin)/(umax-umin);
	double graphscale_yv=(ymax-ymin)/(vmax-vmin);
	double d_u=(umax-umin)/LARGE;
	double d_v=(vmax-vmin)/LARGE;
	double *dev_xu=new double[nu*nv];
	double *dev_yu=new double[nu*nv];
	double *dev_xv=new double[nu*nv];
	double *dev_yv=new double[nu*nv];
	for(int i=0;i<nu*nv;i++)
	{
		dev_xu[i]=std::abs(fx->getDerivativeX(u[i],v[i],d_u))/graphscale_xu;
		dev_xv[i]=std::abs(fx->getDerivativeY(u[i],v[i],d_v))/graphscale_xv;
		dev_yu[i]=std::abs(fy->getDerivativeX(u[i],v[i],d_u))/graphscale_yu;
		dev_yv[i]=std::abs(fy->getDerivativeY(u[i],v[i],d_v))/graphscale_yv;
	}
	xmin=xmax=ymin=ymax=0L;
    i=0;
	for(;i<nu*nv;i++)
	{
		if((!isNaN(x[i]))&&(!isNaN(y[i]))&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,i))
		{
			xmax = x[i];
			xmin=xmax;
			ymin=ymax=y[i];
			break;
		}
	}
	for (; i < nu*nv; i++) {
		if((!isNaN(x[i]))&&(!isNaN(y[i]))&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,i))
		{
            if (x[i] > xmax) xmax = x[i];
            if (x[i] < xmin) xmin = x[i];
            if (y[i] > ymax) ymax = y[i];
            if (y[i] < ymin) ymin = y[i];
		}
    }
    for (int i = 0; i < nu*nv; i++) {
		if((!isNaN(x[i]))&&(!isNaN(y[i]))&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,i))
		{
           verts.add(new Point2d(x[i], y[i]));
		}
		else
		{
			verts.add(new Point2d(0,0));
		}
    }
    for (int i = 0; i < nv - 1; i++)
        for (int j = 0; j < nu - 1; j++) {
			int a=i*nu + j;
			int b=i*nu+j+1;
			int c=(i+1)*nu+j;
			int d=(i+1)*nu+j+1;
			if((!isNaN(x[a])&&!isNaN(y[a]))&&(!isNaN(x[b])&&!isNaN(y[b]))
				&&(!isNaN(x[c])&&!isNaN(y[c]))&&(!isNaN(x[d])&&!isNaN(y[d])
				&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,a)&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,b)
				&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,c)&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,d))
				)
			{
				quads.add(new Quad(a, b, c, d));
			}
        }
	ArrayList<int > edge;
    for (int j = 0; j < nu; j++)
	{
		int a=0*nu+j;
		if(!isNaN(x[a])&&!isNaN(y[a])&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,a))
		{
            edge.add(a);
		}
		else
		{
			if(edge.size()!=0)
			{
				edges.add(edge);
				edge.clear();
			}
		}
	}
    for (int i = 0; i < nv; i++)
	{
		int a=i*nu+nv-1;
		if(!isNaN(x[a])&&!isNaN(y[a])&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,a))
		{
            edge.add(a);
		}
		else
		{
			if(edge.size()!=0)
			{
				edges.add(edge);
				edge.clear();
			}
		}
	}
    for (int j = nu - 1; j >= 0; j--)
	{
		int a=(nu - 1)*nv + j;
		if(!isNaN(x[a])&&!isNaN(y[a])&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,a))
		{
            edge.add(a);
		}
		else
		{
			if(edge.size()!=0)
			{
				edges.add(edge);
				edge.clear();
			}
		}
	}
    for (int i = nv - 1; i >= 0; i--)
    {
		int a=i*nu + 0;
		if(!isNaN(x[a])&&!isNaN(y[a])&&fillIn(dev_xu,dev_xv,dev_yu,dev_yv,a))
		{
            edge.add(a);
		}
		 else
		{
			if(edge.size()!=0)
			{
				edges.add(edge);
				edge.clear();
			}
		}
	}
	edges.add(edge);
    delete[] u;
    delete[] v;
    delete[] x;
    delete[] y;
}

ParametricRegion::~ParametricRegion() {
}

int ParametricRegion::getBaseUNum() {
	
	return 30;
}

int ParametricRegion::getBaseVNum() {
   
	return 30;
}
}
}