#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <string>
#include <sstream>

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
	const std::string name;
	const bool autostart;
	std::function<void(void)> codelet;
	std::vector<std::vector<uint8_t>> out_buffers;

public:
	std::vector<Socket> in;
	std::vector<Socket> out;

	Process(const std::string name, const bool autostart = true);

	std::string get_name();

	bool is_autostart();

	bool can_exec();

	bool last_input_socket(Socket &s_in);

	void exec();

	Socket& operator[](const std::string name);

protected:
	template <typename T>
	void create_socket_in(const std::string name, const size_t n_elmts);

	template <typename T>
	void create_socket_out(const std::string name, const size_t n_elmts);

	void create_codelet(std::function<void(void)> codelet);
};
}
}

#endif /* PROCESS_HPP_ */
