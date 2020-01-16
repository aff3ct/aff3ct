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
			enum class push_1 : size_t { in,  SIZE };
			enum class pull_n : size_t { out, SIZE };
			enum class push_n : size_t { in,  SIZE };
			enum class pull_1 : size_t { out, SIZE };
		}
	}

class Adaptor : public Module, public tools::Interface_waiting
{
protected:
	const size_t n_elmts;
	const size_t n_bytes;
	const std::type_index datatype;
	const size_t buffer_size;
	size_t id;
	size_t cur_id;

	std::shared_ptr<std::vector<std::vector<std::vector<int8_t>>>> buffer;
	std::shared_ptr<std::vector<std::atomic<size_t>>> first;
	std::shared_ptr<std::vector<std::atomic<size_t>>> last;

	std::shared_ptr<std::atomic<bool>> waiting_canceled;

public:
	inline size_t get_n_elmts() const;
	inline size_t get_n_bytes() const;
	inline std::type_index get_datatype() const;
	void send_cancel_signal();
	void reset();

protected:
	inline Adaptor(const size_t n_elmts,
	               const std::type_index datatype,
	               const size_t buffer_size,
	               const int n_frames = 1);
	virtual ~Adaptor();
	virtual void deep_copy(const Adaptor &m);
	inline bool is_full(const size_t id);
	inline bool is_empty(const size_t id);
	inline size_t n_free_slots(const size_t id);
	inline size_t n_fill_slots(const size_t id);

private:
	static size_t compute_bytes(const size_t n_elmts, const std::type_index type);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Adaptor/Adaptor.hxx"
#endif

#endif /* CONNECTOR_HPP_ */
