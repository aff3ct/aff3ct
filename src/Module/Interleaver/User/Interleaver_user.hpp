#ifndef INTERLEAVER_USER_HPP
#define INTERLEAVER_USER_HPP

#include <stdexcept>
#include <fstream>

#include "Tools/Display/bash_tools.h"

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_user : public Interleaver<T>
{
private:
	const std::string filename;
	int call_counter;

public:
	Interleaver_user(int size, const std::string filename, const std::string name = "Interleaver_user")
	: Interleaver<T>(size, 1, name), filename(filename), call_counter(0) { gen_lookup_tables(); }

	void gen_lookup_tables()
	{
		if (call_counter)
		{
			std::clog << tools::bold_yellow("(WW) It is useless to call the generation of the lookup table multiple ")
			          << tools::bold_yellow("times on the USER interleaver.")
			          << std::endl;
		}
		call_counter++;

		std::ifstream file(filename.c_str(), std::ios::in);

		if (file.is_open())
		{
			T val;

			file >> val;
			if (val == (T)this->pi.size())
			{
				for (auto i = 0; i < (int)this->pi.size(); i++)
				{
					file >> val;
					if (val >= 0 && val < (T)this->pi.size())
					{
						auto found = false;
						auto j = 0;
						while(j < i && !found)
						{
							if (this->pi[j] == val)
								found = true;
							j++;
						}

						if (!found)
							this->pi[i] = (T)val;
						else
						{
							file.close();

							throw std::runtime_error("aff3ct::module::Interleaver_user: the interleaver value is "
							                         "wrong, it already exists elsewhere (read: " +
							                         std::to_string(val) + ").");
						}
					}
					else
					{
						file.close();

						throw std::runtime_error("aff3ct::module::Interleaver_user: the interleaver value is "
						                         "wrong (read: " + std::to_string(val) + ", expected: < " +
						                         std::to_string(this->pi.size()) + ").");
					}
				}

				for (auto i = 0; i < (int)this->pi_inv.size(); i++)
					this->pi_inv[this->pi[i]] = i;
			}
			else
			{
				file.close();

				throw std::runtime_error("aff3ct::module::Interleaver_user: the interleaver value is "
				                         "wrong (read: " + std::to_string(val) + ", expected: " +
				                         std::to_string(this->pi.size()) + ").");
			}

			file.close();
		}
		else
		{
			throw std::invalid_argument("aff3ct::module::Interleaver_user: can't open \"" + filename + "\" file.");
		}
	}
};
}
}

#endif	/* INTERLEAVER_USER_HPP */

