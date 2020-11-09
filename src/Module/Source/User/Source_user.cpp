#include <fstream>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Source/User/Source_user.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Source_user<B>
::Source_user(const int K, const std::string &filename, const int start_idx)
: Source<B>(K), source(), src_counter(start_idx)
{
	const std::string name = "Source_user";
	this->set_name(name);

	if (filename.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'filename' should not be empty.");

	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		int n_src = 0, src_size = 0;

		file >> n_src;
		file >> src_size;

		if (n_src <= 0 || src_size <= 0)
		{
			std::stringstream message;
			message << "'n_src', and 'src_size' have to be greater than 0 ('n_src' = " << n_src
			        << ", 'src_size' = " << src_size << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->source.resize(n_src);
		for (auto i = 0; i < n_src; i++)
			this->source[i].resize(src_size);

		if (src_size == this->K)
		{
			for (auto i = 0; i < n_src; i++)
				for (auto j = 0; j < src_size; j++)
				{
					int bit;
					file >> bit;

					this->source[i][j] = bit != 0;
				}
		}
		else
		{
			file.close();

			std::stringstream message;
			message << "The size is wrong (read: " << src_size << ", expected: " << this->K << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		file.close();
	}
	else
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Can't open '" + filename + "' file.");

	src_counter %= (int)source.size();
}

template <typename B>
Source_user<B>* Source_user<B>
::clone() const
{
	auto m = new Source_user(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Source_user<B>
::_generate(B *U_K, const size_t frame_id)
{
	std::copy(this->source[this->src_counter].begin(),
	          this->source[this->src_counter].end  (),
	          U_K);

	this->src_counter = (this->src_counter +1) % (int)this->source.size();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Source_user<B_8>;
template class aff3ct::module::Source_user<B_16>;
template class aff3ct::module::Source_user<B_32>;
template class aff3ct::module::Source_user<B_64>;
#else
template class aff3ct::module::Source_user<B>;
#endif
// ==================================================================================== explicit template instantiation
