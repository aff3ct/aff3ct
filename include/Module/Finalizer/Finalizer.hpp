/*!
 * \file
 * \brief Class module::Finalizer.
 */
#ifndef FINALIZER_HPP_
#define FINALIZER_HPP_

#include <cstdint>
#include <vector>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace fin
	{
		enum class tsk : size_t { finalize, SIZE };

		namespace sck
		{
			enum class finalize : size_t { in, status };
		}
	}

template <typename T = int>
class Finalizer : public Module
{
public:
	inline Task&   operator[](const fin::tsk           t);
	inline Socket& operator[](const fin::sck::finalize s);

protected:
	std::vector<std::vector<T>> final_data;

public:
	Finalizer(const size_t n_elmts);
	virtual ~Finalizer() = default;
	virtual Finalizer* clone() const;

	const std::vector<std::vector<T>>& get_final_data() const;
	void set_n_frames(const size_t n_frames);

	template <class A = std::allocator<T>>
	void finalize(const std::vector<T,A>& in,
	              const int frame_id = -1,
	              const bool managed_memory = true);

	void finalize(const T *in, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _finalize(const T *in, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Finalizer/Finalizer.hxx"
#endif

#endif /* FINALIZER_HPP_ */
