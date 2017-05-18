#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "Dumper.hpp"

using namespace aff3ct::tools;

Dumper
::Dumper(const int n_frames)
: n_frames(n_frames)
{
	if (n_frames <= 0)
		throw std::invalid_argument("aff3ct::tools::Dumper: \"n_frames\" has to be greater than 0.");
}

Dumper
::~Dumper()
{
}

template <typename T>
void Dumper
::register_data(const T *ptr, const unsigned size, const std::string file_ext, const bool binary_mode,
                std::vector<unsigned> headers, const int n_frames)
{
	if (ptr == nullptr)
		throw std::invalid_argument("aff3ct::tools::Dumper: \"ptr\" can't be null.");
	if (size <= 0)
		throw std::invalid_argument("aff3ct::tools::Dumper: \"size\" has to be greater than 0.");
	if (file_ext.empty())
		throw std::invalid_argument("aff3ct::tools::Dumper: \"file_ext\" can't be empty.");
	if (n_frames <= 0 && n_frames != -1)
		throw std::invalid_argument("aff3ct::tools::Dumper: \"n_frames\" has to be greater than 0 (or equal to -1).");

	const auto fra = (n_frames != -1) ? (unsigned)n_frames : (unsigned)this->n_frames;

	this->buffer.push_back(std::vector<std::vector<char>>());

	this->registered_data_ptr   .push_back((char*)ptr );
	this->registered_data_fra   .push_back(fra        );
	this->registered_data_size  .push_back(size       );
	this->registered_data_sizeof.push_back(sizeof(T)  );
	this->registered_data_type  .push_back(typeid(T)  );
	this->registered_data_ext   .push_back(file_ext   );
	this->registered_data_bin   .push_back(binary_mode);
	this->registered_data_head  .push_back(headers    );
}

template <typename T>
void Dumper
::register_data(const mipp::vector<T> &data, const std::string file_ext, const bool binary_mode,
                std::vector<unsigned> headers, const int n_frames)
{
	if (n_frames <= 0 && n_frames != -1)
		throw std::invalid_argument("aff3ct::tools::Dumper: \"n_frames\" has to be greater than 0 (or equal to -1).");

	const auto fra = (n_frames != -1) ? (unsigned)n_frames : (unsigned)this->n_frames;

	this->register_data(data.data(), data.size() / fra, file_ext, binary_mode, headers, n_frames);
}

void Dumper
::add(const int frame_id)
{
	if (frame_id < 0)
		throw std::invalid_argument("aff3ct::tools::Dumper: \"frame_id\" has to be positive.");

	for (auto i = 0; i < (int)this->registered_data_ptr.size(); i++)
	{
		const auto ptr      = this->registered_data_ptr [i];
		const auto n_frames = this->registered_data_fra [i];
		const auto bytes    = this->registered_data_size[i] * this->registered_data_sizeof[i];

		std::vector<char> tmp_buff(bytes);

		std::copy(ptr + bytes * ((frame_id % n_frames) +0),
		          ptr + bytes * ((frame_id % n_frames) +1),
		          tmp_buff.begin());

		this->buffer[i].push_back(tmp_buff);
	}
}

void Dumper
::dump(const std::string& base_path)
{
	if (base_path.empty())
		throw std::invalid_argument("aff3ct::tools::Dumper: \"base_path\" can't be empty.");

	for (auto i = 0; i < (int)this->registered_data_ptr.size(); i++)
	{
		const auto size    = this->registered_data_size  [i];
		const auto size_of = this->registered_data_sizeof[i];
		const auto type    = this->registered_data_type  [i];
		const auto ext     = this->registered_data_ext   [i];
		const auto bin     = this->registered_data_bin   [i];
		const auto head    = this->registered_data_head  [i];

		const std::string path = base_path + "." + ext;

		std::ofstream file;
		if (bin)
		{
			file.open(path, std::ios_base::binary);

			this->write_header_binary(file, this->buffer[i].size(), size, head);
			this->write_body_binary  (file, this->buffer[i], size * size_of);
		}
		else
		{
			file.open(path);

			this->write_header_text(file, this->buffer[i].size(), size, head);
			this->write_body_text  (file, this->buffer[i], size, type);
		}

		file.close();
	}
}

