/*!
 * \file
 * \brief Class module::Binaryop.
 */
#ifndef BINARYOP_HPP_
#define BINARYOP_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "Tools/Math/binaryop.h"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{

namespace module
{
	namespace bop
	{
		enum class tsk : size_t { perform, SIZE };

		namespace sck
		{
			enum class perform : size_t { in1, in2, out, status };
		}
	}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
class Binaryop : public Module
{
public:
	inline Task&   operator[](const bop::tsk          t);
	inline Socket& operator[](const bop::sck::perform s);

protected:
	const size_t n_elmts;

public:
	Binaryop(const size_t n_elmts);
	virtual ~Binaryop() = default;
	virtual Binaryop<TI,TO,BOP>* clone() const;

	size_t get_n_elmts() const;

	template <class AI = std::allocator<TI>, class AO = std::allocator<TO>>
	void perform(const std::vector<TI,AI>& in1,
	             const std::vector<TI,AI>& in2,
	                   std::vector<TO,AO>& out,
	             const int frame_id = -1,
	             const bool managed_memory = true);

	void perform(const TI *in1, const TI *in2, TO *out, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _perform(const TI *in1, const TI *in2, TO *out, const size_t frame_id);
};

template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_add<TI,TO>> using Binaryop_add = Binaryop<TI,TO,BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_sub<TI,TO>> using Binaryop_sub = Binaryop<TI,TO,BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_mul<TI,TO>> using Binaryop_mul = Binaryop<TI,TO,BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_div<TI,TO>> using Binaryop_div = Binaryop<TI,TO,BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_or <TI,TO>> using Binaryop_or  = Binaryop<TI,TO,BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_xor<TI,TO>> using Binaryop_xor = Binaryop<TI,TO,BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_and<TI,TO>> using Binaryop_and = Binaryop<TI,TO,BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_min<TI,TO>> using Binaryop_min = Binaryop<TI,TO,BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI,TO> BOP = tools::bop_max<TI,TO>> using Binaryop_max = Binaryop<TI,TO,BOP>;

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Binaryop/Binaryop.hxx"
#endif

#endif /* BINARYOP_HPP_ */
