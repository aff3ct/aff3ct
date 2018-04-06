#ifndef ARGUMENT_LINKS_HPP_
#define ARGUMENT_LINKS_HPP_

#include <string>
#include <vector>

#include "Argument_tag.hpp"

namespace aff3ct
{
namespace tools
{

class Argument_links : public std::vector<std::pair<Argument_tag, Argument_tag>>
{
public:
	using mother_t = std::vector<value_type>;

public:
	Argument_links();

	virtual ~Argument_links();

	void add(const Argument_tag& tag1, const Argument_tag& tag2);
	void add(const std::pair<Argument_tag, Argument_tag>& tags);

	bool find(const Argument_tag& tag1, const Argument_tag& tag2) const;
	bool find(const std::pair<Argument_tag, Argument_tag>& tags) const;

	size_t find(const Argument_tag& tag, const size_t first_pos = 0) const;
};

}
}

#endif /* ARGUMENT_LINKS_HPP_ */
