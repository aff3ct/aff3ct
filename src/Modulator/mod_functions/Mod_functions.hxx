/*
 * Mod_functions.cpp
 *
 *  Created on: 13 juin 2016
 *      Author: rtajan
 */
#include<cmath>

#include "../mod_functions/Mod_Functions.hpp"

template <typename R>
inline R max_star(const R x,const R y)
{
    R d = fabs(x-y);

    if (isinf(x) && isinf(y)){
        if (x<0 && y<0)
            return -INFINITY;
        else
            return INFINITY;
    }else{
        if (d >= 37)
        {
            d = 0;//exp(-d);
        }
        else if (d < 37 && d >=9)
        {
        	d = exp(-d);
        }
        else
        {
            d = log(1+exp(-d));
        }

        return std::max(x,y) + d;
    }
}


