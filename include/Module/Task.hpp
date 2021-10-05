/*!
 * \file
 * \brief Class module::Task.
 */
#ifndef TASK_HPP_
#define TASK_HPP_

#include <unordered_map>
#include <functional>
#include <cstddef>
#include <limits>
#include <string>
#include <memory>
#include <chrono>
#include <vector>
#include <mipp.h>

#include "Tools/Interface/Interface_clone.hpp"
#include "Tools/Interface/Interface_reset.hpp"

namespace aff3ct
{
namespace module
{
class Module;
class Socket;

enum status_t : int { SUCCESS = 0,
                      FAILURE = 1,
                      FAILURE_STOP = -1,
                      UNKNOWN = -2 };

enum class socket_t : uint8_t { SIN, SOUT };

static std::unordered_map<int,std::string> status_t_to_string = {{                              0, "SUCCESS"     },
                                                                 {                              1, "FAILURE"     },
                                                                 {                             -1, "FAILURE_STOP"},
                                                                 {                             -2, "UNKNOWN"     },
                                                                 {std::numeric_limits<int>::min(), "SKIPPED"     }};

class Task : public tools::Interface_clone, public tools::Interface_reset
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend Socket;
	friend Module;
#endif

protected:
	Module *module;
	const std::string name;
	bool autoalloc;
	bool stats;
	bool fast;
	bool debug;
	bool debug_hex;
	bool no_input_socket;
	int32_t debug_limit;
	uint8_t debug_precision;
	int32_t debug_frame_max;
	std::function<int(Module &m, Task& t, const size_t frame_id)> codelet;
	size_t n_input_sockets;
	size_t n_output_sockets;

	std::vector<int> status;
	std::vector<mipp::vector<uint8_t>> out_buffers;

	// stats
	uint32_t                 n_calls;
	std::chrono::nanoseconds duration_total;
	std::chrono::nanoseconds duration_min;
	std::chrono::nanoseconds duration_max;

	std::vector<std::string             > timers_name;
	std::vector<uint32_t                > timers_n_calls;
	std::vector<std::chrono::nanoseconds> timers_total;
	std::vector<std::chrono::nanoseconds> timers_min;
	std::vector<std::chrono::nanoseconds> timers_max;

	Socket* last_input_socket;
	std::vector<socket_t> socket_type;

	// precomputed values to speedup the task execution
	std::vector            <int8_t*>  sockets_dataptr_init;
	std::vector            <size_t >  sockets_databytes_per_frame;
	std::vector<std::vector<int8_t >> sockets_data;

public:
	std::shared_ptr<Socket> fake_input_socket;
	std::vector<std::shared_ptr<Socket>> sockets;

	Task(      Module &module,
	     const std::string &name,
	     const bool autoalloc = true,
	     const bool stats     = false,
	     const bool fast      = false,
	     const bool debug     = false);

	virtual ~Task() = default;

	void reset();

	void set_autoalloc      (const bool     autoalloc);
	void set_stats          (const bool     stats    );
	void set_fast           (const bool     fast     );
	void set_debug          (const bool     debug    );
	void set_debug_hex      (const bool     debug_hex);
	void set_debug_limit    (const uint32_t limit    );
	void set_debug_precision(const uint8_t  prec     );
	void set_debug_frame_max(const uint32_t limit    );

	inline bool is_autoalloc        (                  ) const;
	inline bool is_stats            (                  ) const;
	inline bool is_fast             (                  ) const;
	inline bool is_debug            (                  ) const;
	inline bool is_debug_hex        (                  ) const;
	inline bool is_last_input_socket(const Socket &s_in) const;
	       bool can_exec            (                  ) const;

	inline Module&     get_module     (               ) const;
	inline std::string get_name       (               ) const;
	inline uint32_t    get_n_calls    (               ) const;
	       socket_t    get_socket_type(const Socket &s) const;

	// get stats
	std::chrono::nanoseconds                     get_duration_total() const;
	std::chrono::nanoseconds                     get_duration_avg  () const;
	std::chrono::nanoseconds                     get_duration_min  () const;
	std::chrono::nanoseconds                     get_duration_max  () const;
	const std::vector<std::string>             & get_timers_name   () const;
	const std::vector<uint32_t>                & get_timers_n_calls() const;
	const std::vector<std::chrono::nanoseconds>& get_timers_total  () const;
	const std::vector<std::chrono::nanoseconds>& get_timers_min    () const;
	const std::vector<std::chrono::nanoseconds>& get_timers_max    () const;

	size_t get_n_input_sockets() const;
	size_t get_n_output_sockets() const;
	size_t get_n_static_input_sockets() const;

	const std::vector<int>& exec(const int frame_id = -1, const bool managed_memory = true);

	inline Socket& operator[](const size_t id);

	inline void update_timer(const size_t id, const std::chrono::nanoseconds &duration);

	Task* clone() const;

	inline const std::vector<int>& get_status() const;

	void   bind     (Socket &s_out, const int priority = -1);
	void   operator=(Socket &s_out                         );
	size_t unbind   (Socket &s_out                         );

	bool is_no_input_socket() const;

protected:
	void set_no_input_socket(const bool no_input_socket);

	void _exec(const int frame_id = -1, const bool managed_memory = true);

	void register_timer(const std::string &key);

	template <typename T>
	size_t create_socket_in(const std::string &name, const size_t n_elmts);
	size_t create_socket_in(const std::string &name, const size_t n_elmts, const std::type_index& datatype);

	template <typename T>
	size_t create_socket_out(const std::string &name, const size_t n_elmts, const bool hack_status = false);
	size_t create_socket_out(const std::string &name, const size_t n_elmts, const std::type_index& datatype,
	                         const bool hack_status = false);

	void create_codelet(std::function<int(Module &m, Task& t, const size_t frame_id)> &codelet);

	void update_n_frames(const size_t old_n_frames, const size_t new_n_frames);

	void update_n_frames_per_wave(const size_t old_n_frames_per_wave, const size_t new_n_frames_per_wave);

private:
	template <typename T>
	inline Socket& create_socket(const std::string &name, const size_t n_elmts, const socket_t type, const bool hack_status = false);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Task.hxx"
#endif

#endif /* TASK_HPP_ */
