/*!
 * \file
 * \brief Class tools::Pattern_polar_std.
 */
#ifndef PATTERN_POLAR_STANDARD_HPP_
#define PATTERN_POLAR_STANDARD_HPP_

#include <string>

#include "Tools/Algo/Tree/Binary/Binary_node.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"

namespace aff3ct
{
namespace tools
{
class Pattern_polar_std : public Pattern_polar_i
{
protected:
	inline Pattern_polar_std(const int &N, const Binary_node<Pattern_polar_i>* node);

public:
	inline Pattern_polar_std();

	virtual inline Pattern_polar_std* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const;

	virtual ~Pattern_polar_std() = default;

	virtual inline polar_node_t type()       const;
	virtual inline std::string  name()       const;
	virtual inline std::string  short_name() const;
	virtual inline std::string  fill_color() const;
	virtual inline std::string  font_color() const;

	virtual inline std::string f() const;
	virtual inline std::string g() const;
	virtual inline std::string h() const;

	virtual inline int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const;

	virtual inline bool is_terminal() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hxx"
#endif

#endif /* PATTERN_POLAR_STANDARD_HPP_ */
