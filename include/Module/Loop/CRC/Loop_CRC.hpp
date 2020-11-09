/*!
 * \file
 * \brief Class module::Loop_CRC.
 */
#ifndef LOOP_CRC_HPP_
#define LOOP_CRC_HPP_

#include <memory>
#include <cstdint>

#include "Module/CRC/CRC.hpp"
#include "Module/Loop/Predicate/Loop_predicate.hpp"

namespace aff3ct
{
namespace module
{
template <typename I = int, typename O = int>
class Loop_CRC : public Loop_predicate<O>
{
	std::shared_ptr<CRC<I>> crc;
	const size_t crc_ite_start;

public:
	Loop_CRC(const tools::Predicate &predicate,
	         const CRC<I> &crc,
	         const size_t n_elmts_in,
	         const size_t n_elmts_out,
	         const size_t crc_ite_start = 0);
	virtual ~Loop_CRC() = default;
	virtual Loop_CRC<I,O>* clone() const;

protected:
	virtual void deep_copy(const Loop_CRC<I,O> &m);

	virtual bool _stop(const int8_t *in, const size_t frame_id);

	virtual bool __stop(const int8_t *in, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Loop/CRC/Loop_CRC.hxx"
#endif

#endif /* LOOP_CRC_HPP_ */
