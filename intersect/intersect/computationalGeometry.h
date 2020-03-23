#pragma once
#include "line.h"
#include "basic.h"
#include "point.h"
#include "circle.h"

class computationalGeometry {
private:
	double angle(Vector vec);
	double calLength(Vector x);
	bool pointOnSegment(const Point& A, const Line& L);
public:
	computationalGeometry() {

	}
	Point globalIntersection;
	bool pointOnSegment2(const Point& A, const Line& L) {
		if (dcmp((L.u - A) ^ ((L.u + L.v) - A))) return false;
		if (L.tp == 'L') return true;
		if (L.tp == 'R') {
			return dcmp((A - L.u) * L.v) > 0;
		}
		return dcmp((A - L.u) * (A - (L.u + L.v))) < 0;
	}
	double calY(const Point& A, const Point& B, double x);
	int lineIntersectionWithLine(const Line& L1, const Line& L2);
	void lineIntersectionWithCircle(const Line& L, const Circle& C, vector<Point>& vec);
	void circleIntersectionWithCircle(const Circle& C1, const Circle& C2, vector<Point>& vec);
};