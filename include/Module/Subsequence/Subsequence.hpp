/*!
 * \file
 * \brief Class module::Subsequence.
 */
#ifndef SUBSEQUENCE_HPP_
#define SUBSEQUENCE_HPP_

#include <memory>
#include <cstddef>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace tools
{
class Sequence;
}
namespace module
{
	namespace ssq
	{
		enum class tsk : size_t { exec, SIZE };
	}

class Subsequence : public Module
{
	std::shared_ptr<tools::Sequence> sequence_cloned;
	                tools::Sequence *sequence_extern;

public:
	inline Task& operator[](const ssq::tsk t);

	explicit Subsequence(const tools::Sequence &sequence);
	explicit Subsequence(      tools::Sequence &sequence);
	virtual ~Subsequence() = default;

	virtual void init();

	virtual Subsequence* clone() const;

	tools::Sequence& get_sequence();

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Subsequence& m);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Subsequence/Subsequence.hxx"
#endif

#endif /* SUBSEQUENCE_HPP_ */
