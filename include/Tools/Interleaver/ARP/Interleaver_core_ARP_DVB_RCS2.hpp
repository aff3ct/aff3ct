/*!
 * \file
 * \brief Class tools::Interleaver_core_ARP_DVB_RCS2.
 */
#ifndef INTERLEAVER_CORE_ARP_DVB2_HPP
#define INTERLEAVER_CORE_ARP_DVB2_HPP

#include <cstdint>

#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_ARP_DVB_RCS2 : public Interleaver_core<T>
{
private:
	int p, q0, q1, q2, q3;

public:
	Interleaver_core_ARP_DVB_RCS2(const int size);
	virtual ~Interleaver_core_ARP_DVB_RCS2() = default;

	virtual Interleaver_core_ARP_DVB_RCS2<T>* clone() const;

protected:
	void gen_lut(T *lut, const size_t frame_id);
};
}
}

#endif	/* INTERLEAVER_CORE_ARP_DVB2_HPP */
