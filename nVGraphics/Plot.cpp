#include <nV/Number.h>
#include "Plot.h"

namespace nV {
    namespace Graphics {

        Plot::Plot(Kernel& k, const Tuple& cmd) : k(k) {
            typedef struct {
                double ymax_d;
                double ymin_d;
            } Y_data,*Py_data;
            //this->k = k;
            filling = FILL_NULL;
            rangeSet = false;
            colorFunctionSet = false;
            var expr = cmd[1];

            var xrange = cmd[2];
            xparam = xrange.tuple()[1];
            xmin = toD(N(k, k.eval(xrange.tuple()[2])).object());
            xmax = toD(N(k, k.eval(xrange.tuple()[3])).object());

	    if(expr.isTuple($.List)) {
                curnum = expr.tuple().size - 1;
            }else {
                curnum = 1;
                expr = list(expr);
            }

            if (cmd[cmd.size - 1].isTuple(SYS(Options))) {
                var opts = cmd[cmd.size - 1];
                readOptions(opts.tuple());
            }

            this->expression = expr;

            curs = new Curve*[curnum];
            //bool hassingularity = false;

            Py_data p_ydata = new Y_data[curnum];
            bool *singularity = new bool[curnum];
            std::vector< std::vector<singularitynode *> > singularityset(curnum);
            std::vector< std::vector<domain *> > nanrangeallset(curnum);

            for(uint i = 0; i < curnum; i++) {
                F1P *f2d = new F1P(k, expr.tuple()[i + 1], xparam);
                singularity[i] = hassingularity(f2d, singularityset[i], p_ydata[i].ymin_d, p_ydata[i].ymax_d, nanrangeallset[i]);
                delete f2d;
            }

            if(rangeSet == 1) {
            } else {
                /*ymin = p_ydata[0].ymin_d;
                ymax = p_ydata[0].ymax_d;
                for (uint i = 1; i < curnum; i++) {
                    if (p_ydata[i].ymin_d < ymin)
                        ymin = p_ydata[i].ymin_d;
                    if (p_ydata[i].ymax_d > ymax)
                        ymax = p_ydata[i].ymax_d;
                }*/
                /*double total_ymax = 0.0, total_ymin = 0.0;
                for(uint i = 0; i < curnum; i++) {
                    total_ymax += p_ydata[i].ymax_d;
                    total_ymin += p_ydata[i].ymin_d;
                }
                ymax = total_ymax / curnum;
                ymin = total_ymin / curnum;*/

                double ymin1,ymin2;
                double ymax1,ymax2;
                bool flag1=false,flag2=false;
                for(uint i = 0; i < curnum; i++) {
                    if(singularity[i]==true)
                    {
                        if(flag1==false)
                        {
                            flag1=true;
                            ymin1=p_ydata[i].ymin_d;
                            ymax1=p_ydata[i].ymax_d;
                        }
                        else
                        {
                            if(p_ydata[i].ymin_d<ymin1)
                                ymin1=p_ydata[i].ymin_d;
                            if(p_ydata[i].ymax_d>ymax1)
                                ymax1=p_ydata[i].ymax_d;
                        }
                    }
                    else
                    {
                        if(flag2==false)
                        {
                            flag2=true;
                            ymin2=p_ydata[i].ymin_d;
                            ymax2=p_ydata[i].ymax_d;
                        }
                        else
                        {
                            if(p_ydata[i].ymin_d<ymin2)
                                ymin2=p_ydata[i].ymin_d;
                            if(p_ydata[i].ymax_d>ymax2)
                                ymax2=p_ydata[i].ymax_d;
                        }
                    }

                }
                if(flag1==true&&flag2==true)
                {
                    if(ymax2>ymax1||ymin2<ymin1)
                    {
                        ymax=ymax1+(ymax1-ymin1)/4;
                        ymin=ymin1-(ymax1-ymin1)/4;
                    }else
                    {
                        ymax=ymax1;
                        ymin=ymin1;
                    }
                }else if(flag1==true&&flag2==false)
                {
                    ymax=ymax1;
                    ymin=ymin1;
                }else if(flag1==false &&flag2==true)
                {
                    ymax=ymax2;
                    ymin=ymin2;
                }

            }

            for(uint i = 0; i < curnum; i++) {
                F1P *f2d = new F1P(k, expr.tuple()[i + 1], xparam);
                if(nanrangeallset[i].size() != 0) {
                    if(!colorFunctionSet) {
                        curs[i] = (Curve *)(new PlotWithDomain(f2d, nanrangeallset[i], ymin, ymax));
                    } else {
                        curs[i] = (Curve *)(new PlotWithDomain(f2d, nanrangeallset[i], cf, ymin, ymax));
                    }
                    delete f2d;
                    continue;
                }
                if(!singularity[i]) {
                    if(!colorFunctionSet) {
                        curs[i] = (Curve *)(new CommonCurveWithPlotRange(f2d, xmin, xmax, ymin, ymax));
                    } else {
                        curs[i] = (Curve *)(new CommonCurveWithPlotRange(f2d, xmin, xmax, ymin, ymax, cf));
                    }
                } else {
                    if(!colorFunctionSet) {
                        curs[i] = (Curve *)(new CommonCurveWithPlotRange(f2d, xmin, xmax, ymin, ymax, singularityset[i]));
                    } else {
                        curs[i] = (Curve *)(new CommonCurveWithPlotRange(f2d, xmin, xmax, ymin, ymax, singularityset[i], cf));
                    }
                }
                delete f2d;
            }

            delete [] p_ydata;
            delete [] singularity;
        }

        Plot::~Plot() {
            for (uint i = 0; i < curnum; i++) {
                delete curs[i];
            }
            delete[] curs;
        }

