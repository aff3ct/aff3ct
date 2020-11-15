#include <utility>
#include <iomanip>
#include <sstream>

#include "Module/Probe/Value/Probe_value.hpp"
#include "Module/Probe/Throughput/Probe_throughput.hpp"
#include "Module/Probe/Latency/Probe_latency.hpp"
#include "Module/Probe/Time/Probe_time.hpp"
#include "Module/Probe/Timestamp/Probe_timestamp.hpp"
#include "Module/Probe/Occurrence/Probe_occurrence.hpp"
#include "Tools/Display/Reporter/Probe/Reporter_probe.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Reporter_probe
::Reporter_probe(const std::string &group_name, const std::string &group_description, const int n_frames)
: Reporter(),
  n_frames(n_frames),
  mtx(100)
{
	if (group_name.empty())
	{
		std::stringstream message;
		message << "'group_name' can't be empty.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_frames <= 0)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->cols_groups.push_back(std::make_pair(std::make_pair(group_name, group_description),
	                            std::vector<Reporter::title_t>()));
}

Reporter_probe
::Reporter_probe(const std::string &group_name, const int n_frames)
: Reporter_probe(group_name, "", n_frames)
{
}

void Reporter_probe
::probe(const std::string &name, const void *data, const size_t frame_id)
{
	const int col = this->name_to_col[name];
	// bool can_push = false;
	     if (this->datatypes[col] == typeid(double )) /* can_push = */ this->push<double >(col, (double* )data);
	else if (this->datatypes[col] == typeid(float  )) /* can_push = */ this->push<float  >(col, (float*  )data);
	else if (this->datatypes[col] == typeid(int64_t)) /* can_push = */ this->push<int64_t>(col, (int64_t*)data);
	else if (this->datatypes[col] == typeid(int32_t)) /* can_push = */ this->push<int32_t>(col, (int32_t*)data);
	else if (this->datatypes[col] == typeid(int16_t)) /* can_push = */ this->push<int16_t>(col, (int16_t*)data);
	else if (this->datatypes[col] == typeid(int8_t )) /* can_push = */ this->push<int8_t >(col, (int8_t* )data);
	else
	{
		std::stringstream message;
		message << "Unsupported type.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

void Reporter_probe
::reset()
{
	std::fill(this->head.begin(), this->head.end(), 0);
	std::fill(this->tail.begin(), this->tail.end(), 0);
}

template <typename T>
bool Reporter_probe
::format_values(const int col, std::stringstream &temp_stream)
{
	std::vector<T> buff(this->datasizes[col]);
	const auto can_pull = this->pull<T>(col, buff.data());
	if (this->datasizes[col] > 1 && can_pull) temp_stream << "[";
	for (size_t v = 0; v < this->datasizes[col] && can_pull; v++)
	{
		const std::string s = std::string((v != 0) ? ", " : "") + std::string((buff[v] >= 0) ? " " : "");
		temp_stream << std::setprecision(this->precisions[col]) << s << +buff[v];
	}
	if (this->datasizes[col]> 1 && can_pull) temp_stream << "]";
	return can_pull;
}

Reporter::report_t Reporter_probe
::report(bool final)
{
	if (this->cols_groups[0].second.size() == 0)
	{
		std::stringstream message;
		message << "'cols_groups[0].second.size()' has to be greater than 0 ('cols_groups[0].second.size()' = "
		        << this->cols_groups[0].second.size() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (final)
		return this->final_report;

	Reporter::report_t the_report(this->cols_groups.size());
	auto& probe_report = the_report[0];

	// bool can_pull = false;
	for (auto f = 0; f < this->n_frames; f++)
	{
		for (size_t col = 0; col < this->buffer.size(); col++)
		{
			std::stringstream stream, temp_stream;
			temp_stream.flags(this->stream_flags[col]);
			     if (this->datatypes[col] == typeid(double )) /* can_pull = */ format_values<double >(col, temp_stream);
			else if (this->datatypes[col] == typeid(float  )) /* can_pull = */ format_values<float  >(col, temp_stream);
			else if (this->datatypes[col] == typeid(int64_t)) /* can_pull = */ format_values<int64_t>(col, temp_stream);
			else if (this->datatypes[col] == typeid(int32_t)) /* can_pull = */ format_values<int32_t>(col, temp_stream);
			else if (this->datatypes[col] == typeid(int16_t)) /* can_pull = */ format_values<int16_t>(col, temp_stream);
			else if (this->datatypes[col] == typeid(int8_t )) /* can_pull = */ format_values<int8_t >(col, temp_stream);
			else
			{
				std::stringstream message;
				message << "Unsupported type.";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			// if (!can_pull)
			// 	temp_stream << std::setprecision(this->precisions[col]) << std::scientific << " -";

			stream << std::setprecision(this->precisions[col] +1) << temp_stream.str();
			probe_report.push_back(stream.str());
		}
	}

	this->final_report = the_report;
	return the_report;
}

size_t B_from_datatype(const std::type_index &type)
{
	if (type == typeid(double )) return 8;
	if (type == typeid(float  )) return 4;
	if (type == typeid(int64_t)) return 8;
	if (type == typeid(int32_t)) return 4;
	if (type == typeid(int16_t)) return 2;
	if (type == typeid(int8_t )) return 1;

	std::stringstream message;
	message << "Unsupported type.";
	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
}

void Reporter_probe
::create_probe_checks(const std::string &name)
{
	if (name_to_col.count(name))
	{
		std::stringstream message;
		message << "'name' already exist in this reporter ('name' = " << name << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->buffer.size() > 100)
	{
		std::stringstream message;
		message << "'buffer.size()' has to be smaller than 'mtx.size()' ('buffer.size()' = " << this->buffer.size()
		        << ", 'mtx.size()' = " << this->mtx.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
module::Probe_value<T>* Reporter_probe
::create_probe_value(const std::string &name,
                     const std::string &unit,
                     const size_t socket_size,
                     const std::ios_base::fmtflags ff,
                     const size_t precision)
{
	this->create_probe_checks(name);
	auto probe = new module::Probe_value<T>(socket_size, name, *this, this->n_frames);
	this->head                 .push_back(0);
	this->tail                 .push_back(0);
	this->buffer               .push_back(std::vector<std::vector<int8_t>>(this->n_frames * 100,
	                                      std::vector<int8_t>(socket_size * B_from_datatype(probe->get_datatype()))));
	this->datatypes            .push_back(probe->get_datatype());
	this->stream_flags         .push_back(ff);
	this->precisions           .push_back(precision);
	this->datasizes            .push_back(socket_size);
	this->cols_groups[0].second.push_back(std::make_pair(name, unit));
	this->name_to_col[name] = this->buffer.size() -1;
	this->col_to_name[this->buffer.size() -1] = name;
	return probe;
}

template <typename T>
module::Probe_throughput<T>* Reporter_probe
::create_probe_throughput(const std::string &name,
                          const size_t socket_size,
                          const std::ios_base::fmtflags ff,
                          const size_t precision)
{
	this->create_probe_checks(name);
	auto probe = new module::Probe_throughput<T>(socket_size, name, *this, this->n_frames);
	this->head                 .push_back(0);
	this->tail                 .push_back(0);
	this->buffer               .push_back(std::vector<std::vector<int8_t>>(this->n_frames * 100,
	                                      std::vector<int8_t>(1 * B_from_datatype(probe->get_datatype()))));
	this->datatypes            .push_back(probe->get_datatype());
	this->stream_flags         .push_back(ff);
	this->precisions           .push_back(precision);
	this->datasizes            .push_back(1);
	this->cols_groups[0].second.push_back(std::make_pair(name, "(Mbps)"));
	this->name_to_col[name] = this->buffer.size() -1;
	this->col_to_name[this->buffer.size() -1] = name;
	return probe;
}

template <typename T>
module::Probe_latency<T>* Reporter_probe
::create_probe_latency(const std::string &name,
                       const size_t socket_size,
                       const std::ios_base::fmtflags ff,
                       const size_t precision)
{
	this->create_probe_checks(name);
	auto probe = new module::Probe_latency<T>(socket_size, name, *this, this->n_frames);
	this->head                 .push_back(0);
	this->tail                 .push_back(0);
	this->buffer               .push_back(std::vector<std::vector<int8_t>>(this->n_frames * 100,
	                                      std::vector<int8_t>(1 * B_from_datatype(probe->get_datatype()))));
	this->datatypes            .push_back(probe->get_datatype());
	this->stream_flags         .push_back(ff);
	this->precisions           .push_back(precision);
	this->datasizes            .push_back(1);
	this->cols_groups[0].second.push_back(std::make_pair(name, "(us)"));
	this->name_to_col[name] = this->buffer.size() -1;
	this->col_to_name[this->buffer.size() -1] = name;
	return probe;
}

template <typename T>
module::Probe_time<T>* Reporter_probe
::create_probe_time(const std::string &name,
                    const size_t socket_size,
                    const std::ios_base::fmtflags ff,
                    const size_t precision)
{
	this->create_probe_checks(name);
	auto probe = new module::Probe_time<T>(socket_size, name, *this, this->n_frames);
	this->head                 .push_back(0);
	this->tail                 .push_back(0);
	this->buffer               .push_back(std::vector<std::vector<int8_t>>(this->n_frames * 100,
	                                      std::vector<int8_t>(1 * B_from_datatype(probe->get_datatype()))));
	this->datatypes            .push_back(probe->get_datatype());
	this->stream_flags         .push_back(ff);
	this->precisions           .push_back(precision);
	this->datasizes            .push_back(1);
	this->cols_groups[0].second.push_back(std::make_pair(name, "(sec)"));
	this->name_to_col[name] = this->buffer.size() -1;
	this->col_to_name[this->buffer.size() -1] = name;
	return probe;
}

template <typename T>
module::Probe_timestamp<T>* Reporter_probe
::create_probe_timestamp(const std::string &name,
                         const size_t socket_size,
                         const std::ios_base::fmtflags ff,
                         const size_t precision)
{
	this->create_probe_checks(name);
	auto probe = new module::Probe_timestamp<T>(socket_size, name, *this, this->n_frames);
	this->head                 .push_back(0);
	this->tail                 .push_back(0);
	this->buffer               .push_back(std::vector<std::vector<int8_t>>(this->n_frames * 100,
	                                      std::vector<int8_t>(1 * B_from_datatype(probe->get_datatype()))));
	this->datatypes            .push_back(probe->get_datatype());
	this->stream_flags         .push_back(ff);
	this->precisions           .push_back(precision);
	this->datasizes            .push_back(1);
	this->cols_groups[0].second.push_back(std::make_pair(name, "(sec)"));
	this->name_to_col[name] = this->buffer.size() -1;
	this->col_to_name[this->buffer.size() -1] = name;
	return probe;
}

template <typename T>
module::Probe_occurrence<T>* Reporter_probe
::create_probe_occurrence(const std::string &name,
                          const std::string &unit,
                          const size_t socket_size,
                          const std::ios_base::fmtflags ff,
                          const size_t precision)
{
	this->create_probe_checks(name);
	auto probe = new module::Probe_occurrence<T>(socket_size, name, *this, this->n_frames);
	this->head                 .push_back(0);
	this->tail                 .push_back(0);
	this->buffer               .push_back(std::vector<std::vector<int8_t>>(this->n_frames * 100,
	                                      std::vector<int8_t>(1 * B_from_datatype(probe->get_datatype()))));
	this->datatypes            .push_back(probe->get_datatype());
	this->stream_flags         .push_back(ff);
	this->precisions           .push_back(precision);
	this->datasizes            .push_back(1);
	this->cols_groups[0].second.push_back(std::make_pair(name, unit));
	this->name_to_col[name] = this->buffer.size() -1;
	this->col_to_name[this->buffer.size() -1] = name;
	return probe;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
template aff3ct::module::Probe_value<R_32>* aff3ct::tools::Reporter_probe::create_probe_value<R_32>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_value<R_64>* aff3ct::tools::Reporter_probe::create_probe_value<R_64>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_value<B_8 >* aff3ct::tools::Reporter_probe::create_probe_value<B_8 >(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_value<B_16>* aff3ct::tools::Reporter_probe::create_probe_value<B_16>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_value<B_32>* aff3ct::tools::Reporter_probe::create_probe_value<B_32>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_value<B_64>* aff3ct::tools::Reporter_probe::create_probe_value<B_64>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);

template aff3ct::module::Probe_throughput<R_32>* aff3ct::tools::Reporter_probe::create_probe_throughput<R_32>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_throughput<R_64>* aff3ct::tools::Reporter_probe::create_probe_throughput<R_64>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_throughput<B_8 >* aff3ct::tools::Reporter_probe::create_probe_throughput<B_8 >(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_throughput<B_16>* aff3ct::tools::Reporter_probe::create_probe_throughput<B_16>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_throughput<B_32>* aff3ct::tools::Reporter_probe::create_probe_throughput<B_32>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_throughput<B_64>* aff3ct::tools::Reporter_probe::create_probe_throughput<B_64>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);

template aff3ct::module::Probe_latency<R_32>* aff3ct::tools::Reporter_probe::create_probe_latency<R_32>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_latency<R_64>* aff3ct::tools::Reporter_probe::create_probe_latency<R_64>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_latency<B_8 >* aff3ct::tools::Reporter_probe::create_probe_latency<B_8 >(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_latency<B_16>* aff3ct::tools::Reporter_probe::create_probe_latency<B_16>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_latency<B_32>* aff3ct::tools::Reporter_probe::create_probe_latency<B_32>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_latency<B_64>* aff3ct::tools::Reporter_probe::create_probe_latency<B_64>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);

template aff3ct::module::Probe_time<R_32>* aff3ct::tools::Reporter_probe::create_probe_time<R_32>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_time<R_64>* aff3ct::tools::Reporter_probe::create_probe_time<R_64>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_time<B_8 >* aff3ct::tools::Reporter_probe::create_probe_time<B_8 >(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_time<B_16>* aff3ct::tools::Reporter_probe::create_probe_time<B_16>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_time<B_32>* aff3ct::tools::Reporter_probe::create_probe_time<B_32>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_time<B_64>* aff3ct::tools::Reporter_probe::create_probe_time<B_64>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);

template aff3ct::module::Probe_timestamp<R_32>* aff3ct::tools::Reporter_probe::create_probe_timestamp<R_32>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_timestamp<R_64>* aff3ct::tools::Reporter_probe::create_probe_timestamp<R_64>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_timestamp<B_8 >* aff3ct::tools::Reporter_probe::create_probe_timestamp<B_8 >(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_timestamp<B_16>* aff3ct::tools::Reporter_probe::create_probe_timestamp<B_16>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_timestamp<B_32>* aff3ct::tools::Reporter_probe::create_probe_timestamp<B_32>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_timestamp<B_64>* aff3ct::tools::Reporter_probe::create_probe_timestamp<B_64>(const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);

template aff3ct::module::Probe_occurrence<R_32>* aff3ct::tools::Reporter_probe::create_probe_occurrence<R_32>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_occurrence<R_64>* aff3ct::tools::Reporter_probe::create_probe_occurrence<R_64>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_occurrence<B_8 >* aff3ct::tools::Reporter_probe::create_probe_occurrence<B_8 >(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_occurrence<B_16>* aff3ct::tools::Reporter_probe::create_probe_occurrence<B_16>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_occurrence<B_32>* aff3ct::tools::Reporter_probe::create_probe_occurrence<B_32>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
template aff3ct::module::Probe_occurrence<B_64>* aff3ct::tools::Reporter_probe::create_probe_occurrence<B_64>(const std::string&, const std::string&, const size_t, const std::ios_base::fmtflags, const size_t);
// ==================================================================================== explicit template instantiation

