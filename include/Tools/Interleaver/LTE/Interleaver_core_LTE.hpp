/*!
 * \file
 * \brief Class tools::Interleaver_core_LTE.
 */
#ifndef INTERLEAVER_CORE_LTE_HPP
#define INTERLEAVER_CORE_LTE_HPP

#include <cstdint>

#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_LTE : public Interleaver_core<T>
{
public:
	Interleaver_core_LTE(const int size);
	virtual ~Interleaver_core_LTE() = default;

	virtual Interleaver_core_LTE<T>* clone() const;

protected:
	void gen_lut(T *lut, const size_t frame_id);

private:
	static inline int pi_LTE(const int &i, const int &f_1, const int &f_2, const int &K);
};
}
}

#endif	/* INTERLEAVER_CORE_LTE_HPP */
