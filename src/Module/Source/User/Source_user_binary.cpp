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
		auto n_src   = (size * CHAR_BIT + K - 1) / K;

		this->source.resize(n_src);
		for (auto i = 0; i < n_src; i++)
			this->source[i].resize(K);

		std::vector<char> full_source_char(size           );
		std::vector<B>    full_source_B   (size * CHAR_BIT);
		file.seekg (0, std::ios::beg);
		file.read (full_source_char.data(), size);
		file.close();

		tools::Bit_packer::unpack(full_source_char, full_source_B);

		for (auto i = 0; i < n_src -1; i++)
			std::copy(full_source_B.begin() + i * K, full_source_B.begin() + (i +1) * K, this->source[i].begin());

		std::copy(full_source_B.begin() + (n_src - 1) * K, full_source_B.end(), this->source[n_src -1].begin());
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
