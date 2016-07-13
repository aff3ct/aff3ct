/*
 * Mod_functions.cpp
 *
 *  Created on: 13 juin 2016
 *      Author: rtajan
 */
#include<cmath>

#include "../mod_functions/Mod_Functions.hpp"

#define the_max(a,b) \
({a > b ? a : b; })


template <typename R>
inline R fast_max(const R x,const R y)
{
	return the_max(x,y);
}



template <typename R>
inline R max_star(const R x,const R y)
{
    R d = std::abs(x-y);

	if (d >= 37 || std::isnan(d))
	{
		d = 0.0;//exp(-d);
	}
	else if (d < 37 && d >=9)
	{
		d = std::exp(-d);
	}
	else
	{
		d = std::log1p(std::exp(-d));
	}

	return the_max(x,y) + d;
}


template <typename R>
inline R max_star_fast(const R& lambda_a, const R& lambda_b)
{
	// the two next statements are equivalent !
	// return std::max(lambda_a, lambda_b) + std::log((R)1 + std::exp(-std::abs(lambda_a - lambda_b)));
	return std::max(lambda_a, lambda_b) + std::log1p(std::exp(-std::abs(lambda_a - lambda_b)));
}
