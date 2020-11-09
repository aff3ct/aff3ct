/*!
 * \file
 * \brief Class tools::Interleaver_core_ARP_DVB_RCS1.
 */
#ifndef INTERLEAVER_CORE_ARP_DVB1_HPP
#define INTERLEAVER_CORE_ARP_DVB1_HPP

#include <vector>
#include <cstdint>

#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_ARP_DVB_RCS1 : public Interleaver_core<T>
{
private:
	int p0, p1, p2, p3;
	std::vector<std::vector<int>> parameters = {{11, 24,  0, 24},
	                                            { 7, 34, 32,  2},
	                                            {13,106,108,  2},
	                                            {23,112,  4,116},
	                                            {17,116, 72,188},
	                                            {11,  6,  8,  2},
	                                            {13,  0,  4,  8},
	                                            {13, 10,  4,  2},
	                                            {19,  2, 16,  6},
	                                            {19,428,224,652},
	                                            {19,  2, 16,  6},
	                                            {19,376,224,600}};

public:
	Interleaver_core_ARP_DVB_RCS1(const int size);
	virtual ~Interleaver_core_ARP_DVB_RCS1() = default;

	virtual Interleaver_core_ARP_DVB_RCS1<T>* clone() const;

protected:
	void gen_lut(T *lut, const size_t frame_id);
};
}
}

#endif	/* INTERLEAVER_CORE_ARP_DVB1_HPP */
