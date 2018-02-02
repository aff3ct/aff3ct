#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <string>
#include <sstream>

#include <map>
#include <chrono>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <typeindex>
#include <functional>
#include <unordered_map>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace module
{
class Module;
class Socket;

enum Socket_type
{
	IN     = 0,
	IN_OUT = 1,
	OUT    = 2
};

class Task
{
	friend Socket;
	friend Module;

protected:
	const Module &module;
	const std::string name;
	bool autoalloc;
	bool autoexec;
	bool stats;
	bool fast;
	bool debug;
	bool debug_hex;
	int32_t debug_limit;
	uint8_t debug_precision;
	std::function<int(void)> codelet;
	std::vector<std::vector<uint8_t>> out_buffers;

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
	std::vector<Socket_type> socket_type;

public:
	std::vector<Socket*> sockets;

	Task(const Module &module,
	     const std::string &name,
	     const bool autoalloc = false,
	     const bool autoexec  = false,
	     const bool stats     = false,
	     const bool fast      = false,
	     const bool debug     = false);

	virtual ~Task();

	void reset_stats();

	void set_autoalloc      (const bool     autoalloc);
	void set_autoexec       (const bool     autoexec );
	void set_stats          (const bool     stats    );
	void set_fast           (const bool     fast     );
	void set_debug          (const bool     debug    );
	void set_debug_hex      (const bool     debug_hex);
	void set_debug_limit    (const uint32_t limit    );
	void set_debug_precision(const uint8_t  prec     );

	inline bool is_autoalloc        (                  ) const { return this->autoalloc;            }
	inline bool is_autoexec         (                  ) const { return this->autoexec;             }
	inline bool is_stats            (                  ) const { return this->stats;                }
	inline bool is_fast             (                  ) const { return this->fast;                 }
	inline bool is_debug            (                  ) const { return this->debug;                }
	inline bool is_debug_hex        (                  ) const { return this->debug_hex;            }
	inline bool is_last_input_socket(const Socket &s_in) const { return last_input_socket == &s_in; }
	inline bool can_exec            (                  ) const;

	inline const Module& get_module     (               ) const { return this->module;  }
	inline std::string   get_name       (               ) const { return this->name;    }
	inline uint32_t      get_n_calls    (               ) const { return this->n_calls; }
	       Socket_type   get_socket_type(const Socket &s) const;

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

	int exec();

	inline Socket& operator[](const int id)
	{
		return *this->sockets[id];
	}

	inline void update_timer(const int id, const std::chrono::nanoseconds &duration)
	{
		if (this->is_stats())
		{
			this->timers_n_calls[id]++;
			this->timers_total[id] += duration;
			if (this->n_calls)
			{
				this->timers_max[id] = std::max(this->timers_max[id], duration);
				this->timers_min[id] = std::min(this->timers_min[id], duration);
			}
			else
			{
				this->timers_max[id] = duration;
				this->timers_min[id] = duration;
			}
		}
	}

protected:
	void register_timer(const std::string &key);

	template <typename T>
	Socket& create_socket_in(const std::string &name, const size_t n_elmts);

	template <typename T>
	Socket& create_socket_in_out(const std::string &name, const size_t n_elmts);

	template <typename T>
	Socket& create_socket_out(const std::string &name, const size_t n_elmts);

	void create_codelet(std::function<int(void)> &codelet);

private:
	template <typename T>
	inline Socket& create_socket(const std::string &name, const size_t n_elmts);
};
}
}

#endif /* PROCESS_HPP_ */
