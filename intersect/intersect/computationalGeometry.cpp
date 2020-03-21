#include "computationalGeometry.h"

double computationalGeometry::angle(Vector vec) {
	return atan2(vec.y, vec.x);
}

double computationalGeometry::calLength(Vector x) {
	return sqrt(x * x);
}

bool computationalGeometry::pointOnSegment(const Point& A, const Line& L) {
	if (L.tp == 'L') return true;
	if (L.tp == 'R') {
		return dcmp((A - L.u) * L.v) >= 0;
	}
	return dcmp((A - L.u) * (A - (L.u + L.v))) <= 0;
}

double computationalGeometry::calY(const Point& A, const Point& B, double x) {
	if (!dcmp(A.y - B.y)) {
		return A.y;
	}
	double k1 = x - A.x, k2 = x - B.x;
	return (k1 * B.y - k2 * A.y) / (k1 - k2);
}

int computationalGeometry::lineIntersectionWithLine(const Line& L1, const Line& L2) {
	if (!dcmp(L1.v ^ L2.v)) return 0;
	Vector u = L1.u - L2.u;
	double t = (L2.v ^ u) / (L1.v ^ L2.v);
	globalIntersection = L1.u + L1.v * t;
	return pointOnSegment(globalIntersection, L1) && pointOnSegment(globalIntersection, L2);
}

void computationalGeometry::lineIntersectionWithCircle(const Line& L, const Circle& C, vector<Point>& vec) {
	Point intersec;
	double t1, t2;
	double a = L.v.x, b = L.u.x - C.c.x, c = L.v.y, d = L.u.y - C.c.y;
	double e = a * a + c * c, f = 2 * (a * b + c * d), g = b * b + d * d - C.r * C.r;
	double delta = f * f - 4 * e * g;
	if (dcmp(delta) < 0) return;
	if (dcmp(delta) == 0) {
		intersec = L.point(-f / (2 * e));
		if (pointOnSegment(intersec, L))
			vec.push_back(intersec);
	}
	else {
		t1 = (-f - sqrt(delta)) / (2 * e); intersec = L.point(t1);
		if (pointOnSegment(intersec, L)) vec.push_back(L.point(t1));
		t2 = (-f + sqrt(delta)) / (2 * e); intersec = L.point(t2);
		if (pointOnSegment(intersec, L)) vec.push_back(L.point(t2));
	}
}


void computationalGeometry::circleIntersectionWithCircle(const Circle& C1, const Circle& C2, vector<Point>& vec) {
	double d = calLength(C1.c - C2.c);
	if (dcmp(d) == 0)  return;
	if (dcmp(C1.r + C2.r - d) < 0) return;
	if (dcmp(fabs(C1.r - C2.r) - d) > 0) return;
	double a = angle(C2.c - C1.c);
	double da = acos((C1.r * C1.r + d * d - C2.r * C2.r) / (2 * C1.r * d));
	Point p1 = C1.point(a - da), p2 = C1.point(a + da);
	vec.push_back(p1);
	if (p1 == p2) return;
	vec.push_back(p2);
}