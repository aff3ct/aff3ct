/*!
 * \file
 * \brief Class module::Interleaver.
 */
#ifndef INTERLEAVER_HPP_
#define INTERLEAVER_HPP_

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace itl
	{
		enum class tsk : size_t { interleave, interleave_reordering, deinterleave, deinterleave_reordering, SIZE };

		namespace sck
		{
			enum class interleave              : size_t { nat, itl, status };
			enum class interleave_reordering   : size_t { nat, itl, status };
			enum class deinterleave            : size_t { itl, nat, status };
			enum class deinterleave_reordering : size_t { itl, nat, status };
		}
	}

template <typename D = int32_t, typename T = uint32_t>
class Interleaver : public Module
{
public:
	inline Task&   operator[](const itl::tsk                          t);
	inline Socket& operator[](const itl::sck::interleave              s);
	inline Socket& operator[](const itl::sck::interleave_reordering   s);
	inline Socket& operator[](const itl::sck::deinterleave            s);
	inline Socket& operator[](const itl::sck::deinterleave_reordering s);

protected:
	const tools::Interleaver_core<T> &core;

public:
	explicit Interleaver(const tools::Interleaver_core<T> &core);

	virtual ~Interleaver() = default;

	virtual Interleaver<D,T>* clone() const;

	const tools::Interleaver_core<T>& get_core() const;

	template <class A = std::allocator<D>>
	inline void interleave(const std::vector<D,A> &nat, std::vector<D,A> &itl, const int frame_id = -1,
	                       const bool managed_memory = true);

	inline void interleave(const D *nat, D *itl, const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<D>>
	inline void interleave_reordering(const std::vector<D,A> &nat, std::vector<D,A> &itl, const int frame_id = -1,
	                                  const bool managed_memory = true);

	inline void interleave_reordering(const D *nat, D *itl, const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<D>>
	inline void deinterleave(const std::vector<D,A> &itl, std::vector<D,A> &nat, const int frame_id = -1,
	                         const bool managed_memory = true);

	inline void deinterleave(const D *itl, D *nat, const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<D>>
	inline void deinterleave_reordering(const std::vector<D,A> &itl, std::vector<D,A> &nat, const int frame_id = -1,
	                                    const bool managed_memory = true);

	inline void deinterleave_reordering(const D *itl, D *nat, const int frame_id = -1, const bool managed_memory = true);

	void set_n_frames_per_wave(const size_t n_frames_per_wave);

private:
	inline void _interleave(const D *in_vec, D *out_vec,
	                        const std::vector<T> &lookup_table,
	                        const size_t frame_id) const;

	inline void _interleave_reordering(const D *in_vec, D *out_vec,
	                                   const std::vector<T> &lookup_table,
	                                   const size_t frame_id) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Interleaver/Interleaver.hxx"
#endif

#endif /* INTERLEAVER_HPP_ */