void Dumper
::clear()
{
	for (auto &b : this->buffer)
		b.clear();
	this->buffer.clear();

	this->registered_data_ptr   .clear();
	this->registered_data_fra   .clear();
	this->registered_data_size  .clear();
	this->registered_data_sizeof.clear();
	this->registered_data_type  .clear();
	this->registered_data_ext   .clear();
	this->registered_data_bin   .clear();
	this->registered_data_head  .clear();
}

void Dumper
::write_header_text(std::ofstream &file, const unsigned n_data, const unsigned data_size,
                    const std::vector<unsigned> &headers)
{
	file << n_data    << std::endl << std::endl;
	file << data_size << std::endl << std::endl;
	for (auto h : headers)
		file << h << " ";
	if (headers.size())
		file << std::endl << std::endl;
}

void Dumper
::write_body_text(std::ofstream &file, const std::vector<std::vector<char>> &buffer, const unsigned size,
                  const std::type_index type)
{
	if      (type == typeid(signed char)) this->_write_body_text<signed char>(file, buffer, size);
	else if (type == typeid(short      )) this->_write_body_text<short      >(file, buffer, size);
	else if (type == typeid(int        )) this->_write_body_text<int        >(file, buffer, size);
	else if (type == typeid(long long  )) this->_write_body_text<long long  >(file, buffer, size);
	else if (type == typeid(float      )) this->_write_body_text<float      >(file, buffer, size);
	else if (type == typeid(double     )) this->_write_body_text<double     >(file, buffer, size);
	else
		throw std::invalid_argument("aff3ct::tools::Dumper: unsupported data type.");
}

template <typename T>
void Dumper
::_write_body_text(std::ofstream &file, const std::vector<std::vector<char>> &buffer, const unsigned size)
{
	for (auto b : buffer)
	{
		const auto data = (T*)b.data();
		for (unsigned i = 0; i < size; i++)
			file << data[i] << " ";
		file << std::endl << std::endl;
	}
}

void Dumper
::write_header_binary(std::ofstream &file, const unsigned n_data, const unsigned data_size,
                      const std::vector<unsigned> &headers)
{
	file.write((char*)&n_data,    sizeof(n_data   ));
	file.write((char*)&data_size, sizeof(data_size));
	for (auto h : headers)
		file.write((char*)&h, sizeof(h));
}

void Dumper
::write_body_binary(std::ofstream &file, const std::vector<std::vector<char>> &buffer, const unsigned bytes)
{
	for (auto b : buffer)
		file.write(reinterpret_cast<char*>(b.data()), b.size());
}

// ==================================================================================== explicit template instantiation
template void Dumper::register_data<signed char>(const signed char*, const unsigned, const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<short      >(const short*,       const unsigned, const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<int        >(const int*,         const unsigned, const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<long long  >(const long long*,   const unsigned, const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<float      >(const float*,       const unsigned, const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<double     >(const double*,      const unsigned, const std::string, const bool, std::vector<unsigned>, const int);

template void Dumper::register_data<signed char>(const mipp::vector<signed char>&, const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<short      >(const mipp::vector<short>&,       const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<int        >(const mipp::vector<int>&,         const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<long long  >(const mipp::vector<long long>&,   const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<float      >(const mipp::vector<float>&,       const std::string, const bool, std::vector<unsigned>, const int);
template void Dumper::register_data<double     >(const mipp::vector<double>&,      const std::string, const bool, std::vector<unsigned>, const int);

template void Dumper::_write_body_text<signed char>(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<short      >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<int        >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<long long  >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<float      >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<double     >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
// ==================================================================================== explicit template instantiation
