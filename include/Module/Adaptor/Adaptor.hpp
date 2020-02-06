/*!
 * \file
 * \brief Class module::Adaptor.
 */
#ifndef CONNECTOR_HPP_
#define CONNECTOR_HPP_

#include <typeindex>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <atomic>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"
#include "Tools/Chain/Chain.hpp"
#include "Tools/Interface/Interface_waiting.hpp"

namespace aff3ct
{
namespace module
{
	namespace adp
	{
		enum class tsk : size_t { push_1, pull_n, push_n, pull_1, SIZE };

		namespace sck
		{
			enum class push_1 : size_t { in1,  in2,  in3,  in4,  SIZE };
			enum class pull_n : size_t { out1, out2, out3, out4, SIZE };
			enum class push_n : size_t { in1,  in2,  in3,  in4,  SIZE };
			enum class pull_1 : size_t { out1, out2, out3, out4, SIZE };
		}
	}

class Adaptor : public Module, public tools::Interface_waiting
{

friend tools::Chain; // Chain is friend to enable the hack mode (0 copy)

protected:
	const std::vector<size_t> n_elmts;
	const std::vector<size_t> n_bytes;
	const std::vector<std::type_index> datatype;
	const size_t buffer_size;
	const size_t n_sockets;
	size_t id;
	size_t cur_id;

	std::shared_ptr<std::vector<std::vector<std::vector<int8_t*>>>> buffer;
	std::vector<int8_t*> buffer_to_free;

	std::shared_ptr<std::vector<std::atomic<size_t>>> first;
	std::shared_ptr<std::vector<std::atomic<size_t>>> last;

	std::shared_ptr<std::atomic<bool>> waiting_canceled;

	bool no_copy_pull;
	bool no_copy_push;

public:
	inline size_t get_n_elmts(const size_t sid = 0) const;
	inline size_t get_n_bytes(const size_t sid = 0) const;
	inline std::type_index get_datatype(const size_t sid = 0) const;
	void send_cancel_signal();
	void reset();

protected:
	inline Adaptor(const size_t n_elmts,
	               const std::type_index datatype,
	               const size_t buffer_size,
	               const int n_frames = 1);
	inline Adaptor(const std::vector<size_t> &n_elmts,
	               const std::vector<std::type_index> &datatype,
	               const size_t buffer_size,
	               const int n_frames = 1);
	virtual ~Adaptor();
	virtual void deep_copy(const Adaptor &m);
	inline bool is_full(const size_t id);
	inline bool is_empty(const size_t id);
	inline size_t n_free_slots(const size_t id);
	inline size_t n_fill_slots(const size_t id);

	void set_no_copy_push(const bool hack_mode);
	void set_no_copy_pull(const bool hack_mode);
	bool is_no_copy_push();
	bool is_no_copy_pull();
	virtual void* get_empty_buffer(const size_t sid) = 0;
	virtual void* get_filled_buffer(const size_t sid) = 0;
	virtual void* get_empty_buffer(const size_t sid, void* swap_buffer) = 0;
	virtual void* get_filled_buffer(const size_t sid, void* swap_buffer) = 0;
	virtual void wake_up_pusher() = 0;
	virtual void wake_up_puller() = 0;

private:
	static             size_t  compute_bytes(const             size_t   n_elmts, const             std::type_index   type);
	static std::vector<size_t> compute_bytes(const std::vector<size_t> &n_elmts, const std::vector<std::type_index> &type);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Adaptor/Adaptor.hxx"
#endif

#endif /* CONNECTOR_HPP_ */
