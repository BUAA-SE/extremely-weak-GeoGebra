#include "core.h"
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
	LL hsval = 1ll * min(x, y) * cnt_l + max(x, y);
	if (vis[x] && vis[y]) {
		hs.insert(hsval);
		return;
	}
	if (cG.lineIntersectionWithLine(line[x], line[y])) {
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
	int cnt_line = 0;
	while (!pq.empty()) {
		vector<int> lines;
		auto node = pq.top();
		sL.setX(node.x.x);
		++cnt_line;
		//node.x.print();
		//printf("%d %d\n", node.id, node.tp);
		sL.sp = -1;
		while (!pq.empty()) {
			auto top = pq.top();
			if (top.x == node.x) {
				pq.pop();
				lines.push_back(top.id);
				if (abs(top.tp) < inf) {
					lines.push_back(top.tp);
				}
				else if (top.tp == inf) {
					vis[top.id] = 2; //right point
				}
				else {
					vis[top.id] = 1; //left point
					sL.insert(line[top.id]);
					auto it = sL.find(line[top.id]);
					if (sL.hasPre(it)) {
						updateInsertion(sL.preLine(it), top.id);
					}
					if (sL.hasNext(it)) {
						updateInsertion(top.id, sL.nextLine(it));
					}
				}
			}
			else {
				break;
			}
		}
		sort(lines.begin(), lines.end());
		auto new_end = unique(lines.begin(), lines.end());
		lines.erase(new_end, lines.end());

		if (lines.size() > 1) {
			vec.push_back(node.x);
		}
		for (int l : lines) if (vis[l] < 2) {
			vis[l] = 1;
			sL.erase(line[l]);
		}
		for (int l : lines) if (vis[l] == 2) {
			auto it = sL.find(line[l]);
			if (sL.hasPre(it) && sL.hasNext(it)) {
				int cnt = 0;
				cnt += cG.lineIntersectionWithLine(line[l], line[sL.preLine(it)]);
				Point p = cG.globalIntersection;
				cnt += cG.lineIntersectionWithLine(line[l], line[sL.nextLine(it)]);
				if (!(cnt == 2 && p == cG.globalIntersection)) {
					updateInsertion(sL.preLine(it), sL.nextLine(it));
				}
			}
			sL.erase(line[l]);
		}
		sL.sp = 1;
		for (int l : lines) if (vis[l] < 2) {
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
		//sL.printList();
	}
	sort(vec.begin(), vec.end());
	auto new_end = unique(vec.begin(), vec.end());
	vec.erase(new_end, vec.end());
	return (int) vec.size();
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
					|| cG.pointOnSegment2(A, line[i]) || cG.pointOnSegment2(B, line[i])) {
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
	return (int) vec.size();
}

bool Core::needBruteForce() {
	return cnt_l <= 2000 || cnt_c;
}