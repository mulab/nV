#include <nV/Graphics.h>
#include "Plot.h"
#include "ContourPlot.h"
#include "ParametricPlot.h"
#include "PolarPlot.h"
#include "DiscretePlot.h"
#include "ListPlot.h"
#include "ParametricRegionPlot.h"
#include "DensityPlot.h"
#include "Plot3D.h"
#include "ParametricCurvePlot3D.h"
#include "ParametricSurfacePlot3D.h"
#include "ContourPlot3D.h"

#ifdef _MSC_VER
#pragma comment(lib,"nVKernel")
#pragma comment(lib,"nVNumber")
#endif

using namespace nV;
using namespace Graphics;

CAPI void Install(Kernel& k) {
    //wcout << "#Graphics# Install Successfully!" << endl;
}
CAPI void Uninstall(nV::Kernel& k) {
    wcout << "#Graphics# Uninstall Successfully!" << endl;
}

CAPI void CVALUE(System,Plot)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 3 && x[2].tuple().size == 4) {
        Plot plot(k, x);
        r = plot.exportGraphics();
    } else if (x.size == 4 && x[2].tuple().size == 4) {
        var temp = x[x.size - 1];
        if (temp.isTuple(SYS(Options))) {
            for (uint i = 0; i < temp.tuple().size - 1; ++i) {
                if (temp.tuple()[i + 1].isTuple(SYS(Rule))) {
                    if (temp.tuple()[i + 1].tuple().size == 3 &&
                        temp.tuple()[i + 1].tuple()[1].isSymbol() &&
                        (temp.tuple()[i + 1].tuple()[1].compare(SYS(PlotRange)) == 0 ||
                        temp.tuple()[i + 1].tuple()[1].compare(SYS(Axes)) == 0 ||
                        temp.tuple()[i + 1].tuple()[1].compare(SYS(FillToCurve)) == 0) &&
                        temp.tuple()[i + 1].tuple()[2].isTuple() &&
                        temp.tuple()[i + 1].tuple()[2].tuple().size == 3) {
                            continue;
                    } else if (temp.tuple()[i + 1].tuple().size == 3 &&
                        temp.tuple()[i + 1].tuple()[1].isSymbol() &&
                        temp.tuple()[i + 1].tuple()[1].compare(SYS(FillToAxes)) == 0 &&
                        temp.tuple()[i + 1].tuple()[2].isTuple() &&
                        temp.tuple()[i + 1].tuple()[2].tuple().size == 2) {
                            continue;
                    } else if (temp.tuple()[i + 1].tuple().size == 3 &&
                        temp.tuple()[i + 1].tuple()[1].isSymbol() &&
                        temp.tuple()[i + 1].tuple()[2].isSymbol() &&
                        temp.tuple()[i + 1].tuple()[1].compare(SYS(PlotRange)) == 0 &&
                        temp.tuple()[i + 1].tuple()[2].compare(SYS(Auto)) == 0) {
                            continue;
                    } else {
                        return;
                    }
                } if (temp.tuple()[i + 1].isTuple(SYS(ColorFunction)) &&
                    temp.tuple()[i + 1].tuple().size == 2) {
                        continue;
                } else {
                    return;
                }
            }

            Plot plot(k, x);
            r = plot.exportGraphics();
        }
    }
}

CAPI void CVALUE(System,ContourPlot)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 4 && x[2].tuple().size == 4 && x[3].tuple().size == 4) {
        ContourPlot contourplot(k, x);
        r = contourplot.exportGraphics();
    }
}

CAPI void CVALUE(System,ParametricPlot)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 3 && x[2].tuple().size == 4) {
        ParametricPlot parametricplot(k, x);
        r = parametricplot.exportGraphics();
    } else if (x.size == 4 && x[2].tuple().size == 4) {
        var temp = x[x.size - 1];
        if (temp.isTuple(SYS(Options)) &&
            temp.tuple().size == 2 &&
            temp.tuple()[1].isTuple(SYS(ColorFunction))) {
                ParametricPlot parametricplot(k, x);
                r = parametricplot.exportGraphics();
        }
    }
}

CAPI void CVALUE(System,PolarPlot)(Kernel& k, var& r, Tuple& x) {
    //times = 0;
    //timetotal = 0.0;
    //PolarPlot polarplot(k, x);
    //r = polarplot.exportGraphics();
    //std::cout << "times" << times << std::endl;
    //std::cout << "timetotaloff1p" << timetotal << std::endl;
    if (x.size == 3 && x[2].tuple().size == 4) {
        //x.print();
        PolarPlot parametricplot(k, x);
        r = parametricplot.exportGraphics();
    } else if (x.size == 4 && x[2].tuple().size == 4) {
        var temp = x[x.size - 1];
        if (temp.isTuple(SYS(Options)) &&
            temp.tuple().size == 2 &&
            temp.tuple()[1].isTuple(SYS(ColorFunction))) {
                PolarPlot parametricplot(k, x);
                r = parametricplot.exportGraphics();
        }
    }
}

