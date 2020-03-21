#pragma once
#include "skipList.h"
class Core {
private:
	struct Node {
		Point x;
		int id, tp;//-inf->left inf->down <right -> intersec (line_{id} & line_{tp})
		Node(Point x, int id, int tp) : x(x), id(id), tp(tp) { }
		bool operator < (const Node& A) const {
			if (!dcmp(x.x - A.x.x)) {
				if (!(tp == A.tp || (abs(tp) < inf && abs(A.tp) < inf))) {
					return tp > A.tp;
				}
				return dcmp(x.y - A.x.y) > 0;
			}
			return dcmp(x.x - A.x.x) > 0;
		}
	};
	skipList sL;
	vector<Point> vec;
	Circle circle[maxn];
	unordered_set<LL> hs;
	Segment segment[maxn];
	priority_queue<Node> pq;
	computationalGeometry cG;
	int cnt_c, cnt_l, cnt_lv, vis[maxn];
	Line line[maxn], lineVertical[maxn];
	int inrange(double x) {
		return x > -1e5 && x < 1e5;
	}
	int isInt(double x) {
		return !dcmp(x - (int)x);
	}
	void classify();
	void transToSegment();
	void updateInsertion(int x, int y);
	int findCircle(const Point& C, const double& r);
	int findLine(const Point& A, const Point& B, const char& tp);
public:
	Core() {
		cnt_c = cnt_l = cnt_lv = 0;
		memset(vis, 0, sizeof(vis));
	}
	void init() {
		cnt_c = cnt_l = cnt_lv = 0;
	}
	int sweepLine();
	int bruteForce();
	bool needBruteForce();
	vector<Line> getLines();
	vector<Circle> getCircles();
	int addCircle(const Point& C, const double& r);
	int delCircle(const Point& C, const double& r);
	int addLine(const Point& A, const Point& B, const char& tp);
	int delLine(const Point& A, const Point& B, const char& tp);
};