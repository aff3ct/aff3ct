/*!
 * \file
 * \brief Class module::Adaptor_n_to_1.
 */
#ifndef CONNECTOR_N_TO_1_HPP_
#define CONNECTOR_N_TO_1_HPP_

#include <typeindex>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Adaptor/Adaptor.hpp"

namespace aff3ct
{
namespace module
{

class Adaptor_n_to_1 : public Adaptor
{
public:
	inline Task&   operator[](const adp::tsk         t);
	inline Socket& operator[](const adp::sck::put_n  s);
	inline Socket& operator[](const adp::sck::pull_1 s);

	const bool active_waiting;

	std::shared_ptr<std::vector<std::condition_variable>> cnd_put;
	std::shared_ptr<std::vector<std::mutex             >> mtx_put;
	std::shared_ptr<            std::condition_variable > cnd_pull;
	std::shared_ptr<            std::mutex              > mtx_pull;

public:
	inline Adaptor_n_to_1(const size_t n_elmts,
	                      const std::type_index datatype,
	                      const size_t buffer_size = 1,
	                      const bool active_waiting = true,
	                      const int n_frames = 1);
	virtual ~Adaptor_n_to_1() = default;
	virtual Adaptor_n_to_1* clone() const;

	template <class A = std::allocator<int8_t>>
	void put_n(const std::vector<int8_t,A>& in, const int frame_id = -1);
	template <class A = std::allocator<int8_t>>
	void pull_1(std::vector<int8_t,A>& out, const int frame_id = -1);

	virtual void put_n (const int8_t *in,  const int frame_id = -1);
	virtual void pull_1(      int8_t *out, const int frame_id = -1);

	void wake_up();
	void cancel_waiting();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Adaptor/Adaptor_n_to_1.hxx"
#endif

#endif /* CONNECTOR_N_TO_1_HPP_ */
