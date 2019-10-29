#include <fstream>
#include <sstream>

#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Source/User/Source_user_binary.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Source_user_binary<B>
::Source_user_binary(const int K, const std::string filename, const int n_frames)
: Source<B>(K, n_frames), source(), src_counter(0)
{
	const std::string name = "Source_user_binary";
	this->set_name(name);

	if (filename.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'filename' should not be empty.");

	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		auto size    = file.tellg();
		auto n_src   = size / K;
		std::vector<char> packed_vec(K);
		file.seekg (0, std::ios::beg);

		this->source.resize(n_src);
		for (auto i = 0; i < n_src; i++)
			this->source[i].resize(K);

		for (auto i = 0; i < n_src; i++)
		{
			this->source[i].resize(K);
			file.read (packed_vec.data(), K);
			tools::Bit_packer::unpack(packed_vec, this->source[i]);
		}
		std::fill(this->source[n_src - 1].begin() + (size % K), this->source[n_src - 1].end(), 0);

		for (auto i = 0; i < 32; i++)
			std::cout << this->source[0][i] << " | ";
		std::cout << std::endl;

		file.close();
	}
	else
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Can't open '" + filename + "' file.");
}

template <typename B>
void Source_user_binary<B>
::_generate(B *U_K, const int frame_id)
{
	std::copy(this->source[this->src_counter].begin(),
	          this->source[this->src_counter].end  (),
	          U_K);

	this->src_counter = (this->src_counter +1) % (int)this->source.size();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Source_user_binary<B_8>;
template class aff3ct::module::Source_user_binary<B_16>;
template class aff3ct::module::Source_user_binary<B_32>;
template class aff3ct::module::Source_user_binary<B_64>;
#else
template class aff3ct::module::Source_user_binary<B>;
#endif
// ==================================================================================== explicit template instantiation
