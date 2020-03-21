#pragma once
#include "point.h"
struct Line {
	int id;
	char tp;
	Point u;
	Vector v;
	Line() { id = 0;  tp = 'L'; }
	Line(Point u, Vector v) :u(u), v(v) { id = 0; tp = 'L'; }
	Line(Point u, Vector v, char tp) :u(u), v(v), tp(tp) { id = 0; }
	bool operator == (const Line& A) const {
		return u == A.u && v == A.v && tp == A.tp;
	}
	Point point(double t) const {
		return u + v * t;
	}
};

struct Segment {
	Point A, B;
};