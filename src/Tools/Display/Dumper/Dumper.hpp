#ifndef DUMPER_HPP_
#define DUMPER_HPP_

#include <tuple>
#include <string>
#include <vector>
#include <fstream>
#include <typeindex>

#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace tools
{
class Dumper_reduction;
class Dumper
{
	friend Dumper_reduction;

protected:
	const int n_frames;

	std::vector<std::vector<std::vector<char>>> buffer;

	std::vector<const char*>           registered_data_ptr;
	std::vector<unsigned>              registered_data_fra;
	std::vector<unsigned>              registered_data_size;
	std::vector<unsigned>              registered_data_sizeof;
	std::vector<std::type_index>       registered_data_type;
	std::vector<std::string>           registered_data_ext;
	std::vector<bool>                  registered_data_bin;
	std::vector<std::vector<unsigned>> registered_data_head;

public:
	Dumper(const int n_frames = 1);
	virtual ~Dumper();

	template <typename T>
	void register_data(const T *ptr, const unsigned size, const std::string file_ext = "dump",
	                   const bool binary_mode = false, std::vector<unsigned> headers = std::vector<unsigned>(),
	                   const int n_frames = -1);
	template <typename T>
	void register_data(const mipp::vector<T> &data, const std::string file_ext = "dump",
	                   const bool binary_mode = false, std::vector<unsigned> headers = std::vector<unsigned>(),
	                   const int n_frames = -1);

	virtual void dump (const std::string& base_path);
	virtual void add  (const int frame_id = 0      );
	virtual void clear(                            );

protected:
	void write_header_text(std::ofstream &file, const unsigned n_data, const unsigned data_size,
	                       const std::vector<unsigned> &headers);
	void write_body_text(std::ofstream &file, const std::vector<std::vector<char>> &buffer, const unsigned size,
	                     const std::type_index type);
	void write_header_binary(std::ofstream &file, const unsigned n_data, const unsigned data_size,
	                         const std::vector<unsigned> &headers);
	void write_body_binary(std::ofstream &file, const std::vector<std::vector<char>> &buffer, const unsigned bytes);

private:
	template <typename T>
	void _write_body_text(std::ofstream &file, const std::vector<std::vector<char>> &buffer, const unsigned size);
};
}
}

#endif /* DUMPER_HPP_ */
