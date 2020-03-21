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

class skipList {
private:
	struct listNode {
		Line key;
		int lNext, rNext, dNext;
		listNode() {
			lNext = rNext = dNext = 0;
		}
		void init() {
			lNext = rNext = dNext = 0;
		}
	}sL[maxn * MAX_LEVEL];
	double scanX, preDiffX;
	computationalGeometry cG;
	int cnt, top, tail, st[MAX_LEVEL + 2], stk[maxn * MAX_LEVEL];
	int randLevel() {
		int ret = 1;
		while (rnd() % 2 && ret <= MAX_LEVEL) {
			++ret;
		}
		return ret;
	}
	int newNode() {
		int r = top ? stk[top--] : ++cnt;
		sL[r].init();
		return r;
	}
	void delNode(int p) {
		stk[++top] = p;
	}
public:
	int ord;
	int rev[maxn];
	int cmp(const Line& L1, const Line& L2) {
		if (L1.id == inf) return 0;
		if (L1.id == L2.id) return 0;
		cG.globalIntersection.x = -1e11;
		cG.lineIntersectionWithLine(L1, L2);
		if (!dcmp(cG.globalIntersection.x + 1e11) || dcmp(cG.globalIntersection.x - scanX)) {
			double y1 = cG.calY(L1.u, L1.u + L1.v, scanX);
			double y2 = cG.calY(L2.u, L2.u + L2.v, scanX);
			return dcmp(y1 - y2) < 0;
		}
		return rev[L1.id] > rev[L2.id];
	}
	skipList() {
		cnt = ord = top = 0;
		scanX = preDiffX = -1e11;
		tail = newNode();
		memset(rev, 0, sizeof(rev));
		memset(stk, 0, sizeof(stk));
		sL[tail].key.id = inf;
		for (int i = 1; i <= MAX_LEVEL; ++i) {
			st[i] = newNode();
			sL[st[i]].key = Line(Point(0, -1e11), Vector(1, 0));
			sL[st[i]].dNext = st[i - 1]; sL[st[i]].rNext = tail;
		}
	}
	void printList() {
		printf("scanX is : %.16lf\n", scanX);
		for (int i = sL[st[1]].rNext; i != tail; i = sL[i].rNext) {
			printf("%.16lf(%d, %d) ", cG.calY(sL[i].key.u, sL[i].key.u + sL[i].key.v, scanX), sL[i].key.id, rev[sL[i].key.id]);
		}
		printf("\n");
	}
	void setX(double x) {
		if (dcmp(x - scanX)) {
			preDiffX = scanX;
		}
		scanX = x;
	}
	double getX() {
		return scanX;
	}
	int find(Line key) {
		int p = st[MAX_LEVEL];
		for (int i = MAX_LEVEL; i; --i) {
			while (cmp(sL[sL[p].rNext].key, key)) {
				p = sL[p].rNext;
			}
			if (i > 1) {
				p = sL[p].dNext;
			}
		}
		return sL[p].rNext;
	}
	void insert(Line key) {
		int p = st[MAX_LEVEL];
		int k = randLevel();
		vector<int> upd;
		int tmp, rNext;
		for (int i = MAX_LEVEL; i; --i) {
			while (cmp(sL[sL[p].rNext].key, key)) {
				p = sL[p].rNext;
			}
			if (i <= k) {
				tmp = newNode(); rNext = sL[p].rNext;
				sL[tmp].lNext = p; sL[tmp].rNext = rNext; sL[tmp].dNext = 0; sL[tmp].key = key;
				sL[p].rNext = tmp; sL[rNext].lNext = tmp;
				upd.push_back(tmp);
			}
			p = sL[p].dNext;
		}
		for (int i = 0; i < (int)upd.size() - 1; ++i) {
			sL[upd[i]].dNext = upd[i + 1];
		}
	}
	void erase(Line key) {
		int p = st[MAX_LEVEL];
		vector<int> upd;
		for (int i = MAX_LEVEL; i; --i) {
			while (cmp(sL[sL[p].rNext].key, key)) {
				p = sL[p].rNext;
			}
			upd.push_back(p);
			p = sL[p].dNext;
		}
		int fd = 0;
		int y, z;
		for (int x : upd) {
			y = sL[x].rNext;
			if (sL[y].key == key) {
				fd = 1;
				z = sL[y].rNext;
				sL[x].rNext = z;
				sL[z].lNext = x;
				delNode(y);
			}
		}
	}
	void revX() {
		swap(scanX, preDiffX);
	}
	bool hasPre(int p) {
		return sL[p].lNext != st[1];
	}
	int preLine(int p) {
		return sL[sL[p].lNext].key.id;
	}
	bool hasNext(int p) {
		return sL[p].rNext != tail;
	}
	int nextLine(int p) {
		return sL[sL[p].rNext].key.id;
	}
};

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
}core;

