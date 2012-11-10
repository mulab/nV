#include <nV/Number.h>
#include "DiscretePlot.h"

namespace nV {
    namespace Graphics {
DiscretePlot::DiscretePlot(Kernel& k, const Tuple& cmd) : k(k)
{
	filling=FILL_AXIS_DISCRETE;
    //filling = FILL_NULL;
	colorFunctionSet=false;
	joined=false;
	XMin=1;
	XStep=1;
	
	var expr = cmd[1];
    var xrange = cmd[2];
	xparam = xrange.tuple()[1];
	switch(xrange.tuple().size)
	{
	case 3:
		XMax=toD(N(k, k.eval(xrange.tuple()[2])).object());
		break;
	case 4:
		XMin=toD(N(k, k.eval(xrange.tuple()[2])).object());
		XMax=toD(N(k, k.eval(xrange.tuple()[3])).object());
		break;
	case 5:
		XMin=toD(N(k, k.eval(xrange.tuple()[2])).object());
		XMax=toD(N(k, k.eval(xrange.tuple()[3])).object());
		XStep=toD(N(k, k.eval(xrange.tuple()[4])).object());
		break;
	}
	
	maxpnum=int((XMax-XMin)/XStep)+1;
	if (cmd[cmd.size - 1].isTuple(SYS(Options))) {
        var opts = cmd[cmd.size - 1];
        readOptions(opts.tuple());
    }

    if(expr.isTuple($.List)) {
        curnum = expr.tuple().size - 1;
    }else {
        curnum = 1;
        expr = list(expr);
    }
    this->expression = expr;

	varplist=list(curnum);
	if(colorFunctionSet)
	{
		makeColorOption();
	}
	//std::vector<std::vector<x_domain *> >domain(curnum);
    xmin=XMin;
	xmax=XMax;
	makePointList();

	

	makeyrange();
	
}
void DiscretePlot::makeyrange()
{
	ymin=toD(N(k, k.eval(varplist.tuple()[1].tuple()[1].tuple()[2])).object());
	ymax=ymin;
	
	double tempy;
	double min_y1,min_y2;
	double max_y1,max_y2;
	bool flag_y1=false ,flag_y2=false;

	for(uint i=0;i<curnum;i++)
	{
		for(uint j=0;j<pnum[i];j++)
		{
			tempy=toD(varplist.tuple()[i+1].tuple()[j+1].tuple()[2].object());
			
			if(singular_flag[i]==true)
			{
				if(flag_y1==false)
				{
					flag_y1=true;
					min_y1=tempy;
					max_y1=tempy;
				}
				else
				{
					if(tempy<min_y1)
				        min_y1=tempy;
			        if(tempy>max_y1)
				        max_y1=tempy;
				}
			}else
			{
				if(flag_y2==false)
				{
					flag_y2=true;
					min_y2=tempy;
					max_y2=tempy;
				}
				else
				{
					if(tempy<min_y2)
				        min_y2=tempy;
			        if(tempy>max_y2)
				        max_y2=tempy;
				}
			}
			
		}
	}
	if(flag_y1==true&&flag_y2==true)
    {
        if(max_y2<max_y1||min_y2>min_y1)
        {
			 ymax=max_y2+1;
		     ymin=min_y2-1;
			
        }else
        {
             ymax=max_y2;
             ymin=min_y2;
        }
    }else if(flag_y1==true&&flag_y2==false)
    {
          ymax=max_y1;
          ymin=min_y1;
    }else if(flag_y1==false &&flag_y2==true)
    {
         ymax=max_y2;
         ymin=min_y2;
    }
	/*if(ymax-ymin>=MAX_YRANGE)
	{
		ymax=(ymax+ymin)/2+10;
		ymin=(ymax+ymin)/2-10;
	}*/
	if(ymin*ymax>0&&ymin>0&&ymin<MIN_DISTANCE)
		ymin=0;
	if(ymin*ymax>0&&ymax<0&&(0-ymax)<MIN_DISTANCE)
		ymax=0;
	
}
int DiscretePlot::getNoNaNsum()
{
	int sum=0;
	for(uint i=0;i<curnum;i++)
	{
		F1P *f2d=new F1P(k, expression.tuple()[i + 1], xparam);
		
		for(uint j=0;j<maxpnum;j++)
		{
			double tempx=XMin+XStep*j;
		    double tempy=f2d->getSingleData(tempx);
			double dx=(xmax-xmin)/LARGE;
			double d_x=f2d->getDerivative(tempx,dx);
			double graphScale=(ymax-ymin)/(xmax-xmin);
			if(!isNaN(tempy)&&(std::abs(d_x)/graphScale)<MAX_DERIVATIVE)
			{
				sum++;
			}
		}
	}
	return sum;
}
int DiscretePlot::getNoNaN_fillCurve()
{
	int sum=0;
	F1P* f2d1=new F1P(k, expression.tuple()[fillcur1 + 1], xparam);
	F1P* f2d2=new F1P(k, expression.tuple()[fillcur2 + 1], xparam);
	for(uint j=0;j<maxpnum;j++)
	{
		double tempx=XMin+XStep*j;
	    double tempy1=f2d1->getSingleData(tempx);
		double tempy2=f2d2->getSingleData(tempx);
		double dx=(xmax-xmin)/LARGE;
		double d_x1=f2d1->getDerivative(tempx,dx);
		double d_x2=f2d2->getDerivative(tempx,dx);
		double graphScale=(ymax-ymin)/(xmax-xmin);
		if((!isNaN(tempy1))&&(!isNaN(tempy2))&&((std::abs(d_x1)/graphScale)<MAX_DERIVATIVE)&&((std::abs(d_x2)/graphScale)<MAX_DERIVATIVE))
		{
		    sum++;
		}
	}
	return sum;
}
int DiscretePlot::getDomainNumber()
{
	int sum=0;
	for(uint i=0;i<curnum;i++)
	{
		bool nanFlag=false;
		F1P* f2d=new F1P(k, expression.tuple()[i + 1], xparam);
		for(uint j=0;j<maxpnum;j++)
		{
			double tempx=XMin+XStep*j;
	        double tempy=f2d->getSingleData(tempx);
			double dx=(xmax-xmin)/LARGE;
			double d_x=f2d->getDerivative(tempx,dx);
			double graphScale=(ymax-ymin)/(xmax-xmin);
			if((nanFlag==false)&&(!isNaN(tempy)&&(std::abs(d_x)/graphScale<MAX_DERIVATIVE)))
			{
				nanFlag=true;
				continue;
			}
			if((nanFlag==true)&&((isNaN(tempy))||(j==maxpnum-1)||(std::abs(d_x)/graphScale>MAX_DERIVATIVE)))
			{
				nanFlag=false;
				sum++;
				continue;
			}
			
		}
	}
	return sum;
}

var DiscretePlot::exportGraphics()
{
    if(joined==true)
    {  
        //cout<<getDomainNumber()<<endl;
        var linevar=list(getDomainNumber(),SYS(Shape));
        int suml=1;
        int x_start,x_end;
        for(uint i=0;i<curnum;i++)
        {
            bool nanFlag=false;
            F1P* f2d=new F1P(k, expression.tuple()[i + 1], xparam);
            for(uint j=0;j<maxpnum;j++)
            {
                double tempx=XMin+XStep*j;
                double tempy=f2d->getSingleData(tempx);
                double dx=(xmax-xmin)/LARGE;
                double d_x=f2d->getDerivative(tempx,dx);
                double graphScale=(ymax-ymin)/(xmax-xmin);
                if((nanFlag==false)&&((!isNaN(tempy))&&(std::abs(d_x)/graphScale<MAX_DERIVATIVE)))
                {
                    nanFlag=true;
                    x_start=j;
                    continue;
                }
                if((nanFlag==true)&&((isNaN(tempy))||(j==maxpnum-1)||(std::abs(d_x)/graphScale>MAX_DERIVATIVE)))
                {
                    nanFlag=false;
                    if((isNaN(tempy))||(std::abs(d_x)/graphScale>MAX_DERIVATIVE))
                        x_end=j-1;
                    else if(j==maxpnum-1)
                        x_end=j;

                    var vtuple=list(x_end-x_start+3);
                    int sum=1;
                    for(int k=x_start;k<=x_end;k++)
                    {
                        double tempx1=XMin+XStep*k;
                        double tempy1=f2d->getSingleData(tempx1);
                        //if(!isNaN(tempy1))
                        //{
                        vtuple.tuple()[sum++]=list(new Real(tempx1,0),new Real(tempy1,0));
                        //}
                    }
                    double y0=0;
                    vtuple.tuple()[sum++]=list(new Real(XMin+XStep*x_end,0),new Real(y0,0));
                    vtuple.tuple()[sum++]=list(new Real(XMin+XStep*x_start,0),new Real(y0,0));
                    //vtuple.print();
                    //cout<<endl;
                    var directiveTuple=list(2,SYS(Directive));
                    var lineTuple=list(2,SYS(Line));
                    var linevert=list(x_end-x_start+1);

                    var PolyTuple=list(2,SYS(Polygon));
                    var poly=list(x_end-x_start+3);

                    int sumv=1;
                    for(int k=x_start;k<=x_end;k++)
                    {
                        linevert.tuple()[sumv]=new Integer(long(k-x_start));
                        poly.tuple()[sumv++]=new Integer(long(k-x_start));
                    }
                    Hue* hue =getHue(i);
                    lineTuple.tuple()[1]=linevert;
                    lineTuple.tuple()[2]=makeHueOption(hue);
                    poly.tuple()[sumv++]=new Integer(long(x_end-x_start+1));
                    poly.tuple()[sumv++]=new Integer(long(x_end-x_start+2));
                    PolyTuple.tuple()[1]=poly;
                    PolyTuple.tuple()[2]=makeHueOption(getLightHue(i));
                    //lineTuple.print();
                    directiveTuple.tuple()[1]=lineTuple;
                    directiveTuple.tuple()[2]=PolyTuple;
                    var complextuple=list(2,SYS(GraphicsComplex));
                    complextuple.tuple()[1]=vtuple;
                    complextuple.tuple()[2]=directiveTuple;
                    linevar.tuple()[suml++]=complextuple;
                    continue;
                }

            }
        }
        var opts = makeGlobalOptions();
        var graphics = makeGraphics(linevar, opts);
        return graphics;


    }else{
        if (filling == FILL_NULL) {

            var shapelist=list(curnum,SYS(Shape));
            if(!colorFunctionSet)
            {
                for (uint i = 0; i < curnum; i++) {
                    Hue* hue = getHue(i);
                    shapelist.tuple()[i+1] = tuple(SYS(PointList),varplist.tuple()[i+1],makeHueOption(hue));
                    delete hue;  
                }
            }else 
            {
                for(uint j=0;j<curnum;j++)
                {
                    var c=list(pnum[j]);
                    for(uint i=0;i<pnum[j];i++)
                    {
                        c.tuple()[i+1]=makeColor255(color[i]);
                    }
                    shapelist.tuple()[j+1] = tuple(SYS(PointList),varplist.tuple()[j+1],c);
                }
            }
            var opts = makeGlobalOptions();
            var graphics = makeGraphics(shapelist, opts);
            return graphics;

        } else if(filling==FILL_CURVE)
        {
            int sum=1;
            var shapelist=list(curnum+getNoNaN_fillCurve(),SYS(Shape));
            for (uint i = 0; i < curnum; i++) {
                int versum=0;
                Hue* hue = getHue(i);
                F1P *f2d1 = new F1P(k, expression.tuple()[i+1], xparam);
                for(uint j=0;j<maxpnum;j++)
                {

                    double tempx=XMin+XStep*j;			
                    double tempy1=f2d1->getSingleData(tempx);
                    double dx=(xmax-xmin)/LARGE;
                    double d_x=f2d1->getDerivative(tempx,dx);
                    double graphScale=(ymax-ymin)/(xmax-xmin);
                    if(!isNaN(tempy1)&&(std::abs(d_x)/graphScale<MAX_DERIVATIVE))
                    {
                        versum++;
                    }

                }

                var ver_tuple=list(versum);
                int ver=1;
                for(uint j=0;j<maxpnum;j++)
                {

                    double tempx=XMin+XStep*j;			
                    double tempy1=f2d1->getSingleData(tempx);
                    double dx=(xmax-xmin)/LARGE;
                    double d_x=f2d1->getDerivative(tempx,dx);
                    double graphScale=(ymax-ymin)/(xmax-xmin);
                    if((!isNaN(tempy1))&&(std::abs(d_x)/graphScale<MAX_DERIVATIVE))
                    {
                        ver_tuple.tuple()[ver++]=list(new Real(tempx, 0), new Real(tempy1, 0));
                    }
                }
                shapelist.tuple()[sum++] = tuple(SYS(PointList),ver_tuple,makeHueOption(hue));
                //shapelist.print();
                delete f2d1;
                delete hue;  

            }
            //for(uint i=0;i<curnum;i++)
            //  {
            F1P *f2d1 = new F1P(k, expression.tuple()[fillcur1 + 1], xparam);
            F1P *f2d2 = new F1P(k, expression.tuple()[fillcur2 + 1], xparam);
            for(uint j=0;j<maxpnum;j++)
            {
                double tempx=XMin+XStep*j;			
                double tempy1=f2d1->getSingleData(tempx);
                double tempy2=f2d2->getSingleData(tempx);
                double dx=(xmax-xmin)/LARGE;
                double d_x1=f2d1->getDerivative(tempx,dx);
                double d_x2=f2d2->getDerivative(tempx,dx);
                double graphScale=(ymax-ymin)/(xmax-xmin);

                Hue* hue =getHue(fillcur1+fillcur2);
                var r = list(2,SYS(Line));
                if((!isNaN(tempy1))&&(!isNaN(tempy2))&&(std::abs(d_x1)/graphScale<MAX_DERIVATIVE)&&(std::abs(d_x2)/graphScale<MAX_DERIVATIVE))
                {
                    //if(tempy1>ymin&&tempy1<ymax)
                    // {
                    //if(tempy2>ymin&&tempy2<ymax)
                    r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(tempy1, 0)),list(new Real(tempx, 0), new Real(tempy2, 0)));
                    //  else if(tempy2<=ymin)
                    //     r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(tempy1, 0)),list(new Real(tempx, 0), new Real(ymin, 0)));
                    /*else if(tempy2>=ymax)
                    r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(tempy1, 0)),list(new Real(tempx, 0), new Real(ymax, 0)));
                    }else if(tempy1<=ymin)
                    {
                    if(tempy2>ymin&&tempy2<ymax)
                    r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(ymin, 0)),list(new Real(tempx, 0), new Real(tempy2, 0)));
                    else if(tempy2<=ymin)
                    r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(ymin, 0)),list(new Real(tempx, 0), new Real(ymin, 0)));
                    else if(tempy2>=ymax)
                    r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(ymin, 0)),list(new Real(tempx, 0), new Real(ymax, 0)));
                    }
                    else if(tempy1>=ymax)
                    {
                    if(tempy2>ymin&&tempy2<ymax)
                    r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(ymax, 0)),list(new Real(tempx, 0), new Real(tempy2, 0)));
                    else if(tempy2<=ymin)
                    r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(ymax, 0)),list(new Real(tempx, 0), new Real(ymin, 0)));
                    else if(tempy2>=ymax)
                    r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(ymax, 0)),list(new Real(tempx, 0), new Real(ymax, 0)));
                    }*/
                    r.tuple()[2]=makeHueOption(hue);
                    shapelist.tuple()[sum++]=r;
                }
                delete hue;
            }
            delete f2d1;
            delete f2d2;
            // }
            var opts = makeGlobalOptions();
            var graphics = makeGraphics(shapelist, opts);
            return graphics;

        }else
        {
            int sum=1;
            var shapelist=list(curnum+getNoNaNsum(),SYS(Shape));
            for (uint i = 0; i < curnum; i++) {
                int versum=0;
                Hue* hue = getHue(i);
                F1P *f2d1 = new F1P(k, expression.tuple()[i+1], xparam);
                for(uint j=0;j<maxpnum;j++)
                {

                    double tempx=XMin+XStep*j;			
                    double tempy1=f2d1->getSingleData(tempx);
                    double dx=(xmax-xmin)/LARGE;
                    double d_x=f2d1->getDerivative(tempx,dx);
                    double graphScale=(ymax-ymin)/(xmax-xmin);
                    if(!isNaN(tempy1)&&(std::abs(d_x)/graphScale<MAX_DERIVATIVE))
                    {
                        versum++;
                    }

                }

                var ver_tuple=list(versum);
                int ver=1;
                for(uint j=0;j<maxpnum;j++)
                {

                    double tempx=XMin+XStep*j;			
                    double tempy1=f2d1->getSingleData(tempx);
                    double dx=(xmax-xmin)/LARGE;
                    double d_x=f2d1->getDerivative(tempx,dx);
                    double graphScale=(ymax-ymin)/(xmax-xmin);
                    if((!isNaN(tempy1))&&(std::abs(d_x)/graphScale<MAX_DERIVATIVE))
                    {
                        ver_tuple.tuple()[ver++]=list(new Real(tempx, 0), new Real(tempy1, 0));
                    }
                }
                shapelist.tuple()[sum++] = tuple(SYS(PointList),ver_tuple,makeHueOption(hue));
                //shapelist.print();
                delete f2d1;
                delete hue;  

            }
            for(uint i=0;i<curnum;i++)
            {
                F1P *f2d = new F1P(k, expression.tuple()[i + 1], xparam);
                Hue* hue =getHue(i);
                for(uint j=0;j<maxpnum;j++)
                {
                    double tempx=XMin+XStep*j;			
                    double tempy=f2d->getSingleData(tempx);
                    double dx=(xmax-xmin)/LARGE;
                    double d_x=f2d->getDerivative(tempx,dx);
                    double graphScale=(ymax-ymin)/(xmax-xmin);
                    double y0=0;

                    if(!isNaN(tempy)&&(std::abs(d_x)/graphScale<MAX_DERIVATIVE))
                    {
                        var r = list(2,SYS(Line));

                        //if(tempy<ymax&&tempy>ymin)
                        r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(tempy, 0)),list(new Real(tempx, 0), new Real(y0, 0)));
                        //else if(tempy<=ymin)
                        // r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(ymin, 0)),list(new Real(tempx, 0), new Real(y0, 0)));
                        //else if(tempy>=ymax)
                        //r.tuple()[1]=list(list(new  Real(tempx, 0), new Real(ymax, 0)),list(new Real(tempx, 0), new Real(y0, 0)));

                        r.tuple()[2]=makeHueOption(hue);
                        //r.print();
                        shapelist.tuple()[sum++]=r;

                    }

                }