        var Plot::exportGraphics() {
            for (uint i = 0; i < curnum; i++) {
                if (isNaN(curs[i]->spy)) {
                    var shapelist = list(tuple($.List, new Real(0.0, 0), new Real(0.0, 0)),
                        tuple($.List, new Real(0.1, 0), new Real(0.0, 0)), SYS(Line));
                    var opts = makeGlobalOptions();
                    var graphics = makeGraphics(shapelist, opts);
                    return graphics;
                }
            }
            if (filling == FILL_NULL) {
                //命令个数为1的情况
                var shapelist;	//命令个数
                if (!colorFunctionSet) {
                    shapelist = list(curnum,SYS(Shape));
                    for (uint i = 0; i < curnum; i++) {
                        Hue* hue = getHue(i);
                        shapelist.tuple()[i+1] = makeLine(curs[i], hue);
                        delete hue;
                    }
                } else {
                    var temp = tuple(curnum);
                    uint size_temp = 0;
                    for (uint i = 0; i < curnum; i++) {
                        temp.tuple()[i] = makeColorfulLines(curs[i]);
                        size_temp += (temp.tuple()[i].tuple().size - 1);
                    }

                    uint totalnum = 1;
                    if(size_temp == 0) {
                    }else {
                        shapelist = list(size_temp, SYS(Shape));
                        for(uint i = 0; i < curnum; ++i) {
                            for(uint j = 0; j < ((temp.tuple()[i]).tuple()).size - 1; ++j) {
                                shapelist.tuple()[totalnum] = temp.tuple()[i].tuple()[j + 1];
                                ++totalnum;
                            }
                        }
                    }

                }
                var opts = makeGlobalOptions();
                var graphics = makeGraphics(shapelist, opts);
                return graphics;
            } else if (filling == FILL_TO_CURVE) {
                var verts;
                var* line_indices = new var[curnum];
                makeDirectiveOfLines(curs, curnum, &verts, line_indices);
                var shapes = list(curnum + 1,SYS(Directive));
                for (uint i = 0; i < curnum; i++) {
                    var l = list(line_indices[i].tuple().size, SYS(Line));
                    for (size_t j = 0; j < (line_indices[i].tuple().size - 1); j++)
                        l.tuple()[j + 1] = line_indices[i].tuple()[j + 1];
                    var opt = makeHueOption(getHue(i));
                    l.tuple()[line_indices[i].tuple().size] = opt;
                    shapes.tuple()[i + 1] = l;
                }

                var poly;
                if(fillcur1 < curnum && fillcur2 < curnum) {
                    //poly = addVecReversed(line_indices[fillcur1].tuple()[1], line_indices[fillcur2].tuple()[1]); //fix it
                    if(curs[fillcur1]->WithDomain || curs[fillcur2]->WithDomain) {
                        poly = addvecreversedwithxrange(&line_indices, expression, verts, curs, fillcur1, fillcur2, curnum);
                    } else {
                        if(!curs[fillcur1]->singularity && !curs[fillcur2]->singularity) {
                            poly = addVecReversed(line_indices[fillcur1], line_indices[fillcur2],
                                expression.tuple()[fillcur1 + 1], expression.tuple()[fillcur2 + 1], verts); //fix it
                        } else if(curs[fillcur1]->singularity && !curs[fillcur2]->singularity) {
                            poly = addVecReversed(line_indices[fillcur2], line_indices[fillcur1], expression.tuple()[fillcur2 + 1],
                                expression.tuple()[fillcur1 + 1], verts, curs[fillcur2], curs[fillcur1], curs[fillcur1]->singularityset);
                        } else if(!curs[fillcur1]->singularity && curs[fillcur2]->singularity) {
                            poly = addVecReversed(line_indices[fillcur1], line_indices[fillcur2], expression.tuple()[fillcur1 + 1],
                                expression.tuple()[fillcur2 + 1], verts, curs[fillcur1], curs[fillcur2], curs[fillcur2]->singularityset);
                        }else {
                            poly=addVecReversed(curs[fillcur1],curs[fillcur2],line_indices[fillcur1],line_indices[fillcur2],expression.tuple()[fillcur1 + 1],
                                expression.tuple()[fillcur2 + 1], curs[fillcur1]->singularityset,curs[fillcur2]->singularityset,verts);
                        }
                    }
                    var opt = makeHueOption(getLightHue(0));
                    //shapes.tuple()[curnum + 1] = poly;
                    //shapes.tuple()[curnum + 2] = opt;
                    if(!curs[fillcur1]->singularity && !curs[fillcur2]->singularity && !curs[fillcur1]->WithDomain && !curs[fillcur2]->WithDomain) {
                        shapes.tuple()[curnum + 1] = list(poly,opt,SYS(Polygon));
                    } else {
                        var old_shapes = shapes;
                        shapes = list(old_shapes.tuple().size + poly.tuple().size - 3,SYS(Directive));
                        uint i = 0;
                        for(; i < old_shapes.tuple().size - 2; i++) {
                            shapes.tuple()[i + 1] = old_shapes.tuple()[i + 1];
                        }
                        for(;i < shapes.tuple().size - 1; i++) {
                            //opt = makeHueOption(getLightHue(i));
                            var temp_poly= poly.tuple()[i - (old_shapes.tuple().size - 3)];
                            var temp_shapes = list(temp_poly,opt,SYS(Polygon));
                            shapes.tuple()[i + 1] = temp_shapes;
                        }
                    }
                }
                delete[] line_indices;
                //var directive = list(shapes,SYS(Directive));
                //var complex = list(verts, directive, SYS(GraphicsComplex));
                var complex = list(verts, shapes, SYS(GraphicsComplex));
                //var complex = list(list(verts, directive), SYS(GraphicsComplex));
                var shape = list(complex, SYS(Shape));
                var opts = makeGlobalOptions();
                var graphics = makeGraphics(shape, opts);
                return graphics;
            } else if (filling == FILL_TO_AXES) {
                var verts;
                var* line_indices = new var[curnum+1];
                ArrayList<Point2d*> vaxis;
                vaxis.clear();
                vaxis.add(new Point2d(xmin, 0));
                vaxis.add(new Point2d(xmax, 0));
                Curve* axis = new Curve(vaxis);
                Curve** newcurves = new Curve*[curnum+1];

                for (int i = 0; i < curnum; i++)
                    newcurves[i] = curs[i];

                newcurves[curnum] = axis;
                //var expr_axis = list(new Integer(0L), key(_W("x")), $.Times);
                var expr_axis = list(new Integer(0L), xparam, $.Times);
                int curnum_bak = curnum++;
                makeDirectiveOfLines(newcurves, curnum, &verts, line_indices);

                var shapes = list(curnum + 1,SYS(Directive));
                for (uint i = 0; i < curnum; i++) {
                    var l = list(line_indices[i].tuple().size, SYS(Line));
                    for (size_t j = 0; j < (line_indices[i].tuple().size - 1); j++)
                        l.tuple()[j + 1] = line_indices[i].tuple()[j + 1];
                    var opt = makeHueOption(getHue(i));
                    l.tuple()[line_indices[i].tuple().size] = opt;
                    shapes.tuple()[i + 1] = l;
                }

                var poly;
                if(fillcur1 < curnum - 1) {
                    if(newcurves[fillcur1]->WithDomain) {
                        var expression_old = expression;
                        expression = list(expression_old.tuple().size);
                        for(uint i = 0; i < expression_old.tuple().size - 1; i++) {
                            expression.tuple()[i + 1] = expression_old.tuple()[i + 1];
                        }
                        expression.tuple()[expression_old.tuple().size] = expr_axis;
                        poly = addvecreversedwithxrange(&line_indices, expression, verts, newcurves, fillcur1, curnum - 1, curnum);
                        expression = expression_old;
                    } else {
                    if(!newcurves[fillcur1]->singularity) {
                        poly = addVecReversed(line_indices[fillcur1], line_indices[curnum - 1]); //fix it
                    } else {
                        poly = addVecReversed(line_indices[curnum - 1], line_indices[fillcur1],
                            expr_axis, expression.tuple()[fillcur1 + 1], verts,
                            newcurves[curnum - 1], newcurves[fillcur1], curs[fillcur1]->singularityset);
                    }
                    }
                    var opt = makeHueOption(getLightHue(0));
                    if(!newcurves[fillcur1]->singularity && !newcurves[fillcur1]->WithDomain) {
                        shapes.tuple()[curnum + 1] = list(poly,opt,SYS(Polygon));
                    } else {
                        var old_shapes = shapes;
                        shapes = list(old_shapes.tuple().size + poly.tuple().size - 3,SYS(Directive));
                        uint i = 0;
                        for(; i < old_shapes.tuple().size - 2; i++) {
                            shapes.tuple()[i + 1] = old_shapes.tuple()[i + 1];
                        }
                        for(;i < shapes.tuple().size - 1; i++) {
                            opt = makeHueOption(getLightHue(0));
                            var temp_poly= poly.tuple()[i - (old_shapes.tuple().size - 3)];
                            var temp_shapes = list(temp_poly,opt,SYS(Polygon));
                            shapes.tuple()[i + 1] = temp_shapes;
                        }
                    }
                }
                delete[] line_indices;
                var complex = list(verts, shapes, SYS(GraphicsComplex));
                var shape = list(complex, SYS(Shape));
                var opts = makeGlobalOptions();
                var graphics = makeGraphics(shape, opts);
                curnum = curnum_bak;
                return graphics;
            }

            return null;
        }

        void Plot::setRule(var title, var rule) {
            if (title == SYS(FillToCurve)) {
                filling = FILL_TO_CURVE;
                fillcur1 = toD(N(k, k.eval(rule.tuple()[1])).object());
                fillcur2 = toD(N(k, k.eval(rule.tuple()[2])).object());
		if (!(fillcur1 < curnum && fillcur2 < curnum) ||
		    !(fillcur1 >= 0 && fillcur2 >= 0)) {
		  filling = FILL_NULL;
		}
            } else if (title == SYS(FillToAxes)) {
                filling = FILL_TO_AXES;
                fillcur1 = toD(N(k, k.eval(rule.tuple()[1])).object());
		if (!(fillcur1 >= 0 && fillcur1 < curnum)) {
		  filling = FILL_NULL;
		}
            } else if (title == SYS(PlotRange)) { //plot range set or auto plot range
                if (rule.tuple().size == 3) {
                    rangeSet = 1;
                    ymin = toD(N(k, k.eval(rule.tuple()[1])).object());
                    ymax = toD(N(k, k.eval(rule.tuple()[2])).object());
                } else {
                    rangeSet = 2; //auto range
                }
            } else if (title == SYS(ColorFunction)) { //has color function
                colorFunctionSet = true;
                var fun = rule;
                cf = new F1P(k,fun, xparam);
            }
            //to do color function in hue(f1,f2,f3) or color(f1,f2,f3)
            setCommonRule(title, rule);
        }
        