void Core::classify() {
	int m = 0; vec.clear();
	for (int i = 1; i <= cnt_l; ++i) {
		if (!dcmp(line[i].v.x)) {
			lineVertical[++cnt_lv] = line[i];
		}
		else {
			line[++m] = line[i];
			line[m].id = m;
		}
	}
	cnt_l = m;
	for (int i = 1; i <= cnt_l; ++i)
		for (int j = 1; j <= cnt_lv; ++j) {
			if (cG.lineIntersectionWithLine(line[i], lineVertical[j])) {
				vec.push_back(cG.globalIntersection);
			}
		}
	sort(vec.begin(), vec.end());
	auto new_end = unique(vec.begin(), vec.end());
	vec.erase(new_end, vec.end());
}

void Core::transToSegment() {
	for (int i = 1; i <= cnt_l; ++i) {
		if (line[i].tp == 'L') {
			segment[i].A = Point(-1e11, cG.calY(line[i].u, line[i].u + line[i].v, -1e11));
			segment[i].B = Point(1e11, cG.calY(line[i].u, line[i].u + line[i].v, 1e11));
		}
		else if (line[i].tp == 'R') {
			if (dcmp(line[i].v.x) > 0) {
				segment[i].A = line[i].u;
				segment[i].B = Point(1e11, cG.calY(line[i].u, line[i].u + line[i].v, 1e11));
			}
			else {
				segment[i].A = Point(-1e11, cG.calY(line[i].u, line[i].u + line[i].v, -1e11));
				segment[i].B = line[i].u;
			}
		}
		else {
			segment[i].A = line[i].u;
			segment[i].B = line[i].u + line[i].v;
			if (segment[i].A.x > segment[i].B.x)
				swap(segment[i].A, segment[i].B);
		}
		pq.push(Node(segment[i].A, i, -inf));
		pq.push(Node(segment[i].B, i, inf));
	}
}

void Core::updateInsertion(int x, int y) {
	if (vis[x] && vis[y]) return;
	if (cG.lineIntersectionWithLine(line[x], line[y])) {
		LL hsval = 1ll * min(x, y) * cnt_l + max(x, y);
		if (hs.find(hsval) == hs.end()) {
			hs.insert(hsval);
			pq.push(Node(cG.globalIntersection, x, y));
		}
	}
}