                delete f2d;
                delete hue;
            }
            var opts = makeGlobalOptions();
            var graphics = makeGraphics(shapelist, opts);
            return graphics;
        }
    }

}
DiscretePlot::~DiscretePlot()
{
	if(colorFunctionSet)
	{
		for(uint j=0;j<curnum;j++)
		{
	    	for(uint i=0;i<maxpnum;i++)
		   {
			    delete color[i];
	    	}
		}
	}
	delete pnum;
	delete singular_flag;
	//delete pnum;
}
void DiscretePlot::makeColorOption()
{
	double cfmin, cfmax;
	cfmin = cf->getSingleData(XMin);
	cfmax = cfmin;
	double cfy;
	for (unsigned int i = 0; i < maxpnum; i++) {
		cfy=cf->getSingleData(XMin+XStep*i);
		if(cfy<cfmin)
			cfmin=cfy;
		if(cfy>cfmax)
			cfmax=cfy;
	}
	for (unsigned int i = 0; i < maxpnum; i++) {
		cfy = cf->getSingleData(XMin+XStep*i);
		double cfy1=(cfy - cfmin) / (cfmax - cfmin);
		double red = 0;
		double green = 0;
		double blue = 0;
		if (cfy1 <= 63.0 / 255) {
			cfy1 = 4 * (cfy1 - 0);
			blue = 1;
			green = cfy1;
		} else if (cfy1 <= 127.0 / 255) {
			cfy1 = 4 * (cfy1 - 64.0 / 255);
			green = 1;
			blue = 1 - cfy1;
		} else if (cfy1 <= 191.0 / 255) {
			cfy1 = 4 * (cfy1 - 128.0 / 255);
			green = 1;
			red = cfy1;
		} else {
			cfy1 = 4 * (cfy1 - 192.0 / 255);
			red = 1;
			green = 1 - cfy1;
		}
			if (red > 1)
				red = 1;
			if (red < 0)
				red = 0;
			if (green > 1)
				green = 1;
			if (green < 0)
				green = 0;
			if (blue > 1)
				blue = 1;
			if (blue < 0)
				blue = 0;
			red *= 255;
			green *= 255;
			blue *= 255;
			
		color.add(i,new Color(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue)));
	}
}
void DiscretePlot::setRule(var title, var rule)
{
	if (title == SYS(FillToCurve)) {
        filling = FILL_CURVE;
        fillcur1 = toD(N(k, k.eval(rule.tuple()[1])).object());
        fillcur2 = toD(N(k, k.eval(rule.tuple()[2])).object());
    } else if (title == SYS(FillToAxis)) {
        filling = FILL_AXIS_DISCRETE;
        fillcur1 = toD(N(k, k.eval(rule.tuple()[1])).object());
    } else if(title==SYS(NoFilling))
	{
	//	rule.print();
		if(rule==SYS(True))
		    filling=FILL_NULL;
		else
			filling=FILL_AXIS_DISCRETE;
	}else if (title == SYS(ColorFunction)) { //has color function
        colorFunctionSet = true;
        var fun = rule;
        cf = new F1P(k,fun, xparam);
    }else if(title == SYS(Joined))
	{
		if(rule==SYS(True))
		{
			joined=true;
		}
		else
			joined=false;
	}
            //to do color function in hue(f1,f2,f3) or color(f1,f2,f3)
    setCommonRule(title, rule);
}
void DiscretePlot::makePointList( )
{
	singular_flag=new bool[curnum];
	pnum=new uint[curnum];

	
	for(uint ci=0;ci<curnum;ci++)
	{
		F1P *f2d = new F1P(k, expression.tuple()[ci + 1], xparam);
    	bool singularity = false;
            //std::vector<singularitynode *> singularityset;
        int bigDataNum = 500;
	    double *x = new double[bigDataNum];
		double *y = new double[bigDataNum];
		for (int i = 0; i < bigDataNum; i++) {
			x[i] = xmin + (xmax - xmin) * i / (bigDataNum - 1);
		}
		f2d->getArrayData(x, y, bigDataNum);
		ArrayList<Point2d*> vert;
		ArrayList<double> d;
		for (int i = 0; i < bigDataNum; i++) {
			vert.add(new Point2d(x[i], y[i]));
		}
		delete x;
		delete y;
		int j=0;
		while(isNaN(y[j]))
		{
			j++;
		}
		ymin = vert[j]->y;
		ymax = vert[j]->y;
		for (uint i = j; i < vert.size(); i++) {
			if(!isNaN(vert[i]->y))
			{
			   if (vert[i]->y > ymax) ymax = vert[i]->y;
			   if (vert[i]->y < ymin) ymin = vert[i]->y;
			}
		}
		double spx = xmax - xmin;
		double spy = ymax - ymin;
		double graphScale = spy / spx;
		
			
		double dx = spx / LARGE;
		for (unsigned int i = 0; i < vert.size(); i++) {
			d.add(f2d->getDerivative(vert[i]->x, dx));
		}

            //
            //去除斜率太大的点
            //
		bool vrm = true; //vert removed
		bool *flag = new bool[vert.size()];
		for(uint i = 0; i < vert.size(); i++) {
			flag[i] = false;
		}
		bool *pflag = flag;

		int count = 0;
		while (vrm) {
			vrm = false;
			for (unsigned int i = 0; i < vert.size(); i++) {
				if(!flag[i]) {
					if ((!isNaN(d[i]))&&(std::abs(d[i]) / graphScale > MAX_DERIVATIVE)) {
							//vert.remove(i);
							//d.remove(i);
                        singularity = true;
						flag[i] = true;
						vrm = true;
					}
				}
			}
			for(uint i = 0; i < vert.size(); i++) {
				if((!isNaN(vert[i]->y))&&(!flag[i])) {
					ymin = vert[i]->y;
					ymax = vert[i]->y;
					break;
				}
			}
			for (uint i = 0; i < vert.size(); i++) {
				if((!isNaN(vert[i]->y))&&(!flag[i])) {
					if (vert[i]->y > ymax) ymax = vert[i]->y;
					if (vert[i]->y < ymin) ymin = vert[i]->y;
				}
			}
			spy = ymax - ymin;
			graphScale = spy / spx;
            if(graphScale < MIN_GRAPHSCALE_MINIMUM_CURVE) {
                   break;
            }
			if(count >= MAX_DELETE_AMOUNT) {
				break;
			}
				count++;
		}

		singular_flag[ci]=singularity;
		pnum[ci]=0;
		for(uint j=0;j<maxpnum;j++)
		{
			double tempx=XMin+XStep*j;
			double tempy=f2d->getSingleData(tempx);
			double d_x=f2d->getDerivative(tempx,dx);
			if((!isNaN(tempy))&&(std::abs(d_x)/graphScale<MAX_DERIVATIVE))
			{
				pnum[ci]++;
			}
		 	    
		}
		int sum=1;
		var v= list(pnum[ci]);
		for(uint j=0;j<maxpnum;j++)
		{
			double tempx=XMin+XStep*j;

			double d_x=f2d->getDerivative(tempx,dx);
			//if((std::abs(d_x)/ graphScale)<MAX_DERIVATIVE)
			//{
                double tempy=f2d->getSingleData(tempx);
				if((!isNaN(tempy))&&((std::abs(d_x)/graphScale)<MAX_DERIVATIVE))
				{
			        var temp=list(new Real(tempx, 0), new Real(tempy, 0));
				    v.tuple()[sum++]=temp;
				}
			//}
		}
		//v.print();
		varplist.tuple()[ci+1]=v;   
        delete f2d;
    } 
	}
}
}
