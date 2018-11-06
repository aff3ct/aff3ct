#ifndef ARGUMENT_MAP_GROUP_HPP_
#define ARGUMENT_MAP_GROUP_HPP_

#include <string>
#include <map>

namespace aff3ct
{
namespace tools
{

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

#endif /* ARGUMENT_MAP_GROUP_HPP_ */
