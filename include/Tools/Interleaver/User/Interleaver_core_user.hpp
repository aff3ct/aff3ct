/*!
 * \file
 * \brief Class tools::Interleaver_core_user.
 */
#ifndef INTERLEAVER_CORE_USER_HPP
#define INTERLEAVER_CORE_USER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_user : public Interleaver_core<T>
{
private:
	std::vector<std::vector<T>> pi_buffer;
	int cur_itl_id;

public:
	Interleaver_core_user(const int size, const std::string &filename);
	virtual ~Interleaver_core_user() = default;

	virtual Interleaver_core_user<T>* clone() const;

	virtual void reinitialize();

protected:
	void gen_lut(T *lut, const size_t frame_id);
};
}
}

#endif	/* INTERLEAVER_CORE_USER_HPP */

