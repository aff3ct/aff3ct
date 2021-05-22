/*!
 * \file
 * \brief Class module::Reducer.
 */
#ifndef INCREMENTER_HPP_
#define INCREMENTER_HPP_

#include <cstdint>
#include <vector>

#include "Tools/Math/binaryop.h"
#include "Module/Module.hpp"

namespace aff3ct
{

namespace module
{
	namespace red
	{
		enum class tsk : size_t { reduce, SIZE };

		namespace sck
		{
			enum class reduce : size_t { in, out, status };
		}
	}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
class Reducer : public Module
{
public:
	inline Task&   operator[](const red::tsk         t);
	inline Socket& operator[](const red::sck::reduce s);

protected:
	const size_t n_elmts;

public:
	Reducer(const size_t n_elmts);
	virtual ~Reducer() = default;
	virtual Reducer<TI,TO,BOP>* clone() const;

	size_t get_n_elmts() const;

	template <class AI = std::allocator<TI>, class AO = std::allocator<TO>>
	void reduce(const std::vector<TI,AI>& in,
	                  std::vector<TO,AO>& out,
	            const int frame_id = -1,
	            const bool managed_memory = true);

	void reduce(const TI *in, TO *out, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _reduce(const TI *in, TO *out, const size_t frame_id);
};

template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_add<TI, TO>> using Reducer_add = Reducer<TI, TO, BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_sub<TI, TO>> using Reducer_sub = Reducer<TI, TO, BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_mul<TI, TO>> using Reducer_mul = Reducer<TI, TO, BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_div<TI, TO>> using Reducer_div = Reducer<TI, TO, BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_or <TI, TO>> using Reducer_or  = Reducer<TI, TO, BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_xor<TI, TO>> using Reducer_xor = Reducer<TI, TO, BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_and<TI, TO>> using Reducer_and = Reducer<TI, TO, BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_min<TI, TO>> using Reducer_min = Reducer<TI, TO, BOP>;
template <typename TI, typename TO = TI, tools::proto_bop<TI, TO> BOP = tools::bop_max<TI, TO>> using Reducer_max = Reducer<TI, TO, BOP>;

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Reducer/Reducer.hxx"
#endif

#endif /* INCREMENTER_HPP_ */
