#ifndef INTERLEAVER_HPP_
#define INTERLEAVER_HPP_

#include <string>
#include <vector>

#include "Tools/Interleaver/Interleaver_core.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace itl
	{
		namespace tsk
		{
			enum list { interleave, deinterleave, SIZE };
		}

		namespace sck
		{
			namespace interleave   { enum list { nat, itl, SIZE }; }
			namespace deinterleave { enum list { itl, nat, SIZE }; }
		}
	}

template <typename D = int32_t, typename T = uint32_t>
class Interleaver : public Module
{
protected:
	const tools::Interleaver_core<T> &core;

public:
	explicit Interleaver(const tools::Interleaver_core<T> &core);

	virtual ~Interleaver();

	const tools::Interleaver_core<T>& get_core() const;

	template <class A = std::allocator<D>>
	inline void interleave(const std::vector<D,A> &nat, std::vector<D,A> &itl, const int frame_id = -1) const;

	inline void interleave(const D *nat, D *itl, const int frame_id = -1) const;

	inline void interleave(const D *nat, D *itl, const int frame_id, const int n_frames,
	                       const bool frame_reordering = false) const;

	template <class A = std::allocator<D>>
	inline void deinterleave(const std::vector<D,A> &itl, std::vector<D,A> &nat, const int frame_id = -1) const;

	inline void deinterleave(const D *itl, D *nat, const int frame_id = -1) const;

	inline void deinterleave(const D *itl, D *nat, const int frame_id, const int n_frames,
	                         const bool frame_reordering = false) const;

private:
	inline void _interleave(const D *in_vec, D *out_vec,
	                        const std::vector<T> &lookup_table,
	                        const bool frame_reordering,
	                        const int  n_frames,
	                        const int  frame_id) const;
};
}
}
#include "Interleaver.hxx"

#endif /* INTERLEAVER_HPP_ */
