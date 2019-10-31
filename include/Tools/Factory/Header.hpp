/*!
 * \file
 * \brief Class tools::Header.
 */
#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <string>
#include <vector>

namespace aff3ct
{
namespace factory
{
class Factory;
}
}

namespace aff3ct
{
namespace tools
{
using header_list = std::vector<std::pair<std::string,std::string>>;

struct Header
{
	static void print_parameters(std::string grp_key, std::string grp_name, header_list header, int max_n_chars,
	                             std::ostream& stream = std::cout);

	static void print_parameters(const std::vector<factory::Factory*> &factories, const bool full = true,
	                             std::ostream& stream = std::cout);

	static void compute_max_n_chars(const header_list& header, int& max_n_chars);
};

}
}

#endif /* HEADER_HPP */