#include "core.h"
Core core;

int main() {
    core.init();
    core.addLine(Point(0, 0), Point(20, 20), 'L');
    core.addLine(Point(0, 20), Point(20, 0), 'S');
    core.addLine(Point(10, 20), Point(-10, 30), 'S');
    core.addLine(Point(-10, 0), Point(20, 30), 'R');
    core.addLine(Point(-10, 0), Point(-20, 40), 'R');
    core.addLine(Point(-20, 0), Point(0, -10), 'R');
    core.addCircle(Point(0, 0), 40);
    core.addCircle(Point(30, 0), 10);
    core.addCircle(Point(0, 0), 20);
    core.addCircle(Point(100, 100), 30);
    core.bruteForce();
    return 0;
}