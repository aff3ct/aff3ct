/*!
 * \file
 * \brief Class module::Unaryop.
 */
#ifndef UNARYOP_HPP_
#define UNARYOP_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "Tools/Math/unaryop.h"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{

namespace module
{
	namespace uop
	{
		enum class tsk : size_t { perform, SIZE };

		namespace sck
		{
			enum class perform : size_t { in, out, status };
		}
	}

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
class Unaryop : public Module
{
public:
	inline Task&   operator[](const uop::tsk          t);
	inline Socket& operator[](const uop::sck::perform s);

protected:
	const size_t n_elmts;

public:
	Unaryop(const size_t n_elmts);
	virtual ~Unaryop() = default;
	virtual Unaryop<TI,TO,UOP>* clone() const;

	size_t get_n_elmts() const;

	template <class AI = std::allocator<TI>, class AO = std::allocator<TO>>
	void perform(const std::vector<TI,AI>& in,
	                   std::vector<TO,AO>& out,
	             const int frame_id = -1,
	             const bool managed_memory = true);

	void perform(const TI *in, TO *out, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _perform(const TI *in, TO *out, const size_t frame_id);
};

template <typename TI, typename TO = TI, tools::proto_uop<TI,TO> UOP = tools::uop_abs    <TI,TO>> using Unaryop_abs     = Unaryop<TI,TO,UOP>;
template <typename TI, typename TO = TI, tools::proto_uop<TI,TO> UOP = tools::uop_not    <TI,TO>> using Unaryop_not     = Unaryop<TI,TO,UOP>;
template <typename TI, typename TO = TI, tools::proto_uop<TI,TO> UOP = tools::uop_not_abs<TI,TO>> using Unaryop_not_abs = Unaryop<TI,TO,UOP>;
template <typename TI, typename TO = TI, tools::proto_uop<TI,TO> UOP = tools::uop_sign   <TI,TO>> using Unaryop_sign    = Unaryop<TI,TO,UOP>;

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Unaryop/Unaryop.hxx"
#endif

#endif /* UNARYOP_HPP_ */
