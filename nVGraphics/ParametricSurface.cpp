#include <nV/Graphics.h>
#include "ParametricSurface.h"
#include <cmath>

namespace nV {
namespace Graphics {

	bool ParametricSurface::inRange(std::vector <struct v_xyz> vert,int i)
	{
		if((!isNaN(vert[i].x))&&(!isNaN(vert[i].y))&&(!isNaN(vert[i].z)))
			return true;
		else
			return false;
	}
	bool ParametricSurface::inSurfaceRange(std::vector<struct v_xyz>vert,int i)
	{
		if(vert[i].x<=xmax&&vert[i].x>=xmin&&vert[i].y<=ymax&&vert[i].y>=ymin&&vert[i].z<=zmax&&vert[i].z>=zmin)
			return true;
		else
			return false;
	}
bool ParametricSurface::fillIn(std::vector <struct dev> dev1,int i)
{
	if((!isNaN(dev1[i].dev_xu))&&(!isNaN(dev1[i].dev_xv))&&(!isNaN(dev1[i].dev_yu))&&(!isNaN(dev1[i].dev_yv))
		&&(!isNaN(dev1[i].dev_zu))&&(!isNaN(dev1[i].dev_zv))&&dev1[i].dev_xu<=MAX_DERIVATIVE&&dev1[i].dev_xv<=MAX_DERIVATIVE
		&&dev1[i].dev_yu<=MAX_DERIVATIVE&&dev1[i].dev_yv<=MAX_DERIVATIVE&&dev1[i].dev_zu<=MAX_DERIVATIVE&&dev1[i].dev_zv<=MAX_DERIVATIVE)
		return true;
	else
		return false;
}
//fix me
ParametricSurface::ParametricSurface(F2P *ffx, F2P *ffy, F2P *ffz, double umin, double umax, double vmin, double vmax) {
    fx = ffx;
    fy = ffy;
    fz = ffz;
    this->umin = umin;
    this->umax = umax;
    this->vmin = vmin;
    this->vmax = vmax;
    spu = umax - umin;
    spv = vmax - vmin;
    numesh = getUMeshNum();
    nvmesh = getVMeshNum();
    wumesh = getUMeshWidth();
    wvmesh = getVMeshWidth();
    nu = (numesh + 1) * wumesh + 1;
    nv = (nvmesh + 1) * wvmesh + 1;
    double *u = new double[nu*nv];
    double *v = new double[nu*nv];
    double *x = new double[nu*nv];
    double *y = new double[nu*nv];
    double *z = new double[nu*nv];

	//std::vector<struct uv>uv_vec;
    
	for (int i = 0; i < nv; i++) {
        for (int j = 0; j < nu; j++) {
            u[i*nu+j] = umin + (umax - umin) * (double)j / (double)(nu - 1);
        }
    }
    for (int i = 0; i < nv; i++) {
        for (int j = 0; j < nu; j++) {
            v[i*nu+j] = vmin + (vmax - vmin) * (double)i / (double)(nv - 1);
			/*struct uv uv1={u[i],v[i]};
			uv_vec.push_back(uv1);*/
		}
    }
    fx->getArrayData(u, v, x, nu*nv);
    fy->getArrayData(u, v, y, nu*nv);
    fz->getArrayData(u, v, z, nu*nv);

	xmin=xmax=ymin=ymax=zmin=zmax=0;
	int i=0;

	for (; i < nu*nv; i++) {
		if ((!isNaN(z[i]))&&(!isNaN(x[i]))&&(!isNaN(y[i])))
		{
			zmax=z[i];
			zmin=zmax;
			xmax=x[i];
			xmin=xmax;
			ymax=y[i];
			ymin=ymax;
			break;
		}	
	}
	i++;
	for (; i < nu*nv; i++) {

		if((!isNaN(z[i]))&&(!isNaN(x[i]))&&(!isNaN(y[i])))
		{
			if (z[i] > zmax) zmax = z[i];
			if (z[i] < zmin) zmin = z[i];
			if (x[i] > xmax) xmax = x[i];
			if (x[i] < xmin) xmin = x[i];
			if (y[i] > ymax) ymax = y[i];
			if (y[i] < ymin) ymin = y[i];
		}
		else
			continue;
	}
	double graphscale_xu=(xmax-xmin)/(umax-umin);
	double graphscale_xv=(xmax-xmin)/(vmax-vmin);
	double graphscale_yu=(ymax-ymin)/(umax-umin);
	double graphscale_yv=(ymax-ymin)/(vmax-vmin);
	double graphscale_zu=(zmax-zmin)/(umax-umin);
	double graphscale_zv=(zmax-zmin)/(vmax-vmin);

	double d_u=(umax-umin)/LARGE;
	double d_v=(vmax-vmin)/LARGE;

	std::vector<struct dev> dev_vec;
	std::vector<struct v_xyz> vert_vec;
	/*double *dev_xu=new double[nu*nv];
	double *dev_yu=new double[nu*nv];
	double *dev_xv=new double[nu*nv];
	double *dev_yv=new double[nu*nv];
	double *dev_zu=new double[nu*nv];
	double *dev_zv=new double[nu*nv];*/

	for(int i=0;i<nu*nv;i++)
	{
		double dev_xu=std::abs(fx->getDerivativeX(u[i],v[i],d_u))/graphscale_xu;
		double dev_xv=std::abs(fx->getDerivativeY(u[i],v[i],d_v))/graphscale_xv;
		double dev_yu=std::abs(fy->getDerivativeX(u[i],v[i],d_u))/graphscale_yu;
		double dev_yv=std::abs(fy->getDerivativeY(u[i],v[i],d_v))/graphscale_yv;
		double dev_zu=std::abs(fz->getDerivativeX(u[i],v[i],d_u))/graphscale_zu;
		double dev_zv=std::abs(fz->getDerivativeY(u[i],v[i],d_v))/graphscale_zv;
		struct dev dev1= {dev_xu,dev_xv,dev_yu,dev_yv,dev_zu,dev_zv};
		dev_vec.push_back(dev1);
	}
	xmin=xmax=ymin=ymax=zmin=zmax=0;
    i=0;
	for(;i<nu*nv;i++)
	{
		struct v_xyz vert={x[i],y[i],z[i]};
		vert_vec.push_back(vert);
		if((!isNaN(z[i]))&&(!isNaN(x[i]))&&(!isNaN(y[i]))&&fillIn(dev_vec,i))
		{
			
			xmax = x[i];
			xmin=xmax;
			ymin=ymax=y[i];
			zmax=zmin=z[i];
			break;
		}
	}
	i++;
	for (; i < nu*nv; i++) {
		struct v_xyz vert={x[i],y[i],z[i]};
		vert_vec.push_back(vert);
		if((!isNaN(x[i]))&&(!isNaN(y[i]))&&(!isNaN(z[i]))&&fillIn(dev_vec,i))
		{
            if (x[i] > xmax) xmax = x[i];
            if (x[i] < xmin) xmin = x[i];
            if (y[i] > ymax) ymax = y[i];
            if (y[i] < ymin) ymin = y[i];
			if (z[i] > zmax) zmax = z[i];
			if (z[i] < zmin) zmin = z[i];
		}
    }
    /*xmin = x[0];
    xmax = x[0];
    ymin = y[0];
    ymax = y[0];
    zmin = y[0];
    zmax = y[0];
    for (int i = 0; i < nu*nv; i++) {
        if (x[i] > xmax) xmax = x[i];
        if (x[i] < xmin) xmin = x[i];
        if (y[i] > ymax) ymax = y[i];
        if (y[i] < ymin) ymin = y[i];
        if (z[i] > zmax) zmax = z[i];
        if (z[i] < zmin) zmin = z[i];
    }*/
	delete[] u;
    delete[] v;
    delete[] x;
    delete[] y;
    delete[] z;

	ArrayList<Quad*> fillQuads;

	for (int i = 0; i < nv - 1; i++)
		for (int j = 0; j < nu - 1; j++) 
		{
			int a=i*nu + j;
			int b=i*nu + j + 1;
			int c=(i + 1)*nu + j;
			int d=(i + 1)*nu + j + 1;
			//int depth=1;

			//splitSurface(uv_vec ,dev_vec ,vert_vec ,a , b, c, d, depth,fillQuads);

			//for(size_t i=0;i<fillQuads.size();i++)
			//{
			/*int a=fillQuads[i]->p11;
			int b=fillQuads[i]->p12;
			int c=fillQuads[i]->p21;
			int d=fillQuads[i]->p22;*/
			if(inRange(vert_vec,a)&&inRange(vert_vec,b)&&inRange(vert_vec,c)&&inRange(vert_vec,d)
				&&fillIn(dev_vec,a)&&fillIn(dev_vec,b)&&fillIn(dev_vec,c)&&fillIn(dev_vec,d)&&
				inSurfaceRange(vert_vec,a)&&inSurfaceRange(vert_vec,b)&&
				inSurfaceRange(vert_vec,c)&&inSurfaceRange(vert_vec,d))
			{
				Point3d * p=new Point3d(vert_vec[a].x,vert_vec[a].y,vert_vec[a].z);
				int a1=findInVerts(p);

				if(a1==-1)
				{
					verts.add(p);
					a1=verts.size()-1;
				}
				//a=a1;
				p=new Point3d(vert_vec[b].x,vert_vec[b].y,vert_vec[b].z);
				int b1=findInVerts(p);
				if(b1==-1)
				{
					verts.add(p);
					b1=verts.size()-1;
				}
				//b=b1;
				p=new Point3d(vert_vec[c].x,vert_vec[c].y,vert_vec[c].z);
				int c1=findInVerts(p);
				if(c1==-1)
				{
					verts.add(p);
					c1=verts.size()-1;
				}
				//c=c1;
				p=new Point3d(vert_vec[d].x,vert_vec[d].y,vert_vec[d].z);
				int d1=findInVerts(p);

				if(d1==-1)
				{
					verts.add(p);
					d1=verts.size()-1;
				}

				//if(inSurfaceRange(vert_vec,a)&&inSurfaceRange(vert_vec,b)&&
				//	inSurfaceRange(vert_vec,c)&&inSurfaceRange(vert_vec,d))
				//{
					quads.add(new Quad(a1,b1,c1,d1));
				//}
			}
		}
			//quads.add(new Quad(i*nu + j, i*nu + j + 1, (i + 1)*nu + j, (i + 1)*nu + j + 1));
		//}
  
		for (int j = 0; j < numesh; j++) {
			Mesh *mesh = new Mesh;
			mesh->p.clear();
			for (int ni = 0; ni < nv; ni++)
			{
				int a=ni*nu+j*wumesh;
				//cout<<ni <<"  "<<a<<" ";
				if(inRange(vert_vec,a)&&fillIn(dev_vec,a)&&vert_vec[a].x<=xmax&&vert_vec[a].x>=xmin&&
					vert_vec[a].y<=ymax&&vert_vec[a].y>=ymin&&vert_vec[a].z<=zmax&&vert_vec[a].z>=zmin)
				{
					Point3d * p1=new Point3d(vert_vec[a].x,vert_vec[a].y,vert_vec[a].z);
					int a1= findInVerts(p1);
					delete p1;
					//cout<<a1<<" ";
					if(a1!=-1)
					{
				    	mesh->p.add(a1);
					}
					else
					{
						if(mesh->p.size()!=0)
						{
							if(mesh->p.size()>=2)
							{
						    	meshs.add(mesh);
							}
							mesh = new Mesh;
							mesh->p.clear();
							//cout<<mesh->p.size()<<"  ";
						}
					}
				}
				else
				{
					if(mesh->p.size()!=0)
					{
						if(mesh->p.size()>=2)
						{
							meshs.add(mesh);
						}
						mesh = new Mesh;
						mesh->p.clear();
						//cout<<mesh->p.size()<<"  ";
					}
				}

			}
			if(mesh->p.size()>=2)
			{
		    	meshs.add(mesh);
			}
		}

    for (int ni = 0; ni < nvmesh; ni++) {
        Mesh *mesh = new Mesh;
        mesh->p.clear();
        for (int j = 0; j < nu; j++)
		{
			int a=ni*wvmesh*nu+j;
			//cout<<ni <<"  "<<a<<" ";
			if(inRange(vert_vec,a)&&fillIn(dev_vec,a)&&vert_vec[a].x<=xmax&&vert_vec[a].x>=xmin&&
				vert_vec[a].y<=ymax&&vert_vec[a].y>=ymin&&vert_vec[a].z<=zmax&&vert_vec[a].z>=zmin)
			{
				Point3d * p1=new Point3d(vert_vec[a].x,vert_vec[a].y,vert_vec[a].z);
				int a1= findInVerts(p1);
				delete p1;
				if(a1!=-1)
				{
					mesh->p.add(a1);
				}
				else
				{
					if(mesh->p.size()!=0)
					{
						if(mesh->p.size()>=2)
						{
							meshs.add(mesh);
						}
						mesh = new Mesh;
						mesh->p.clear();
						//cout<<mesh->p.size()<<"  ";
					}
					
				}
			}
			else
			{
				if(mesh->p.size()!=0)
				{
					if(mesh->p.size()>=2)
					{
						meshs.add(mesh);
					}
					mesh = new Mesh;
					mesh->p.clear();
					//cout<<mesh->p.size()<<"  ";
				}
			}
		}
        if(mesh->p.size()>=2)
		{
		    meshs.add(mesh);
		}
    }
   /* delete[] u;
    delete[] v;
    delete[] x;
    delete[] y;
    delete[] z;*/
}
int ParametricSurface::findInVerts(Point3d* p)
{
	size_t i=0;
	for(;i<verts.size();i++)
	{
		if(verts[i]->x==p->x&&verts[i]->y==p->y&&verts[i]->z==p->z)
		{
			return i;
		}
	}
	return -1;
}
void ParametricSurface::changeRange(std::vector<struct dev>&dev_vec,std::vector<struct v_xyz> &vert,int i)
{
	if(inRange(vert,i))
	{
		if(fillIn(dev_vec,i))
		{
			if(vert[i].x>xmax)
				xmax=vert[i].x;
			if(vert[i].x<xmin)
				xmin=vert[i].x;
			if(vert[i].y>ymax)
				ymax=vert[i].y;
			if(vert[i].y<ymin)
				ymin=vert[i].y;
			if(vert[i].z>zmax)
				zmax=vert[i].z;
			if(vert[i].z<zmin)
				zmin=vert[i].z;
		}
	}
}
/*void ParametricSurface::splitSurface(std::vector <struct uv> & uv_vec,std::vector<struct dev>& dev_vec,std::vector <struct v_xyz>&vert_vec,
		              int a,int b,int c,int d, int depth,ArrayList<Quad*>& fillQuads)
{
	if(depth<=MAX_DEPTH_SPLIT)
	{
		int * num=new int[4];
		num[0]=a;
		num[1]=b;
		num[2]=c;
		num[3]=d;
		int node_sum=0;
		for(int i=0;i<4;i++)
		{
			if(inRange(vert_vec,a)&&inRange(vert_vec,b)&&inRange(vert_vec,c)&&inRange(vert_vec,d)
				&&fillIn(dev_vec,a)&&fillIn(dev_vec,b)&&fillIn(dev_vec,c)&&fillIn(dev_vec,d))
				continue;
			else
				node_sum++;
		}
		if(node_sum==0)
		{
			fillQuads.push_back(new Quad(a,b,c,d));
			return ;
		}
		else 
		{
			double u_middle=(uv_vec[a].u+uv_vec[c].u)/2;
			double v_middle=(uv_vec[a].v+uv_vec[c].v)/2;
			struct uv uv1={u_middle,v_middle};
			uv_vec.push_back(uv1);

			double x_middle=fx->getSingleData(u_middle,v_middle);
			double y_middle=fy->getSingleData(u_middle,v_middle);
			double z_middle=fz->getSingleData(u_middle,v_middle);
			struct v_xyz v={x_middle,y_middle,z_middle};
			vert_vec.push_back(v);
			changeRange(dev_vec,vert_vec,vert_vec.size()-1);

			double x_ab_middle=fx->getSingleData(u_middle,uv_vec[a].v);
			double y_ab_middle=fy->getSingleData(u_middle,uv_vec[a].v);
			double z_ab_middle=fz->getSingleData(u_middle,uv_vec[a].v);
			v.x=x_ab_middle;
			v.y=y_ab_middle;
			v.z=z_ab_middle;
			vert_vec.push_back(v);

			uv1.u=u_middle;
			uv1.v=uv_vec[a].v;
			uv_vec.push_back(uv1);
			changeRange(dev_vec,vert_vec,vert_vec.size()-1);

			double x_bc_middle=fx->getSingleData(uv_vec[b].u,v_middle);
			double y_bc_middle=fy->getSingleData(uv_vec[b].u,v_middle);
			double z_bc_middle=fz->getSingleData(uv_vec[b].u,v_middle);
			v.x=x_bc_middle;
			v.y=y_bc_middle;
			v.z=z_bc_middle;
			vert_vec.push_back(v);

			uv1.u=uv_vec[b].u;
			uv1.v=v_middle;
			uv_vec.push_back(uv1);
			changeRange(dev_vec,vert_vec,vert_vec.size()-1);

			double x_cd_middle=fx->getSingleData(u_middle,uv_vec[d].v);
			double y_cd_middle=fy->getSingleData(u_middle,uv_vec[d].v);
			double z_cd_middle=fz->getSingleData(u_middle,uv_vec[d].v);
			v.x=x_cd_middle;
			v.y=y_cd_middle;
			v.z=z_cd_middle;
			vert_vec.push_back(v);

			uv1.u=u_middle;
			uv1.v=uv_vec[d].v;
			uv_vec.push_back(uv1);
			changeRange(dev_vec,vert_vec,vert_vec.size()-1);

			double x_da_middle=fx->getSingleData(uv_vec[a].u,v_middle);
			double y_da_middle=fy->getSingleData(uv_vec[a].u,v_middle);
			double z_da_middle=fz->getSingleData(uv_vec[a].u,v_middle);
			v.x=x_da_middle;
			v.y=y_da_middle;
			v.z=z_da_middle;
			vert_vec.push_back(v);

			uv1.u=uv_vec[a].u;
			uv1.v=v_middle;
			uv_vec.push_back(uv1);
			changeRange(dev_vec,vert_vec,vert_vec.size()-1);

			double graphscale_xu=(xmax-xmin)/(umax-umin);
			double graphscale_xv=(xmax-xmin)/(vmax-vmin);
			double graphscale_yu=(ymax-ymin)/(umax-umin);
			double graphscale_yv=(ymax-ymin)/(vmax-vmin);
			double graphscale_zu=(zmax-zmin)/(umax-umin);
			double graphscale_zv=(zmax-zmin)/(vmax-vmin);

			double d_u=(umax-umin)/LARGE;
			double d_v=(vmax-vmin)/LARGE;

			struct dev dev1={std::abs(fx->getDerivativeX(u_middle,v_middle,d_u))/graphscale_xu,
				std::abs(fx->getDerivativeY(u_middle,v_middle,d_v))/graphscale_xv,
				std::abs(fy->getDerivativeX(u_middle,v_middle,d_u))/graphscale_yu,
				std::abs(fy->getDerivativeY(u_middle,v_middle,d_v))/graphscale_yv,
				std::abs(fz->getDerivativeX(u_middle,v_middle,d_u))/graphscale_zu,
				std::abs(fz->getDerivativeY(u_middle,v_middle,d_v))/graphscale_zv};
			dev_vec.push_back(dev1);

			dev1.dev_xu=std::abs(fx->getDerivativeX(u_middle,uv_vec[a].v,d_u))/graphscale_xu;
			dev1.dev_xv=std::abs(fx->getDerivativeY(u_middle,uv_vec[a].v,d_v))/graphscale_xv;
			dev1.dev_yu=std::abs(fy->getDerivativeX(u_middle,uv_vec[a].v,d_u))/graphscale_yu;
			dev1.dev_yv=std::abs(fy->getDerivativeY(u_middle,uv_vec[a].v,d_v))/graphscale_yv;
			dev1.dev_zu=std::abs(fz->getDerivativeX(u_middle,uv_vec[a].v,d_u))/graphscale_zu;
			dev1.dev_zv=std::abs(fz->getDerivativeY(u_middle,uv_vec[a].v,d_v))/graphscale_zv;
			dev_vec.push_back(dev1);

			dev1.dev_xu=std::abs(fx->getDerivativeX(uv_vec[b].u,v_middle,d_u))/graphscale_xu;
			dev1.dev_xv=std::abs(fx->getDerivativeY(uv_vec[b].u,v_middle,d_v))/graphscale_xv;
			dev1.dev_yu=std::abs(fy->getDerivativeX(uv_vec[b].u,v_middle,d_u))/graphscale_yu;
			dev1.dev_yv=std::abs(fy->getDerivativeY(uv_vec[b].u,v_middle,d_v))/graphscale_yv;
			dev1.dev_zu=std::abs(fz->getDerivativeX(uv_vec[b].u,v_middle,d_u))/graphscale_zu;
			dev1.dev_zv=std::abs(fz->getDerivativeY(uv_vec[b].u,v_middle,d_v))/graphscale_zv;
			dev_vec.push_back(dev1);

			dev1.dev_xu=std::abs(fx->getDerivativeX(u_middle,uv_vec[d].v,d_u))/graphscale_xu;
			dev1.dev_xv=std::abs(fx->getDerivativeY(u_middle,uv_vec[d].v,d_v))/graphscale_xv;
			dev1.dev_yu=std::abs(fy->getDerivativeX(u_middle,uv_vec[d].v,d_u))/graphscale_yu;
			dev1.dev_yv=std::abs(fy->getDerivativeY(u_middle,uv_vec[d].v,d_v))/graphscale_yv;
			dev1.dev_zu=std::abs(fz->getDerivativeX(u_middle,uv_vec[d].v,d_u))/graphscale_zu;
			dev1.dev_zv=std::abs(fz->getDerivativeY(u_middle,uv_vec[d].v,d_v))/graphscale_zv;
			dev_vec.push_back(dev1);

			dev1.dev_xu=std::abs(fx->getDerivativeX(uv_vec[a].u,v_middle,d_u))/graphscale_xu;
			dev1.dev_xv=std::abs(fx->getDerivativeY(uv_vec[a].u,v_middle,d_v))/graphscale_xv;
			dev1.dev_yu=std::abs(fy->getDerivativeX(uv_vec[a].u,v_middle,d_u))/graphscale_yu;
			dev1.dev_yv=std::abs(fy->getDerivativeY(uv_vec[a].u,v_middle,d_v))/graphscale_yv;
			dev1.dev_zu=std::abs(fz->getDerivativeX(uv_vec[a].u,v_middle,d_u))/graphscale_zu;
			dev1.dev_zv=std::abs(fz->getDerivativeY(uv_vec[a].u,v_middle,d_v))/graphscale_zv;
			dev_vec.push_back(dev1);

			splitSurface(uv_vec,dev_vec,vert_vec,a,vert_vec.size()-4,vert_vec.size()-5,vert_vec.size()-1,depth+1,fillQuads);
			splitSurface(uv_vec,dev_vec,vert_vec,vert_vec.size()-4,b,vert_vec.size()-3,vert_vec.size()-5,depth+1,fillQuads);
			splitSurface(uv_vec,dev_vec,vert_vec,vert_vec.size()-5,vert_vec.size()-3,c,vert_vec.size()-2,depth+1,fillQuads);
			splitSurface(uv_vec,dev_vec,vert_vec,vert_vec.size()-1,vert_vec.size()-5,vert_vec.size()-2,d,depth+1,fillQuads);
		}
	}
	else
	{
		if(inRange(vert_vec,a)&&inRange(vert_vec,b)&&inRange(vert_vec,c)&&inRange(vert_vec,d)
				&&fillIn(dev_vec,a)&&fillIn(dev_vec,b)&&fillIn(dev_vec,c)&&fillIn(dev_vec,d))
	       fillQuads.push_back(new Quad(a,b,c,d));
		return ;
	}
}*/
ParametricSurface::~ParametricSurface() {
}

int ParametricSurface::getUMeshNum() {
    return 10;
}

int ParametricSurface::getVMeshNum() {
    return 10;
}

int ParametricSurface::getUMeshWidth() {
    return 3;
}

int ParametricSurface::getVMeshWidth() {
    return 3;
}
}
}
