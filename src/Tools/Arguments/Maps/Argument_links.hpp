#ifndef ARGUMENT_LINKS_HPP_
#define ARGUMENT_LINKS_HPP_

#include <string>
#include <vector>

#include "Argument_tag.hpp"

namespace aff3ct
{
namespace tools
{

struct Argument_link
{
	Argument_tag first;
	Argument_tag second;
	bool (*callback)(const void*, const void*);

	Argument_link(const Argument_tag& first, const Argument_tag& second, bool (*callback)(const void*, const void*) = nullptr);
	Argument_link(Argument_link&& other);

	bool operator==(const Argument_link& link) const;
	bool operator==(const Argument_tag& tag) const;

	const Argument_tag& other_tag(const Argument_tag& tag) const;
	bool is_first_tag(const Argument_tag& tag) const;
};


class Argument_links : public std::vector<Argument_link>
{
public:
	using mother_t = std::vector<value_type>;

public:
	Argument_links();

	virtual ~Argument_links();

	void add(const Argument_tag& tag1, const Argument_tag& tag2, bool (*callback)(const void*, const void*) = nullptr);

	bool find(const Argument_tag& tag1, const Argument_tag& tag2) const;

	bool find(const Argument_link& link) const;

	size_t find(const Argument_tag& tag, const size_t first_pos = 0) const;
};

}
}

#endif /* ARGUMENT_LINKS_HPP_ */
