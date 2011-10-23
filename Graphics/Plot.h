#pragma once
#include <vector>
#include <algorithm>
#include "GraphicsMaker2D.h"
#include "CommonCurveWithPlotRange.h"
#include "CommonCurve.h"
#include "singularityWithPlotRange.h"
#include "PlotWithDomain.h"

namespace nV {
	namespace Graphics {
#define FILL_NULL 0
#define FILL_TO_AXES 1
#define FILL_TO_CURVE 2
#define MAX_DERIVATIVE 20 //before 20
#define SINGLAR_THRESHOLD 0.000001

		class Plot : GraphicsMaker2D {

		private:
			uint curnum;
			Curve **curs;

			//change by options
			int filling;
			int fillcur1, fillcur2;
			int mode;
			int rangeSet;
			bool colorFunctionSet;
			F1P* cf;
			var xparam;

			//kernel
			Kernel &k;

			//expression
			var expression;

			//是否有奇点
			//bool singularity;
			//std::vector<int> singularityset;

            void getyrange(double &ymax, double &ymin, double &graphScale, double &spx, double &spy, 
                const std::vector<Point2d *> &vert, const std::vector<bool> &flag, 
                const std::vector<bool> &nanflag);

            void detectnanborder(F1P *f, std::vector<domain *> &nanrangeset,
                std::vector<uint> &nannodesite, const std::vector<bool> &nanflag, 
                const std::vector<bool> &derivativeflag,
                const std::vector<Point2d *> &vert);

            double getrightborder(F1P *f, double lx, double rx, uint deepth);

            void getsingularityset(std::vector<bool>::iterator flag,
                std::vector<bool>::iterator nanflag, uint size,
                std::vector<singularitynode *> &singularityset,
                double t_xmax, double t_xmin, ArrayList<double>::iterator d, 
                ArrayList<Point2d *>::iterator vert);

            bool getsimilarvalue(F1P *f2d, double x, singularitynode &node);

		public:
			Plot(Kernel& k, const Tuple& expr);
			~Plot();
			var exportGraphics();

		protected:
			void setRule(var title, var rule);

			//两条函数填充
			var addVecReversed(var vec1, var vec2, var expr1, var expr2, var &out_verts);

			//坐标轴填充
			var addVecReversed(var vec1, var vec2);

            //奇点函数的坐标轴填充
            /*var addVecReversed(var vec1, var vec2, var expr1, var &out_verts, 
                 Curve *cur1, Curve *cur2, const std::vector<singularitynode *> &singularityset);*/

			//一个有奇点，另一个没有奇点的两个函数填充
			var addVecReversed(var vec1, var vec2, var expr1, var expr2,
                var &out_verts, Curve *cur1, Curve *cur2,
                const std::vector<singularitynode *> &singularityset);

            //两个都有奇点的两个函数填充
            var addVecReversed(Curve * cur1,Curve * cur2,var vec1, var vec2, 
                var expr1, var expr2, std::vector<singularitynode *> &node_a1,
                std::vector<singularitynode *> &node_a2,var &out_verts);

            var addvecreversedwithxrange(var **line_indices_f, var expr_f, var &verts_f,
                Curve **curs_f, int fillcur1_f, int fillcur2_f, int curnum_f);

            var addVecReversed(var **line_indices_f, var expr_f, var &verts_f,
                Curve ** curs_f, int fillcur1_f, int fillcur2_f, int curnum_f, uint i);
            var addVecReversed(var **line_indices_f, var expr_f, var &verts_f,
                Curve ** curs_f, int fillcur1_f, int fillcur2_f, int curnum_f, uint i,uint j);

			//判断x区间上函数是否有奇点
            bool hassingularity(F1P *f, std::vector<singularitynode *> &singularityset,
                double &ymin, double &ymax, std::vector<domain *> &nanrangeset,
                double spx = 0.0, double spy = 0.0,
                double graphScale = 0.0, double dx = 0.0);
		};
	}
}
