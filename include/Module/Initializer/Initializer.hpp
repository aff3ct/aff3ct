/*!
 * \file
 * \brief Class module::Initializer.
 */
#ifndef INITIALIZER_HPP_
#define INITIALIZER_HPP_

#include <cstdint>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace ini
	{
		enum class tsk : size_t { initialize, SIZE };

		namespace sck
		{
			enum class initialize : size_t { out, status };
		}
	}

template <typename T = int>
class Initializer : public Module
{
public:
	inline Task&   operator[](const ini::tsk             t);
	inline Socket& operator[](const ini::sck::initialize s);

protected:
	std::vector<std::vector<T>> init_data;

public:
	Initializer(const size_t n_elmts);
	virtual ~Initializer() = default;
	virtual Initializer* clone() const;

	const std::vector<std::vector<T>>& get_init_data() const;

	void set_init_data(const std::vector<T>& init_data);
	void set_init_data(const std::vector<std::vector<T>>& init_data);
	void set_n_frames(const size_t n_frames);

	template <class A = std::allocator<T>>
	void initialize(std::vector<T,A>& out,
	                const int frame_id = -1,
	                const bool managed_memory = true);

	void initialize(T *out, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _initialize(T *out, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Initializer/Initializer.hxx"
#endif

#endif /* INITIALIZER_HPP_ */