CAPI void CVALUE(System, DiscretePlot)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 3 && (x[2].tuple().size == 4 || x[2].tuple().size == 5)) {
        DiscretePlot Discreteplot(k, x);
        r = Discreteplot.exportGraphics();
    } else if (x.size == 4 && (x[2].tuple().size == 4 || x[2].tuple().size == 5)) {
        var temp = x[x.size - 1];
        if (temp.isTuple(SYS(Options))) {
            for (uint i = 0; i < temp.tuple().size - 1; ++i) {
                if (temp.tuple()[i + 1].isTuple(SYS(Rule))) {
                    if (temp.tuple()[i + 1].tuple().size == 3 &&
                        temp.tuple()[i + 1].tuple()[1].isSymbol() &&
                        temp.tuple()[i + 1].tuple()[1].compare(SYS(FillToCurve)) == 0 &&
                        temp.tuple()[i + 1].tuple()[2].tuple().size == 3) {
                            continue;
                    } else if (temp.tuple()[i + 1].tuple().size == 3 &&
                        temp.tuple()[i + 1].tuple()[1].isSymbol() &&
                        temp.tuple()[i + 1].tuple()[1].compare(SYS(FillToAxes)) == 0 &&
                        temp.tuple()[i + 1].tuple()[2].tuple().size == 2) {
                            continue;
                    } else if (temp.tuple()[i + 1].tuple().size == 3 &&
                        temp.tuple()[i + 1].tuple()[1].isSymbol() &&
                        (temp.tuple()[i + 1].tuple()[1].compare(SYS(NoFilling)) == 0 ||
                        temp.tuple()[i + 1].tuple()[1].compare(SYS(Joined)) == 0) &&
                        temp.tuple()[i + 1].tuple()[2].isSymbol() &&
                        temp.tuple()[i + 1].tuple()[2].compare(SYS(True)) == 0) {
                            continue;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            }

            DiscretePlot Discreteplot(k, x);
            r = Discreteplot.exportGraphics();
            //}
        }
    }
}

CAPI void CVALUE(System,ListPlot)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2) {
        ListPlot listplot(k, x);
        r = listplot.exportGraphics();
    } else if (x.size == 3) {
        var temp = x[x.size - 1];
        if (temp.isTuple(SYS(Options))) {
            for (uint i = 0; i < temp.tuple().size - 1; ++i) {
                if (temp.tuple()[i + 1].isTuple(SYS(Rule))) {
                    if (temp.tuple()[i + 1].tuple().size == 3 &&
                        temp.tuple()[i + 1].tuple()[1].isSymbol() &&
                        (temp.tuple()[i + 1].tuple()[1].compare(SYS(FillToAxes)) == 0 ||
                        temp.tuple()[i + 1].tuple()[1].compare(SYS(Joined)) == 0)) {
                            continue;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            }

            ListPlot listplot(k, x);
            r = listplot.exportGraphics();
        }
    }
}

CAPI void CVALUE(System,ParametricRegionPlot)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 4 && x[2].isTuple() && x[2].tuple().size == 4 && x[3].isTuple() && x[3].tuple().size == 4) {
        ParametricRegionPlot ParametricRegionplot(k, x);
        r = ParametricRegionplot.exportGraphics();
    }
}

CAPI void CVALUE(System,DensityPlot)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 4 && x[2].isTuple() && x[2].tuple().size == 4 && x[3].isTuple() && x[3].tuple().size == 4) {
        DensityPlot Densityplot(k, x);
        r = Densityplot.exportGraphics();
    }
}

CAPI void CVALUE(System,Plot3D)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 4 && x[2].isTuple() && x[2].tuple().size == 4 && x[3].isTuple() && x[3].tuple().size == 4) {
        Plot3D plot3d(k, x);
        r = plot3d.exportGraphics();
    }
}

CAPI void CVALUE(System,ParametricPlot3D)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 3 && x[2].isTuple() && x[2].tuple().size == 4) {
        ParametricCurvePlot3D parametriccurveplot3d(k, x);
        r = parametriccurveplot3d.exportGraphics();
    }
}

CAPI void CVALUE(System,ParametricSurfacePlot3D)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 4 && x[2].isTuple() && x[2].tuple().size == 4 && x[3].isTuple() && x[3].tuple().size == 4) {
        ParametricSurfacePlot3D ParametricSurfacePlot(k, x);
        r = ParametricSurfacePlot.exportGraphics();
    }
}

CAPI void CVALUE(System,ContourPlot3D)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 5 && x[2].isTuple() && x[2].tuple().size == 4 &&
        x[3].isTuple() && x[3].tuple().size == 4 &&
        x[4].isTuple() && x[4].tuple().size == 4) {
            ContourPlot3D contourplot3d(k, x);
            r = contourplot3d.exportGraphics();
    }
}