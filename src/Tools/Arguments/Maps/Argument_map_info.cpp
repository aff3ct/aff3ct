#include <stdexcept>

#include "Argument_map_info.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Argument_map_info
::Argument_map_info()
{ }

Argument_map_info
::Argument_map_info(const Argument_map_info& other)
{
	other.clone(*this);
}

Argument_map_info
::~Argument_map_info()
{
	clear();
}

Argument_map_info& Argument_map_info
::operator=(const Argument_map_info& other)
{
	other.clone(*this);
	return *this;
}

void Argument_map_info
::add(const Argument_tag& tags, Argument_type* arg_t, const std::string& doc, const arg_rank rank,
	  const std::string key)
{
	if (tags.size() == 0)
		throw std::invalid_argument("No tag has been given ('tag.size()' == 0).");

	if (arg_t == nullptr)
		throw std::invalid_argument("No argument type has been given ('arg_t' == 0).");

	if (exist(tags))
		erase(tags);

	(*this)[tags] = new Argument_info(arg_t, doc, rank, key);
}

void Argument_map_info
::add_link(const Argument_tag& tag1, const Argument_tag& tag2, bool (*callback)(const void*, const void*))
{
	links.add(tag1, tag2, callback);
}

bool Argument_map_info
::has_link(const Argument_tag& tag) const
{
	return links.find(tag) != links.size();
}

const Argument_links& Argument_map_info
::get_links() const
{
	return links;
}

void Argument_map_info
::erase(const Argument_tag& tags)
{
	auto it = this->find(tags);
	if (it != this->end())
	{
		if (it->second != nullptr)
			delete it->second;

		mother_t::erase(it);
	}
}

void Argument_map_info
::clear()
{
	for (auto it = this->begin(); it != this->end(); it++)
		if (it->second != nullptr)
			delete it->second;

	mother_t::clear();
}

Argument_map_info* Argument_map_info
::clone() const
{
	auto* other = new Argument_map_info();

	for (auto it = this->begin(); it != this->end(); it++)
		(*other)[it->first] = it->second->clone();

	return other;
}

void Argument_map_info
::clone(Argument_map_info& other) const
{
	other.clear();

	for (auto it = this->begin(); it != this->end(); it++)
		other[it->first] = it->second->clone();
}

bool Argument_map_info
::exist(const Argument_tag &tags)
{
	return (this->find(tags) != this->end());
}
