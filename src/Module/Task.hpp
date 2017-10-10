#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <string>
#include <sstream>

#include <map>
#include <chrono>
#include <vector>
#include <typeinfo>
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
	const std::string &module_name;
	const std::string name;
	bool autoalloc;
	bool autoexec;
	bool stats;
	bool fast;
	bool debug;
	int32_t debug_limit;
	uint8_t debug_precision;
	std::function<int(void)> codelet;
	std::vector<std::vector<uint8_t>> out_buffers;

	// stats
	uint32_t                 n_calls;
	std::chrono::nanoseconds duration_total;
	std::chrono::nanoseconds duration_min;
	std::chrono::nanoseconds duration_max;

	std::vector<std::string                          > registered_timers;
	std::map   <std::string, uint32_t                > registered_n_calls;
	std::map   <std::string, std::chrono::nanoseconds> registered_timers_total;
	std::map   <std::string, std::chrono::nanoseconds> registered_timers_min;
	std::map   <std::string, std::chrono::nanoseconds> registered_timers_max;

	Socket* last_input_socket;
	std::map<std::string,Socket_type> socket_type;
	std::map<std::string,Socket*    > sockets_map;

public:
	std::vector<Socket*> sockets;

	Task(const Module &module,
	     const std::string name,
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
	void set_debug_limit    (const uint32_t limit    );
	void set_debug_precision(const uint8_t  prec     );

	inline bool is_autoalloc        (                  ) const { return this->autoalloc;            }
	inline bool is_autoexec         (                  ) const { return this->autoexec;             }
	inline bool is_stats            (                  ) const { return this->stats;                }
	inline bool is_fast             (                  ) const { return this->fast;                 }
	inline bool is_debug            (                  ) const { return this->debug;                }
	inline bool is_last_input_socket(const Socket &s_in) const { return last_input_socket == &s_in; }
	inline bool can_exec            (                  ) const;

	inline const std::string& get_module_name(               ) const { return this->module_name; }
	inline const Module&      get_module     (               ) const { return this->module;      }
	inline std::string        get_name       (               ) const { return this->name;        }
	inline uint32_t           get_n_calls    (               ) const { return this->n_calls;     }
	inline Socket_type        get_socket_type(const Socket &s) const;

	// get stats
	std::chrono::nanoseconds        get_duration_total         (                     ) const;
	std::chrono::nanoseconds        get_duration_avg           (                     ) const;
	std::chrono::nanoseconds        get_duration_min           (                     ) const;
	std::chrono::nanoseconds        get_duration_max           (                     ) const;
	const std::vector<std::string>& get_registered_timers      (                     ) const;
	uint32_t                        get_registered_n_calls     (const std::string key) const;
	std::chrono::nanoseconds        get_registered_timers_total(const std::string key) const;
	std::chrono::nanoseconds        get_registered_timers_avg  (const std::string key) const;
	std::chrono::nanoseconds        get_registered_timers_min  (const std::string key) const;
	std::chrono::nanoseconds        get_registered_timers_max  (const std::string key) const;

	int exec();

	inline Socket& operator[](const std::string name)
	{
		const auto &it = this->sockets_map.find(name);
		if (it != this->sockets_map.end())
			return *it->second;
		else
		{
			std::stringstream message;
			message << "The socket does not exist ('socket.name' = " << name << ", 'task.name' = " << this->get_name()
			        << ", 'module.name' = " << get_module_name()
			        << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	inline void update_timer(const std::string key, const std::chrono::nanoseconds &duration)
	{
		if (this->is_stats())
		{
			this->registered_n_calls[key]++;
			this->registered_timers_total[key] += duration;
			if (this->n_calls)
			{
				this->registered_timers_max[key] = std::max(this->registered_timers_max[key], duration);
				this->registered_timers_min[key] = std::min(this->registered_timers_min[key], duration);
			}
			else
			{
				this->registered_timers_max[key] = duration;
				this->registered_timers_min[key] = duration;
			}
		}
	}

protected:
	void register_timer(const std::string key);

	template <typename T>
	Socket& create_socket_in(const std::string name, const size_t n_elmts);

	template <typename T>
	Socket& create_socket_in_out(const std::string name, const size_t n_elmts);

	template <typename T>
	Socket& create_socket_out(const std::string name, const size_t n_elmts);

	void create_codelet(std::function<int(void)> codelet);

private:
	template <typename T>
	inline Socket& create_socket(const std::string name, const size_t n_elmts);
};
}
}

#endif /* PROCESS_HPP_ */
