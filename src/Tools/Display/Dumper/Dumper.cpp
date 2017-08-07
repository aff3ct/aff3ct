#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "Tools/Exception/exception.hpp"

#include "Dumper.hpp"

using namespace aff3ct::tools;

Dumper
::Dumper(const int32_t n_frames)
: n_frames(n_frames)
{
	if (n_frames <= 0)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Dumper
::~Dumper()
{
}

template <typename T>
void Dumper
::register_data(const T *ptr, const unsigned size, const std::string file_ext, const bool binary_mode,
                std::vector<unsigned> headers)
{
	if (ptr == nullptr)
		throw invalid_argument(__FILE__, __LINE__, __func__, "'ptr' can't be null.");

	if (size <= 0)
	{
		std::stringstream message;
		message << "'size' has to be greater than 0 ('size' = " << size << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (file_ext.empty())
		throw invalid_argument(__FILE__, __LINE__, __func__, "'file_ext' can't be empty.");

	if (n_frames <= 0 && n_frames != -1)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 or equal to -1 ('n_frames' = " << n_frames << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->buffer.push_back(std::vector<std::vector<char>>());

	this->registered_data_ptr   .push_back((char*)ptr );
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
                std::vector<unsigned> headers)
{
	if (n_frames <= 0 && n_frames != -1)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 or equal to -1 ('n_frames' = " << n_frames << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	};

	this->register_data(data.data(), (unsigned)(data.size() / this->n_frames), file_ext, binary_mode, headers);
}

void Dumper
::add(const int32_t frame_id)
{
	if (frame_id < 0 || frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be positive and smaller than 'n_frames' ('frame_id' = " << frame_id
		        << ", 'n_frames' = " << this->n_frames << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto i = 0; i < (int32_t)this->registered_data_ptr.size(); i++)
	{
		const auto ptr   = this->registered_data_ptr [i];
		const auto bytes = this->registered_data_size[i] * this->registered_data_sizeof[i];

		this->buffer[i].push_back(std::vector<char>(bytes));

		std::copy(ptr + bytes * (frame_id +0),
		          ptr + bytes * (frame_id +1),
		          this->buffer[i][this->buffer[i].size() -1].begin());
	}
}

void Dumper
::dump(const std::string& base_path)
{
	if (base_path.empty())
		throw invalid_argument(__FILE__, __LINE__, __func__, "'base_path' can't be empty.");

	for (auto i = 0; i < (int32_t)this->registered_data_ptr.size(); i++)
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
			file.open(path, std::ofstream::out | std::ios_base::binary);

			this->write_header_binary(file, (unsigned)this->buffer[i].size(), size, head);
			this->write_body_binary  (file, this->buffer[i], size * size_of);
		}
		else
		{
			file.open(path, std::ofstream::out);

			this->write_header_text(file, (unsigned)this->buffer[i].size(), size, head);
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
	if      (type == typeid(int8_t )) this->_write_body_text<int8_t >(file, buffer, size);
	else if (type == typeid(int16_t)) this->_write_body_text<int16_t>(file, buffer, size);
	else if (type == typeid(int32_t)) this->_write_body_text<int32_t>(file, buffer, size);
	else if (type == typeid(int64_t)) this->_write_body_text<int64_t>(file, buffer, size);
	else if (type == typeid(float  )) this->_write_body_text<float  >(file, buffer, size);
	else if (type == typeid(double )) this->_write_body_text<double >(file, buffer, size);
	else
		throw invalid_argument(__FILE__, __LINE__, __func__, "Unsupported data type.");
}

template <typename T>
void Dumper
::_write_body_text(std::ofstream &file, const std::vector<std::vector<char>> &buffer, const unsigned size)
{
	for (auto b : buffer)
	{
		const auto data = (T*)b.data();
		for (unsigned i = 0; i < size; i++)
			file << +data[i] << " ";
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
template void Dumper::register_data<int8_t >(const int8_t*,  const unsigned, const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<int16_t>(const int16_t*, const unsigned, const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<int32_t>(const int32_t*, const unsigned, const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<int64_t>(const int64_t*, const unsigned, const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<float  >(const float*,   const unsigned, const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<double >(const double*,  const unsigned, const std::string, const bool, std::vector<unsigned>);

template void Dumper::register_data<int8_t >(const mipp::vector<int8_t>&,  const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<int16_t>(const mipp::vector<int16_t>&, const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<int32_t>(const mipp::vector<int32_t>&, const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<int64_t>(const mipp::vector<int64_t>&, const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<float  >(const mipp::vector<float>&,   const std::string, const bool, std::vector<unsigned>);
template void Dumper::register_data<double >(const mipp::vector<double>&,  const std::string, const bool, std::vector<unsigned>);

template void Dumper::_write_body_text<int8_t >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<int16_t>(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<int32_t>(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<int64_t>(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<float  >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<double >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
// ==================================================================================== explicit template instantiation
