#pragma once
#include "basic.h"

struct Point {
	double x, y;
	Point(double x = 0, double y = 0) : x(x), y(y) {}
	void read() {
		cin >> x >> y;
	}
	void print() {
		printf("%.10lf %.10lf\n", x, y);
	}
	bool operator < (const Point& B) const { return dcmp(x - B.x) < 0 || (!dcmp(x - B.x) && dcmp(y - B.y) < 0); }
};
typedef Point Vector;

inline Vector operator + (const Vector& A, const Vector& B) { return Vector(A.x + B.x, A.y + B.y); }
inline Vector operator - (const Vector& A, const Vector& B) { return Vector(A.x - B.x, A.y - B.y); }
inline double operator * (const Vector& A, const Vector& B) { return A.x * B.x + A.y * B.y; }
inline Point operator * (const Vector& A, const double& p) { return Point(A.x * p, A.y * p); }
inline bool operator == (const Point& A, const Point& B) {  return !dcmp(A.x - B.x) && !dcmp(A.y - B.y); }
inline double operator ^ (const Vector& A, const Vector& B) { return A.x * B.y - A.y * B.x; }
