#ifndef POINT_HPP_
#define POINT_HPP_

#include <utility>

namespace aff3ct
{
namespace tools
{

template <typename T>
class Point
{
protected:
	T _x, _y;

public:
	Point(const T& x, const T& y);
	Point(const Point<T>& p);
	Point();
	~Point();

	void x(const T& x); // set x
	void y(const T& y); // set y
	const T& x() const; // read x
	const T& y() const; // read y

	bool operator==(const Point<T>& p) const; // true when x and y are equal
};

/*
 * return true when p1.x() < p2.x()
 */
template <typename T>
bool x_cmp_lt(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.x() <= p2.x()
 */
template <typename T>
bool x_cmp_le(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.x() > p2.x()
 */
template <typename T>
bool x_cmp_gt(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.x() >= p2.x()
 */
template <typename T>
bool x_cmp_ge(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.x() == p2.x()
 */
template <typename T>
bool x_cmp_eq(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.y() < p2.y()
 */
template <typename T>
bool y_cmp_lt(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.y() <= p2.y()
 */
template <typename T>
bool y_cmp_le(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.y() > p2.y()
 */
template <typename T>
bool y_cmp_gt(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.y() >= p2.y()
 */
template <typename T>
bool y_cmp_ge(const Point<T>& p1, const Point<T>& p2);

/*
 * return true when p1.y() == p2.y()
 */
template <typename T>
bool y_cmp_eq(const Point<T>& p1, const Point<T>& p2);

}
}

#include "Point.hxx"

#endif // POINT_HPP_