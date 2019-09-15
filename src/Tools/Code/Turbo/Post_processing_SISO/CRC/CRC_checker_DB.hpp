#ifndef CRC_CHECKER_DB_HPP
#define CRC_CHECKER_DB_HPP

#include <vector>
#include <mipp.h>

#include "Module/CRC/CRC.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class CRC_checker_DB : public CRC_checker<B,R>
{
protected:
	std::vector<R> apost;

public:
	CRC_checker_DB(module::CRC<B> &crc, const int start_crc_check_ite = 2, const int simd_inter_frame_level = 1);

	virtual ~CRC_checker_DB() = default;

	virtual bool siso_n(const int ite,
	                    const mipp::vector<R>& sys,
	                          mipp::vector<R>& ext,
	                          mipp::vector<B>& s);
};
}
}

#endif /* CRC_CHECKER_DB_HPP */