int Core::sweepLine() {
	vec.clear();
	classify();
	transToSegment();
	while (!pq.empty()) {
		auto node = pq.top(); pq.pop();
		//		node.x.print();
		//		printf("%d %d\n", node.id, node.tp);
		if (node.tp == -inf) {
			//left endpoint
			sL.setX(node.x.x);
			sL.insert(line[node.id]);
			auto it = sL.find(line[node.id]);
			if (sL.hasPre(it)) {
				updateInsertion(sL.preLine(it), node.id);
			}
			if (sL.hasNext(it)) {
				updateInsertion(node.id, sL.nextLine(it));
			}
		}
		else if (node.tp == inf) {
			//right endpoint
			sL.setX(node.x.x);
			auto it = sL.find(line[node.id]);
			if (sL.hasPre(it) && sL.hasNext(it)) {
				updateInsertion(sL.preLine(it), sL.nextLine(it));
			}
			sL.erase(line[node.id]);
		}
		else {
			//intersect
			vector<int> lines;
			vec.push_back(node.x);
			lines.push_back(node.id);
			lines.push_back(node.tp);
			while (!pq.empty()) {
				auto top = pq.top();
				if (abs(top.tp) < inf && top.x == node.x) {
					pq.pop();
					lines.push_back(top.id);
					lines.push_back(top.tp);
				}
				else {
					break;
				}
			}
			sL.revX();
			sort(lines.begin(), lines.end(), [&](const int& a, const int& b) { return a == b ? 0 : sL.cmp(line[a], line[b]); });
			auto new_end = unique(lines.begin(), lines.end());
			lines.erase(new_end, lines.end());
			sL.revX();
			if (!dcmp(node.x.x - sL.getX())) {
				for (int l : lines)
					sL.rev[l] = --sL.ord;
			}
			for (int l : lines) {
				vis[l] = 1;
				sL.erase(line[l]);
			}
			if (dcmp(node.x.x - sL.getX())) {
				for (int l : lines)
					sL.rev[l] = --sL.ord;
			}
			for (int l : lines) {
				sL.rev[l] *= -1;
			}
			sL.setX(node.x.x);
			for (int l : lines) {
				sL.insert(line[l]);
				auto it = sL.find(line[l]);
				if (sL.hasPre(it)) {
					updateInsertion(sL.preLine(it), l);
				}
				if (sL.hasNext(it)) {
					updateInsertion(l, sL.nextLine(it));
				}
			}
			for (int l : lines) {
				vis[l] = 0;
			}
		}
		//		sL.printList();
	}
	sort(vec.begin(), vec.end());
	auto new_end = unique(vec.begin(), vec.end());
	vec.erase(new_end, vec.end());
	return vec.size();
}

vector<Circle> Core::getCircles() {
	vector<Circle> vecCircle;
	for (int i = 1; i <= cnt_c; ++i)
		vecCircle.push_back(circle[i]);
	return vecCircle;
}

vector<Line> Core::getLines() {
	vector<Line> vecLine;
	for (int i = 1; i <= cnt_l; ++i)
		vecLine.push_back(line[i]);
	for (int i = 1; i <= cnt_lv; ++i)
		vecLine.push_back(lineVertical[i]);
	return vecLine;
}

int Core::findCircle(const Point& C, const double& r) {
	for (int i = 1; i <= cnt_c; ++i) {
		if (circle[i].c == C && !dcmp(circle[i].r - r)) {
			return i;
		}
	}
	return 0;
}

int Core::addCircle(const Point& C, const double& r) {
	if (dcmp(r) <= 0) {
		cerr << "the radius of the circle should be greater than 0" << endl;
		return -1;
	}
	if (!isInt(r)) {
		cerr << "the radius should be an integer" << endl;
		return -2;
	}
	if (!inrange(C.x) || !inrange(C.y)) {
		cerr << "the coordinate range should be within (-1e5, 1e5)" << endl;
		return -3;
	}
	if (!isInt(C.x) || !isInt(C.y)) {
		cerr << "the coordinates should be integers" << endl;
		return -4;
	}
	if (cnt_c <= 1000) {
		for (int i = 1; i <= cnt_c; ++i)
			if (circle[i].c == C && !dcmp(circle[i].r - r)) {
				cerr << "the current circle coincides with an existing circle" << endl;
				return -5;
			}
	}
	circle[++cnt_c] = Circle(C, r);
	return 0;
}

int Core::delCircle(const Point& C, const double& r) {
	int pos = findCircle(C, r);
	if (!pos) {
		cerr << "the circle you want to delete does not exist" << endl;
		return -1;
	}
	swap(circle[pos], circle[cnt_c--]);
	return 0;
}

int Core::findLine(const Point& A, const Point& B, const char& tp) {
	Line l = Line(A, B - A, tp);
	for (int i = 1; i <= cnt_l; ++i) {
		if (line[i] == l) {
			return i;
		}
	}
	return 0;
}

