#pragma once
#include "computationalGeometry.h"
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
	double scanX;
	vector<int> upd;
	computationalGeometry cG;
	int cnt, top, tail, st[MAX_LEVEL + 2], stk[maxn * MAX_LEVEL];
	int randLevel() {
		int ret = 1;
		while (rand() % 2 && ret <= MAX_LEVEL) {
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
	int sp;
	int id2pos[maxn];
	int cmp(const Line& L1, const Line& L2) {
		if (L1.id == inf) return 0;
		if (L1.id == L2.id) return 0;
		if (dcmp(L1.v ^ L2.v)) {
			if (L1.id < L2.id) {
				Vector u = L1.u - L2.u;
				double t = (L2.v ^ u) / (L1.v ^ L2.v);
				cG.globalIntersection = L1.u + L1.v * t;
			}
			else {
				Vector u = L2.u - L1.u;
				double t = (L1.v ^ u) / (L2.v ^ L1.v);
				cG.globalIntersection = L2.u + L2.v * t;
			}
		}
		else {
			return dcmp(cG.calY(L1.u, L1.u + L1.v, scanX) - cG.calY(L2.u, L2.u + L2.v, scanX)) < 0;
		}
		if (dcmp(cG.globalIntersection.x - scanX)) {
			return dcmp(cG.calY(L1.u, L1.u + L1.v, scanX) - cG.calY(L2.u, L2.u + L2.v, scanX)) < 0;
		}
		if (sp < 0) {
			return dcmp(cG.calY(L1.u, L1.u + L1.v, -1e11) - cG.calY(L2.u, L2.u + L2.v, -1e11)) < 0;
		}
		return dcmp(cG.calY(L1.u, L1.u + L1.v, 1e11) - cG.calY(L2.u, L2.u + L2.v, 1e11)) < 0;
	}
	skipList() {
		cnt = top = sp = 0;
		scanX = -1e11;
		tail = newNode();
		memset(stk, 0, sizeof(stk));
		sL[tail].key.id = inf;
		for (int i = 1; i <= MAX_LEVEL; ++i) {
			st[i] = newNode();
			sL[st[i]].key = Line(Point(0, -1e11), Vector(1, 0));
			sL[st[i]].dNext = st[i - 1]; sL[st[i]].rNext = tail;
		}
	}
	void printList() {
		double lsty = -1e30;
		printf("scanX is : %.16lf\n", scanX);
		for (int i = sL[st[1]].rNext; i != tail; i = sL[i].rNext) {
			double newy = cG.calY(sL[i].key.u, sL[i].key.u + sL[i].key.v, scanX);
			printf("%.16lf(%d) ", newy, sL[i].key.id);
			lsty = newy;
		}
	}
	void setX(double x) {
		scanX = x;
	}
	double getX() {
		return scanX;
	}
	inline int find(Line key) {
		return id2pos[key.id];
	}
	void insert(Line key) {
		int p = st[MAX_LEVEL];
		int k = randLevel();
		int tmp, rNext;
		upd.clear();
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
		id2pos[key.id] = upd.back();
		for (int i = 0; i < (int)upd.size() - 1; ++i) {
			sL[upd[i]].dNext = upd[i + 1];
		}
	}
	void erase(Line key) {
		upd.clear();
		int p = st[MAX_LEVEL];
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
			if (sL[y].key.id == key.id) {
				fd = 1;
				z = sL[y].rNext;
				sL[x].rNext = z;
				sL[z].lNext = x;
				delNode(y);
			}
		}
		/*if (!fd) {
			cout << "GG:" << endl;
			cout << sL[sL[y].lNext].key.id << " " << sL[y].key.id << endl;
			printList();
			exit(0);
		}*/
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