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

namespace aff3ct
{
namespace module
{
	namespace adp
	{
		enum class tsk : size_t { put_1, pull_n, put_n, pull_1, SIZE };

		namespace sck
		{
			enum class put_1  : size_t { in,  SIZE };
			enum class pull_n : size_t { out, SIZE };
			enum class put_n  : size_t { in,  SIZE };
			enum class pull_1 : size_t { out, SIZE };
		}
	}

class Adaptor : public Module
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

public:
	virtual Adaptor* clone() const;
	inline size_t get_n_elmts() const;
	inline size_t get_n_bytes() const;
	inline std::type_index get_datatype() const;

protected:
	inline Adaptor(const size_t n_elmts,
	               const std::type_index datatype,
	               const size_t buffer_size,
	               const int n_frames = 1);
	virtual ~Adaptor();
	virtual void deep_copy(const Adaptor &m);
	inline bool is_full(const size_t id);
	inline bool is_empty(const size_t id);

private:
	static size_t compute_bytes(const size_t n_elmts, const std::type_index type);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Adaptor/Adaptor.hxx"
#endif

#endif /* CONNECTOR_HPP_ */
