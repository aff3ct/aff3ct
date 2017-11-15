#ifndef ARGUMENT_MAP_HPP_
#define ARGUMENT_MAP_HPP_

#include <string>
#include <vector>
#include <stdexcept>
#include <map>

#include "Argument_type.hpp"
#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

using Argument_tag = std::vector<std::string>;

struct Argument_info
{
    Argument_info();
    Argument_info(Argument_type* type, std::string doc);

	virtual ~Argument_info();

	virtual void clear();

	virtual Argument_info* clone() const;

	Argument_type* type = nullptr;
	std::string    doc  = "";
};


class Argument_map_info : public std::map<Argument_tag, Argument_info*>
{
public:
	using mother_t = std::map<Argument_tag, Argument_info*>;

public:
	Argument_map_info();

	Argument_map_info(const Argument_map_info& other);

	virtual ~Argument_map_info();

	Argument_map_info& operator=(const Argument_map_info& other);

	void add(const Argument_tag& tags, Argument_type* arg_t, const std::string& doc);

	void erase(const Argument_tag& tags);

	void clear();

	/* \brief: clone itself in the 'other' map
	 * \return a pointer to the clone map
	 */
	Argument_map_info* clone() const;

	/* \brief: clone itself in the 'other' map
	 * \param 'other' is the other map in which this class will be cloned. Clear it first of all
	 */
	void clone(Argument_map_info& other) const;

	bool exist(const Argument_tag &tags);
};


class Argument_map_value : public std::map<Argument_tag, std::string>
{
public:
	using mother_t = std::map<Argument_tag, std::string>;

public:

	bool exist(const Argument_tag &tags) const;

	/*!
	 * \brief Returns the value for an argument.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}
	 *
	 * \return the integer value of an argument with its tags (to use after the parse_arguments method).
	 */
	int to_int(const Argument_tag &tags) const;

	/*!
	 * \brief Returns the value for an argument.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return the floating-point value of an argument with its tags (to use after the parse_arguments method).
	 */
	float to_float(const Argument_tag &tags) const;
};


class Argument_map_group : public std::map<std::string, std::string>
{
public:
	using mother = std::map<std::string, std::string>;

public:

	bool exist(const std::string &prefix) const;

	void add(const std::string& prefix, const std::string& title);
};

}
}

#endif /* ARGUMENT_MAP_HPP_ */
