#include <nV/Number.h>
#include "DensityPlot.h"
#include <cmath>

namespace nV {
namespace Graphics {
	DensityPlot::DensityPlot(Kernel& k, const Tuple& cmd) : k(k) {
		
		var expr = cmd[1];
        var xrange = cmd[2];
        var yrange = cmd[3];

        if (cmd[cmd.size - 1].isTuple(SYS(Optional))) {
           var opts = cmd[cmd.size - 1];
           readOptions(opts.tuple());
        }
		var xparam = xrange.tuple()[1];
		//xparam.print();
        xmin =toD(N(k, k.eval(xrange.tuple()[2])).object());
        xmax =toD(N(k, k.eval(xrange.tuple()[3])).object());

        var yparam = yrange.tuple()[1];
		//yparam.print();
        ymin =toD(N(k, k.eval(yrange.tuple()[2])).object());
        ymax =toD(N(k, k.eval(yrange.tuple()[3])).object());

		polynum = 1;
        var funs = expr;

        fz = new F2P(k, expr, xparam,yparam);
		
		vert_sum=0;

		nx=getBaseNum();
		ny=getBaseNum();

		double *x=new double [nx*ny];
		double *y=new double [nx*ny];
		double *z=new double [nx*ny];
        
		for (int i = 0; i < nx; i++) {
           for (int j = 0; j < ny; j++) {
               x[i*ny+j] = xmin + (xmax - xmin) * (double)j / (double)(ny - 1);
           }
        }

        for (int i = 0; i < nx; i++) {
           for (int j = 0; j < ny; j++) {
               y[i*ny+j] = ymin + (ymax - ymin) * (double)i / (double)(nx - 1);

           }
        }

		fz->getArrayData(x, y, z, nx*ny);
		std::vector<struct v_xyz  >vec_v;
	   
		zmin=zmax=0;
        int i=0;
		
	    for (; i < nx*ny; i++) {
			
			
			struct v_xyz vec_n_vert={x[i],y[i],z[i]};
		    vec_v.push_back(vec_n_vert);
           if (!isNaN(z[i]))
           {
			  
			   zmax = z[i];
			   zmin=zmax;
			   break;
		   }	
		   
	   }
		i++;
       for (; i < nx*ny; i++) {
		   
		   struct v_xyz vec_n_vert={x[i],y[i],z[i]};
		       vec_v.push_back(vec_n_vert);
		   if(!isNaN(z[i]))
		   {
			   
               if (z[i] > zmax) zmax = z[i];
               if (z[i] < zmin) zmin = z[i];
		    }
		   else
			   continue;
       }
	   delete []x;
	   delete []y;
	   delete []z;
	   double graphscale_zx=(zmax-zmin)/(xmax-xmin);
	   double graphscale_zy=(zmax-zmin)/(ymax-ymin);
	   double d_x=(xmax-xmin)/LARGE;
	   double d_y=(ymax-ymin)/LARGE;
	   
	   std::vector<struct two_dev >dev_z;
	   for(int i=0;i<nx*ny;i++)
	   {
		   double dev1=std::abs(fz->getDerivativeX(vec_v[i].x,vec_v[i].y,d_x))/graphscale_zx;
		   double dev2=std::abs(fz->getDerivativeY(vec_v[i].x,vec_v[i].y,d_y))/graphscale_zy;
		   struct two_dev dev={dev1,dev2};
		   dev_z.push_back(dev);
	   }
	   zmin=zmax=0;
	   i=0;
	   for(;i<nx*ny;i++)
	   {
	    	if((!isNaN(vec_v[i].z))&&fillIn(dev_z,i))
	    	{
				zmax = vec_v[i].z;
		    	zmin=zmax;
		    	break;
		    }
	   }
	   i++;
	   for (; i < nx*ny; i++) {
	    	if((!isNaN(vec_v[i].z))&&fillIn(dev_z,i))
	    	{
               if (vec_v[i].z > zmax) zmax = vec_v[i].z;
               if (vec_v[i].z < zmin) zmin = vec_v[i].z;
		    }
       }
	   if(zmin==0&&zmax==0)
	   {
		   xmin=0;
		   xmax=0;
		   ymin=0;
		   ymax=0;
	   }//z值均无效，图不能画出
	   std::vector<struct two_dev >poly_dev;
	   for(int i=0;i<nx-1;i++)
		{
			for(int j=0;j<ny-1;j++)
			{
				
				int a=i*ny + j;
				int b=i*ny+j+1;
				int c=(i+1)*ny+j+1;
				int d=(i+1)*ny+j;
				int * num=new int[4];
				num[0]=a;
				num[1]=b;
				num[2]=c;
				num[3]=d;
			
				ArrayList<Quad *>fillQuads;
				int V_depth=1;
			
					splitPoly(vec_v,dev_z,a,b,c,d,V_depth,fillQuads);
				
					for(size_t i=0;i<fillQuads.size();i++)
					{
						int a=fillQuads[i]->p11;
						int b=fillQuads[i]->p12;
						int c=fillQuads[i]->p21;
						int d=fillQuads[i]->p22;
						if((isNaN(vec_v[a].z))&&(isNaN(vec_v[b].z))&&
							(isNaN(vec_v[c].z))&&(isNaN(vec_v[d].z)))
							continue;
						else
						{

							Point2d * p=new Point2d(vec_v[a].x,vec_v[a].y);
							int a1=findInVerts(p);
							if(a1==-1)
							{
								verts.add(p);
								vec_v[a].z=Z_inPlotRange(vec_v[a].z);
								z_value.add(vec_v[a].z);
								poly_dev.push_back(dev_z[a]);
								a1=verts.size()-1;
							}
							//a=a1;
							p=new Point2d(vec_v[b].x,vec_v[b].y);
							int b1=findInVerts(p);
							if(b1==-1)
							{
								verts.add(p);
								vec_v[b].z=Z_inPlotRange(vec_v[b].z);
								z_value.add(vec_v[b].z);
								poly_dev.push_back(dev_z[b]);
								b1=verts.size()-1;
							}
							//b=b1;
							p=new Point2d(vec_v[c].x,vec_v[c].y);
							int c1=findInVerts(p);
							if(c1==-1)
							{
								verts.add(p);
								vec_v[c].z=Z_inPlotRange(vec_v[c].z);
								z_value.add(vec_v[c].z);
								poly_dev.push_back(dev_z[c]);
								c1=verts.size()-1;
							}
							//c=c1;
							p=new Point2d(vec_v[d].x,vec_v[d].y);
							int d1=findInVerts(p);
							if(d1==-1)
							{
								verts.add(p);
								vec_v[d].z=Z_inPlotRange(vec_v[d].z);
								z_value.add(vec_v[d].z);
								poly_dev.push_back(dev_z[d]);
								d1=verts.size()-1;
							}

							constructTriangle(verts,poly_dev,a1,b1,c1,d1,1);
						}
					}
						
				
				
			}
		}

}
int DensityPlot::findInVerts( Point2d * p)
{
	size_t i=0;
	for(;i<verts.size();i++)
	{
		if(verts[i]->x==p->x&&verts[i]->y==p->y)
		{
			return i;
		}
	}
	return -1;
}
double DensityPlot::Z_inPlotRange(double z)
{
	if(!isNaN(z))
	{
		if (z>=zmax)
			return zmax;
		else if(z<=zmin)
			return zmin;
		else
			return z;
	}else
	{
	    return z;
	}
}
void DensityPlot::splitPoly(std::vector<struct v_xyz >& vec_v,std::vector<struct two_dev >& dev_z,int a,int b,int c,
	                        int d,int depth, ArrayList<Quad *> & fill_quads)
{
	int * num=new int[4];
	num[0]=a;
	num[1]=b;
	num[2]=c;
	num[3]=d;
	int node_sum=0;
	for(int i=0;i<4;i++)
	{
		if((!isNaN(vec_v[num[i]].z))/*&&fillIn(dev_z,num[i])*/)
			continue;
		else
			node_sum++;
	}
	if(node_sum==0||depth>=MAX_DEPTH_SPLIT)
	{
		fill_quads.push_back(new Quad(a,b,c,d));
		return ;
	}
	
	else 
	{
		double x_middle=(vec_v[a].x+vec_v[c].x)/2;
		double y_middle=(vec_v[a].y+vec_v[c].y)/2;
		double z_middle=fz->getSingleData(x_middle,y_middle);
		z_middle=Z_inPlotRange(z_middle);
		struct v_xyz v={x_middle,y_middle,z_middle};
		vec_v.push_back(v);
		double z_ab_middle=fz->getSingleData(x_middle,vec_v[a].y);
		z_ab_middle=Z_inPlotRange(z_ab_middle);
		v.x=x_middle;
	    v.y=vec_v[a].y;
		v.z=z_ab_middle;
		vec_v.push_back(v);
		double z_bc_middle=fz->getSingleData(vec_v[b].x,y_middle);
		z_bc_middle=Z_inPlotRange(z_bc_middle);
		v.x=vec_v[b].x;
		v.y=y_middle;
		v.z=z_bc_middle;
		vec_v.push_back(v);
		double z_cd_middle=fz->getSingleData(x_middle,vec_v[d].y);
	    z_cd_middle=Z_inPlotRange(z_cd_middle);
		v.x=x_middle;
		v.y=vec_v[d].y;
		v.z=z_cd_middle;
		vec_v.push_back(v);
		double z_da_middle=fz->getSingleData(vec_v[a].x,y_middle);
		z_da_middle=Z_inPlotRange(z_da_middle);
		v.x=vec_v[a].x;
		v.y=y_middle;
		v.z=z_da_middle;
		vec_v.push_back(v);

	   double graphscale_zx=(zmax-zmin)/(xmax-xmin);
	   double graphscale_zy=(zmax-zmin)/(ymax-ymin);
	   double d_x=(xmax-xmin)/LARGE;
	   double d_y=(ymax-ymin)/LARGE;
	   struct two_dev dev={std::abs(fz->getDerivativeX(x_middle,y_middle,d_x))/graphscale_zx,
	                       std::abs(fz->getDerivativeY(x_middle,y_middle,d_y))/graphscale_zy};
	   dev_z.push_back(dev);
	   dev.dev_x=std::abs(fz->getDerivativeX(x_middle,vec_v[a].y,d_x))/graphscale_zx;
	   dev.dev_y=std::abs(fz->getDerivativeY(x_middle,vec_v[a].y,d_y))/graphscale_zy;
	   dev_z.push_back(dev);
	   dev.dev_x=std::abs(fz->getDerivativeX(vec_v[b].x,y_middle,d_x))/graphscale_zx;
	   dev.dev_y=std::abs(fz->getDerivativeY(vec_v[b].x,y_middle,d_y))/graphscale_zy;
	   dev_z.push_back(dev);
	   dev.dev_x=std::abs(fz->getDerivativeX(x_middle,vec_v[d].y,d_x))/graphscale_zx;
	   dev.dev_y=std::abs(fz->getDerivativeY(x_middle,vec_v[d].y,d_y))/graphscale_zy;
	   dev_z.push_back(dev);
	   dev.dev_x=std::abs(fz->getDerivativeX(vec_v[a].x,y_middle,d_x))/graphscale_zx;
	   dev.dev_y=std::abs(fz->getDerivativeY(vec_v[a].x,y_middle,d_y))/graphscale_zy;
	   dev_z.push_back(dev);

	   splitPoly(vec_v,dev_z,a,vec_v.size()-4,vec_v.size()-5,vec_v.size()-1,depth+1,fill_quads);
	   splitPoly(vec_v,dev_z,vec_v.size()-4,b,vec_v.size()-3,vec_v.size()-5,depth+1,fill_quads);
	   splitPoly(vec_v,dev_z,vec_v.size()-5,vec_v.size()-3,c,vec_v.size()-2,depth+1,fill_quads);
	   splitPoly(vec_v,dev_z,vec_v.size()-1,vec_v.size()-5,vec_v.size()-2,d,depth+1,fill_quads);
	}
}
void DensityPlot::constructTriangle(ArrayList<Point2d*> &verts,std::vector<struct two_dev >& poly_dev,int a,int b,int c,int d,int depth)
	{
		int * num=new int[4];
		num[0]=a;
		num[1]=b;
		num[2]=c;
		num[3]=d;
		bool hasFlag=false;
		double z_vmin,z_vmax;
		bool z_Big=true;
		for(int i=0;i<4;i++)
		{
			if(isNaN(z_value[num[i]])||(!fillIn(poly_dev,num[i])))
			{
				hasFlag=true;
				break;
			}
		}
		if(hasFlag==false)
		{
			z_vmin=z_value[num[0]];
			z_vmax=z_vmin;
			for(int i=1;i<4;i++)
			{
				if(z_value[num[i]]<z_vmin)
					z_vmin=z_value[num[i]];
				if(z_value[num[i]]>z_vmax)
					z_vmax=z_value[num[i]];
			}
			double z_scale=(zmax-zmin)*MIN_Z_SCALE;

			if(((zmax-zmin)<z_scale))
			{
				z_Big=false;
			}
		}
		
		if(depth>=MAX_DEPTH_ADD)
		{
			if((!isNaN(z_value[a]))&&fillIn(poly_dev,a)&&(!isNaN(z_value[b]))&&fillIn(poly_dev,b)
				&&(!isNaN(z_value[c]))&&fillIn(poly_dev,c))
			{
			    Tris.add(new Triangle(a,b,c,false));
			}
			
			if((!isNaN(z_value[b]))&&fillIn(poly_dev,b)&&(!isNaN(z_value[c]))&&fillIn(poly_dev,c)
				&&(!isNaN(z_value[d]))&&fillIn(poly_dev,d))
			{
			    Tris.add(new Triangle(b,c,d,false));
			}
			
			if((!isNaN(z_value[c]))&&fillIn(poly_dev,c)&&(!isNaN(z_value[d]))&&fillIn(poly_dev,d)
				&&(!isNaN(z_value[a]))&&fillIn(poly_dev,a))
			{
		    	Tris.add(new Triangle(c,d,a,false));
			}
			
			if((!isNaN(z_value[d]))&&fillIn(poly_dev,d)&&(!isNaN(z_value[a]))&&fillIn(poly_dev,a)
				&&(!isNaN(z_value[b]))&&fillIn(poly_dev,b))
			{
				Tris.add(new Triangle(d,a,b,false));
			}
			
				return;
			
		}
		else if(z_Big==false)
		{
			if((!isNaN(z_value[a]))&&fillIn(poly_dev,a)&&(!isNaN(z_value[b]))&&fillIn(poly_dev,b)
				&&(!isNaN(z_value[c]))&&fillIn(poly_dev,c)&&(!isNaN(z_value[d]))&&fillIn(poly_dev,d))
			{
			    quads.add(new Quads(a,b,c,d,false));
			}
			
				return;
			
		}
		else 
		{
			double graphscale_zx=(zmax-zmin)/(xmax-xmin);
			double graphscale_zy=(zmax-zmin)/(ymax-ymin);
			double d_x=(xmax-xmin)/LARGE;
			double d_y=(ymax-ymin)/LARGE;
			struct two_dev dev;

			double x_middle=(verts[a]->x+verts[c]->x)/2;
			double y_middle=(verts[a]->y+verts[c]->y)/2;

			Point2d *p=new Point2d(x_middle,y_middle);
			int a1=findInVerts(p);
			if(a1==-1)
			{
				double z_middle=fz->getSingleData(x_middle,y_middle);
				z_middle=Z_inPlotRange(z_middle);
				verts.add(p);
				z_value.add(z_middle);
				dev.dev_x=std::abs(fz->getDerivativeX(x_middle,y_middle,d_x))/graphscale_zx;
				dev.dev_y=std::abs(fz->getDerivativeY(x_middle,y_middle,d_y))/graphscale_zy;
				poly_dev.push_back(dev);
				a1=verts.size()-1;
			}
			p=new Point2d(x_middle,verts[a]->y);
			int b1=findInVerts(p);
			if(b1==-1)
			{
				double z_ab_middle=fz->getSingleData(x_middle,verts[a]->y);
				z_ab_middle=Z_inPlotRange(z_ab_middle);
				verts.add(p);
				z_value.add(z_ab_middle);
				dev.dev_x=std::abs(fz->getDerivativeX(x_middle,verts[a]->y,d_x))/graphscale_zx;
				dev.dev_y=std::abs(fz->getDerivativeY(x_middle,verts[a]->y,d_y))/graphscale_zy;
				poly_dev.push_back(dev);
				b1=verts.size()-1;
			}
			p=new Point2d(verts[b]->x,y_middle);
			int c1=findInVerts(p);
			if(c1==-1)
			{
				double z_bc_middle=fz->getSingleData(verts[b]->x,y_middle);
				z_bc_middle=Z_inPlotRange(z_bc_middle);
				verts.add(p);
				z_value.add(z_bc_middle);
				dev.dev_x=std::abs(fz->getDerivativeX(verts[b]->x,y_middle,d_x))/graphscale_zx;
				dev.dev_y=std::abs(fz->getDerivativeY(verts[b]->x,y_middle,d_y))/graphscale_zy;
				poly_dev.push_back(dev);
				c1=verts.size()-1;
			}
			p=new Point2d(x_middle,verts[d]->y);
			int d1=findInVerts(p);
			if(d1==-1)
			{
				double z_cd_middle=fz->getSingleData(x_middle,verts[d]->y);
				z_cd_middle=Z_inPlotRange(z_cd_middle);
				verts.add(p);
				z_value.add(z_cd_middle);
				dev.dev_x=std::abs(fz->getDerivativeX(x_middle,verts[d]->y,d_x))/graphscale_zx;
				dev.dev_y=std::abs(fz->getDerivativeY(x_middle,verts[d]->y,d_y))/graphscale_zy;
				poly_dev.push_back(dev);
				d1=verts.size()-1;
			}
			p=new Point2d(verts[a]->x,y_middle);
			int e1=findInVerts(p);
			if(e1==-1)
			{
				double z_da_middle=fz->getSingleData(verts[a]->x,y_middle);
				z_da_middle=Z_inPlotRange(z_da_middle);
				verts.add(p);
				z_value.add(z_da_middle);
				dev.dev_x=std::abs(fz->getDerivativeX(verts[a]->x,y_middle,d_x))/graphscale_zx;
				dev.dev_y=std::abs(fz->getDerivativeY(verts[a]->x,y_middle,d_y))/graphscale_zy;
				poly_dev.push_back(dev);
				e1=verts.size()-1;
			}
			
			
			constructTriangle(verts,poly_dev,a,b1,a1,e1,depth+1);
			constructTriangle(verts,poly_dev,b1,b,c1,a1,depth+1);
			constructTriangle(verts,poly_dev,a1,c1,c,d1,depth+1);
			constructTriangle(verts,poly_dev,e1,a1,d1,d,depth+1);
			
		}
        
	}
	bool DensityPlot::fillIn(std::vector <struct two_dev> &dev_z,int i)
	{
		return ((!isNaN(dev_z[i].dev_x))&&(dev_z[i].dev_x<=MAX_DERIVATIVE)
			&&(!isNaN(dev_z[i].dev_y))&&(dev_z[i].dev_y<=MAX_DERIVATIVE));
	}
	Color* DensityPlot::getRainbowColor(double x) {
			double red ;
			double green ;
			double blue ;
			
			if(!isNaN(x))
			{
				 red=int(std::abs(x))%255;
				 green=red;
				 blue=255;
			}
			else
			{
				red=green=blue=255;
			}
		
			
			return new Color(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));
		}
	
