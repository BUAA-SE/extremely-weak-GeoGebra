#include "intersect.h"

bool pointOnSegment(const Point& A, const Line& L) {
	if (L.tp == 'L') return true;
	if (L.tp == 'R') {
		return dcmp((A - L.u) * L.v) >= 0;
	}
	return dcmp((A - L.u) * (A - (L.u + L.v))) <= 0;
}

int lineIntersectionWithLine(const Line& L1, const Line& L2) {
	if (!dcmp(L1.v ^ L2.v)) return 0;
	Vector u = L1.u - L2.u;
	double t = (L2.v ^ u) / (L1.v ^ L2.v);
	globalIntersection = L1.u + L1.v * t;
	return pointOnSegment(globalIntersection, L1) && pointOnSegment(globalIntersection, L2);
}

void lineIntersectionWithCircle(const Line& L, const Circle& C) {
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

double calLength(Vector x) {
	return sqrt(x * x);
}

double angle(Vector vec) {
	return atan2(vec.y, vec.x);
}

void circleIntersectionWithCircle(const Circle& C1, const Circle& C2) {
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

int solveBasic() {
	//line & line
	for (int i = 1; i <= cnt_l; ++i)
		for (int j = i + 1; j <= cnt_l; ++j) {
			if (lineIntersectionWithLine(line[i], line[j])) {
				vec.push_back(globalIntersection);
			}
		}
	//circle & circle
	for (int i = 1; i <= cnt_c; ++i)
		for (int j = i + 1; j <= cnt_c; ++j) {
			circleIntersectionWithCircle(circle[i], circle[j]);
		}
	//line & circle
	for (int i = 1; i <= cnt_l; ++i)
		for (int j = 1; j <= cnt_c; ++j) {
			lineIntersectionWithCircle(line[i], circle[j]);
		}
	sort(vec.begin(), vec.end());
	auto new_end = unique(vec.begin(), vec.end());
	vec.erase(new_end, vec.end());
	return vec.size();
}

int main(int argc, char* argv[])
{
	errno_t err;
	FILE* stream;
	for (int i = 0; i < argc; ++i) {
		if (argv[i][0] == '-' && argv[i][1] == 'o')
			err = freopen_s(&stream, argv[i + 1], "w", stdout);
		if (argv[i][0] == '-' && argv[i][1] == 'i')
			err = freopen_s(&stream, argv[i + 1], "r", stdin);
	}
	ios::sync_with_stdio(false);
	Point A, B; char tp[10];
	cin >> n;
	for (int i = 1; i <= n; ++i) {
		cin >> tp;
		if (tp[0] == 'C') {
			++cnt_c;
			circle[cnt_c].c.read();
			cin >> circle[cnt_c].r;
		}
		else {
			++cnt_l;
			A.read();
			B.read();
			line[cnt_l].u = A;
			line[cnt_l].v = B - A;
			line[cnt_l] = Line(A, B - A, tp[0]);
		}
	}
	printf("%d\n", solveBasic());
	return 0;
}