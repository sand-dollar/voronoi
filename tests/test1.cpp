#include "test1.h"
#include "geometry.h"
#include <iostream>
#include <algorithm>


static const double DELTA = 0.0001;

/// Convenience compare function for Voronoi::Point
#define COMPARE(actual, expected) \
QVERIFY( \
	actual.x() - DELTA <= expected.x() && \
	actual.x() + DELTA >= expected.x() && \
	actual.y() - DELTA <= expected.y() && \
	actual.y() + DELTA >= expected.y() \
);


namespace
{
/// Simple debug function to print all edges for gnuplot
void printEdgesGnuplot(const std::list<Voronoi::Edge> & edges)
{
	std::cout << "# Edge count: " << edges.size() << std::endl;
	for (const auto & edge : edges) {
		auto begin = edge.begin();
		auto end = edge.end();
		std::cout << begin.x() << "\t" << begin.y() << "\t"
				  << end.x() - begin.x() << "\t" << end.y() - begin.y()
				  << std::endl;
	}
}


/// Simple debug function to print all edges for the user
void printEdgesUser(const std::list<Voronoi::Edge> & edges)
{
	std::cout << "# Edge count: " << edges.size() << std::endl;
	for (const auto & edge : edges) {
		auto begin = edge.begin();
		auto end = edge.end();
		std::cout << "beg(" << begin.x() << ", " << begin.y() << ")";
		std::cout << " -> ";
		std::cout << "end(" << end.x() << ", " << end.y() << ")";
		auto left = edge.left();
		auto right = edge.right();
		std::cout << "  |  left(" << left.x() << ", " << left.y() << ")";
		std::cout << " -> ";
		std::cout << "right(" << right.x() << ", " << right.y() << ")\n";
	}
}
}  // end of anonymous namespace


void VoronoiTest::toUpper_data()
{
	QTest::addColumn<std::vector<Voronoi::Point>>("sites");
	QTest::addColumn<size_t>("edgeCount");
	QTest::addColumn<Voronoi::Point>("begin");
	QTest::addColumn<Voronoi::Point>("end");
	/*
	// TODO Replace with initializer list when supported
	// Points are on [0, 1] interval
	// Test A
	std::vector<Voronoi::Point> s_1;
	s_1.emplace_back(0.2, 0.7);
	s_1.emplace_back(0.4, 0.1);
	const Voronoi::Point e_1(0.0, 0.3);
	const Voronoi::Point b_1(1.0, 0.633333);
	const size_t c_1 = 1;
	QTest::newRow("Test A") << s_1 << c_1 << b_1 << e_1;

	// Test B
	std::vector<Voronoi::Point> s_2;
	s_2.emplace_back(0.2, 0.7);
	s_2.emplace_back(0.3, 0.6);
	s_2.emplace_back(0.4, 0.1);
	const Voronoi::Point b_2(0.0, 0.28);
	const Voronoi::Point e_2(1.0, 0.48);
	const size_t c_2 = 2;
	QTest::newRow("Test B") << s_2 << c_2 << b_2 << e_2;
	*/
	/*
	// Test C
	std::vector<Voronoi::Point> s_3;
	s_3.emplace_back(0.2, 0.7);
	s_3.emplace_back(0.2, 0.2);
	s_3.emplace_back(0.4, 0.1);
	const Voronoi::Point b_3(0.225, 0);
	const Voronoi::Point e_3(0.45, 0.45);
	const size_t c_3 = 3;
	QTest::newRow("Test C") << s_3 << c_3 << b_3 << e_3;
	*/
	/*
	// Test E
	std::vector<Voronoi::Point> s_5;
	s_5.emplace_back(0.2, 0.7);
	s_5.emplace_back(0.2, 0.2);
	s_5.emplace_back(0.4, 0.1);
	s_5.emplace_back(0.9, 0.7);  // (0.9, 0.701) odstrani bug
	const Voronoi::Point b_5(0.0, 0.45);  // TODO Nejni to spravna hodnota
	const Voronoi::Point e_5(0.45, 0.45);  // TODO Nejni to spravna hodnota
	const size_t c_5 = 5;
	QTest::newRow("Test E") << s_5 << c_5 << b_5 << e_5;
	*/

	// TODO Testovat i zrcadlovy pripad, kdy 
	std::vector<Voronoi::Point> s_5;
	s_5.emplace_back(0.2, 0.7);
	s_5.emplace_back(0.9, 0.2);
	s_5.emplace_back(0.6, 0.1);
	s_5.emplace_back(0.9, 0.7);
	const Voronoi::Point b_5(0.0, 0.45);  // TODO Nejni to spravna hodnota
	const Voronoi::Point e_5(0.45, 0.45);  // TODO Nejni to spravna hodnota
	const size_t c_5 = 5;
	QTest::newRow("Test E") << s_5 << c_5 << b_5 << e_5;
}


void VoronoiTest::circumcenter()
{
	Voronoi::Point a(0.2, 0.7);
	Voronoi::Point b(0.2, 0.2);
	Voronoi::Point c(0.4, 0.1);
	auto p = Voronoi::Circumcenter(a, b, c);
	auto r = Voronoi::Circumcenter(c, b, a);
	auto s = Voronoi::Circumcenter(b, a, c);
	auto t = Voronoi::Circumcenter(b, c, a);
	COMPARE(Voronoi::Point(*p), Voronoi::Point(*r));
	COMPARE(Voronoi::Point(*s), Voronoi::Point(*r));
	COMPARE(Voronoi::Point(*t), Voronoi::Point(*r));
}


void VoronoiTest::toUpper()
{
	QFETCH(std::vector<Voronoi::Point>, sites);
	QFETCH(size_t, edgeCount);
	QFETCH(Voronoi::Point, begin);
	QFETCH(Voronoi::Point, end);

	Voronoi::Generator generator(sites);
	auto edges = generator.getEdges();
	edges.sort();
	auto it = edges.cbegin();

	printEdgesUser(edges);

	QCOMPARE(edges.size(), edgeCount);
	COMPARE(it->begin(), begin);  // vraci spatne [0.3, 0.4]
	COMPARE(it->end(), end);
}

