#include "ListPlot.h"
#include <cstring>

//using namespace std;

// Color BLUE(0,0,255);
// Color GREEN(0,255,0);
// Color RED(255,0,0);
// int colorListNum = 3;
// Color colorList[] = {BLUE,RED,GREEN};
//
// Hue hue0(0.67, 0.6, 0.6);
// Hue hue1(0.9060679774997897, 0.6, 0.6);
// Hue hue2(0.14213595499957954, 0.6, 0.6);
// Hue hue3(0.37820393249936934, 0.6, 0.6);
// int hueListNum = 4;
// Hue hueList[] = {hue0,hue1,hue2,hue3};
namespace nV {
    namespace Graphics {
        ListPlot::ListPlot(Kernel &k, const Tuple &cmd) :k(k) {
            filling = FILL_NULL;
            //varplist = At(cmd, 0);
            //cmd.print();
            //varplist = cmd[1];
            //if(cmd[1].tuple().isObject($.Real))
            int count2 = 0, count3 = 0, count5 = 0, count7 = 0;
            for(uint i = 0; i < cmd[1].tuple().size - 1; i++) {
                if(cmd[1].tuple()[i + 1].isObject($.Real) || cmd[1].tuple()[i + 1].isObject($.Integer)) {
                    count2++;
                } else if(cmd[1].tuple()[i + 1].isTuple($.List)) {
                    for(uint j = 0; j < cmd[1].tuple()[i + 1].tuple().size - 1; j++) {
                        if(cmd[1].tuple()[i + 1].tuple()[j + 1].isObject($.Real) || cmd[1].tuple()[i + 1].tuple()[j + 1].isObject($.Integer)){
                            if (cmd[1].tuple()[i + 1].tuple().size == 3) {
                                count3++;
                            } else {
                                count5++;
                            }
                        } else if(cmd[1].tuple()[i + 1].tuple()[j + 1].isTuple($.List) && cmd[1].tuple()[i + 1].tuple()[j + 1].tuple().size == 3) {
                            count5++;
                        } else {
                            count7++;
                        }
                    }
                }
            }
            if(count7 == 0) {
                islist = false;
                if(count2 != 0 && count5 == 0 && count3 == 0) {
                    uint sizeofcmd = count2;
                    varplist = list(sizeofcmd);
                    for(uint i = 0; i < sizeofcmd; i++) {
                        var temp = list(new Integer(i), cmd[1].tuple()[i + 1]);
                        //toD(cmd[1].tuple()[i + 1].object());
                        varplist.tuple()[i + 1] = temp;
                    }
                    joined = new bool(false);
                } else if(count3 != 0 && count2 == 0 && count5 == 0) {
                    //uint sizeofcmd = count3;
                    //varplist = list(sizeofcmd);
                    //for(uint i = 0; i < sizeofcmd; i++) {
                    //    varplist.tuple()[i + 1] = cmd[1].tuple()[i + 1];
                    //}
                    varplist = cmd[1];
                    joined = new bool(false);
                } else if(count5 != 0 && count2 == 0 && count3 == 0) {
                    //varplist = list(cmd[1].tuple().size - 1);
                    //for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                    //    varplist.tuple()[i + 1] = list(cmd[1].tuple()[i + 1]);
                    //    for(uint j = 0; j < varplist.tuple()[i + 1].tuple().size - 1; j++) {
                    //        varplist.tuple()[i + 1].tuple()[j + 1] = cmd[1].tuple()[i + 1].tuple()[j + 1];
                    //    }
                    //}
                    //varplist = cmd[1];
                    varplist = list(cmd[1].tuple().size - 1);
                    for (uint i = 0; i < varplist.tuple().size - 1; i++) {
                        if (cmd[1].tuple()[i + 1].tuple()[cmd[1].tuple()[i + 1].tuple().size - 1].isTuple($.List)) {
                            varplist.tuple()[i + 1] = cmd[1].tuple()[i + 1];
                        } else {
                            var cmdtemp = list(cmd[1].tuple()[i + 1].tuple().size - 1);
                            for (uint j = 0; j < cmd[1].tuple()[i + 1].tuple().size - 1; ++j) {
                                cmdtemp.tuple()[j + 1] = list(new Integer(j), cmd[1].tuple()[i + 1].tuple()[j + 1]);
                            }
                            varplist.tuple()[i + 1] = cmdtemp;
                        }
                    }

                    islist = true;
                    joined = new bool[cmd[1].tuple().size - 1];
                    memset((char *)joined, 0, sizeof(bool) * (cmd[1].tuple().size - 1));
                } else {varplist = null; islist = false; joined = NULL;}
            } else {varplist = null; islist = false; joined = NULL;}
            if (cmd[cmd.size - 1].isTuple(SYS(Options))) {
                var opts = cmd[cmd.size - 1];
                readOptions(opts.tuple());
            }
            //xmin = plist[0].x;
            //xmax = plist[0].x;
            //ymin = plist[0].y;
            //ymax = plist[0].y;
            //for (int i = 1; i < pnum; i++) {
            //    if (plist[i].x < xmin)
            //        xmin = plist[i].x;
            //    if (plist[i].x > xmax)
            //        xmax = plist[i].x;
            //    if (plist[i].y < ymin)
            //        ymin = plist[i].y;
            //    if (plist[i].y > ymax)
            //        ymax = plist[i].y;
            //}
            xmin = 0; xmax = 0; ymax = 0; ymin = 0;
            if(!islist && varplist.isTuple()) {
                xmin = toD(varplist.tuple()[1].tuple()[1].object());
                xmax = xmin;
                ymin = toD(varplist.tuple()[1].tuple()[2].object());
                ymax = ymin;
                for(uint i = 1; i < varplist.tuple().size - 1; i++) {
                    if(toD(varplist.tuple()[i + 1].tuple()[1].object()) < xmin) {
                        xmin = toD(varplist.tuple()[i + 1].tuple()[1].object());
                    }
                    if(toD(varplist.tuple()[i + 1].tuple()[1].object()) > xmax) {
                        xmax = toD(varplist.tuple()[i + 1].tuple()[1].object());
                    }
                    if(toD(varplist.tuple()[i + 1].tuple()[2].object()) < ymin) {
                        ymin = toD(varplist.tuple()[i + 1].tuple()[2].object());
                    }
                    if(toD(varplist.tuple()[i + 1].tuple()[2].object()) > ymax) {
                        ymax = toD(varplist.tuple()[i + 1].tuple()[2].object());
                    }
                }
            }else if(islist && varplist.isTuple()) {
                xmin = toD(varplist.tuple()[1].tuple()[1].tuple()[1].object());
                xmax = xmin;
                ymin = toD(varplist.tuple()[1].tuple()[1].tuple()[1].object());
                ymax = ymin;
                for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                    for(uint j = 0; j < varplist.tuple()[i + 1].tuple().size - 1; j++) {
                        if(toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[1].object()) < xmin) {
                            xmin = toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[1].object());
                        }
                        if(toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[1].object()) > xmax) {
                            xmax = toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[1].object());
                        }
                        if(toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[2].object()) < ymin) {
                            ymin = toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[2].object());
                        }
                        if(toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[2].object()) > ymax) {
                            ymax = toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[2].object());
                        }
                    }
                }
            }
        }

        ListPlot::~ListPlot() {
            //delete plist;
            if (joined != NULL) {
                if(islist) {
                    delete [] joined;
                } else {
                    delete joined;
                }
            }
        }

        var ListPlot::exportGraphics() {
            if(filling == FILL_NULL) {
                var shape = list(0,SYS(Shape));
                //var shape;
                //Hue *hue = getHue(0);
                //var shape = list(makeHueOption(hue),SYS(Shape));
                //if (hue != NULL) {
                //    delete hue;
                //}
                if(!islist && varplist.isTuple()) {
                    if(*joined) {
                        Hue* hue = getHue(0);
                        var opt = makeHueOption(hue);
                        shape = list(2,SYS(Shape));
                        shape.tuple()[2] = list(varplist, opt, SYS(Line));
                        delete hue;
                    } else {
                        shape = list( 1, SYS(Shape));
                    }
                    Hue* hue = getHue(0);
                    var opt = makeHueOption(hue);
                    shape.tuple()[1] = list(varplist, opt, SYS(PointList));
                    delete hue;
                } else if(islist && varplist.isTuple()) {
                    uint size_temp = 0;
                    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                        if(joined[i]) {
                           size_temp++; 
                        }
                    }
                    shape = list(varplist.tuple().size - 1 + size_temp, SYS(Shape));
                    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                        if(joined[i]) {
                            Hue *hue = getHue(i);
                            var opt = makeHueOption(hue);
                            shape.tuple()[varplist.tuple().size - 1 + size_temp--] = list(varplist.tuple()[i + 1], opt, SYS(Line));
                            delete hue;
                        }
                    }
                    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                        Hue *hue = getHue(i);
                        var opt = makeHueOption(hue);
                        shape.tuple()[i + 1] = list(varplist.tuple()[i + 1], opt, SYS(PointList));
                        delete hue;
                    }
                }
                var opts = makeGlobalOptions();
                var graphics = makeGraphics(shape, opts);
                return graphics;
                //return null;
            } else if(filling == FILL_AXIS_LIST) {
                var vec_indices = list(0),line_indices = list(0);
                makePointList(vec_indices, line_indices);
                uint joinednum = 0;

                if(!islist && varplist.isTuple()) {
                    if(*joined) {
                        joinednum++;
                    } else {
                        joinednum += varplist.tuple().size - 1;
                    }
                } else if(varplist.isTuple()) {
                    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                        if(joined[i]) {
                            joinednum++;
                        } else {
                            joinednum += varplist.tuple()[i + 1].tuple().size - 1;
                        }
                    }
                }

                var shapes = list(0);
                if(!islist && varplist.isTuple()) {
                    shapes = list(1 + joinednum, SYS(Directive));
                    //var temp = list(line_indices.tuple().size - 1, SYS(PointList));
                    //for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                    //    temp.tuple()[i + 1] = line_indices.tuple()[i + 1];
                    //}
                    //temp.tuple()[line_indices.tuple().size] = makeHueOption(getHue(0));
                    var temp = list(line_indices,makeHueOption(getHue(0)), SYS(PointList));
                    shapes.tuple()[1] = temp;
                    if(*joined) {
                        shapes.tuple()[1 + joinednum] = addVecReversed(vec_indices, line_indices,getHue(0));
                    }else {
                        for(uint i = 0; i < joinednum; i++) {
                            var line_indice = list(line_indices.tuple()[i + 1]);
                            shapes.tuple()[i + 2] = addVecReversed(vec_indices, line_indice, getHue(i));
                        }
                    }
                } else if(varplist.isTuple()) {
                    shapes = list(varplist.tuple().size - 1 + joinednum, SYS(Directive));
                    uint count = 1;
                    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                        shapes.tuple()[count++] = list(line_indices.tuple()[i + 1], makeHueOption(getHue(i)), SYS(PointList));
                        if(joined[i]) {
                            shapes.tuple()[count++] = addVecReversed(vec_indices, line_indices.tuple()[i + 1],getLightHue(0));
                        }else {
                            for(uint j = 0; j < varplist.tuple()[i + 1].tuple().size - 1; j++) {
                                var line_indice = list(line_indices.tuple()[i + 1].tuple()[j + 1]);
                                shapes.tuple()[count++] = addVecReversed(vec_indices, line_indice, getHue(i));
                            }
                        }
                    }

                }
                
                //var shape = list(pnum + 1, SYS(Shape));
                //Hue* hue = getHue(0);
                //var opt = makeHueOption(hue);
                //shape.tuple()[1] = list(varplist, opt, SYS(PointList));
                //Point2d * ppoint = plist;
                //for(uint i = 1; i < pnum + 1; i++) {
                //    ArrayList<Point2d *> temp;
                //    temp.add(new Point2d(ppoint->x,0.0));
                //    Point2d *point_temp = new Point2d(*(ppoint++)); 
                //    temp.add(point_temp);
                //    shape.tuple()[i + 1] = makeLine(&(Curve(temp)),hue);
                //}
                //delete hue;
                //var shape;
                //uint count = 1;
                //if(!islist && varplist.isTuple()) {
                //    if(*joined) {
                //        Hue* hue = getHue(0);
                //        var opt = makeHueOption(hue);
                //        shape = list(1 + varplist.tuple().size,SYS(Shape));
                //        shape.tuple()[varplist.tuple().size] = list(varplist, opt, SYS(Line));
                //        delete hue;
                //    } else {
                //        shape = list(varplist.tuple().size, SYS(Shape));
                //    }
                //    //shape = list(varplist.tuple().size, SYS(Shape));
                //    Hue *hue = getHue(0);
                //    var opt = makeHueOption(hue);
                //    shape.tuple()[count++] = list(varplist, opt, SYS(PointList));
                //    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                //        ArrayList<Point2d *> temp;
                //        temp.add(new Point2d(toD(varplist.tuple()[i + 1].tuple()[1].object()), 0.0));
                //        Point2d *point_temp = new Point2d(toD(varplist.tuple()[i + 1].tuple()[1].object()), toD(varplist.tuple()[i + 1].tuple()[2].object()));
                //        temp.add(point_temp);
                //        shape.tuple()[count++] = makeLine(&(Curve(temp)), hue);
                //    }
                //    delete hue;
                //} else if(islist && varplist.isTuple()) {
                //    uint size_temp = 0;
                //    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                //        if(joined[i]) {
                //           size_temp++; 
                //        }
                //    }
                //    shape = list(varplist.tuple().size - 1 + size_temp, SYS(Shape));
                //    
                //    uint totalpoint = 0;
                //    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                //        if(joined[i]) {
                //            totalpoint++;
                //        } else {
                //            totalpoint += varplist.tuple()[i + 1].tuple().size - 1;
                //        }
                //        //for(uint j = 0; j < varplist.tuple()[i + 1].tuple().size - 1; j++) {
                //        //    totalpoint++;
                //        //}
                //    }

                //    shape = list(varplist.tuple().size - 1 + totalpoint + size_temp, SYS(Shape));

                //    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                //        if(joined[i]) {
                //            Hue *hue = getHue(i);
                //            var opt = makeHueOption(hue);
                //            shape.tuple()[varplist.tuple().size - 1 + totalpoint + size_temp--] = list(varplist.tuple()[i + 1], opt, SYS(Line));
                //            delete hue;
                //        }
                //    }

                //    for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                //        Hue *hue = getHue(i);
                //        var opt = makeHueOption(hue);
                //        shape.tuple()[count++] = list(varplist.tuple()[i + 1], opt, SYS(PointList));
                //        if(joined[i]) {
                //            var poly = list(varplist.tuple()[i + 1].tuple().size + 1,SYS(Polygon));
                //            uint count_polypoint = 1;
                //            for(uint z = 0; z < varplist.tuple()[i + 1].tuple().size - 1; z++) {
                //                poly.tuple()[z + 1] = varplist.tuple()[i + 1].tuple()[z + 1];
                //            }
                //            poly.tuple()[varplist.tuple()[i + 1].tuple().size] = list(varplist.tuple()[i + 1].tuple()[varplist.tuple()[i + 1].tuple().size - 1].tuple()[1], new Real(0));
                //            poly.tuple()[varplist.tuple()[i + 1].tuple().size + 1] = list(varplist.tuple()[i + 1].tuple()[1].tuple()[1], new Real(0));
                //            shape.tuple()[count++] = poly;
                //        } else {
                //            for(uint j = 0; j < varplist.tuple()[i + 1].tuple().size - 1; j++) {
                //                ArrayList<Point2d *> temp;
                //                temp.add(new Point2d(toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[1].object()), 0.0));
                //                Point2d *point_temp = new Point2d(toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[1].object()), toD(varplist.tuple()[i + 1].tuple()[j + 1].tuple()[2].object()));
                //                temp.add(point_temp);
                //                shape.tuple()[count++] = makeLine(&(Curve(temp)), hue);
                //            }
                //        }
                //        delete hue;
                //    }
                //}
                var complex = list(vec_indices, shapes, SYS(GraphicsComplex));
                var shape = list(complex, SYS(Shape));
                var opts = makeGlobalOptions();
                var graphics = makeGraphics(shape, opts);
                return graphics;
            }
            return null;
        }

        void ListPlot::setRule(var title, var rule) {
            //if (title == SYS(FillToCurve)) {
            //    filling = FILL_TO_CURVE;
            //    fillcur1 = toD(N(k, k.eval(rule.tuple()[1])).object());
            //    fillcur2 = toD(N(k, k.eval(rule.tuple()[2])).object());
            //} else if (title == SYS(FillToAxis)) {
            if(title == SYS(FillToAxes)) {
                //rule.print();
                //if (rule.($.True)) {
                if (rule.isSymbol() && rule.compare($.True) == 0) {
                    filling = FILL_AXIS_LIST;
                }
                //}
                //fillcur1 = toD(N(k, k.eval(rule.tuple()[1])).object());
            //} else if (title == SYS(PlotRange)) { //plot range set or auto plot range
                //if (rule.tuple().size == 3) {
                //    rangeSet = 1;
                //    ymin = toD(N(k, k.eval(rule.tuple()[1])).object());
                //    ymax = toD(N(k, k.eval(rule.tuple()[2])).object());
                //} else {
                //    rangeSet = 2; //auto range
                //}
            } else if (title == SYS(ColorFunction)) { //has color function
                //colorFunctionSet = true;
                //var fun = rule.tuple()[1];
                //cf = new F1P(k,fun, xparam);
            } else if(title == SYS(Joined)) {
                if(rule.isTuple($.List)) {
                   if(islist) {
                       for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                           if(rule.tuple().size == varplist.tuple().size) {
                               if(rule.tuple()[i + 1] == SYS(True)) {
                                   joined[i] = true;
                               }
                           }
                       }
                   } else if(varplist.isTuple()) {
                       if(rule.tuple()[1] == SYS(True)) {
                           *joined = true;
                       }
                   }
                }
            }
            setCommonRule(title, rule);
        }

        void ListPlot::makePointList(var &vec_indices,var &line_indices) {
            uint count = 0;
            if (!islist && varplist.isTuple()) {
                vec_indices = list(varplist.tuple().size - 1);
                line_indices = list(varplist.tuple().size - 1);
                for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                    vec_indices.tuple()[i + 1] = varplist.tuple()[i + 1];
                    line_indices.tuple()[i + 1] = new Integer(count++);
                }
            } else if(varplist.isTuple()){
                uint totalnum = 0;
                for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                    totalnum += varplist.tuple()[i + 1].tuple().size - 1;
                }
                vec_indices = list(totalnum);
                line_indices = list(varplist.tuple().size - 1);
                for(uint i = 0; i < varplist.tuple().size - 1; i++) {
                    var r = list(varplist.tuple()[i + 1].tuple().size - 1);
                    for(uint j = 0; j < varplist.tuple()[i + 1].tuple().size - 1; j++) {
                        vec_indices.tuple()[count + 1] = varplist.tuple()[i + 1].tuple()[j + 1];
                        r.tuple()[j + 1] = new Integer(count++);
                    }
                    line_indices.tuple()[i + 1] = r;
                }
            }
        }

        var ListPlot::addVecReversed(var &vec_indices, const var &line_indice, Hue *hue) {
            var polygon = list(line_indice.tuple().size + 1);
            var vec_indices_old = vec_indices;
            vec_indices = list(vec_indices_old.tuple().size + 1);
            uint totalnum = 0;
            for(uint i = 0; i < vec_indices_old.tuple().size - 1; i++) {
                vec_indices.tuple()[i + 1] = vec_indices_old.tuple()[i + 1];
                totalnum++;
            }
            //line_indice.print();
            vec_indices.tuple()[vec_indices_old.tuple().size] = list(vec_indices_old.tuple()[line_indice.tuple()[1].cast<Integer>().toUI() + 1].tuple()[1], new Real(0));
            vec_indices.tuple()[vec_indices_old.tuple().size + 1] = list(vec_indices_old.tuple()[line_indice.tuple()[line_indice.tuple().size - 1].cast<Integer>().toUI() + 1].tuple()[1], new Real(0));
            for(uint i = 0; i < line_indice.tuple().size - 1; i++) {
                polygon.tuple()[i + 1] = line_indice.tuple()[i + 1];
            }
            polygon.tuple()[line_indice.tuple().size] = new Integer(totalnum + 1);
            polygon.tuple()[line_indice.tuple().size + 1] = new Integer(totalnum);
            var opt = makeHueOption(hue);
            //polygon.tuple()[line_indice.tuple().size] = opt;
            var polygon_final = list(polygon, opt, SYS(Polygon));
            return polygon_final;
        }
    }
}
