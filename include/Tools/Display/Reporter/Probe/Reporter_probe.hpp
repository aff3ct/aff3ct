/*!
 * \file
 * \brief Class tools::Reporter_probe.
 */
#ifndef REPORTER_PROBE_HPP_
#define REPORTER_PROBE_HPP_

#include <typeindex>
#include <iostream>
#include <cstddef>
#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <ios>

#include "Tools/Display/Reporter/Reporter.hpp"

namespace aff3ct
{
namespace module
{
	template <typename T> class Probe_value;
	template <typename T> class Probe_throughput;
	template <typename T> class Probe_latency;
	template <typename T> class Probe_time;
	template <typename T> class Probe_timestamp;
	template <typename T> class Probe_occurrence;
}
namespace tools
{

class Reporter_probe : public Reporter
{
protected:
	std::vector<size_t> head;
	std::vector<size_t> tail;
	std::vector<std::vector<std::vector<int8_t>>> buffer;
	std::vector<std::type_index> datatypes;
	std::vector<std::ios_base::fmtflags> stream_flags;
	std::vector<size_t> precisions;
	std::vector<size_t> datasizes;
	std::map<std::string, int> name_to_col;
	std::map<int, std::string> col_to_name;
	const int n_frames;
	Reporter::report_t final_report;
	std::vector<std::mutex> mtx;

public:
	Reporter_probe(const std::string &group_name,
	               const std::string &group_description,
	               const int n_frames = 1);

	explicit Reporter_probe(const std::string &group_name,
	                        const int n_frames = 1);

	virtual ~Reporter_probe() = default;

	virtual report_t report(bool final = false);

	template <typename T>
	module::Probe_value<T>* create_probe_value(const std::string &name,
	                                           const std::string &unit = "",
	                                           const size_t socket_size = 1,
	                                           const std::ios_base::fmtflags ff = std::ios_base::scientific,
	                                           const size_t precision = 3);

	template <typename T>
	module::Probe_throughput<T>* create_probe_throughput(const std::string &name,
	                                                     const size_t socket_size = 1,
	                                                     const std::ios_base::fmtflags ff = std::ios_base::dec | std::ios_base::fixed,
	                                                     const size_t precision = 3);

	template <typename T>
	module::Probe_latency<T>* create_probe_latency(const std::string &name,
	                                               const size_t socket_size = 1,
	                                               const std::ios_base::fmtflags ff = std::ios_base::scientific,
	                                               const size_t precision = 3);

	template <typename T>
	module::Probe_time<T>* create_probe_time(const std::string &name,
	                                         const size_t socket_size = 1,
	                                         const std::ios_base::fmtflags ff = std::ios_base::dec | std::ios_base::fixed,
	                                         const size_t precision = 2);

	template <typename T>
	module::Probe_timestamp<T>* create_probe_timestamp(const std::string &name,
	                                                   const size_t socket_size = 1,
	                                                   const std::ios_base::fmtflags ff = std::ios_base::scientific,
	                                                   const size_t precision = 2);

	template <typename T>
	module::Probe_occurrence<T>* create_probe_occurrence(const std::string &name,
	                                                     const std::string &unit = "",
	                                                     const size_t socket_size = 1,
	                                                     const std::ios_base::fmtflags ff = std::ios_base::scientific,
	                                                     const size_t precision = 3);



	virtual void probe(const std::string &name, const void *data, const size_t frame_id);

	virtual void reset();

protected:
	void create_probe_checks(const std::string &name);

	template <typename T>
	size_t col_size(const int col);

	template <typename T>
	bool push(const int col, const T *data);

	template <typename T>
	bool pull(const int col, T *data);

private:
	template <typename T>
	bool format_values(const int col, std::stringstream &temp_stream);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Display/Reporter/Probe/Reporter_probe.hxx"
#endif

#endif /* REPORTER_PROBE_HPP_ */