int Core::addLine(const Point& A, const Point& B, const char& tp) {
	if (tp != 'L' && tp != 'R' && tp != 'S') {
		cerr << "wrong type" << endl;
		return -1;
	}
	if (A == B) {
		cerr << "two ends coincide" << endl;
		return -2;
	}
	if (!inrange(A.x) || !inrange(A.y) || !inrange(B.x) || !inrange(B.y)) {
		cerr << "the coordinate range should be within (-1e5, 1e5)" << endl;
		return -3;
	}
	if (!isInt(A.x) || !isInt(A.y) || !isInt(B.x) || !isInt(B.y)) {
		cerr << "the coordinates should be integers" << endl;
		return -4;
	}
	if (cnt_l <= 1000) {
		Vector v = B - A;
		Line l = Line(A, B - A, tp);
		for (int i = 1; i <= cnt_l; ++i) {
			if (!dcmp(line[i].v ^ v)) {
				if (cG.pointOnSegment2(line[i].u, l) || cG.pointOnSegment2(line[i].u + line[i].v, l)
				||  cG.pointOnSegment2(A, line[i]) || cG.pointOnSegment2(B, line[i])) {
					printf("%.10lf %.10lf\n", A.x, A.y); line[i].u.print(); (line[i].u + line[i].v).print();
					cerr << cG.pointOnSegment2(line[i].u, l) << cG.pointOnSegment2(line[i].u + line[i].v, l) << endl;
					cerr << cG.pointOnSegment2(A, line[i]) << cG.pointOnSegment2(B, line[i]) << endl;
					cerr << "the current line coincides with an existing line" << endl;
					return -5;
				}
			}
		}
	}
	line[++cnt_l] = Line(A, B - A, tp);
	return 0;
}

int Core::delLine(const Point& A, const Point& B, const char& tp) {
	int pos = findLine(A, B, tp);
	if (!pos) {
		cerr << "the line you want to delete does not exist" << endl;
		return -1;
	}
	swap(line[pos], line[cnt_l--]);
	return 0;
}

int Core::bruteForce() {
	//line & line
	vec.clear();
	for (int i = 1; i <= cnt_l; ++i)
		for (int j = i + 1; j <= cnt_l; ++j) {
			if (cG.lineIntersectionWithLine(line[i], line[j])) {
				vec.push_back(cG.globalIntersection);
			}
		}
	//circle & circle
	for (int i = 1; i <= cnt_c; ++i)
		for (int j = i + 1; j <= cnt_c; ++j) {
			cG.circleIntersectionWithCircle(circle[i], circle[j], vec);
		}
	//line & circle
	for (int i = 1; i <= cnt_l; ++i)
		for (int j = 1; j <= cnt_c; ++j) {
			cG.lineIntersectionWithCircle(line[i], circle[j], vec);
		}
	sort(vec.begin(), vec.end());
	auto new_end = unique(vec.begin(), vec.end());
	vec.erase(new_end, vec.end());
	return vec.size();
}

bool Core::needBruteForce() {
	return cnt_l <= 1000 || cnt_c;
}

int main(int argc, char* argv[])
{
	int n;
	double r;
	Point A, B;
	char tp[10];
	errno_t err;
	FILE* stream;
	for (int i = 0; i < argc; ++i) {
		if (argv[i][0] == '-' && argv[i][1] == 'o')
			err = freopen_s(&stream, argv[i + 1], "w", stdout);
		if (argv[i][0] == '-' && argv[i][1] == 'i')
			err = freopen_s(&stream, argv[i + 1], "r", stdin);
	}
	ios::sync_with_stdio(false);
	core.addLine(Point(0, 0), Point(2, 2), 'R');
	core.addLine(Point(1, 1), Point(3, 3), 'R');
	exit(0);
	cin >> n;
	for (int i = 1; i <= n; ++i) {
		cin >> tp;
		if (strlen(tp) > 1) {
			cerr << "wrong type" << endl;
			continue;
		}
		if (tp[0] == 'C') {
			A.read(); cin >> r;
			core.addCircle(A, r);
		}
		else {
			A.read(); B.read();
			core.addLine(A, B, tp[0]);
		}
	}
	if (core.needBruteForce()) {
		printf("%d\n", core.bruteForce());
	}
	else {
		printf("%d\n", core.sweepLine());
	}
	return 0;
}