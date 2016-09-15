#include "test_GA.hpp"
#include <iostream>

Test_GA::
Test_GA(const int K, const int N, const float sigma)
: K(K),
  N(N),
  sigma(sigma),
  ft(),
  fb_generator_GA(K,N,sigma),
  frozen_bits_GA(N),
  fb_generator_TV(K,N,"../awgn_polar_codes/TV","../lib/polar_bounds/bin/polar_bounds", sigma),
  frozen_bits_TV(N)
{
	mipp::vector<B> diff(N);

	// generate GA frozen bits
	std::cout << "GA" << std::endl;
	fb_generator_GA.generate(frozen_bits_GA);
	ft.display_bit_vector(frozen_bits_GA);

	// generate TV frozen bits
	std::cout << "TV" << std::endl;
	fb_generator_TV.generate(frozen_bits_TV);
	ft.display_bit_vector(frozen_bits_TV);



	//generate diff

	const auto loop_size = (int)frozen_bits_TV.size();

	std::cout << mipp::nElReg<B>() << std::endl;

	for (auto i = 0; i < loop_size; i +=  mipp::nElReg<B>())
	{	
		mipp::Reg<B> reg_GA(&frozen_bits_GA[i]);
		mipp::Reg<B> reg_TV(&frozen_bits_TV[i]);

		std::cout << "reg_GA" << std::endl;
		std::cout << reg_GA   << std::endl;
		std::cout << "reg_TV" << std::endl;
		std::cout << reg_TV   << std::endl;

		mipp::Reg<B> reg_diff;
		reg_diff = reg_GA - reg_TV;
		
		std::cout << "reg_diff" << std::endl;
		std::cout << reg_TV   << std::endl;		

		reg_diff.store(&diff[i]);

	} 


	std::cout << "diff" << std::endl;	
	ft.display_bit_vector(diff);

	exit(EXIT_SUCCESS);
}
