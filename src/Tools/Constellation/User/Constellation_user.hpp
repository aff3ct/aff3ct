#ifndef CONSTELLATION_USER_HPP__
#define CONSTELLATION_USER_HPP__

#include <vector>

#include "../Constellation.hpp"


namespace aff3ct
{
namespace tools
{
template <typename R>
class Constellation_user : public Constellation<R>
{
public:
	using typename Constellation<R>::S;

	/*
	 * \param n_bps is the number of bits per symbol
	 */
	explicit Constellation_user(const std::string& const_path);

private:
	static std::vector<S> read_constellation(const std::string& const_path);
};

}
}

#include "Constellation_user.hxx"

#endif // CONSTELLATION_USER_HPP__