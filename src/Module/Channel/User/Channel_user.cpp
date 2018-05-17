#include <fstream>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Channel_user.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_user<R>
::Channel_user(const int N, const std::string &filename, const bool add_users, const bool additive_noise,
               const int n_frames)
: Channel<R>(N, n_frames), add_users(add_users), additive_noise(additive_noise), noise_buff(), noise_counter(0)
{
	const std::string name = "Channel_user";
	this->set_name(name);

	if (filename.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'filename' should not be empty.");

	try
	{
		open_as_text(filename);
	}
	catch(const tools::runtime_error&)
	{
		open_as_binary(filename);
	}
}

template<typename R>
void Channel_user<R>::open_as_text(const std::string &filename)
{
	std::ifstream file(filename.c_str());

	if (file.is_open())
	{
		unsigned n_fra = 0;
		int fra_size = 0;

		file >> n_fra;
		file >> fra_size;

		if (n_fra == 0 || fra_size <= 0)
		{
			std::stringstream message;
			message << "'n_fra' and 'fra_size' have to be bigger than 0 ('n_fra' = "
			        << n_fra << ", 'fra_size' = " << fra_size << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (fra_size == this->N)
		{
			this->noise_buff.resize(n_fra);
			for (unsigned i = 0; i < (unsigned)n_fra; i++)
				this->noise_buff[i].resize(fra_size);

			for (unsigned i = 0; i < n_fra; i++)
				for (auto j = 0; j < fra_size; j++)
				{
					if (file.eof())
					{
						std::stringstream message;
						message << "Not enough data in the file (got " << i << " frames only).";
						throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
					}

					R value;
					file >> value;
					this->noise_buff[i][j] = value;
				}
		}
		else
		{
			file.close();

			std::stringstream message;
			message << "The frame size is wrong (read: " << fra_size << ", expected: " << this->N << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
	else
	{
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Can't open '" + filename + "' file");
	}
}

template<typename R>
void Channel_user<R>::open_as_binary(const std::string &filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);

	if (file.is_open())
	{
		unsigned n_fra = 0;
		int fra_size = 0;

		file.read((char*)&n_fra,    sizeof(n_fra));
		file.read((char*)&fra_size, sizeof(fra_size));

		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize length = file.gcount();
		file.clear(); // since ignore will have set eof.
		file.seekg(8, std::ios_base::beg);

		if (n_fra == 0 || fra_size <= 0)
		{
			std::stringstream message;
			message << "'n_fra' and 'fra_size' have to be bigger than 0 ('n_fra' = "
			        << n_fra << ", 'fra_size' = " << fra_size << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		const unsigned sizeof_float = (unsigned)length / (n_fra * fra_size);

		if (fra_size == this->N)
		{
			this->noise_buff.resize(n_fra);
			for (unsigned i = 0; i < (unsigned)n_fra; i++)
				this->noise_buff[i].resize(fra_size);

			if (sizeof_float == sizeof(R))
			{
				for (unsigned i = 0; i < n_fra; i++)
					file.read(reinterpret_cast<char*>(&this->noise_buff[i][0]), fra_size * sizeof(R));
			}
			else
			{
				if (sizeof_float == sizeof(double))
				{
					std::vector<double> tmp(fra_size);
					for (unsigned i = 0; i < n_fra; i++)
					{
						file.read(reinterpret_cast<char*>(tmp.data()), fra_size * sizeof(double));
						for (auto j = 0; j < fra_size; j++)
							this->noise_buff[i][j] = (R)tmp[j];
					}
				}
				else if (sizeof_float == sizeof(float))
				{
					std::vector<float> tmp(fra_size);
					for (unsigned i = 0; i < n_fra; i++)
					{
						file.read(reinterpret_cast<char*>(tmp.data()), fra_size * sizeof(float));
						for (auto j = 0; j < fra_size; j++)
							this->noise_buff[i][j] = (R)tmp[j];
					}
				}
				else
				{
					file.close();

					std::stringstream message;
					message << "Something went wrong ('sizeof_float' = " << sizeof_float << ").";
					throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
				}
			}
		}
		else
		{
			file.close();

			std::stringstream message;
			message << "The frame size is wrong (read: " << fra_size << ", expected: " << this->N << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		file.close();
	}
	else
	{
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Can't open '" + filename + "' file");
	}
}

template <typename R>
Channel_user<R>
::~Channel_user()
{
}

template <typename R>
void Channel_user<R>
::add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	if (additive_noise)
	{
		if (add_users && this->n_frames > 1)
		{
			if (frame_id != -1)
			{
				std::stringstream message;
				message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}

			std::fill(Y_N, Y_N + this->N, (R) 0);
			for (auto f = 0; f < this->n_frames; f++)
				for (auto i = 0; i < this->N; i++)
					Y_N[i] += X_N[f * this->N + i];

			std::copy(this->noise_buff[this->noise_counter].begin(),
			          this->noise_buff[this->noise_counter].end(),
			          this->noise.data());

			this->noise_counter = (this->noise_counter +1) % (int)this->noise_buff.size();

			for (auto i = 0; i < this->N; i++)
				Y_N[i] += this->noise[i];
		}
		else
		{
			const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
			const auto f_stop = (frame_id < 0) ? this->n_frames : f_start + 1;

			for (auto f = f_start; f < f_stop; f++)
			{
				std::copy(this->noise_buff[this->noise_counter].begin(),
				          this->noise_buff[this->noise_counter].end(),
				          this->noise.data() + f * this->N);

				for (auto i = 0; i < this->N; i++)
					Y_N[f * this->N + i] = X_N[f * this->N + i] + this->noise[f * this->N + i];

				this->noise_counter = (this->noise_counter + 1) % (int) this->noise_buff.size();
			}
		}
	}
	else
	{
		if (add_users && this->n_frames > 1)
		{
			if (frame_id != -1)
			{
				std::stringstream message;
				message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}

			std::copy(this->noise_buff[this->noise_counter].begin(),
			          this->noise_buff[this->noise_counter].end(),
			          this->noise.data());

			std::copy(this->noise_buff[this->noise_counter].begin(),
			          this->noise_buff[this->noise_counter].end(),
			          Y_N);

			this->noise_counter = (this->noise_counter +1) % (int)this->noise_buff.size();

		}
		else
		{
			const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
			const auto f_stop = (frame_id < 0) ? this->n_frames : f_start + 1;

			for (auto f = f_start; f < f_stop; f++)
			{
				std::copy(this->noise_buff[this->noise_counter].begin(),
				          this->noise_buff[this->noise_counter].end(),
				          this->noise.data() + f * this->N);

				std::copy(this->noise_buff[this->noise_counter].begin(),
				          this->noise_buff[this->noise_counter].end(),
				          Y_N + f * this->N);

				this->noise_counter = (this->noise_counter + 1) % (int) this->noise_buff.size();
			}
		}
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_user<R_32>;
template class aff3ct::module::Channel_user<R_64>;
#else
template class aff3ct::module::Channel_user<R>;
#endif
// ==================================================================================== explicit template instantiation
