/*!
 * \file
 * \brief Class factory::Header.
 */
#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{

struct Header
{
	static void print_parameters(std::string grp_key, std::string grp_name, header_list header, int max_n_chars,
	                             std::ostream& stream = std::cout);

	static void print_parameters(const std::vector<Factory*> &factories, const bool full = true,
	                             std::ostream& stream = std::cout);

	static void compute_max_n_chars(const header_list& header, int& max_n_chars);
};

}
}

#endif /* HEADER_HPP */