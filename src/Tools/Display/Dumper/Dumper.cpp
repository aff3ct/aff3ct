#include <algorithm>
#include <iostream>
#include <cstdint>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

const std::string aff3ct::tools::Dumper::default_ext = "dump";

Dumper
::Dumper()
: add_threshold(0), write_headers(true)
{
}

void Dumper
::set_write_headers(const bool write_headers)
{
	this->write_headers = write_headers;
}

template <typename T>
void Dumper
::register_data(const T *ptr, const unsigned size, const unsigned add_threshold, const std::string &file_ext,
                const bool binary_mode, const unsigned n_frames, std::vector<unsigned> headers)
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

	if (n_frames == 0)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->buffer.push_back(std::vector<std::vector<char>>());

	this->registered_data_ptr     .push_back((char*)ptr );
	this->registered_data_size    .push_back(size       );
	this->registered_data_sizeof  .push_back(sizeof(T)  );
	this->registered_data_type    .push_back(typeid(T)  );
	this->registered_data_ext     .push_back(file_ext   );
	this->registered_data_bin     .push_back(binary_mode);
	this->registered_data_head    .push_back(headers    );
	this->registered_data_n_frames.push_back(n_frames   );

	this->add_threshold = add_threshold;
}

template <typename T, class A>
void Dumper
::register_data(const std::vector<T,A> &data, const unsigned add_threshold, const std::string &file_ext,
                const bool binary_mode, const unsigned n_frames, std::vector<unsigned> headers)
{
	if (n_frames == 0)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0('n_frames' = " << n_frames << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	};

	this->register_data(data.data(), (unsigned)(data.size() / n_frames), add_threshold,
	                    file_ext, binary_mode, n_frames, headers);
}


void Dumper
::add(const unsigned n_err, const size_t frame_id)
{
	if (n_err < this->add_threshold)
		return;

	for (auto i = 0; i < (int)this->registered_data_ptr.size(); i++)
	{
		if ((unsigned)frame_id < this->registered_data_n_frames[i])
		{
			const auto ptr   = this->registered_data_ptr [i];
			const auto bytes = this->registered_data_size[i] * this->registered_data_sizeof[i];

			this->buffer[i].push_back(std::vector<char>(bytes));

			std::copy(ptr + bytes * (frame_id +0),
			          ptr + bytes * (frame_id +1),
			          this->buffer[i][this->buffer[i].size() -1].begin());
		}
	}
}

void Dumper
::dump(const std::string& base_path)
{
	if (base_path.empty())
		throw invalid_argument(__FILE__, __LINE__, __func__, "'base_path' can't be empty.");

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
			file.open(path, std::ofstream::out | std::ios_base::binary);

			if (this->write_headers)
				this->write_header_binary(file, (unsigned)this->buffer[i].size(), size, head);
			this->write_body_binary(file, this->buffer[i], size * size_of);
		}
		else
		{
			file.open(path, std::ofstream::out);

			if (this->write_headers)
				this->write_header_text(file, (unsigned)this->buffer[i].size(), size, head);
			this->write_body_text(file, this->buffer[i], size, type);
		}

		file.close();
	}
}

