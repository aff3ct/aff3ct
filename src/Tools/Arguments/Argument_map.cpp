#include "Argument_map.hpp"

using namespace aff3ct;
using namespace tools;

/***********************************
 * Argument_info class
 ***********************************/
Argument_info
::Argument_info()
{ }

Argument_info
::Argument_info(Argument_type* type, std::string doc)
: type(type), doc(doc)
{}


Argument_info
::~Argument_info()
{
	clear();
}

void Argument_info
::clear()
{
	if (type != nullptr)
		delete type;

	type = nullptr;
}

Argument_info* Argument_info
::clone() const
{
		Argument_type* arg_t = nullptr;

		if (type != nullptr)
			arg_t = type->clone();

		return new Argument_info(arg_t, doc);
}



/***********************************
 * Argument_map_info class
 ***********************************/
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
::add(const Argument_tag& tags, Argument_type* arg_t, const std::string& doc)
{
	if (tags.size() == 0)
		throw std::invalid_argument("No tag has been given ('tag.size()' == 0).");

	if (arg_t == nullptr)
		throw std::invalid_argument("No argument type has been given ('arg_t' == 0).");

	(*this)[tags] = new Argument_info(arg_t, doc);
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

/* \brief: clone itself in the 'other' map
 * \return a pointer to the clone map
 */
Argument_map_info* Argument_map_info
::clone() const
{
	auto* other = new Argument_map_info();

	for (auto it = this->begin(); it != this->end(); it++)
		(*other)[it->first] = it->second->clone();

	return other;
}

/* \brief: clone itself in the 'other' map
 * \param 'other' is the other map in which this class will be cloned. Clear it first of all
 */
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


/***********************************
 * Argument_map_value class
 ***********************************/
bool Argument_map_value
::exist(const Argument_tag &tags) const
{
	return (this->find(tags) != this->end());
}

/*!
 * \brief Returns the value for an argument.
 *
 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}
 *
 * \return the integer value of an argument with its tags (to use after the parse_arguments method).
 */
int Argument_map_value
::to_int(const Argument_tag &tags) const
{
	try
	{
		return std::stoi(this->at(tags));
	}
	catch (std::exception&)
	{
		return 0;
	}
}

/*!
 * \brief Returns the value for an argument.
 *
 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
 *
 * \return the floating-point value of an argument with its tags (to use after the parse_arguments method).
 */
float Argument_map_value
::to_float(const Argument_tag &tags) const
{
	try
	{
		return std::stof(this->at(tags));
	}
	catch (std::exception&)
	{
		return 0.0f;
	}
}


/***********************************
 * Argument_map_group class
 ***********************************/
bool Argument_map_group
::exist(const std::string &prefix) const
{
	return (this->find(prefix) != this->end());
}

void Argument_map_group
::add(const std::string& prefix, const std::string& title)
{
	if (prefix.size() == 0)
		throw std::invalid_argument("No prefix has been given ('prefix.size()' == 0).");

	if (title.size() == 0)
		throw std::invalid_argument("No title has been given ('title.size()' == 0).");

	(*this)[prefix] = title;
}
