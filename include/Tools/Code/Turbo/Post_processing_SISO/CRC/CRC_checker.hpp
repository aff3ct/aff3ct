/*!
 * \file
 * \brief Class tools::CRC_checker.
 */
#ifndef CRC_CHECKER_HPP
#define CRC_CHECKER_HPP

#include <memory>
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
	const int                       start_crc_check_ite;
	std::shared_ptr<module::CRC<B>> crc;

public:
	CRC_checker(const module::CRC<B> &crc, const int start_crc_check_ite = 2);

	virtual ~CRC_checker() = default;

	virtual CRC_checker<B,R>* clone() const;

	virtual bool siso_n(const int ite,
	                    const mipp::vector<R>& sys,
	                          mipp::vector<R>& ext,
	                          mipp::vector<B>& s);

	virtual void set_n_frames(const size_t n_frames);
protected:
	void deep_copy(const CRC_checker<B,R>& t);
};
}
}

#endif /* CRC_CHECKER_HPP */
