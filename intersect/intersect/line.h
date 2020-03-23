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
		if (tp != A.tp) {
			return 0;
		}
		if (tp == 'S') {
			return u == A.u && v == A.v;
		}
		if (tp == 'R') {
			return u == A.u && !dcmp(v.y * A.v.x - A.v.y * v.x);
		}
		return !dcmp(v.y * A.v.x - A.v.y * v.x) && !dcmp((u - A.u) ^ (u - (A.u + A.v)));
	}
	void print() {
		u.print();
		(u + v).print();
		printf("id is : %d tp is : %c\n", id, tp);
	}
	Point point(double t) const {
		return u + v * t;
	}
};

struct Segment {
	Point A, B;
};