/*!
 * \file
 * \brief Class tools::Interface_get_trellis.
 */
#ifndef Interface_get_trellis_HPP__
#define Interface_get_trellis_HPP__

#include <vector>

namespace aff3ct
{
namespace tools
{
class Interface_get_trellis
{
public:
	virtual std::vector<std::vector<int>> get_trellis() = 0;

};
}
}

#endif // Interface_get_trellis_HPP__