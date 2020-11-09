#include <sstream>
#include <fstream>
#include <ios>

#include "Tools/Exception/exception.hpp"

#include "Tools/Display/Dumper/Dumper_reduction.hpp"

using namespace aff3ct::tools;

Dumper_reduction
::Dumper_reduction(std::vector<std::unique_ptr<Dumper>> &dumpers)
: Dumper(), dumpers(dumpers)
{
	this->checks();
}

void Dumper_reduction
::checks()
{
	if (dumpers.size())
	{
		const auto n_buff_ref = dumpers[0]->buffer.size();

		for (auto i = 1; i < (int)dumpers.size(); i++)
		{
			const auto n_buff_cur = dumpers[i]->buffer.size();

			if (n_buff_cur != n_buff_ref)
			{
				std::stringstream message;
				message << "'n_buff_cur' should be equal to 'n_buff_ref' ('n_buff_cur' = " << n_buff_cur
				        << ", 'n_buff_ref' = " << n_buff_ref << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}

		for (unsigned i = 0; i < n_buff_ref; i++)
		{
			const auto size_ref   = dumpers[0]->registered_data_size    [i];
			const auto sizeof_ref = dumpers[0]->registered_data_sizeof  [i];
			const auto type_ref   = dumpers[0]->registered_data_type    [i];
			const auto ext_ref    = dumpers[0]->registered_data_ext     [i];
			const auto bin_ref    = dumpers[0]->registered_data_bin     [i];
			const auto head_ref   = dumpers[0]->registered_data_head    [i];
			const auto fra_ref    = dumpers[0]->registered_data_n_frames[i];

			for (auto j = 1; j < (int)dumpers.size(); j++)
			{
				const auto size_cur   = dumpers[j]->registered_data_size    [i];
				const auto sizeof_cur = dumpers[j]->registered_data_sizeof  [i];
				const auto type_cur   = dumpers[j]->registered_data_type    [i];
				const auto ext_cur    = dumpers[j]->registered_data_ext     [i];
				const auto bin_cur    = dumpers[j]->registered_data_bin     [i];
				const auto head_cur   = dumpers[j]->registered_data_head    [i];
				const auto fra_cur    = dumpers[j]->registered_data_n_frames[i];

				if (size_cur != size_ref)
				{
					std::stringstream message;
					message << "'size_cur' should be equal to 'size_ref' ('size_cur' = " << size_cur
					        << ", 'size_ref' = " << size_ref << ").";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}

				if (sizeof_cur != sizeof_ref)
				{
					std::stringstream message;
					message << "'sizeof_cur' should be equal to 'sizeof_ref' ('sizeof_cur' = " << sizeof_cur
					        << ", 'sizeof_ref' = " << sizeof_ref << ").";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}

				if (type_cur != type_ref)
				{
					std::stringstream message;
					message << "'type_cur' should be equal to 'type_ref' ('type_cur' = " << type_cur.name()
					        << ", 'type_ref' = " << type_ref.name()  << ").";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}

				if (ext_cur != ext_ref)
				{
					std::stringstream message;
					message << "'ext_cur' should be equal to 'ext_ref' ('ext_cur' = " << ext_cur
					        << ", 'ext_ref' = " << ext_ref << ").";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}

				if (bin_cur != bin_ref)
				{
					std::stringstream message;
					message << "'bin_cur' should be equal to 'bin_ref' ('bin_cur' = " << bin_cur
					        << ", 'bin_ref' = " << bin_ref << ").";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}

				if (head_cur != head_ref)
				{
					std::stringstream message;
					message << "'head_cur' should be equal to 'head_ref'.";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}

				if (fra_cur != fra_ref)
				{
					std::stringstream message;
					message << "'fra_cur' should be equal to 'fra_ref' ('fra_cur' = " << fra_cur
					        << ", 'fra_ref' = " << fra_ref << ").";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}
			}
		}
	}
	else
	{
		std::stringstream message;
		message << "'dumpers.size()' should be greater than 0 ('dumpers.size()' = " << dumpers.size() << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

void Dumper_reduction
::add(const size_t frame_id)
{
	throw invalid_argument(__FILE__, __LINE__, __func__, "This method can't be called on this class.");
}

void Dumper_reduction
::dump(const std::string& base_path)
{
	this->checks();

	if (base_path.empty())
		throw invalid_argument(__FILE__, __LINE__, __func__, "'base_path' can't be empty.");

	this->buffer             .resize(dumpers[0]->buffer.size());
	this->registered_data_ptr.resize(dumpers[0]->registered_data_ptr.size());

	this->registered_data_size   = dumpers[0]->registered_data_size;
	this->registered_data_sizeof = dumpers[0]->registered_data_sizeof;
	this->registered_data_type   = dumpers[0]->registered_data_type;
	this->registered_data_ext    = dumpers[0]->registered_data_ext;
	this->registered_data_bin    = dumpers[0]->registered_data_bin;
	this->registered_data_head   = dumpers[0]->registered_data_head;

	for (auto i = 0; i < (int)this->registered_data_ptr.size(); i++)
	{
		unsigned n_data = (unsigned)this->buffer[i].size();
		for (auto& d : this->dumpers)
			n_data += (unsigned)d->buffer[i].size();

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
				this->write_header_binary(file, n_data, size, head);
			this->write_body_binary(file, this->buffer[i], size * size_of);
			for (auto& d : this->dumpers)
				this->write_body_binary(file, d->buffer[i], size * size_of);
		}
		else
		{
			file.open(path, std::ofstream::out);

			if (this->write_headers)
				this->write_header_text(file, n_data, size, head);
			this->write_body_text(file, this->buffer[i], size, type);
			for (auto& d : this->dumpers)
				this->write_body_text(file, d->buffer[i], size, type);
		}

		file.close();
	}
}

void Dumper_reduction
::clear()
{
	Dumper::clear();
	for (auto& d : this->dumpers)
		if (d != nullptr)
			d->clear();
}
