#include <algorithm>
#include <fstream>
#include <sstream>
#include <ios>

#include "Tools/Exception/exception.hpp"

#include "Tools/Interleaver/User/Interleaver_core_user.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_user<T>
::Interleaver_core_user(const int size, const std::string &filename)
: Interleaver_core<T>(size, "user", false), cur_itl_id(0)
{
	if (filename.empty())
		throw invalid_argument(__FILE__, __LINE__, __func__, "'filename' should not be empty.");

	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		int n_itl;
		file >> n_itl;

		if (n_itl > 0)
		{
			if (n_itl > 1)
				this->uniform = true;

			pi_buffer.resize(n_itl);
			for (auto i = 0; i < n_itl; i++)
				pi_buffer[i].resize(this->get_size());

			T val;
			file >> val;
			if (val == (T)this->get_size())
			{
				for (auto itl = 0; itl < n_itl; itl++)
				{
					for (auto i = 0; i < (int)this->get_size(); i++)
					{
						file >> val;
						if (val >= 0 && val < (T)this->get_size())
						{
							auto found = false;
							auto j = 0;
							while(j < i && !found)
							{
								if (this->pi_buffer[itl][j] == val)
									found = true;
								j++;
							}

							if (!found)
								this->pi_buffer[itl][i] = (T)val;
							else
							{
								file.close();

								std::stringstream message;
								message << "The interleaver value is wrong, it already exists elsewhere (read: "
								        << val << ").";
								throw runtime_error(__FILE__, __LINE__, __func__, message.str());
							}
						}
						else
						{
							file.close();

							std::stringstream message;
							message << "The interleaver value is wrong (read: " << val
							        << ", expected: < " << this->get_size() << ").";
							throw runtime_error(__FILE__, __LINE__, __func__, message.str());
						}
					}
				}
			}
			else
			{
				file.close();

				std::stringstream message;
				message << "The interleaver value is wrong (read: " << val << ", expected: "
				        << this->get_size() << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			file.close();
		}
		else
		{
			file.close();

			std::stringstream message;
			message << "'n_itl' should be greater than 0 ('n_itl' = " << n_itl << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
	else
	{
		std::stringstream message;
		message << "Can't open '" + filename + "' file.";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->init();
}

template <typename T>
Interleaver_core_user<T>* Interleaver_core_user<T>
::clone() const
{
	auto t = new Interleaver_core_user(*this);
	return t;
}

template <typename T>
void Interleaver_core_user<T>
::gen_lut(T *lut, const size_t frame_id)
{
	std::copy(this->pi_buffer[cur_itl_id].begin(), this->pi_buffer[cur_itl_id].end(), lut);
	cur_itl_id = (cur_itl_id +1) % pi_buffer.size();
}

template <typename T>
void Interleaver_core_user<T>
::reinitialize()
{
	this->cur_itl_id = 0;
	this->init();
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_user<uint8_t >;
template class aff3ct::tools::Interleaver_core_user<uint16_t>;
template class aff3ct::tools::Interleaver_core_user<uint32_t>;
template class aff3ct::tools::Interleaver_core_user<uint64_t>;
// ==================================================================================== explicit template instantiation

