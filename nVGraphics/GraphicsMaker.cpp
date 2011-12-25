#include "GraphicsMaker.h"
#include <iostream>

namespace nV {
    namespace Graphics {

        GraphicsMaker::GraphicsMaker() {
            optlist.clear();
        }

        void GraphicsMaker::readOptions(const Tuple& opts) {
            for (size_t i = 0; i < opts.size-1; i++) {
                var op = opts[i+1];
                if (op.isTuple($.Rule)) {
                    setRule(op.tuple()[1], op.tuple()[2]);
                } else {
                    setRule(op.tuple()[0], op.tuple()[1]);
                }
            }
        }

        var GraphicsMaker::makeRange(double xmin, double xmax, double ymin, double ymax) {
            var xrange = list(new Real(xmin, 0), new Real(xmax, 0));
            var yrange = list(new Real(ymin, 0), new Real(ymax, 0));
            return tuple(SYS(PlotRange), xrange, yrange);
        }

        var GraphicsMaker::makeRange(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {
            var xrange = list(new Real(xmin, 0), new Real(xmax, 0));
            var yrange = list(new Real(ymin, 0), new Real(ymax, 0));
            var zrange = list(new Real(zmin, 0), new Real(zmax, 0));
            return tuple(SYS(PlotRange), xrange, yrange, zrange);
        }

        void GraphicsMaker::readVerts2d(const Tuple& expr, int* num, Point2d** vert) {
            *num = expr.size - 1;
            *vert = new Point2d[*num];
            for (int i = 0; i < *num; i++) {
                readPoint2d(expr[i + 1].tuple(), *vert + i);
            }
        }

        void GraphicsMaker::readPoint2d(const Tuple& expr, Point2d* vert) {
            vert->x = toD(expr[1].object());
            vert->y = toD(expr[2].object());
        }

        var GraphicsMaker::makeColor255(Color* color) {
            return tuple(SYS(Color255), new Integer((long)color->r), new Integer((long)color->g), new Integer((long)color->b));
        }

        var GraphicsMaker::makeColor255Option(Color* color) {
            return tuple(SYS(Options), makeColor255(color));
        }

        var GraphicsMaker::makeHueOption(Hue* hue) {
            var cvar = tuple(SYS(Hue), new Real(hue->h, 0), new Real(hue->s, 0), new Real(hue->b, 0));
            return tuple(SYS(Options), cvar);
        }

        Hue* GraphicsMaker::getLightHue(int i) {
            Hue* hue = new Hue(double((HUE_HUEBASIC + HUE_HUEADD*i) % 100) / 100, HUE_SLIGHT, HUE_BLIGHT);
            return hue;
        }

        Hue* GraphicsMaker::getHue(int i) {
            Hue* hue = new Hue(double((HUE_HUEBASIC + HUE_HUEADD*i) % 100) / 100, HUE_SBASIC, HUE_BBASIC);
            return hue;
        }

        var GraphicsMaker::makeLine(Curve* cur, Color* color) {
            var r = list(cur->lineStrips.size() + 1, SYS(Line));
            for (size_t i = 0; i < cur->lineStrips.size(); i++) {
                var v = list(cur->lineStrips[i]->vert.size());
                for (size_t j = 0; j < cur->lineStrips[i]->vert.size(); j++) {
                    v.tuple()[j + 1] = tuple($.List, new Real(cur->lineStrips[i]->vert[j]->x, 0), new Real(cur->lineStrips[i]->vert[j]->y, 0));
                }
                r.tuple()[i + 1] = v;
            }
            r.tuple()[cur->lineStrips.size() + 1] = makeColor255Option(color);
            return r;
        }

        var GraphicsMaker::makeLine(Curve3D* cur, Color* color) {
            var v = list(cur->vert.size());
            for (size_t i = 0; i < cur->vert.size(); ++i) {
                v.tuple()[i + 1] = tuple($.List, new Real(cur->vert[i]->x, 0), new Real(cur->vert[i]->y, 0), new Real(cur->vert[i]->z, 0));
            }
            return tuple(SYS(Line), v, makeColor255Option(color));
        }

        var GraphicsMaker::makeLine(Curve* cur, Hue* hue) {
            var r = list(cur->lineStrips.size() + 1, SYS(Line));
            for (size_t i = 0; i < cur->lineStrips.size(); i++) {
                var v = list(cur->lineStrips[i]->vert.size());
                for (unsigned int j = 0; j < cur->lineStrips[i]->vert.size(); j++) {
                    v.tuple()[j + 1] = tuple($.List, new Real(cur->lineStrips[i]->vert[j]->x, 0), new Real(cur->lineStrips[i]->vert[j]->y, 0));
                }
                r.tuple()[i + 1] = v;
            }
            r.tuple()[cur->lineStrips.size() + 1] = makeHueOption(hue);
            return r;
        }

        var GraphicsMaker::makeLine(Curve3D* cur, Hue* hue) {
            var v = list(cur->vert.size());
            for (size_t i = 0; i < cur->vert.size(); ++i) {
                v.tuple()[i + 1] = tuple($.List, new Real(cur->vert[i]->x, 0), new Real(cur->vert[i]->y, 0), new Real(cur->vert[i]->z, 0));
            }
            return tuple(SYS(Line), v, makeHueOption(hue));
        }

        var GraphicsMaker::makeColorfulLines(Curve* cur) {
            var lines = list(cur->lineStrips.size());
            //var r = list(cur->lineStrips.size() * 2, SYS(ColorfulLine));
            for (size_t i = 0; i < cur->lineStrips.size(); i++) {
                var v = list(cur->lineStrips[i]->vert.size());
                var c = list(cur->lineStrips[i]->vert.size());
                for (size_t j = 0; j < cur->lineStrips[i]->vert.size(); j++) {
                    v.tuple()[j + 1] = tuple($.List, new Real(cur->lineStrips[i]->vert[j]->x, 0), new Real(cur->lineStrips[i]->vert[j]->y, 0));
                    c.tuple()[j + 1] = makeColor255(cur->lineStrips[i]->color[j]);
                }
                lines.tuple()[i + 1] = tuple(SYS(ColorfulLine), v, c);
                //r.tuple()[i + 1] = v; r.tuple()[cur->lineStrips.size() + i + 1] = c;
            }
            return lines;
            //return r;
        }

        void GraphicsMaker::makeDirectiveOfLines(Curve** curs, int curnum, var* out_verts, var* out_lines) {
            int* index_add = new int[curnum];
            int* curvertnum = new int[curnum];
            int vertnum = 0;
            for (int i = 0; i < curnum; i++) {
                curvertnum[i] = 0;
                for (size_t j = 0; j < curs[i]->lineStrips.size(); j++)
                    curvertnum[i] += curs[i]->lineStrips[j]->vert.size();
            }
            index_add[0] = 0;
            vertnum = curvertnum[0];
            for (int i = 1; i < curnum; i++) {
                index_add[i] = index_add[i-1] + curvertnum[i-1];
                vertnum += curvertnum[i];
            }

            *out_verts = list(vertnum);

            for (int i = 0; i < curnum; i++) {
                int add = 0;
                out_lines[i] = list(curs[i]->lineStrips.size());
                for (size_t j = 0; j < curs[i]->lineStrips.size(); j++) {
                    for (size_t k = 0; k < curs[i]->lineStrips[j]->vert.size(); k++) {
                        out_verts->tuple()[index_add[i] + add + k + 1] = tuple($.List, new Real(curs[i]->lineStrips[j]->vert[k]->x, 0), 
                            new Real(curs[i]->lineStrips[j]->vert[k]->y, 0));
                    }
                    var v = list(curs[i]->lineStrips[j]->vert.size());
                    for (size_t k = 0; k < curs[i]->lineStrips[j]->vert.size(); k++) {
                        v.tuple()[k + 1] = new Integer((long)index_add[i] + add + k);
                    }
                    out_lines[i].tuple()[j + 1] = v;
                    add += curs[i]->lineStrips[j]->vert.size();
                }
            }
        }

        var GraphicsMaker::makeIndexedPolygon(IndexedQuadArray* poly, int color_id) {
            var v = list(poly->verts.size());
            for (size_t i = 0; i < poly->verts.size(); ++i) {
                v.tuple()[i + 1] = list(new Real(poly->verts[i]->x, 0), new Real(poly->verts[i]->y, 0));
            }
			var directive = list(poly->quads.size()+poly->edges.size()+1,SYS(Directive));
			int sum=1;
            //quad list
            var quads = list(poly->quads.size());
            for (size_t i = 0; i < poly->quads.size(); i++) {
                var indice = list(new Integer((long)poly->quads[i]->p11),
                    new Integer((long)poly->quads[i]->p12),
                    new Integer((long)poly->quads[i]->p22),
                    new Integer((long)poly->quads[i]->p21));
                quads.tuple()[i + 1] = indice;
            }
            Hue* hue1 = getLightHue(color_id);
            var qopt = makeHueOption(hue1);
            var equad = tuple(SYS(QuadArray), quads, qopt);
			directive.tuple()[sum++]=equad;
            //edge
			//var eline=list(poly->quads.size()+1);
			size_t i=0;
			for( ;i<poly->quads.size();i++)
			{
				var line=list(4);
				line.tuple()[1]=new Integer((long)poly->quads[i]->p11);
				line.tuple()[2]=new Integer((long)poly->quads[i]->p12);
				line.tuple()[3]=new Integer((long)poly->quads[i]->p22);
				line.tuple()[4]= new Integer((long)poly->quads[i]->p21);
				Hue *hue2=getLightHue(-1);
				var lopt=makeHueOption(hue2);
				var eline1 = tuple(SYS(Line), line, lopt);
				directive.tuple()[sum++]=eline1;
			}
			Hue* hue2 = getHue(color_id);
            var lopt = makeHueOption(hue2);
            //var line = list(poly->edges.size());
            for (size_t j = 0; j < poly->edges.size(); j++) {
				var l=list(poly->edges[j].size());
				for(size_t k=0;k<poly->edges[j].size();k++)
					l.tuple()[k+1]=new Integer((long)poly->edges[j][k]);
                directive.tuple()[sum++] = tuple(SYS(Line),l,lopt);
            }
			//eline.tuple()[i+1]=line;
            
			//var eline1=tuple(SYS(Line), eline, lopt);
			//directive.tuple()[sum++] = tuple(SYS(Line), line, lopt);
            //graphics complex
           // var directive = tuple(SYS(Directive), equad, eline);
            return tuple(SYS(GraphicsComplex), v, directive);
        }
    }
}