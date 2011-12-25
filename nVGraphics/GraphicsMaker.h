#pragma once
#include "Curve.h"
#include "Curve3D.h"
#include "IndexedQuadArray.h"

namespace nV {
    namespace Graphics {

        class GraphicsMaker {

        protected:
            ArrayList<var> optlist;

        public:
            GraphicsMaker();

            //readers
            static void readVerts2d(const Tuple& expr, int* num, Point2d** vert);
            static void readPoint2d(const Tuple& expr, Point2d* vert);

            //var makers
            static var makeRange(double xmin, double xmax, double ymin, double ymax);
            static var makeRange(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
            static var makeColor255(Color* color);
            static var makeColor255Option(Color* color);
            static var makeHueOption(Hue* hue);

            static Hue* getHue(int i);
            static Hue* getLightHue(int i);


            //shape makers
            static var makeColorfulLines(Curve* cur);
            //return the list of some [ColorfulLine]s
            static var makeLine(Curve* cur, Color* color); //return a [Line]
            static var makeLine(Curve3D* cur, Color* color); //return a [Line]
            static var makeLine(Curve* cur, Hue* hue); //return a [Line]
            static var makeLine(Curve3D* cur, Hue* hue); //return a [Line]

            static void makeDirectiveOfLines(Curve** curs, int curnum, var* out_verts, var* out_lines);
            //make verts into out_verts, and make indices of lines into out_lines
            static var makeIndexedPolygon(IndexedQuadArray* poly, int color_id);


        protected:
            //to be override
            virtual void readOptions(const Tuple& opts);
            virtual var makeGlobalOptions() = 0;
            virtual var makeGraphics(var shape, var opts) = 0;
            virtual var makeGraphics(var shape) = 0;
            virtual void setRule(var title, var rule) {}
            virtual void setCommonRule(var title, var rule) {}
        };
    }
}