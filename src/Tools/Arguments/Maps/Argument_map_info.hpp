#ifndef ARGUMENT_MAP_INFO_HPP_
#define ARGUMENT_MAP_INFO_HPP_

#include <string>
#include <vector>
#include <map>

#include "Argument_info.hpp"
#include "Argument_tag.hpp"
#include "Argument_links.hpp"

namespace aff3ct
{
namespace tools
{

class Argument_map_info : public std::map<Argument_tag, Argument_info*>
{
public:
	using mother_t = std::map<Argument_tag, Argument_info*>;

protected:
	Argument_links links;

public:
	Argument_map_info();

	Argument_map_info(const Argument_map_info& other);

	virtual ~Argument_map_info();

	Argument_map_info& operator=(const Argument_map_info& other);

	void add(const Argument_tag& tags, Argument_type* arg_t, const std::string& doc,
	         const arg_rank rank = arg_rank::OPT, const std::string key = "");

	void add_link(const Argument_tag& tag1, const Argument_tag& tag2, bool (*callback)(const void*, const void*) = nullptr);
	bool has_link(const Argument_tag& tag) const;
	const Argument_links& get_links() const;

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

}
}

#endif /* ARGUMENT_MAP_INFO_HPP_ */
