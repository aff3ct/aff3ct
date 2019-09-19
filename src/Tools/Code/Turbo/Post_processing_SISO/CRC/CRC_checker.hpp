#ifndef CRC_CHECKER_HPP
#define CRC_CHECKER_HPP

#include <mipp.h>

#include "Module/CRC/CRC.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class CRC_checker : public Post_processing_SISO<B,R>
{
protected:
	const int             start_crc_check_ite;
	const int             simd_inter_frame_level;
	      module::CRC<B> &crc;

public:
	CRC_checker(module::CRC<B> &crc, const int start_crc_check_ite = 2, const int simd_inter_frame_level = 1);

	virtual ~CRC_checker() = default;

	virtual bool siso_n(const int ite,
	                    const mipp::vector<R>& sys,
	                          mipp::vector<R>& ext,
	                          mipp::vector<B>& s);
};
}
}

#endif /* CRC_CHECKER_HPP */
