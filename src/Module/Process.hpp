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

#include "Socket.hpp"

namespace aff3ct
{
namespace module
{
class Module;

class Process
{
	friend Socket;
	friend Module;

protected:
	const Module &module;

	const std::string name;
	bool autostart;
	bool stats;
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

	std::vector<std::string> registered_duration;
	std::map<std::string, uint32_t                > registered_n_calls;
	std::map<std::string, std::chrono::nanoseconds> registered_duration_total;
	std::map<std::string, std::chrono::nanoseconds> registered_duration_min;
	std::map<std::string, std::chrono::nanoseconds> registered_duration_max;

public:
	std::vector<Socket> s_in;
	std::vector<Socket> s_in_out;
	std::vector<Socket> s_out;

	Process(const Module &module,
	        const std::string name,
	        const bool autostart = true,
	        const bool stats     = false,
	        const bool debug     = false);

	void reset_stats();

	void set_autostart      (const bool     autostart);
	void set_stats          (const bool     stats    );
	void set_debug          (const bool     debug    );
	void set_debug_limit    (const uint32_t limit    );
	void set_debug_precision(const uint8_t  prec     );

	bool is_autostart();
	bool is_stats    ();
	bool is_debug    ();
	bool can_exec    ();

	std::string                     get_name                     (                     );
	uint32_t                        get_n_calls                  (                     );
	std::chrono::nanoseconds        get_duration_total           (                     );
	std::chrono::nanoseconds        get_duration_avg             (                     );
	std::chrono::nanoseconds        get_duration_min             (                     );
	std::chrono::nanoseconds        get_duration_max             (                     );
	const std::vector<std::string>& get_registered_duration      (                     );
	uint32_t                        get_registered_n_calls       (const std::string key);
	std::chrono::nanoseconds        get_registered_duration_total(const std::string key);
	std::chrono::nanoseconds        get_registered_duration_avg  (const std::string key);
	std::chrono::nanoseconds        get_registered_duration_min  (const std::string key);
	std::chrono::nanoseconds        get_registered_duration_max  (const std::string key);

	bool last_input_socket(Socket &s_in);

	int exec();

	Socket& operator[](const std::string name);

protected:
	void register_duration(const std::string key);

	void update_duration(const std::string key, const std::chrono::nanoseconds &duration);

	template <typename T>
	void create_socket_in(const std::string name, const size_t n_elmts);

	template <typename T>
	void create_socket_in_out(const std::string name, const size_t n_elmts);

	template <typename T>
	void create_socket_out(const std::string name, const size_t n_elmts);

	void create_codelet(std::function<int(void)> codelet);
};
}
}

#endif /* PROCESS_HPP_ */
