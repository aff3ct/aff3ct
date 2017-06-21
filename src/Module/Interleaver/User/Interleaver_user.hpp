#ifndef INTERLEAVER_USER_HPP
#define INTERLEAVER_USER_HPP

#include <fstream>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_user : public Interleaver<T>
{
private:
	std::vector<mipp::vector<T>> pi_buffer;
	int cur_itl_id;

public:
	Interleaver_user(int size, const std::string filename, const int n_frames = 1,
	                 const std::string name = "Interleaver_user")
	: Interleaver<T>(size, false, n_frames, name), cur_itl_id(0)
	{
		if (filename.empty())
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'filename' should not be empty.");

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
									throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
								}
							}
							else
							{
								file.close();

								std::stringstream message;
								message << "The interleaver value is wrong (read: " << val
								        << ", expected: < " << this->get_size() << ").";
								throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
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
					throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
				}

				file.close();
			}
			else
			{
				file.close();

				std::stringstream message;
				message << "'n_itl' should be greater than 0 ('n_itl' = " << n_itl << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}
		else
		{
			std::stringstream message;
			message << "Can't open '" + filename + "' file.";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	virtual ~Interleaver_user()
	{
	}

protected:
	void gen_lut(T *lut, const int frame_id)
	{
		std::copy(this->pi_buffer[cur_itl_id].begin(), this->pi_buffer[cur_itl_id].end(), lut);
		cur_itl_id = (cur_itl_id +1) % pi_buffer.size();
	}
};
}
}

#endif	/* INTERLEAVER_USER_HPP */

