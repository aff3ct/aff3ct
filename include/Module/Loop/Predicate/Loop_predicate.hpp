/*!
 * \file
 * \brief Class module::Loop_predicate.
 */
#ifndef LOOP_PREDICATE_HPP_
#define LOOP_PREDICATE_HPP_

#include <memory>
#include <cstdint>

#include "Tools/Algo/Predicate.hpp"
#include "Module/Loop/Loop.hpp"

namespace aff3ct
{
namespace module
{
template <typename O = float>
class Loop_predicate : public Loop
{
	std::shared_ptr<tools::Predicate> predicate;
	const bool use_hack;
	std::vector<int8_t> hack;

public:
	Loop_predicate(const tools::Predicate &predicate, const size_t n_elmts_out, const int n_frames = 1);
	virtual ~Loop_predicate() = default;
	virtual Loop_predicate<O>* clone() const;

	tools::Predicate& get_predicate();

	virtual void reset();

	virtual bool stop(const int8_t *in, const int frame_id = -1);

protected:
	Loop_predicate(const tools::Predicate &predicate,
	               const size_t n_elmts_in,
	               const std::type_index datatype_in,
	               const size_t n_elmts_out,
	               const int n_frames = 1);

	virtual void deep_copy(const Loop_predicate<O> &m);
};
}
}

#endif /* LOOP_PREDICATE_HPP_ */
