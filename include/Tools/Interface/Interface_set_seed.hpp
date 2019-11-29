/*!
 * \file
 * \brief Class tools::Interface_set_seed.
 */
#ifndef INTERFACE_SET_SEED_HPP__
#define INTERFACE_SET_SEED_HPP__

namespace aff3ct
{
namespace tools
{
class Interface_set_seed
{
public:
	virtual void set_seed(const int seed) = 0;
};

}
}

#endif // INTERFACE_SET_SEED_HPP__