void Dumper
::clear()
{
	for (auto &b : this->buffer)
		b.clear();
//	this->buffer.clear();

//	this->registered_data_ptr   .clear();
//	this->registered_data_size  .clear();
//	this->registered_data_sizeof.clear();
//	this->registered_data_type  .clear();
//	this->registered_data_ext   .clear();
//	this->registered_data_bin   .clear();
//	this->registered_data_head  .clear();
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
	if      (type == typeid( int8_t )) this->_write_body_text< int8_t >(file, buffer, size);
	else if (type == typeid(uint8_t )) this->_write_body_text<uint8_t >(file, buffer, size);
	else if (type == typeid( int16_t)) this->_write_body_text< int16_t>(file, buffer, size);
	else if (type == typeid(uint16_t)) this->_write_body_text<uint16_t>(file, buffer, size);
	else if (type == typeid( int32_t)) this->_write_body_text< int32_t>(file, buffer, size);
	else if (type == typeid(uint32_t)) this->_write_body_text<uint32_t>(file, buffer, size);
	else if (type == typeid( int64_t)) this->_write_body_text< int64_t>(file, buffer, size);
	else if (type == typeid(uint64_t)) this->_write_body_text<uint64_t>(file, buffer, size);
	else if (type == typeid(float   )) this->_write_body_text<float   >(file, buffer, size);
	else if (type == typeid(double  )) this->_write_body_text<double  >(file, buffer, size);
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
template void Dumper::register_data<int8_t  >(const int8_t*,   const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint8_t >(const uint8_t*,  const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int16_t >(const int16_t*,  const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint16_t>(const uint16_t*, const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int32_t >(const int32_t*,  const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint32_t>(const uint32_t*, const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int64_t >(const int64_t*,  const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint64_t>(const uint64_t*, const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<float   >(const float*,    const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<double  >(const double*,   const unsigned, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);

template void Dumper::register_data<int8_t,   std::allocator<int8_t  >>(const std::vector<int8_t,   std::allocator<int8_t  >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint8_t,  std::allocator<uint8_t >>(const std::vector<uint8_t,  std::allocator<uint8_t >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int16_t,  std::allocator<int16_t >>(const std::vector<int16_t,  std::allocator<int16_t >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint16_t, std::allocator<uint16_t>>(const std::vector<uint16_t, std::allocator<uint16_t>>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int32_t,  std::allocator<int32_t >>(const std::vector<int32_t,  std::allocator<int32_t >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint32_t, std::allocator<uint32_t>>(const std::vector<uint32_t, std::allocator<uint32_t>>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int64_t,  std::allocator<int64_t >>(const std::vector<int64_t,  std::allocator<int64_t >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint64_t, std::allocator<uint64_t>>(const std::vector<uint64_t, std::allocator<uint64_t>>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<float,    std::allocator<float   >>(const std::vector<float,    std::allocator<float   >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<double,   std::allocator<double  >>(const std::vector<double,   std::allocator<double  >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);

#include <mipp.h>
template void Dumper::register_data<int8_t,   mipp::allocator<int8_t  >>(const std::vector<int8_t,   mipp::allocator<int8_t  >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint8_t,  mipp::allocator<uint8_t >>(const std::vector<uint8_t,  mipp::allocator<uint8_t >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int16_t,  mipp::allocator<int16_t >>(const std::vector<int16_t,  mipp::allocator<int16_t >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint16_t, mipp::allocator<uint16_t>>(const std::vector<uint16_t, mipp::allocator<uint16_t>>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int32_t,  mipp::allocator<int32_t >>(const std::vector<int32_t,  mipp::allocator<int32_t >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint32_t, mipp::allocator<uint32_t>>(const std::vector<uint32_t, mipp::allocator<uint32_t>>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<int64_t,  mipp::allocator<int64_t >>(const std::vector<int64_t,  mipp::allocator<int64_t >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<uint64_t, mipp::allocator<uint64_t>>(const std::vector<uint64_t, mipp::allocator<uint64_t>>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<float,    mipp::allocator<float   >>(const std::vector<float,    mipp::allocator<float   >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);
template void Dumper::register_data<double,   mipp::allocator<double  >>(const std::vector<double,   mipp::allocator<double  >>&, const unsigned, const std::string&, const bool, const unsigned, std::vector<unsigned>);

template void Dumper::_write_body_text<int8_t >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<int16_t>(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<int32_t>(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<int64_t>(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<float  >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
template void Dumper::_write_body_text<double >(std::ofstream&, const std::vector<std::vector<char>>&, const unsigned);
// ==================================================================================== explicit template instantiation
