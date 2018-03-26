#ifndef POINT_HXX_
#define POINT_HXX_

#include "Point.hpp"
#include "Tools/Math/utils.h"

namespace aff3ct
{
namespace tools
{

template <typename T>
Point<T>
::Point(const T& x, const T& y)
{
	x(x);
	y(y);
}

template <typename T>
Point<T>
::Point(const Point<T>& p)
{
	x(p.x());
	y(p.y());
}

template <typename T>
Point<T>
::Point()
{
	x((T)0);
	y((T)0);
}

template <typename T>
Point<T>
::~Point()
{
}

template <typename T>
inline void Point<T>
::x(const T& x)
{
	this->_x = x;
}

template <typename T>
inline void Point<T>
::y(const T& y)
{
	this->_y = y;
}

template <typename T>
inline const T& Point<T>
::x() const
{
	return this->_x;
}

template <typename T>
inline const T& Point<T>
::y() const
{
	return this->_y;
}

template <typename T>
inline bool Point<T>
::operator==(const Point<T>& p) const
{
	return y_cmp_eq(*this, p) && x_cmp_eq(*this, p);
}

template <typename T>
inline bool x_cmp_lt(const Point<T>& p1, const Point<T>& p2)
{
	return p1.x() < p2.x();
}

template <typename T>
inline bool x_cmp_le(const Point<T>& p1, const Point<T>& p2)
{
	return p1.x() <= p2.x();
}

template <typename T>
inline bool x_cmp_gt(const Point<T>& p1, const Point<T>& p2)
{
	return p1.x() > p2.x();
}

template <typename T>
inline bool x_cmp_ge(const Point<T>& p1, const Point<T>& p2)
{
	return p1.x() >= p2.x();
}

template <typename T>
inline bool x_cmp_eq(const Point<T>& p1, const Point<T>& p2)
{
	return comp_equal(p1.x(), p2.x());
}

template <typename T>
inline bool y_cmp_lt(const Point<T>& p1, const Point<T>& p2)
{
	return p1.y() < p2.y();
}

template <typename T>
inline bool y_cmp_le(const Point<T>& p1, const Point<T>& p2)
{
	return p1.y() <= p2.y();
}

template <typename T>
inline bool y_cmp_gt(const Point<T>& p1, const Point<T>& p2)
{
	return p1.y() > p2.y();
}

template <typename T>
inline bool y_cmp_ge(const Point<T>& p1, const Point<T>& p2)
{
	return p1.y() >= p2.y();
}

template <typename T>
inline bool y_cmp_eq(const Point<T>& p1, const Point<T>& p2)
{
	return comp_equal(p1.y(), p2.y());
}



}
}

#endif // POINT_HXX_