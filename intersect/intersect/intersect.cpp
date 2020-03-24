#pragma once
/*
#include "core.h"
Core core;

int main(int argc, char* argv[])
{
	int n;
	double r;
	Point A, B;
	char tp[10];
	errno_t err;
	FILE* stream;	
	srand((unsigned int) time(0));
	for (int i = 0; i < argc; ++i) {
		if (argv[i][0] == '-' && argv[i][1] == 'o')
			err = freopen_s(&stream, argv[i + 1], "w", stdout);
		if (argv[i][0] == '-' && argv[i][1] == 'i')
			err = freopen_s(&stream, argv[i + 1], "r", stdin);
	}
	ios::sync_with_stdio(false);
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

*/