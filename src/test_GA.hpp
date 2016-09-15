#ifndef TEST_GA_HPP_
#define TEST_GA_HPP_

#include "Tools/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"
#include "Tools/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/MIPP/mipp.h"
#include "Tools/types.h"
#include "Tools/Frame_trace/Frame_trace.hpp"

class Test_GA 
{
private:
	const int   K;
	const int   N;
	const float sigma;

	Frame_trace<B>             ft;
	Frozenbits_generator_GA<B> fb_generator_GA;
	mipp::vector<B>            frozen_bits_GA;
	Frozenbits_generator_TV<B> fb_generator_TV;
	mipp::vector<B>            frozen_bits_TV;

public:
	Test_GA(const int K, const int N, const float sigma);

	void test_ga();
};

#endif /* TEST_GA_HPP_ */