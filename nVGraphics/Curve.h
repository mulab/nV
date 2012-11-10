#pragma once
#include <nV/Graphics.h>

namespace nV {
	namespace Graphics {
#define ADD_POINT_ANGLE_CURVE 0.05
#define MIN_DELTA_DIS_CURVE 0.001

		typedef struct {
			double x;
			double lefty;
			double righty;
			double leftd;
			double rightd;
		} singularitynode;

        typedef struct {
            double min;
            double max;

            std::vector<singularitynode *> singularityset;
        }domain;

		class Curve {

		public:
			struct LineStrip {
				ArrayList<Point2d*> vert;
				ArrayList<Color*> color;
			};

		public:
			double spx, spy, xmin, xmax, ymin, ymax;
			int dataNum;

			//about color functions
			bool isColorfulCurve;

			ArrayList<LineStrip*> lineStrips;

			// «∑Ò”–∆Êµ„
			bool singularity;
			std::vector<singularitynode *> singularityset;

            bool WithDomain;
            std::vector<domain *> Domains;

		public:
			Curve();
			Curve(ArrayList<Point2d*> v);
			virtual ~Curve();

		protected:
			Color* getRainbowColor(double code);
			void uninitsingularityset();
            void uninitdomain();
		};

		inline void Curve::uninitsingularityset() {
			for(std::vector<singularitynode *>::iterator it = singularityset.begin(); it != singularityset.end(); ++it) {
				delete (*it);
			}
            singularityset.clear();
		}

        inline void Curve::uninitdomain() {
            for(std::vector<domain *>::iterator it = Domains.begin(); it != Domains.end(); ++it) {
                delete (*it);
            }
            Domains.clear();
        }
	}
}
