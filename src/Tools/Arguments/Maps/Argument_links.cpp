#include <stdexcept>
#include <utility>
#include <algorithm>
#include "Argument_links.hpp"

using namespace aff3ct::tools;

Argument_link
::Argument_link(const Argument_tag& first, const Argument_tag& second, bool (*callback)(const void*, const void*))
: first(first), second(second), callback(callback)
{
	if (first.size() == 0)
		throw std::invalid_argument("No tag has been given ('first.size()' == 0).");

	if (second.size() == 0)
		throw std::invalid_argument("No second has been given ('second.size()' == 0).");

	if (first == second)
		throw std::invalid_argument("first can't be identical to second.");

}

Argument_link
::Argument_link(Argument_link&& other)
: first(other.first), second(other.second), callback(other.callback)
{

}

bool Argument_link
::operator==(const Argument_link& link) const
{
	return (first == link.first && second == link.second) || (first == link.second && second == link.first);
}

bool Argument_link
::operator==(const Argument_tag& tag) const
{
	return (first == tag) || (second == tag);
}

const Argument_tag& Argument_link
::other_tag(const Argument_tag& tag) const
{
	return (first == tag) ? second : first;
}

bool Argument_link
::is_first_tag(const Argument_tag& tag) const
{
	return first == tag;
}


Argument_links
::Argument_links()
{
}

Argument_links
::~Argument_links()
{
}

void Argument_links
::add(const Argument_tag& tag1, const Argument_tag& tag2, bool (*callback)(const void*, const void*))
{
	Argument_link new_link(tag1, tag2, callback);

	if (!find(new_link))
		this->push_back(std::move(new_link));
}

bool Argument_links
::find(const Argument_tag& tag1, const Argument_tag& tag2) const
{
	Argument_link link(tag1, tag2);

	return find(link);
}

bool Argument_links
::find(const Argument_link& link) const
{
	return std::find(this->begin(), this->end(), link) != this->end();
}

size_t Argument_links
::find(const Argument_tag& tag, const size_t first_pos) const
{
	size_t i = first_pos;

	while (i < this->size())
	{
		const auto& link = *(this->data() + i);

		if (link == tag)
			break;

		i++;
	}

	return i;
}