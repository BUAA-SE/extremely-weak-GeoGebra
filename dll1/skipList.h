#pragma once
#ifndef SKIPLIST_H
#define SKIPLIST_H
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
    double scanX, preDiffX;
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
#endif