	DensityPlot::~DensityPlot()
	{
		for(size_t i=0;i<quads.size();i++)
		{
			delete quads[i];
		}
		quads.clear();
		for(size_t i=0;i<verts.size();i++)
		{
			delete verts[i];
		}
		verts.clear();
		for(size_t i=0;i<Tris.size();i++)
		{
			delete Tris[i];
		}
		Tris.clear();
		z_value.clear();
		delete fz;
	}

	var DensityPlot::exportGraphics()
	{
		var shapelist= list(polynum,SYS(Shape));
		var v,colorPolygon,directive;
		

		for (int i = 0; i < polynum; i++) {
			v = list(verts.size());
			for (size_t i = 0; i < verts.size(); ++i) {
				v.tuple()[i + 1] = list(new Real(verts[i]->x, 0), new Real(verts[i]->y, 0));
			}
			int tris_size=0;
			for(size_t j=0;j<Tris.size();j++)
			{
				if(Tris[j]->flag==false)
					tris_size++;
			}
			int quad_size=0;
			for(size_t j=0;j<quads.size();j++)
			{
				if(quads[j]->flag==false)
					quad_size++;
			}
			colorPolygon = list(tris_size+quad_size+1,SYS(ColorPolygon));
			
			var colors=list(tris_size+quad_size);

			int sum=1;
			double z_avg;
			for (size_t i = 0; i <quads.size(); i++) {
				if(quads[i]->flag==false)
				{
					var indice = list(new Integer((long)quads[i]->p11),
						new Integer((long)quads[i]->p12),
						new Integer((long)quads[i]->p21),
						new Integer((long)quads[i]->p22));
					z_avg=(z_value[quads[i]->p11]+z_value[quads[i]->p12]+
						z_value[quads[i]->p21]+z_value[quads[i]->p22])/4.0;
				
					z_avg=(z_avg-zmin)/(zmax-zmin)*254.;
					Color* color1 = getRainbowColor(z_avg);
					var qopt = makeColor255Option(color1);
					var colorOption=list(3);
					colorOption.tuple()[1]=new Integer((long)color1->r);
					colorOption.tuple()[2]=new Integer((long)color1->g);
					colorOption.tuple()[3]=new Integer((long)color1->b);
					colorPolygon.tuple()[sum]=indice;
					colors.tuple()[sum++]=colorOption;
				}
			}
			for (size_t i = 0; i <Tris.size(); i++) {
				if(Tris[i]->flag==false)
				{
					var indice = list(new Integer((long)Tris[i]->p11),
						new Integer((long)Tris[i]->p12),
						new Integer((long)Tris[i]->p13));
					z_avg=(z_value[Tris[i]->p11]+z_value[Tris[i]->p12]+z_value[Tris[i]->p13])/3.0;
					
					z_avg=(z_avg-zmin)/(zmax-zmin)*254.;
					
					Color* color1 = getRainbowColor(z_avg);
					var qopt = makeColor255Option(color1);
					var colorOption=list(3);
					colorOption.tuple()[1]=new Integer((long)color1->r);
					colorOption.tuple()[2]=new Integer((long)color1->g);
					colorOption.tuple()[3]=new Integer((long)color1->b);
					colorPolygon.tuple()[sum]=indice;
					colors.tuple()[sum++]=colorOption;
				}
			}
			colorPolygon.tuple()[sum++]=colors;
			
			directive=list(colorPolygon,SYS(Directive));
			directive.tuple()[1]=colorPolygon;
		}
		shapelist.tuple()[1]=tuple(SYS(GraphicsComplex), v, directive);
		var opts = makeGlobalOptions();
		var graphics = makeGraphics(shapelist, opts);
		return graphics;
		/*var shapelist= list(polynum,SYS(Shape));
		var v,directive;
		std::vector<double> v_zavg;
		for (int i = 0; i < polynum; i++) {
			v = list(verts.size());
			for (size_t i = 0; i < verts.size(); ++i) {
				v.tuple()[i + 1] = list(new Real(verts[i]->x, 0), new Real(verts[i]->y, 0));
			}
			int tris_size=0;
			for(size_t j=0;j<Tris.size();j++)
			{
				if(Tris[j]->flag==false)
					tris_size++;
			}
			int quad_size=0;
			for(size_t j=0;j<quads.size();j++)
			{
				if(quads[j]->flag==false)
					quad_size++;
			}
			directive = list(tris_size+quad_size,SYS(Directive));
			int sum=1;
			
			double z_avg;
			for (size_t i = 0; i <quads.size(); i++) {
				if(quads[i]->flag==false)
				{
					var indice = list(new Integer((long)quads[i]->p11),
						new Integer((long)quads[i]->p12),
						new Integer((long)quads[i]->p21),
						new Integer((long)quads[i]->p22));
					z_avg=(z_value[quads[i]->p11]+z_value[quads[i]->p12]+
						z_value[quads[i]->p21]+z_value[quads[i]->p22])/4.0;
				
					z_avg=(z_avg-zmin)/(zmax-zmin)*254.;
					Color* color1 = getRainbowColor(z_avg);
					var qopt = makeColor255Option(color1);
					var equad = tuple(SYS(Polygon), indice, qopt);
					directive.tuple()[sum++]=equad;
				}
			}
			for (size_t i = 0; i <Tris.size(); i++) {
				if(Tris[i]->flag==false)
				{
					var indice = list(new Integer((long)Tris[i]->p11),
						new Integer((long)Tris[i]->p12),
						new Integer((long)Tris[i]->p13));
					z_avg=(z_value[Tris[i]->p11]+z_value[Tris[i]->p12]+z_value[Tris[i]->p13])/3.0;
					
					z_avg=(z_avg-zmin)/(zmax-zmin)*254.;
					v_zavg.push_back(z_avg);
					Color* color1 = getRainbowColor(z_avg);
					var qopt = makeColor255Option(color1);
					var etris = tuple(SYS(Polygon), indice, qopt);
					directive.tuple()[sum++]=etris;
				}
			}
		}
		shapelist.tuple()[1]=tuple(SYS(GraphicsComplex), v, directive);
		var opts = makeGlobalOptions();
		var graphics = makeGraphics(shapelist, opts);
		return graphics;*/
	}
	Hue* DensityPlot::getPolygonHue(int i)
	{
		Hue* hue = new Hue(STD_E,STD_S ,i );
        return hue;
	}
	void DensityPlot::setRule(var title, var rule) {
     
		setCommonRule(title, rule);
	}

	int DensityPlot::getBaseNum()
	{
		return 16;
	}
}
}
