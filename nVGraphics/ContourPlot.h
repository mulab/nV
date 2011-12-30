#pragma once

#include "GraphicsMaker2D.h"
#include "F2P.h"
#include "ImplicitCurve.h"

namespace nV {
    namespace Graphics {

        class ContourPlot : GraphicsMaker2D {
        private:
            int curnum;
            ImplicitCurve *cur;

            Kernel &k;
            var expression;

        public:
            ContourPlot(Kernel &k, const Tuple &expr);
            ~ContourPlot();
            var exportGraphics();

        protected:
            void setRule(var title, var rule);
        };

    }
}