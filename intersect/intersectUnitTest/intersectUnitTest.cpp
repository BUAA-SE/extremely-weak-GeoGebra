#include "pch.h"
#include "CppUnitTest.h"
#include "../intersect/core.h"
#include "../intersect/core.cpp"
#include "../intersect/computationalGeometry.h"
#include "../intersect/computationalGeometry.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

Core core;
namespace intersectUnitTest
{
	TEST_CLASS(intersectUnitTest)
	{
	public:
		TEST_METHOD(testAbnormalLine)
		{
            core.init();
            Assert::AreEqual(core.addLine(Point(0, 0), Point(3, 6), 'A'), -1);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(0, 0), 'L'), -2);
            Assert::AreEqual(core.addLine(Point(1e6, 0), Point(0, 0), 'L'), -3);
            Assert::AreEqual(core.addLine(Point(0, -1e6), Point(0, 0), 'L'), -3);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(1e6, 0), 'L'), -3);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(0, -1e6), 'L'), -3);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(3.3, 0), 'L'), -4);
            Assert::AreEqual(core.addLine(Point(1.1, 0), Point(0, 0), 'L'), -4);
            Assert::AreEqual(core.addLine(Point(0, 2.2), Point(0, 0), 'L'), -4);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(0, 4.4), 'L'), -4);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(1, 1), 'L'), 0);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(-1, -1), 'L'), -5);
            core.init();
            Assert::AreEqual(core.addLine(Point(0, 0), Point(1, 1), 'S'), 0);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(-1, -1), 'S'), 0);
            Assert::AreEqual(core.addLine(Point(1, 1), Point(-1, -1), 'R'), -5);
            core.init();
            Assert::AreEqual(core.addLine(Point(0, 0), Point(2, 2), 'R'), 0);
            Assert::AreEqual(core.addLine(Point(0, 1), Point(3, 4), 'R'), 0);
            Assert::AreEqual(core.addLine(Point(-1, -1), Point(1, 1), 'R'), -5);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(-1, -1), 'R'), 0);
            Assert::AreEqual(core.addLine(Point(0, 0), Point(10, 10), 'L'), -5);
		}
        TEST_METHOD(testAbnormalCircle)
        {
            core.init();
            core.addCircle(Point(0, 0), -1);
            core.addCircle(Point(99999, 99999), 1);
            core.addCircle(Point(1.1, 0), 1);
            core.addCircle(Point(0, 1.1), 1);
            core.addCircle(Point(0, 0), 5);
            core.addCircle(Point(0, 0), 5);
        }
		TEST_METHOD(line_line)
		{
            core.init();
            core.addLine(Point(0, 0), Point(3, 6), 'L');
            core.addLine(Point(0, 1), Point(1, 2), 'L');
            Assert::AreEqual(core.bruteForce(), (int)1);

            core.init();
            core.addLine(Point(0, 0), Point(1, 2), 'L');
            core.addLine(Point(1, 3), Point(3, 7), 'L');
            Assert::AreEqual(core.bruteForce(), (int)0);

            //
            core.init();
            core.addLine(Point(0, 0), Point(3, 6), 'R');
            core.addLine(Point(2, 0), Point(2, 5), 'R');
            Assert::AreEqual(core.bruteForce(), (int)1);

            core.init();
            core.addLine(Point(0, 0), Point(1, 2), 'R');
            core.addLine(Point(-7, -7), Point(-9, -9), 'R');
            Assert::AreEqual(core.bruteForce(), (int)0);

            //
            core.init();
            core.addLine(Point(0, 0), Point(3, 6), 'S');
            core.addLine(Point(2, 0), Point(2, 5), 'S');
            Assert::AreEqual(core.bruteForce(), (int)1);

            core.init();
            core.addLine(Point(0, 0), Point(1, 2), 'S');
            core.addLine(Point(-7, -7), Point(-9, -9), 'S');
            Assert::AreEqual(core.bruteForce(), (int)0);
		}
        TEST_METHOD(line_circle)
        {
            core.init();
            core.addLine(Point(1, 2), Point(3, 6), 'L');
            core.addCircle(Point(0, 0), 10);
            Assert::AreEqual(core.bruteForce(), (int)2);

            core.init();
            core.addLine(Point(1, 2), Point(3, 6), 'L');
            core.addCircle(Point(5, -5), 1);
            Assert::AreEqual(core.bruteForce(), (int)0);

            core.init();
            core.addLine(Point(0, 2), Point(0, 5), 'R');
            core.addCircle(Point(0, 0), 2);
            Assert::AreEqual(core.bruteForce(), (int)1);

            core.init();
            core.addLine(Point(0, 3), Point(0, 5), 'R');
            core.addCircle(Point(0, 0), 1);
            Assert::AreEqual(core.bruteForce(), (int)0);

            core.init();
            core.addLine(Point(-1, 0), Point(1, 0), 'S');
            core.addCircle(Point(0, 0), 1);
            Assert::AreEqual(core.bruteForce(), (int)2);

            core.init();
            core.addLine(Point(0, 0), Point(0, 1), 'S');
            core.addCircle(Point(0, 0), 10);
            Assert::AreEqual(core.bruteForce(), (int)0);
        }
        TEST_METHOD(circle_circle)
        {
            core.init();
            core.addCircle(Point(0, 0), 10);
            core.addCircle(Point(0, 0), 1);
            Assert::AreEqual(core.bruteForce(), (int)0);

            core.init();
            core.addCircle(Point(0, 0), 1);
            core.addCircle(Point(5, 0), 1);
            Assert::AreEqual(core.bruteForce(), (int)0);

            core.init();
            core.addCircle(Point(0, 0), 10);
            core.addCircle(Point(1, 0), 10);
            Assert::AreEqual(core.bruteForce(), (int)2);

            core.init();
            core.addCircle(Point(0, 0), 1);
            core.addCircle(Point(2, 0), 1);
            Assert::AreEqual(core.bruteForce(), (int)1);

            core.init();
            core.addCircle(Point(0, 0), 3);
            core.addCircle(Point(2, 0), 1);
            Assert::AreEqual(core.bruteForce(), (int)1);
        }
        TEST_METHOD(test_all)
        {
            core.init();
            core.addLine(Point(0, 0), Point(2, 2), 'L');
            core.addLine(Point(0, 2), Point(2, 0), 'S');
            core.addLine(Point(1, 2), Point(-1, 3), 'S');
            core.addLine(Point(-1, 0), Point(2, 3), 'R');
            core.addLine(Point(-1, 0), Point(-2, 4), 'R');
            core.addLine(Point(-2, 0), Point(0, -1), 'R');
            Assert::AreEqual(core.bruteForce(), (int)5);
        }
        TEST_METHOD(test_all2)
        {
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
            Assert::AreEqual(core.bruteForce(), (int)23);
        }
	};
}