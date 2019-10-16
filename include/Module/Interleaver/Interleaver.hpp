#ifndef INTERLEAVER_HPP_
#define INTERLEAVER_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace itl
	{
		enum class tsk : uint8_t { interleave, deinterleave, SIZE };

		namespace sck
		{
			enum class interleave   : uint8_t { nat, itl, SIZE };
			enum class deinterleave : uint8_t { itl, nat, SIZE };
		}
	}

template <typename D = int32_t, typename T = uint32_t>
class Interleaver : public Module
{
public:
	inline Task&   operator[](const itl::tsk               t) { return Module::operator[]((int)t);                              }
	inline Socket& operator[](const itl::sck::interleave   s) { return Module::operator[]((int)itl::tsk::interleave  )[(int)s]; }
	inline Socket& operator[](const itl::sck::deinterleave s) { return Module::operator[]((int)itl::tsk::deinterleave)[(int)s]; }

protected:
	const tools::Interleaver_core<T> &core;

public:
	explicit Interleaver(const tools::Interleaver_core<T> &core);

	virtual ~Interleaver() = default;

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
#include "Module/Interleaver/Interleaver.hxx"

#endif /* INTERLEAVER_HPP_ */