        var Plot::addvecreversedwithxrange(var **line_indices_f, var expr_f, var &verts_f, Curve **curs_f, int fillcur1_f, int fillcur2_f, int curnum_f) {
            var poly;
            if(curs_f[fillcur1_f]->WithDomain && !curs_f[fillcur2_f]->WithDomain) {
                double xmax_old = xmax;
                double xmin_old = xmin;
                var r, temp;
                temp = list(curs_f[fillcur1_f]->Domains.size());
                uint total = 0;
                for(uint i = 0; i < curs_f[fillcur1_f]->Domains.size(); i++) {
                    xmin = curs_f[fillcur1_f]->Domains[i]->min;
                    xmax = curs_f[fillcur1_f]->Domains[i]->max;
                    r = addVecReversed(line_indices_f, expr_f, verts_f, curs_f, fillcur1_f, fillcur2_f, curnum_f, i);
                    total += r.tuple().size - 1;
                    temp.tuple()[i + 1] = r;
                }
 
                poly = list(total);
                uint count = 1;
                for(uint i = 0; i < temp.tuple().size - 1; i++) {
                    for(uint j = 0; j < temp.tuple()[i + 1].tuple().size - 1; j++) {
                        poly.tuple()[count++] = temp.tuple()[i + 1].tuple()[j + 1];
                    }
                }
                xmax = xmax_old;
                xmin = xmin_old;
                return poly;
            } else if(!curs_f[fillcur1_f]->WithDomain && curs_f[fillcur2_f]->WithDomain) {
                double xmax_old = xmax;
                double xmin_old = xmin;
                var r, temp;
                temp = list(curs_f[fillcur2_f]->Domains.size());
                uint total = 0;
                for(uint i = 0; i < curs_f[fillcur2_f]->Domains.size(); i++) {
                    xmin = curs_f[fillcur2_f]->Domains[i]->min;
                    xmax = curs_f[fillcur2_f]->Domains[i]->max;
                    r = addVecReversed(line_indices_f, expr_f, verts_f, curs_f, fillcur2_f, fillcur1_f, curnum_f, i);
                    total += r.tuple().size - 1;
                    temp.tuple()[i + 1] = r;
                }

                poly = list(total);
                uint count = 1;
                for(uint i = 0; i < temp.tuple().size - 1; i++) {
                    for(uint j = 0; j < temp.tuple()[i + 1].tuple().size - 1; j++) {
                        poly.tuple()[count++] = temp.tuple()[i + 1].tuple()[j + 1];
                    }
                }
                xmax = xmax_old;
                xmin = xmin_old;
                return poly;
            } else if(curs_f[fillcur1_f]->WithDomain && curs_f[fillcur2_f]->WithDomain)
			{
				double max_old=xmax;
				double min_old=xmin;
				var r,temp;
				uint sum=0;

				for(uint i = 0; i < curs_f[fillcur1_f]->Domains.size(); i++) {
					for(uint j=0;j<curs_f[fillcur2_f]->Domains.size();j++)
					{
						double x_min1=curs_f[fillcur1_f]->Domains[i]->min;
						double x_min2=curs_f[fillcur2_f]->Domains[j]->min;
						double x_max1=curs_f[fillcur1_f]->Domains[i]->max;
						double x_max2=curs_f[fillcur2_f]->Domains[j]->max;
						if((x_min1>=x_max2)||(x_max1<=x_min2))
							continue;
						else
							sum++;
					}
                    //xmin = curs_f[fillcur2_f]->Domains[i]->min;
                    //xmax = curs_f[fillcur2_f]->Domains[i]->max;
                   /* r = addVecReversed(line_indices_f, expr_f, verts_f, curs_f, fillcur2_f, fillcur1_f, curnum_f, i);
                    total += r.tuple().size - 1;
                    temp.tuple()[i + 1] = r;*/
                }
                temp=list(sum); 
				uint total = 0;
                for(uint i = 0; i < curs_f[fillcur1_f]->Domains.size(); i++) {
                    for(uint j=0;j<curs_f[fillcur2_f]->Domains.size();j++)
					{
						double x_min1=curs_f[fillcur1_f]->Domains[i]->min;
						double x_min2=curs_f[fillcur2_f]->Domains[j]->min;
						double x_max1=curs_f[fillcur1_f]->Domains[i]->max;
						double x_max2=curs_f[fillcur2_f]->Domains[j]->max;
						if((x_min1>=x_max2)||(x_max1<=x_min2))
							continue;
						else
						{
							if(x_min1<x_min2)
								xmin=x_min2;
							else
								xmin=x_min1;
							if(x_max1<x_max2)
								xmax=x_max1;
							else
								xmax=x_max2;
							r = addVecReversed(line_indices_f, expr_f, verts_f, curs_f, fillcur2_f, fillcur1_f, curnum_f, i,j);
							total += r.tuple().size - 1;
                            temp.tuple()[i + 1] = r;
						}
					}
                }
				poly = list(total);
                uint count = 1;
                for(uint i = 0; i < temp.tuple().size - 1; i++) {
                    for(uint j = 0; j < temp.tuple()[i + 1].tuple().size - 1; j++) {
                        poly.tuple()[count++] = temp.tuple()[i + 1].tuple()[j + 1];
                    }
                }
                xmax = max_old;
                xmin = min_old;
                return poly;
			}
            return null;
        }

        var Plot::addVecReversed(var **line_indices_f, var expr_f, var &verts_f, Curve ** curs_f, int fillcur1_f, int fillcur2_f, int curnum_f, uint i) {
            var poly;
            std::vector<singularitynode *> singularityset_f;
            if(curs_f[fillcur2_f]->singularity) {
                for(uint i = 0; i < curs_f[fillcur2_f]->singularityset.size(); i++) {
                    if(curs_f[fillcur2_f]->singularityset[i]->x <= xmax && curs_f[fillcur2_f]->singularityset[i]->x >= xmin) {
                        singularityset_f.push_back(curs_f[fillcur2_f]->singularityset[i]);
                    }
                }
            }
            if(curs_f[fillcur1_f]->Domains[i]->singularityset.size() == 0 && singularityset_f.size() == 0) {
                var poly_temp;
                poly_temp = addVecReversed((*line_indices_f)[fillcur1_f], (*line_indices_f)[fillcur2_f],
                    expr_f.tuple()[fillcur1_f + 1], expr_f.tuple()[fillcur2_f + 1], verts_f); //fix it
                poly = list(poly_temp);
            } else if(curs_f[fillcur1_f]->Domains[i]->singularityset.size() != 0 && singularityset_f.size() == 0) {
                poly = addVecReversed((*line_indices_f)[fillcur2_f], (*line_indices_f)[fillcur1_f], expr_f.tuple()[fillcur2_f + 1],
                    expr_f.tuple()[fillcur1_f + 1], verts_f, curs_f[fillcur2_f], curs_f[fillcur1_f], curs_f[fillcur1_f]->Domains[i]->singularityset);
            } else if(curs_f[fillcur1_f]->Domains[i]->singularityset.size() == 0 && singularityset_f.size() != 0) {
                poly = addVecReversed((*line_indices_f)[fillcur1_f], (*line_indices_f)[fillcur2_f], expr_f.tuple()[fillcur1_f + 1],
                    expr_f.tuple()[fillcur2_f + 1], verts_f, curs_f[fillcur1_f], curs_f[fillcur2_f], singularityset_f);
            }else {
                poly=addVecReversed(curs_f[fillcur1_f],curs_f[fillcur2_f],(*line_indices_f)[fillcur1_f],(*line_indices_f)[fillcur2_f],expr_f.tuple()[fillcur1_f + 1],
                    expr_f.tuple()[fillcur2_f + 1], curs_f[fillcur1_f]->Domains[i]->singularityset,singularityset_f,verts_f);
            }
            return poly;
        }

		var Plot::addVecReversed(var **line_indices_f, var expr_f, var &verts_f, Curve ** curs_f, int fillcur1_f, int fillcur2_f, int curnum_f, uint i,uint j)
		{
			var poly;
            std::vector<singularitynode *> singularityset_f1;
            std::vector<singularitynode *> singularityset_f2;

			for(uint fi = 0; fi < curs_f[fillcur1_f]->Domains[i]->singularityset.size(); fi++) {
				if(curs_f[fillcur1_f]->Domains[i]->singularityset[fi]->x <= xmax && curs_f[fillcur1_f]->Domains[i]->singularityset[fi]->x >= xmin) {
					singularityset_f1.push_back(curs_f[fillcur1_f]->Domains[i]->singularityset[fi]);
                    }
                }
			for(uint fi = 0; fi < curs_f[fillcur2_f]->Domains[j]->singularityset.size(); fi++) {
				if(curs_f[fillcur2_f]->Domains[j]->singularityset[fi]->x <= xmax && curs_f[fillcur2_f]->Domains[j]->singularityset[fi]->x >= xmin) {
					singularityset_f2.push_back(curs_f[fillcur2_f]->Domains[j]->singularityset[fi]);
                    }
                }
			if(singularityset_f2.size()== 0 && singularityset_f1.size() == 0) {
                var poly_temp;
                poly_temp = addVecReversed((*line_indices_f)[fillcur1_f], (*line_indices_f)[fillcur2_f],
                    expr_f.tuple()[fillcur1_f + 1], expr_f.tuple()[fillcur2_f + 1], verts_f); //fix it
                poly = list(poly_temp);
            } else if(singularityset_f2.size() == 0 && singularityset_f1.size() != 0) {
                poly = addVecReversed((*line_indices_f)[fillcur2_f], (*line_indices_f)[fillcur1_f], expr_f.tuple()[fillcur2_f + 1],
                    expr_f.tuple()[fillcur1_f + 1], verts_f, curs_f[fillcur2_f], curs_f[fillcur1_f], singularityset_f1);
			} else if(singularityset_f2.size() != 0 && singularityset_f1.size() == 0) {
                poly = addVecReversed((*line_indices_f)[fillcur1_f], (*line_indices_f)[fillcur2_f], expr_f.tuple()[fillcur1_f + 1],
                    expr_f.tuple()[fillcur2_f + 1], verts_f, curs_f[fillcur1_f], curs_f[fillcur2_f], singularityset_f2);
            }else {
                poly=addVecReversed(curs_f[fillcur1_f],curs_f[fillcur2_f],(*line_indices_f)[fillcur1_f],(*line_indices_f)[fillcur2_f],expr_f.tuple()[fillcur1_f + 1],
                    expr_f.tuple()[fillcur2_f + 1], singularityset_f1,singularityset_f2, verts_f);
            }
            return poly;
		}

