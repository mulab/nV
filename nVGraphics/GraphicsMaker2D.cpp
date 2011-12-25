#include "GraphicsMaker2D.h"

namespace nV {
    namespace Graphics {

        GraphicsMaker2D::GraphicsMaker2D() {
            axeX = true;
            axeY = true;
        }

        var GraphicsMaker2D::makeGlobalOptions() {
            var range = GraphicsMaker::makeRange(xmin, xmax, ymin, ymax);
            optlist.add(range);
            var opts = list(optlist.size(), SYS(Options));
            for (size_t i = 0; i < optlist.size(); i++) {
                opts.tuple()[i + 1] = optlist[i];
            }
            return opts;
        }

        var GraphicsMaker2D::makeGraphics(var shape, var opts) {
            return tuple(SYS(Graphics2D), shape, opts);
        }

        var GraphicsMaker2D::makeGraphics(var shape) {
            var opts = makeGlobalOptions();
            return makeGraphics(shape, opts);
        }

        void GraphicsMaker2D::setCommonRule(var title, var rule) {
            if (title == SYS(Axes)) {
                if(rule.tuple().size == 3) {
                    var op = tuple(title, rule.tuple()[1], rule.tuple()[2]);
                    optlist.add(op);
                }
            }
            if (title == SYS(AxesLabel)) {
                if(rule.tuple().size == 3) {
                    //var op = tuple(title, rule.tuple()[1], rule.tuple()[2]);
                    //var op = list(rule, title);
                    var temp = list(rule.tuple()[1], rule.tuple()[2]);
                    //var op = tuple(title, temp);
                    var op = list(temp, SYS(AxesLabel));
                    optlist.add(op);
                }
            }
            if (title == SYS(GraphLabel)) {
                var op = tuple(title, rule);
                optlist.add(op);
            }
        }
    }
}