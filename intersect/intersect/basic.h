#pragma once
#include<ctime>
#include<queue>
#include<cstdio>
#include<vector>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<unordered_set>
#define mp make_pair
#define pb push_back
#define pii pair<int, int>
using namespace std;
typedef long long LL;
const int inf = 1 << 30;
const int maxn = 500010;
const int MAX_LEVEL = 10;
const double eps = 1e-10;
inline int dcmp(double x) {
	return (x > eps) - (x < -eps);
}