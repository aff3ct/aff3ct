#include <stdexcept>
#include <algorithm>
#include "Argument_links.hpp"

using namespace aff3ct::tools;


Argument_links
::Argument_links()
{
}

Argument_links
::~Argument_links()
{
}

void Argument_links
::add(const Argument_tag& tag1, const Argument_tag& tag2)
{
	add(std::make_pair(tag1, tag2));
}

void Argument_links
::add(const std::pair<Argument_tag, Argument_tag>& tags)
{
	if (!find(tags))
		this->push_back(tags);
}

bool Argument_links
::find(const Argument_tag& tag1, const Argument_tag& tag2) const
{
	return find(std::make_pair(tag1, tag2));
}

bool Argument_links
::find(const std::pair<Argument_tag, Argument_tag>& tags) const
{
	if (tags.first.size() == 0)
		throw std::invalid_argument("No tags.first has been given ('tags.first.size()' == 0).");

	if (tags.second.size() == 0)
		throw std::invalid_argument("No tags.second has been given ('tags.second.size()' == 0).");

	if (tags.first == tags.second)
		throw std::invalid_argument("tags can't be identical.");

	auto it = std::find(this->begin(), this->end(), tags);

	if (it == this->end())
		it = std::find(this->begin(), this->end(), std::make_pair(tags.second, tags.first));

	return it != this->end();
}

size_t Argument_links
::find(const Argument_tag& tag, const size_t first_pos) const
{
	size_t i = first_pos;

	while (i < this->size())
	{
		auto val = *(this->data() + i);

		if (val.first == tag || val.second == tag)
			break;

		i++;
	}

	return i;
}