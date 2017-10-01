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

	std::vector<std::string                          > registered_duration;
	std::map   <std::string, uint32_t                > registered_n_calls;
	std::map   <std::string, std::chrono::nanoseconds> registered_duration_total;
	std::map   <std::string, std::chrono::nanoseconds> registered_duration_min;
	std::map   <std::string, std::chrono::nanoseconds> registered_duration_max;

	std::map<std::string,Socket_type> socket_type;
public:
	std::vector<Socket> socket;

	Task(const Module &module,
	     const std::string name,
	     const bool autoalloc = false,
	     const bool autoexec  = false,
	     const bool stats     = false,
	     const bool debug     = false);

	virtual ~Task();

	void reset_stats();

	void set_autoalloc      (const bool     autoalloc);
	void set_autoexec       (const bool     autoexec );
	void set_stats          (const bool     stats    );
	void set_debug          (const bool     debug    );
	void set_debug_limit    (const uint32_t limit    );
	void set_debug_precision(const uint8_t  prec     );

	bool is_autoalloc() const;
	bool is_autoexec () const;
	bool is_stats    () const;
	bool is_debug    () const;
	bool can_exec    () const;

	const Module&                   get_module                   (                     ) const;
	std::string                     get_name                     (                     ) const;
	uint32_t                        get_n_calls                  (                     ) const;
	std::chrono::nanoseconds        get_duration_total           (                     ) const;
	std::chrono::nanoseconds        get_duration_avg             (                     ) const;
	std::chrono::nanoseconds        get_duration_min             (                     ) const;
	std::chrono::nanoseconds        get_duration_max             (                     ) const;
	const std::vector<std::string>& get_registered_duration      (                     ) const;
	uint32_t                        get_registered_n_calls       (const std::string key) const;
	std::chrono::nanoseconds        get_registered_duration_total(const std::string key) const;
	std::chrono::nanoseconds        get_registered_duration_avg  (const std::string key) const;
	std::chrono::nanoseconds        get_registered_duration_min  (const std::string key) const;
	std::chrono::nanoseconds        get_registered_duration_max  (const std::string key) const;
	Socket_type                     get_socket_type              (const Socket      &s ) const;

	bool last_input_socket(const Socket &s_in) const;

	int exec();

	      Socket& operator[](const std::string name);
	const Socket& operator[](const std::string name) const;

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

private:
	template <typename T>
	inline Socket& create_socket(const std::string name, const size_t n_elmts);
};
}
}

#endif /* PROCESS_HPP_ */