        var Plot::addVecReversed(var vec1, var vec2, var expr1, var expr2, var &out_verts) {
            uint totaldata = 0;

            var old_out_verts = out_verts;
            out_verts = list(old_out_verts.tuple().size + 3);

            for(uint i = 0; i < old_out_verts.tuple().size - 1; i++) {
                out_verts.tuple()[i + 1] = old_out_verts.tuple()[i + 1];
            }

            F1P f1 = F1P(k, expr1, xparam);
            F1P f2 = F1P(k, expr2, xparam);

            out_verts.tuple()[out_verts.tuple().size - 4] = tuple($.List, new Real(xmax, 0), new Real(ymax, 0));
            out_verts.tuple()[out_verts.tuple().size - 3] = tuple($.List, new Real(xmax, 0), new Real(ymin, 0));
            out_verts.tuple()[out_verts.tuple().size - 2] = tuple($.List, new Real(xmin, 0), new Real(ymax, 0));
            out_verts.tuple()[out_verts.tuple().size - 1] = tuple($.List, new Real(xmin, 0), new Real(ymin, 0));

            for(uint i = 0; i < vec1.tuple().size - 1; i++) {
                totaldata += (vec1.tuple()[i + 1].tuple().size - 1);
            }

            //y1,y2分别为对应1号和2号函数在x边界处的函数值
            double y1 = f1.getSingleData(xmax);
            double y2 = f2.getSingleData(xmax);
            if(!((y1 > ymax && y2 > ymax) || (y1 < ymin && y2 < ymin) || (( y1 > ymin) && (y1 < ymax)) && ((y2 > ymin) && (y2 < ymax)))) {
                if(y1 >= ymax) {
                    totaldata++;
                    if(y2 <= ymin) {
                        totaldata++;
                    }
                }else if(y1 <= ymin) {
                    totaldata++;
                    if(y2 >= ymax) {
                        totaldata++;
                    }
                }else {
                    if(y2 >= ymax) {
                        totaldata++;
                    }else if(y2 <= ymin) {
                        totaldata++;
                    }
                }
            }

            for(uint i = 0; i < vec2.tuple().size - 1; i++) {
                totaldata += (vec2.tuple()[i + 1].tuple().size - 1);
            }

            y1 = f1.getSingleData(xmin);
            y2 = f2.getSingleData(xmin);
            if(!((y1 > ymax && y2 > ymax) || (y1 < ymin && y2 < ymin) || (( y1 > ymin) && (y1 < ymax)) && ((y2 > ymin) && (y2 < ymax)))) {
                if(y2 >= ymax) {
                    totaldata++;
                    if(y1 <= ymin) {
                        totaldata++;
                    }
                }else if(y2 <= ymin) {
                    totaldata++;
                    if(y1 >= ymax) {
                        totaldata++;
                    }
                }else {
                    if(y1 >= ymax) {
                        totaldata++;
                    }else if(y1 <= ymin) {
                        totaldata++;
                    }
                }
            }

            var v = list(totaldata);
            uint temp = 1;
            for(uint i = 0; i < vec1.tuple().size - 1; i++) {
                for(uint j = 0; j < vec1.tuple()[i + 1].tuple().size - 1; j++) {
                    v.tuple()[temp++] = vec1.tuple()[i + 1].tuple()[j + 1];
                }
            }

            y1 = f1.getSingleData(xmax);
            y2 = f2.getSingleData(xmax);
            if(!((y1 > ymax && y2 > ymax) || (y1 < ymin && y2 < ymin) || (( y1 > ymin) && (y1 < ymax)) && ((y2 > ymin) && (y2 < ymax)))) {
                if(y1 >= ymax) {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 5));
                    if(y2 <= ymin) {
                        v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 4));
                    }
                }else if(y1 <= ymin) {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 4));
                    if(y2 >= ymax) {
                        v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 5));
                    }
                }else {
                    if(y2 >= ymax) {
                        v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 5));
                    }else if(y2 <= ymin){
                        v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 4));
                    }
                }
            }

            for(uint i = vec2.tuple().size - 1; i > 0; i--) {
                for(uint j = vec2.tuple()[i].tuple().size - 1; j > 0; j--) {
                    v.tuple()[temp++] = vec2.tuple()[i].tuple()[j];
                }
            }

            y1 = f1.getSingleData(xmin);
            y2 = f2.getSingleData(xmin);
            if(!((y1 > ymax && y2 > ymax) || (y1 < ymin && y2 < ymin) || (( y1 > ymin) && (y1 < ymax)) && ((y2 > ymin) && (y2 < ymax)))) {
                if(y2 >= ymax) {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 3));
                    if(y1 <= ymin) {
                        v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 2));
                    }
                }else if(y2 <= ymin) {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 2));
                    if(y1 >= ymax) {
                        v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 3));
                    }
                }else {
                    if(y1 >= ymax) {
                        v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 3));
                    }else if(y1 <= ymin){
                        v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 2));
                    }
                }
            }
            //return v;
            return v;
        }

        var Plot::addVecReversed(var vec1, var vec2) {
            uint totaldata = 0;

            for(uint i = 0; i < vec1.tuple().size - 1; i++) {
                totaldata += (vec1.tuple()[i + 1].tuple().size - 1);
            }
            for(uint i = 0; i < vec2.tuple().size - 1; i++) {
                totaldata += (vec2.tuple()[i + 1].tuple().size - 1);
            }

            var v = list(totaldata);
            uint temp = 1;
            for(uint i = 0; i < vec1.tuple().size - 1; i++) {
                for(uint j = 0; j < vec1.tuple()[i + 1].tuple().size - 1; j++) {
                    v.tuple()[temp++] = vec1.tuple()[i + 1].tuple()[j + 1];
                }
            }

            for(uint i = vec2.tuple().size - 1; i > 0; i--) {
                for(uint j = vec2.tuple()[i].tuple().size - 1; j > 0; j--) {
                    v.tuple()[temp++] = vec2.tuple()[i].tuple()[j];
                }
            }

            return v;
        }

        //
        //
        //判定奇点，并限定ymin和ymax
        //
        //
        bool Plot::hassingularity(F1P *f, std::vector<singularitynode *> &singularityset,
            double &ymin, double &ymax, std::vector<domain *> &nanrangeset,
            double spx, double spy,
            double graphScale, double dx) {

            bool singularity = false;
            //std::vector<singularitynode *> singularityset;
            int bigDataNum = 500;
			double *x = new double[bigDataNum];
			double *y = new double[bigDataNum];
			for (int i = 0; i < bigDataNum; i++) {
				x[i] = xmin + (xmax - xmin) * i / (bigDataNum - 1);
			}
			f->getArrayData(x, y, bigDataNum);
			ArrayList<Point2d*> vert;
			ArrayList<double> d;
			for (int i = 0; i < bigDataNum; i++) {
				vert.add(new Point2d(x[i], y[i]));
			}
			delete [] x;
			delete [] y;
            uint i = 0;
            ymin = ymax = 0L;
            for(; i < vert.size(); i++) {
                if(!isNaN(vert[i]->y)) {
                    ymin = vert[i]->y;
                    ymax = vert[i]->y;
                    break;
                }
            }
			for (; i < vert.size(); i++) {
                if(!isNaN(vert[i]->y)) {
                    if (vert[i]->y > ymax) ymax = vert[i]->y;
                    if (vert[i]->y < ymin) ymin = vert[i]->y;
                }
			}
			spx = xmax - xmin;
			spy = ymax - ymin;
			graphScale = spy / spx;
			dx = spx / LARGE;
			for (unsigned int i = 0; i < vert.size(); i++) {
				d.add(f->getDerivative(vert[i]->x, dx));
			}

            //
            //去除斜率太大的点
            //
			bool vrm = true; //vert removed
            bool isnanfunction = false;
            std::vector<bool> flag(vert.size(), false); //if true, the vert's derivative is too big
            std::vector<bool> nanflag(vert.size(), false);  //if true, the vert's value is NaN
            //std::vector<domain> nanrangeset;    //the set of one curve's range
            std::vector<uint> nannodesite;  //the site of the node which is the border

			int count = 0;
			while (vrm) {
				vrm = false;
				for (unsigned int i = 0; i < vert.size(); i++) {
                    if(isNaN(d[i])) {
                        flag[i] = true;
                        nanflag[i] = true;
                        isnanfunction = true;
                    } else {
                        if(!flag[i]) {
                            if(graphScale != 0) {
                                if (std::abs(d[i]) / graphScale > MAX_DERIVATIVE) {
                                    //vert.remove(i);
                                    //d.remove(i);
                                    singularity = true;
                                    flag[i] = true;
                                    vrm = true;
                                }
                            }
                        }
                    }
				}

                getyrange(ymax, ymin, graphScale, spx, spy, vert, flag, nanflag);

                if(graphScale < MIN_GRAPHSCALE_MINIMUM_CURVE) {
                    break;
                }
				if(count >= MAX_DELETE_AMOUNT) {
					break;
				}
				count++;
			}

            //
            //探测函数的定义域
            //
            if(isnanfunction) {
                detectnanborder(f, nanrangeset, nannodesite, nanflag, flag, vert);
            }

            //
            //判定奇点的位置
            //
            if(singularity) {
                if(isnanfunction) {
                    for(uint i = 0; i < nanrangeset.size(); i++) {
                        if(flag[nannodesite[i * 2]] != nanflag[nannodesite[i * 2]]) {
                            if(nannodesite[i * 2] < nannodesite[i * 2 + 1]) {
                                nannodesite[i * 2]++;
                            }
                        }
                        if(flag[nannodesite[i * 2 + 1]] != nanflag[nannodesite[i * 2 + 1]]) {
                            if(nannodesite[i * 2 + 1] > nannodesite[i * 2]) {
                                nannodesite[i * 2 + 1]--;
                            }
                        }
                        getsingularityset(flag.begin() + nannodesite[i * 2], nanflag.begin() + nannodesite[i * 2], nannodesite[i * 2 + 1] - nannodesite[i * 2] + 1,
                            nanrangeset[i]->singularityset, nanrangeset[i]->max, nanrangeset[i]->min, d.begin() + nannodesite[i * 2], vert.begin() + nannodesite[i * 2]);
                    }
                } else {
                    getsingularityset(flag.begin(), nanflag.begin(), vert.size(), singularityset, xmax, xmin, d.begin(), vert.begin());
                }

                //for(uint i = 0; i < singularityset.size(); i++) {
                //    std::cout << singularityset[i]->x << ' ' << singularityset[i]->leftd << ' ' << singularityset[i]->rightd << std::endl;
                //}
            }

            for(ArrayList<Point2d*>::iterator it = vert.begin(); it != vert.end(); it++) {
                delete (*it);
            }
            vert.clear();

            return singularity;
        }

        void Plot::getsingularityset(std::vector<bool>::iterator flag, std::vector<bool>::iterator nanflag, uint size,std::vector<singularitynode *> &singularityset,
            double t_xmax, double t_xmin, ArrayList<double>::iterator d, ArrayList<Point2d *>::iterator vert) {
                singularitynode *temp;
                if(flag[0]) {
                    temp = new singularitynode;
                    temp->x = t_xmin;
                    uint m = 1;
                    for(; m < size - 1 && flag[m]; m++) {
                    }

                    if(m != size - 1) {
                        temp->lefty = vert[m-1]->y;
                        temp->leftd = d[m-1];
                        temp->righty = vert[m-1]->y;
                        temp->rightd = d[m-1];
                    }else {
                        temp->lefty = vert[0]->y;
                        temp->leftd = d[0];
                        temp->righty = vert[0]->y;
                        temp->rightd = d[0];
                    }
                    singularityset.push_back(temp);
                }

                uint k = 0;

                for(; k < size - 1; k++) {
                    if(!flag[k] && flag[k + 1]) {
                        uint j = k + 1;
                        for(; j < size - 1; j++) {
                            if(flag[j] && !flag[j + 1]) {
                                temp = new singularitynode;
                                temp->x = (vert[k]->x + vert[j + 1]->x) / 2.0;
                                temp->lefty = vert[k]->y;
                                temp->leftd = d[k];
                                temp->righty = vert[j + 1]->y;
                                temp->rightd = d[j + 1];
                                singularityset.push_back(temp);
                                k = j;
                                break;
                            }
                        }
                        if(j == (size - 1) && flag[size - 1]) {
                            temp = new singularitynode;
                            //temp->x = vert[size - 1]->x;
                            temp->x = t_xmax;
                            temp->lefty = vert[k]->y;
                            temp->leftd = d[k];
                            temp->righty = vert[k]->y;
                            temp->rightd = d[k];
                            singularityset.push_back(temp);
                        }
                    }
                }
        }

        void Plot::getyrange(double &ymax, double &ymin, double &graphScale, double &spx, double &spy, 
            const std::vector<Point2d *> &vert, const std::vector<bool> &flag, const std::vector<bool> &nanflag) {
            for(uint i = 0; i < vert.size(); i++) {
                if(!flag[i] && !nanflag[i]) {
                    ymin = vert[i]->y;
                    ymax = vert[i]->y;
                    break;
                }
            }
            for (uint i = 0; i < vert.size(); i++) {
                if(!flag[i] && !nanflag[i]) {
                    if (vert[i]->y > ymax) ymax = vert[i]->y;
                    if (vert[i]->y < ymin) ymin = vert[i]->y;
                }
            }
            spy = ymax - ymin;
            graphScale = spy / spx;
        }

        void Plot::detectnanborder(F1P *f, std::vector<domain *> &nanrangeset, std::vector<uint> &nannodesite,
            const std::vector<bool> &nanflag, const std::vector<bool> &derivativeflag,
            const std::vector<Point2d *> &vert) {
                double xright;
                uint deepth = 5;
                domain *node;
                bool locationflag = false;
                for(uint i = 0; i < nanflag.size() - 1; i++) {
                    if(nanflag[i] && !nanflag[i + 1]) {
                        double x = (vert[i]->x + vert[i + 1]->x) / 2;
                        double y = f->getSingleData(x);
                        if(isNaN(y)) {
                            xright = getrightborder(f, x, vert[i + 1]->x, deepth);
                        } else {
                            xright = getrightborder(f, vert[i]->x, x, deepth);
                        }
                        locationflag = true;
                        node = new domain;
                        node->min = xright;
                        nannodesite.push_back(i + 1);
                    } else if(!nanflag[i] && nanflag[i + 1]) {
                        double x = (vert[i]->x + vert[i + 1]->x) / 2;
                        double y = f->getSingleData(x);
                        if(isNaN(y)) {
                            xright = getrightborder(f, x, vert[i]->x, deepth);
                        } else {
                            xright = getrightborder(f, vert[i + 1]->x, x, deepth);
                        }
                        if(!locationflag) {
                            node = new domain;
                            node->min = vert[0]->x;
                            nannodesite.push_back(0);
                        }
                        locationflag = false;
                        node->max = xright;
                        nannodesite.push_back(i);
                        nanrangeset.push_back(node);
                    }
                }

                if(locationflag) {
                    node->max = vert[vert.size() - 1]->x;
                    nannodesite.push_back(vert.size() - 1);
                    nanrangeset.push_back(node);
                }
        }

        double Plot::getrightborder(F1P *f, double lx, double rx, uint deepth) {
            double x = (lx + rx) / 2.0;
            double y = f->getSingleData(x);
            double xright;
            if(deepth <= 0) {
                if(!isNaN(y)) {
                    return x;
                } else {
                    return rx;
                }
            }
            if(isNaN(y)) {
                xright = getrightborder(f, x, rx, --deepth);
            } else {
                xright = getrightborder(f, lx, x, --deepth);
            }
            return xright;
        }

        var Plot::addVecReversed(var vec1, var vec2, 
            var expr1, var expr2, 
            var &out_verts, Curve * cur1, Curve * cur2,
            const std::vector<singularitynode *> &singularityset) {

                uint totaldata = 0;
                var old_out_verts = out_verts;
                //size of out_verts equal old_out_verts的大小加上
                //每两个相邻分界点处奇函数对应的两个ymax和两个ymin，和此两处连续函数对应的y值 加上
                //两个边界点处两个ymax和两个ymin
                out_verts = list(old_out_verts.tuple().size + (singularityset.size() * 6) + 5);

                for(uint i = 0; i < old_out_verts.tuple().size - 1; i++) {
                    out_verts.tuple()[i + 1] = old_out_verts.tuple()[i + 1];
                }

                F1P f1 = F1P(k, expr1, xparam);
                F1P f2 = F1P(k, expr2, xparam);

                double old_xmin = xmin, old_xmax = xmax;
                double old_ymin = ymin, old_ymax = ymax;
                double xmax_sy, xmin_sy;
                var vtotal = list(singularityset.size() + 1);
                uint z = 0;
                for(; z < (singularityset.size() + 1); ) {
                    do {
                        if(z == 0) {
                            xmin = old_xmin;
                            xmax = singularityset[z]->x;

                            if(singularityset[z]->leftd > 0.0) {
                                xmax_sy = ymax;
                            } else {
                                xmax_sy = ymin;
                            }
                            if(xmin == xmax) {
                                if(singularityset[z]->rightd > 0.0) {
                                    xmax_sy = xmin_sy = ymin;
                                } else {
                                    xmax_sy = xmin_sy = ymax;
                                }
                                //xmin_sy = xmax_sy;
                            } else {
                                if(!isNaN(f2.getSingleData(xmin))) {
                                    xmin_sy = f2.getSingleData(xmin);
                                } else {
                                    singularitynode node;
                                    getsimilarvalue(&f2, xmin, node);
                                    if(node.rightd > 0.0) {
                                        xmin_sy = ymin;
                                    } else if(node.rightd < 0.0) {
                                        xmin_sy = ymax;
                                    }
                                }
                            }
                            break;
                        }

                        if(z != (singularityset.size())) {
                            xmin = singularityset[z - 1]->x;
                            xmax = singularityset[z]->x;
                            if(singularityset[z - 1]->rightd > 0.0) {
                                xmin_sy = ymin;
                            } else {
                                xmin_sy = ymax;
                            }
                            if(singularityset[z]->leftd > 0.0) {
                                xmax_sy = ymax;
                            } else {
                                xmax_sy = ymin;
                            }

                            break;
                        }

                        xmin = singularityset[z - 1]->x;
                        xmax = old_xmax;
                        if(singularityset[z - 1]->rightd > 0.0) {
                            xmin_sy = ymin;
                        } else {
                            xmin_sy = ymax;
                        }

                        if(xmin == xmax) {
                            if(singularityset[z - 1]->leftd > 0.0) {
                                xmin_sy = xmax_sy = ymax;
                            } else {
                                xmin_sy = xmax_sy = ymin;
                            }
                            //xmax_sy = xmin_sy;
                        } else {
                            xmax_sy = f2.getSingleData(xmax);
                            if(!isNaN(xmax_sy)) {
                            } else {
                                singularitynode node;
                                getsimilarvalue(&f2, xmax, node);
                                if(node.leftd > 0.0) {
                                    xmax_sy = ymax;
                                } else if(node.leftd < 0.0) {
                                    xmax_sy = ymin;
                                }
                            }
                        }
                    }while(0);

                    totaldata = 0;
                    double y_xmax_1 = f1.getSingleData(xmax), y_xmin_1 = f1.getSingleData(xmin);
                    if(isNaN(y_xmax_1)) {
                        singularitynode node;
                        getsimilarvalue(&f1, xmax, node);
                        if(node.leftd >= 0.0) {
                            y_xmax_1 = ymax;
                        } else if(node.leftd < 0.0) {
                            y_xmax_1 = ymin;
                        }
                    }
                    if(isNaN(y_xmin_1)) {
                        singularitynode node;
                        getsimilarvalue(&f1, xmin, node);
                        if(node.rightd >= 0.0) {
                            y_xmin_1 = ymin;
                        } else if(node.rightd < 0.0) {
                            y_xmin_1 = ymax;
                        }
                    }
                    out_verts.tuple()[z*6 + 0 + old_out_verts.tuple().size] = tuple($.List, new Real(xmax, 0), new Real(ymax, 0));
                    out_verts.tuple()[z*6 + 1 + old_out_verts.tuple().size] = tuple($.List, new Real(xmax, 0), new Real(ymin, 0));
                    out_verts.tuple()[z*6 + 2 + old_out_verts.tuple().size] = tuple($.List, new Real(xmin, 0), new Real(ymax, 0));
                    out_verts.tuple()[z*6 + 3 + old_out_verts.tuple().size] = tuple($.List, new Real(xmin, 0), new Real(ymin, 0));
                    out_verts.tuple()[z*6 + 4 + old_out_verts.tuple().size] = tuple($.List, new Real(xmax, 0), new Real(y_xmax_1, 0));
                    out_verts.tuple()[z*6 + 5 + old_out_verts.tuple().size] = tuple($.List, new Real(xmin, 0), new Real(y_xmin_1, 0));

                    for(uint i = 0; i < vec1.tuple().size - 1; i++) {
                        if(vec1.tuple()[i + 1].tuple().size > 1) {
                            if(cur1->lineStrips[i]->vert[0]->x > xmax || cur1->lineStrips[i]->vert[cur1->lineStrips[i]->vert.size() - 1]->x < xmin) {
                                continue;
                            } else {
                                for(uint j = 0; j < vec1.tuple()[i + 1].tuple().size - 1; j++) {
                                    if(cur1->lineStrips[i]->vert[j]->x <= xmax && cur1->lineStrips[i]->vert[j]->x >= xmin) {
                                        totaldata++;
                                    }
                                }
                            }
                        }
                    }
                    //y1,y2分别为对应1号和2号函数在x边界处的函数值
                    double y1 = f1.getSingleData(xmax);
                    if(!isNaN(y1)) {
                    } else {
                        singularitynode node;
                        getsimilarvalue(&f1, xmax, node);
                        if(node.leftd > 0.0) {
                            y1 = ymax;
                        } else if(node.leftd < 0.0) {
                            y1 = ymin;
                        }
                    }
                    double y2 = xmax_sy;
                    if(y1 <= ymax && y1 >= ymin) {
                        totaldata++;
                    }
                    if(!((y1 > ymax && y2 > ymax) || (y1 < ymin && y2 < ymin) || (( y1 > ymin) && (y1 < ymax)) && ((y2 > ymin) && (y2 < ymax)))) {
                        if(y1 >= ymax) {
                            totaldata++;
                            if(y2 <= ymin) {
                                totaldata++;
                            }
                        }else if(y1 <= ymin) {
                            totaldata++;
                            if(y2 >= ymax) {
                                totaldata++;
                            }
                        }else {
                            if(y2 >= ymax) {
                                totaldata++;
                            }else if(y2 <= ymin) {
                                totaldata++;
                            }
                        }
                    }

                    for(uint i = vec2.tuple().size - 1; i > 0; i--) {
                        if(vec2.tuple()[i].tuple().size > 1) {
                            if(cur2->lineStrips[i - 1]->vert[0]->x > xmax || cur2->lineStrips[i - 1]->vert[cur2->lineStrips[i - 1]->vert.size() - 1]->x < xmin) {
                                continue;
                            } else {
                                for(uint j = vec2.tuple()[i].tuple().size - 1; j > 0; j--) {
                                    if(cur2->lineStrips[i - 1]->vert[j - 1]->x <= xmax && cur2->lineStrips[i - 1]->vert[j - 1]->x >= xmin) {
                                        totaldata++;
                                    }
                                }
                            }
                        }
                    }
                    y1 = f1.getSingleData(xmin);
                    if(!isNaN(y1)) {
                    } else {
                        singularitynode node;
                        getsimilarvalue(&f1, xmin, node);
                        if(node.rightd > 0.0) {
                            y1 = ymin;
                        } else if(node.rightd < 0.0) {
                            y1 = ymax;
                        }
                    }
                    y2 = xmin_sy;
                    if(!((y1 > ymax && y2 > ymax) || (y1 < ymin && y2 < ymin) || (( y1 > ymin) && (y1 < ymax)) && ((y2 > ymin) && (y2 < ymax)))) {
                        if(y2 >= ymax) {
                            totaldata++;
                            if(y1 <= ymin) {
                                totaldata++;
                            }
                        }else if(y2 <= ymin) {
                            totaldata++;
                            if(y1 >= ymax) {
                                totaldata++;
                            }
                        }else {
                            if(y1 >= ymax) {
                                totaldata++;
                            }else if(y1 <= ymin) {
                                totaldata++;
                            }
                        }
                    }
                    if(y1 <= ymax && y1 >= ymin) {
                        totaldata++;
                    }

                    var v = list(totaldata);
                    uint temp = 1;
 
                    for(uint i = 0; i < vec1.tuple().size - 1; i++) {
                        if(vec1.tuple()[i + 1].tuple().size > 1) {
                            if(cur1->lineStrips[i]->vert[0]->x > xmax || cur1->lineStrips[i]->vert[cur1->lineStrips[i]->vert.size() - 1]->x < xmin) {
                                continue;
                            } else {
                                for(uint j = 0; j < vec1.tuple()[i + 1].tuple().size - 1; j++) {
                                    if(cur1->lineStrips[i]->vert[j]->x <= xmax && cur1->lineStrips[i]->vert[j]->x >= xmin) {
                                        v.tuple()[temp++] = vec1.tuple()[i + 1].tuple()[j + 1];
                                    }
                                }
                            }
                        }
                    }

                    y1 = f1.getSingleData(xmax);
                    if(!isNaN(y1)) {
                    } else {
                        singularitynode node;
                        getsimilarvalue(&f1, xmax, node);
                        if(node.leftd > 0.0) {
                            y1 = ymax;
                        } else if(node.leftd < 0.0) {
                            y1 = ymin;
                        }
                    }
                    y2 = xmax_sy;
                    if(y1 <= ymax && y1 >= ymin) {
                        v.tuple()[temp++] = new Integer((long)(z*6 + 3 + old_out_verts.tuple().size));
                    }
                    if(!((y1 > ymax && y2 > ymax) || (y1 < ymin && y2 < ymin) || (( y1 > ymin) && (y1 < ymax)) && ((y2 > ymin) && (y2 < ymax)))) {
                        if(y1 >= ymax) {
                            v.tuple()[temp++] = new Integer((long)(z*6 - 1 + old_out_verts.tuple().size));
                            if(y2 <= ymin) {
                                v.tuple()[temp++] = new Integer((long)(z*6 + old_out_verts.tuple().size));
                            }
                        }else if(y1 <= ymin) {
                            v.tuple()[temp++] = new Integer((long)(z*6 + old_out_verts.tuple().size));
                            if(y2 >= ymax) {
                                v.tuple()[temp++] = new Integer((long)(z*6 - 1 + old_out_verts.tuple().size));
                            }
                        }else {
                            if(y2 >= ymax) {
                                v.tuple()[temp++] = new Integer((long)(z*6 - 1 + old_out_verts.tuple().size));
                            }else if(y2 <= ymin){
                                v.tuple()[temp++] = new Integer((long)(z*6 + old_out_verts.tuple().size));
                            }
                        }
                    }

                    for(uint i = vec2.tuple().size - 1; i > 0; i--) {
                        if(vec2.tuple()[i].tuple().size > 1) {
                            if(cur2->lineStrips[i - 1]->vert[0]->x > xmax || cur2->lineStrips[i - 1]->vert[cur2->lineStrips[i - 1]->vert.size() - 1]->x < xmin) {
                                continue;
                            } else {
                                for(uint j = vec2.tuple()[i].tuple().size - 1; j > 0; j--) {
                                    if(cur2->lineStrips[i - 1]->vert[j - 1]->x <= xmax && cur2->lineStrips[i - 1]->vert[j - 1]->x >= xmin) {
                                        v.tuple()[temp++] = vec2.tuple()[i].tuple()[j];
                                    }
                                }
                            }
                        }
                    }

                    y1 = f1.getSingleData(xmin);
                    if(!isNaN(y1)) {
                    } else {
                        singularitynode node;
                        getsimilarvalue(&f1, xmin, node);
                        if(node.rightd > 0.0) {
                            y1 = ymin;
                        } else if(node.rightd < 0.0) {
                            y1 = ymax;
                        }
                    }
                    y2 = xmin_sy;
                    if(!((y1 > ymax && y2 > ymax) || (y1 < ymin && y2 < ymin) || (( y1 > ymin) && (y1 < ymax)) && ((y2 > ymin) && (y2 < ymax)))) {
                        if(y2 >= ymax) {
                            v.tuple()[temp++] = new Integer((long)(z*6 + 1 + old_out_verts.tuple().size));
                            if(y1 <= ymin) {
                                v.tuple()[temp++] = new Integer((long)(z*6 + 2 + old_out_verts.tuple().size));
                            }
                        }else if(y2 <= ymin) {
                            v.tuple()[temp++] = new Integer((long)(z*6 + 2 + old_out_verts.tuple().size));
                            if(y1 >= ymax) {
                                v.tuple()[temp++] = new Integer((long)(z*6 + 1 + old_out_verts.tuple().size));
                            }
                        }else {
                            if(y1 >= ymax) {
                                v.tuple()[temp++] = new Integer((long)(z*6 + 1 + old_out_verts.tuple().size));
                            }else if(y1 <= ymin){
                                v.tuple()[temp++] = new Integer((long)(z*6 + 2 + old_out_verts.tuple().size));
                            }
                        }
                    }
                    if(y1 <= ymax && y1 >= ymin) {
                        v.tuple()[temp++] = new Integer((long)(z*6 + 4 + old_out_verts.tuple().size));
                    }
                    //return v;
                    vtotal.tuple()[z + 1] = v;
                    z++;
                }

                xmin = old_xmin; xmax = old_xmax;
                ymin = old_ymin; ymax = old_ymax;
                return vtotal;
        }

        bool lessmark( const singularitynode *s1,const singularitynode *s2)
        {
            return (s1->x<s2->x);
        }


        var Plot::addVecReversed(Curve * cur1,Curve * cur2,var vec1, var vec2,
            var expr1, var expr2, std::vector<singularitynode *> &node_a1,
            std::vector<singularitynode *> &node_a2,var &out_verts)
        {
            F1P f1 = F1P(k, expr1, xparam);
            F1P f2 = F1P(k, expr2, xparam);

            std::vector <singularitynode *> singular;
            if(xmin!=(*node_a1.begin())->x&&xmin!=(*node_a2.begin())->x)
            {
                singularitynode * snode=new singularitynode;
                snode->x=xmin;
                singular.push_back(snode);
            }
            for(std::vector< singularitynode *>::iterator i=node_a1.begin();i!=node_a1.end();i++)
            {
                singular.push_back(*i);
            }
            for(std::vector<singularitynode  *>::iterator i=node_a2.begin();i!=node_a2.end();i++)
            {
                singular.push_back(*i);
            }
            if(xmax!=(*(node_a1.end()-1))->x&&xmax!=(*(node_a2.end()-1))->x)
            {
                singularitynode * snode1=new singularitynode;
                snode1->x=xmax;
                singular.push_back(snode1);
            }
            std::sort(singular.begin(),singular.end(),lessmark);
            for(std::vector<singularitynode *>::iterator i=singular.begin();i!=singular.end()-1;)
            {
                if(std::abs((*i)->x-((*(i+1))->x)) <SINGLAR_THRESHOLD)
                {
                    singular.erase(i);
                    i = singular.begin();  
                    continue; 
                }
                else
                    i++;
            }

            var vtotal = list(singular.size()-1); 
            var old_out_verts = out_verts;

            out_verts = list(old_out_verts.tuple().size +(singular.size()-1)*8-1);

            for(uint i = 0; i < old_out_verts.tuple().size - 1; i++) {
                out_verts.tuple()[i + 1] = old_out_verts.tuple()[i + 1];
            }
            int sum=1;
            for(std::vector <singularitynode *>::iterator si=singular.begin();si!=singular.end()-1;si++)
            {
                double x1=(*si)->x;
                double x2=(*(si+1))->x;
                uint totaldata = 0;
                out_verts.tuple()[out_verts.tuple().size - sum*8] = tuple($.List, new Real(x1, 0), new Real(ymax, 0));
                out_verts.tuple()[out_verts.tuple().size - (sum*8-1)] = tuple($.List, new Real(x1, 0), new Real(ymin, 0));
                out_verts.tuple()[out_verts.tuple().size - (sum*8-2)] = tuple($.List, new Real(x2, 0), new Real(ymax, 0));
                out_verts.tuple()[out_verts.tuple().size - (sum*8-3)] = tuple($.List, new Real(x2, 0), new Real(ymin, 0));
                std::vector<singularitynode *>::iterator s1=node_a1.end();
                std::vector<singularitynode *>::iterator s2=node_a2.end();
                for(std::vector <singularitynode *>::iterator i=node_a1.begin();i!=node_a1.end();i++)
                {
                    if((*i)->x==x1)
                    {	s1=i;
                    break;
                    }
                }
                for(std::vector <singularitynode *>::iterator i=node_a2.begin();i!=node_a2.end();i++)
                {
                    if((*i)->x==x1)
                    {	s2=i;
                    break;
                    }
                }
                Curve *first, *second;
                var vec_add1,  vec_add2;
                if(s1!=node_a1.end())
                {
                    first=cur1;
                    second=cur2;
                    vec_add1=vec1;
                    vec_add2=vec2;
                }else
                {
                    first=cur2;
                    second =cur1;
                    vec_add1=vec2;
                    vec_add2=vec1;
                }

                for(uint i = 0; i < vec_add1.tuple().size - 1; i++) {
                    if(vec_add1.tuple()[i + 1].tuple().size > 1) {
                        if(first->lineStrips[i]->vert[0]->x > x2 || first->lineStrips[i]->vert[first->lineStrips[i]->vert.size() - 1]->x < x1) {
                            continue;
                        } else {
                            for(uint j = 0; j < vec_add1.tuple()[i + 1].tuple().size - 1; j++) {
                                if(first->lineStrips[i]->vert[j]->x <= x2 && first->lineStrips[i]->vert[j]->x >= x1) {
                                    totaldata++;
                                }
                            }
                        }
                    }
                }

                for(uint i = vec_add2.tuple().size - 1; i > 0; i--) {
                    if(vec_add2.tuple()[i].tuple().size > 1) {
                        if(second->lineStrips[i - 1]->vert[0]->x > x2 || second->lineStrips[i - 1]->vert[second->lineStrips[i - 1]->vert.size() - 1]->x < x1) {
                            continue;
                        } else {
                            for(uint j = vec_add2.tuple()[i].tuple().size - 1; j > 0; j--) {
                                if(second->lineStrips[i - 1]->vert[j - 1]->x <= x2 && second->lineStrips[i - 1]->vert[j - 1]->x >= x1) {
                                    totaldata++;
                                }
                            }
                        }
                    }
                }
                double y1,y2;
                std::vector<singularitynode *>::iterator s3=node_a1.end();
                std::vector<singularitynode *>::iterator s4=node_a2.end();
                for(std::vector <singularitynode *>::iterator i=node_a1.begin();i!=node_a1.end();i++)
                {
                    if((*i)->x==x2)
                    {	s3=i;
                    break;
                    }
                }
                for(std::vector <singularitynode *>::iterator i=node_a2.begin();i!=node_a2.end();i++)
                {
                    if((*i)->x==x2)
                    {	s4=i;
                    break;
                    }
                }
                if(first==cur1)
                {
                    if(s3!=node_a1.end())
                    {
                        if((*s3)->leftd>0)
                            y1=ymax;
                        else
                            y1=ymin;
                    }else 
                    {
                        y1=f1.getSingleData(x2);

                    }
                    if(s4!=node_a2.end())
                    {
                        if((*s4)->leftd>0)
                            y2=ymax;
                        else
                            y2=ymin;
                    }else
                    {
                        y2=f2.getSingleData(x2);
                    }
                }else{
                    if(s4!=node_a2.end())
                    {
                        if((*s4)->leftd>0)
                            y1=ymax;
                        else
                            y1=ymin;
                    }else 
                    {
                        y1=f2.getSingleData(x2);
                    }
                    if(s3!=node_a1.end())
                    {
                        if(((*s3))->leftd>0)
                            y2=ymax;
                        else
                            y2=ymin;
                    }else
                    {
                        y2=f1.getSingleData(x2);
                    }
                }
                out_verts.tuple()[out_verts.tuple().size - (sum*8-4)] = tuple($.List, new Real(x2, 0), new Real(y1, 0));
                out_verts.tuple()[out_verts.tuple().size - (sum*8-5)] = tuple($.List, new Real(x2, 0), new Real(y2, 0));


                double y3,y4;
                s3=node_a1.end();
                s4=node_a2.end();
                for(std::vector <singularitynode *>::iterator i=node_a1.begin();i!=node_a1.end();i++)
                {
                    if((*i)->x==x1)
                    {	s3=i;
                    break;
                    }
                }
                for(std::vector <singularitynode *>::iterator i=node_a2.begin();i!=node_a2.end();i++)
                {
                    if((*i)->x==x1)
                    {	s4=i;
                    break;
                    }
                }
                if(second==cur1)
                {
                    if(s3!=node_a1.end())
                    {
                        if((*s3)->rightd>0)
                            y3=ymin;
                        else
                            y3=ymax;
                    }else 
                        y3=f1.getSingleData(x1);
                    if(s4!=node_a2.end())
                    {
                        if((*s4)->rightd>0)
                            y4=ymin;
                        else
                            y4=ymax;
                    }else
                        y4=f2.getSingleData(x1);
                }else{
                    if(s4!=node_a2.end())
                    {
                        if((*s4)->rightd>0)
                            y3=ymin;
                        else
                            y3=ymax;
                    }else 
                        y3=f2.getSingleData(x1);
                    if(s3!=node_a1.end())
                    {
                        if((*s3)->rightd>0)
                            y4=ymin;
                        else
                            y4=ymax;
                    }else
                        y4=f1.getSingleData(x1);
                }

                totaldata+=4;
                out_verts.tuple()[out_verts.tuple().size - (sum*8-6)] = tuple($.List, new Real(x1, 0), new Real(y3, 0));
                out_verts.tuple()[out_verts.tuple().size - (sum*8-7)] = tuple($.List, new Real(x1, 0), new Real(y4, 0));
                var v = list(totaldata);
                uint temp = 1;
                for(uint i = 0; i < vec_add1.tuple().size - 1; i++) {
                    if(vec_add1.tuple()[i + 1].tuple().size > 1) {
                        if(first->lineStrips[i]->vert[0]->x > x2 || first->lineStrips[i]->vert[first->lineStrips[i]->vert.size() - 1]->x < x1) {
                            continue;
                        } else {
                            for(uint j = 0; j < vec_add1.tuple()[i + 1].tuple().size - 1; j++) {
                                if(first->lineStrips[i]->vert[j]->x <= x2 && first->lineStrips[i]->vert[j]->x >= x1) {
                                    v.tuple()[temp++] = vec_add1.tuple()[i + 1].tuple()[j + 1];
                                }
                            }
                        }
                    }
                }
                if(y1>ymax)
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-2)));
                }else if(y1<=ymax &&y1>ymin)
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-4)));
                }else
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-3)));
                }
                if(y2>ymax)
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-2)));
                }else if(y2<=ymax &&y2>ymin)
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 -(sum*8-5)));
                }else
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-3)));
                }

                for(uint i = vec_add2.tuple().size - 1; i > 0; i--) {
                    if(vec_add2.tuple()[i].tuple().size > 1) {
                        if(second->lineStrips[i - 1]->vert[0]->x > x2 || second->lineStrips[i - 1]->vert[second->lineStrips[i - 1]->vert.size() - 1]->x < x1) {
                            continue;
                        } else {
                            for(uint j = vec_add2.tuple()[i].tuple().size - 1; j > 0; j--) {
                                if(second->lineStrips[i - 1]->vert[j - 1]->x <= x2 && second->lineStrips[i - 1]->vert[j - 1]->x >= x1) {
                                    v.tuple()[temp++] = vec_add2.tuple()[i].tuple()[j];
                                }
                            }
                        }
                    }
                }
                if(y3>ymax)
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8)));
                }else if(y3<=ymax &&y3>ymin)
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-6)));
                }else
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-1)));
                }
                if(y4>ymax)
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8)));
                }else if(y4<=ymax &&y4>ymin)
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-7)));
                }else
                {
                    v.tuple()[temp++] = new Integer((long)(out_verts.tuple().size - 1 - (sum*8-1)));
                }
                vtotal.tuple()[sum] = v;
                sum++;
            }		
            return vtotal;
        }
        //var addVecReversed(var vec1, var vec2, var expr1, var &out_verts, 
        //    Curve *cur1, Curve *cur2, const std::vector<singularitynode *> &singularityset) {
        //        $.Times
        //}
        bool Plot::getsimilarvalue(F1P *f2d, double x, singularitynode &node) {
            node.x = node.leftd = node.lefty = node.rightd = node.righty = 0.0;
            if(!isNaN((*f2d).getSingleData(xmin))) {
                return false;
            } else {
                double dx_temp = (xmax - xmin) / LARGE;
                double x_change_once = (xmax - xmin) / 54;
                uint i = 1;
                double d_xr_sy, d_xl_sy;
                while(true) {
                    d_xr_sy = (*f2d).getDerivative(x + x_change_once * i, dx_temp);
                    if(!isNaN(d_xr_sy) && !isNaN((*f2d).getSingleData(x + x_change_once * i))) {
                        node.x = x;
                        node.righty = (*f2d).getSingleData(x + x_change_once * i);
                        node.rightd = d_xr_sy;
                        break;
                    }
                    if(xmax < (x + x_change_once * i)) {
                        break;
                    }
                    i++;
                }
                i = 0;
                while(true) {
                    d_xl_sy = (*f2d).getDerivative(x - x_change_once * i, dx_temp);
                    if(!isNaN(d_xl_sy) && !isNaN((*f2d).getSingleData(x - x_change_once * i))) {
                        node.x = x;
                        node.lefty = (*f2d).getSingleData(x - x_change_once * i);
                        node.rightd = d_xl_sy;
                        break;
                    }
                    if(xmin > (x - x_change_once * i)) {
                        break;
                    }
                    i++;
                }
                return true;
            }
        }
    }
}
