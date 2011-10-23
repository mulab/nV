#pragma once

#include "GraphicsMaker2D.h"

#define FILL_NULL 0
#define FILL_AXIS 1

#define JOINED_NULL flase
#define JOINED true

namespace nV {
    namespace Graphics {
        class ListPlot : GraphicsMaker2D {

        private:
            //int pnum;
            int filling;
            bool islist;
            //Point2d* plist;
            var varplist;
            Color* color;
            bool *joined;

            Kernel &k;

        public:
            ListPlot(Kernel &k, const Tuple &expr);
            ~ListPlot();
            var exportGraphics();

        private:
            void makePointList(var &vec_indices, var &line_indices);
            var makeColorOption(Color* color);
            var addVecReversed(var &vec_indices, const var &line_indice, Hue *hue);

        protected:
            void setRule(var title, var rule);
        };

    }
}