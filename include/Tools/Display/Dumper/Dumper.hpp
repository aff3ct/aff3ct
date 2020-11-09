/*!
 * \file
 * \brief Class tools::Dumper.
 */
#ifndef DUMPER_HPP_
#define DUMPER_HPP_

#include <typeindex>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace aff3ct
{
namespace tools
{
class Dumper_reduction;
class Dumper
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend Dumper_reduction;
#endif

protected:
	static const std::string default_ext;

	std::vector<std::vector<std::vector<char>>> buffer;

	unsigned                           add_threshold;
	std::vector<const char*>           registered_data_ptr;
	std::vector<unsigned>              registered_data_size;
	std::vector<unsigned>              registered_data_sizeof;
	std::vector<std::type_index>       registered_data_type;
	std::vector<std::string>           registered_data_ext;
	std::vector<bool>                  registered_data_bin;
	std::vector<std::vector<unsigned>> registered_data_head;
	std::vector<unsigned>              registered_data_n_frames;
	bool write_headers;

public:
	Dumper(/*const int n_frames = 1*/);
	virtual ~Dumper() = default;

	template <typename T>
	void register_data(const T *ptr, const unsigned size, const unsigned add_threshold = 0,
	                   const std::string &file_ext = default_ext, const bool binary_mode = false, const unsigned n_frames = 1,
	                   std::vector<unsigned> headers = std::vector<unsigned>());
	template <typename T, class A = std::allocator<T>>
	void register_data(const std::vector<T,A> &data, const unsigned add_threshold = 0,
	                   const std::string &file_ext = default_ext, const bool binary_mode = false, const unsigned n_frames = 1,
	                   std::vector<unsigned> headers = std::vector<unsigned>());

	virtual void dump (const std::string& base_path                   );
	virtual void add  (const unsigned n_err, const size_t frame_id = 0);
	virtual void clear(                                               );

	void set_write_headers(const bool write_headers);

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
