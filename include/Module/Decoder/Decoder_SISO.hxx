#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{

template <typename R>
Decoder_SISO<R>
::Decoder_SISO(const int K, const int N, const int n_frames, const int simd_inter_frame_level)
: Decoder(K, N, n_frames, simd_inter_frame_level),
  Y_N1   (this->n_inter_frame_rest ? this->simd_inter_frame_level * this->N : 0),
  Y_N2   (this->n_inter_frame_rest ? this->simd_inter_frame_level * this->N : 0)
{
	const std::string name = "Decoder_SISO";
	this->set_name(name);

	auto &p = this->create_task("decode_siso", (int)dec::tsk::decode_siso);
	auto &ps_Y_N1 = this->template create_socket_in <R>(p, "Y_N1", this->N * this->n_frames);
	auto &ps_Y_N2 = this->template create_socket_out<R>(p, "Y_N2", this->N * this->n_frames);
	this->create_codelet(p, [this, &ps_Y_N1, &ps_Y_N2]() -> int
	{
		this->decode_siso(static_cast<R*>(ps_Y_N1.get_dataptr()),
		                  static_cast<R*>(ps_Y_N2.get_dataptr()));

		return 0;
	});
}

template <typename R>
template <class A>
void Decoder_SISO<R>
::decode_siso(const std::vector<R,A> &sys, const std::vector<R,A> &par, std::vector<R,A> &ext,
              const int n_frames, const int frame_id)
{
	if (n_frames != -1 && n_frames <= 0)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 or equal to -1 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;

	if ((this->K + this->tail_length() / 2) * real_n_frames != (int)sys.size())
	{
		std::stringstream message;
		message << "'sys.size()' has to be equal to ('K' + 'tail_length()' / 2) * 'real_n_frames' "
		        << "('sys.size()' = " << sys.size()
		        << ", 'K' = " << this->K
		        << ", 'tail_length()' = " << this->tail_length()
		        << ", 'real_n_frames' = " << real_n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (((this->N - this->K) - this->tail_length() / 2) * real_n_frames != (int)par.size())
	{
		std::stringstream message;
		message << "'par.size()' has to be equal to (('N' - 'K') - 'tail_length()' / 2) * "
		        << "'real_n_frames' ('par.size()' = " << par.size()
		        << ", 'N' = " << this->N
		        << ", 'K' = " << this->K
		        << ", 'tail_length()' = " << this->tail_length()
		        << ", 'real_n_frames' = " << real_n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->K * real_n_frames != (int)ext.size())
	{
		std::stringstream message;
		message << "'ext.size()' has to be equal to 'K' * 'real_n_frames' ('ext.size()' = " << ext.size()
		        << ", 'K' = " << this->K << ", 'real_n_frames' = " << real_n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= real_n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'real_n_frames' ('frame_id' = "
		        << frame_id << ", 'real_n_frames' = " << real_n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decode_siso(sys.data(), par.data(), ext.data(), real_n_frames, frame_id);
}

template <typename R>
void Decoder_SISO<R>
::decode_siso(const R *sys, const R *par, R *ext, const int n_frames, const int frame_id)
{
	const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;

	const auto n_dec_waves_siso = real_n_frames / this->simd_inter_frame_level;

	const auto w_start = (frame_id < 0) ? 0 : frame_id % n_dec_waves_siso;
	const auto w_stop  = (frame_id < 0) ? n_dec_waves_siso : w_start +1;

	for (auto w = w_start; w < w_stop; w++)
		this->_decode_siso(sys + w * (          this->K) * this->simd_inter_frame_level,
		                   par + w * (this->N - this->K) * this->simd_inter_frame_level,
		                   ext + w * (          this->K) * this->simd_inter_frame_level,
		                   w * this->simd_inter_frame_level);
}

template <typename R>
template <class A>
void Decoder_SISO<R>
::decode_siso(const std::vector<R,A> &Y_N1, std::vector<R,A> &Y_N2, const int frame_id)
{
	if (this->N * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decode_siso(Y_N1.data(), Y_N2.data(), frame_id);
}

template <typename R>
void Decoder_SISO<R>
::decode_siso(const R *Y_N1, R *Y_N2, const int frame_id)
{
	if (frame_id < 0 || this->simd_inter_frame_level == 1)
	{
		const auto w_start = (frame_id < 0) ? 0 : frame_id % this->n_dec_waves;
		const auto w_stop  = (frame_id < 0) ? this->n_dec_waves : w_start +1;

		auto w = 0;
		for (w = w_start; w < w_stop -1; w++)
			this->_decode_siso(Y_N1 + w * this->N * this->simd_inter_frame_level,
			                   Y_N2 + w * this->N * this->simd_inter_frame_level,
			                   w * this->simd_inter_frame_level);

		if (this->n_inter_frame_rest == 0)
			this->_decode_siso(Y_N1 + w * this->N * this->simd_inter_frame_level,
			                   Y_N2 + w * this->N * this->simd_inter_frame_level,
			                   w * this->simd_inter_frame_level);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level * this->N;
			std::copy(Y_N1 + waves_off1,
			          Y_N1 + waves_off1 + this->n_inter_frame_rest * this->N,
			          this->Y_N1.begin());

			this->_decode_siso(this->Y_N1.data(), this->Y_N2.data(), w * simd_inter_frame_level);

			const auto waves_off2 = w * this->simd_inter_frame_level * this->N;
			std::copy(this->Y_N2.begin(),
			          this->Y_N2.begin() + this->n_inter_frame_rest * this->N,
			          Y_N2 + waves_off2);
		}
	}
	else
	{
		const auto w = (frame_id % this->n_frames) / this->simd_inter_frame_level;
		const auto w_pos = frame_id % this->simd_inter_frame_level;

		std::fill(this->Y_N1.begin(), this->Y_N1.end(), (R)0);
		std::copy(Y_N1 + ((frame_id % this->n_frames) + 0) * this->N,
		          Y_N1 + ((frame_id % this->n_frames) + 1) * this->N,
		          this->Y_N1.begin() + w_pos * this->N);

		this->_decode_siso(this->Y_N1.data(), this->Y_N2.data(), w * this->simd_inter_frame_level);

		std::copy(this->Y_N2.begin() + (w_pos +0) * this->N,
		          this->Y_N2.begin() + (w_pos +1) * this->N,
		          Y_N2 + (frame_id % this->n_frames) * this->N);
	}
}

template <typename R>
int Decoder_SISO<R>
::tail_length() const
{
	return 0;
}

template <typename R>
void Decoder_SISO<R>
::_decode_siso(const R *sys, const R *par, R *ext, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename R>
void Decoder_SISO<R>
::_decode_siso(const R *Y_N1, R *Y_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
