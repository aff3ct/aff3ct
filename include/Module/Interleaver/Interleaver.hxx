#include <string>
#include <sstream>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Module/Interleaver/Interleaver.hpp"

namespace aff3ct
{
namespace module
{

template <typename D, typename T>
Interleaver<D,T>::
Interleaver(const tools::Interleaver_core<T> &core)
: Module(core.get_n_frames()),
  core(core)
{
	const std::string name = "Interleaver_" + core.get_name();
	this->set_name(name);
	const std::string short_name = "Interleaver";
	this->set_short_name(short_name);


	auto &p1 = this->create_task("interleave");
	auto &p1s_nat = this->template create_socket_in <D>(p1, "nat", this->core.get_size() * this->n_frames);
	auto &p1s_itl = this->template create_socket_out<D>(p1, "itl", this->core.get_size() * this->n_frames);
	this->create_codelet(p1, [this, &p1s_nat, &p1s_itl]() -> int
	{
		this->interleave(static_cast<D*>(p1s_nat.get_dataptr()),
		                 static_cast<D*>(p1s_itl.get_dataptr()));

		return 0;
	});

	auto &p2 = this->create_task("deinterleave");
	auto &p2s_itl = this->template create_socket_in <D>(p2, "itl", this->core.get_size() * this->n_frames);
	auto &p2s_nat = this->template create_socket_out<D>(p2, "nat", this->core.get_size() * this->n_frames);
	this->create_codelet(p2, [this, &p2s_itl, &p2s_nat]() -> int
	{
		this->deinterleave(static_cast<D*>(p2s_itl.get_dataptr()),
		                   static_cast<D*>(p2s_nat.get_dataptr()));

		return 0;
	});
}

template <typename D, typename T>
const tools::Interleaver_core<T>& Interleaver<D,T>::
get_core() const
{
	return this->core;
}

template <typename D, typename T>
template <class A>
void Interleaver<D,T>::
interleave(const std::vector<D,A> &nat, std::vector<D,A> &itl, const int frame_id) const
{
	if (nat.size() != itl.size())
	{
		std::stringstream message;
		message << "'natural_vec.size()' has to be equal to 'interleaved_vec.size()' ('nat.size()' = "
		        << nat.size() << ", 'interleaved_vec.size()' = " << itl.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)nat.size() < this->core.get_size() * this->n_frames)
	{
		std::stringstream message;
		message << "'nat.size()' has to be equal or greater than 'get_size()' * 'n_frames' "
		        << "('nat.size()' = " << nat.size() << ", 'get_size()' = "
		        << this->core.get_size() << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->interleave(nat.data(), itl.data(), frame_id);
}

template <typename D, typename T>
void Interleaver<D,T>::
interleave(const D *nat, D *itl, const int frame_id) const
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->interleave(nat + f * this->core.get_size(),
		                 itl + f * this->core.get_size(),
		                 f, 1);
}

template <typename D, typename T>
void Interleaver<D,T>::
interleave(const D *nat, D *itl, const int frame_id, const int n_frames,
           const bool frame_reordering) const
{
	this->_interleave(nat, itl, core.get_lut(), frame_reordering, n_frames, frame_id);
}

template <typename D, typename T>
template <class A>
void Interleaver<D,T>::
deinterleave(const std::vector<D,A> &itl, std::vector<D,A> &nat, const int frame_id) const
{
	if (nat.size() != itl.size())
	{
		std::stringstream message;
		message << "'natural_vec.size()' has to be equal to 'interleaved_vec.size()' ('natural_vec.size()' = "
		        << nat.size() << ", 'interleaved_vec.size()' = " << itl.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)nat.size() < this->core.get_size() * this->n_frames)
	{
		std::stringstream message;
		message << "'natural_vec.size()' has to be equal or greater than 'get_size()' * 'n_frames' "
		        << "('natural_vec.size()' = " << nat.size() << ", 'get_size()' = "
		        << this->core.get_size() << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->deinterleave(itl.data(), nat.data(), frame_id);
}

template <typename D, typename T>
void Interleaver<D,T>::
deinterleave(const D *itl, D *nat, const int frame_id) const
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->deinterleave(itl + f * this->core.get_size(),
		                   nat + f * this->core.get_size(),
		                   f, 1);
}

template <typename D, typename T>
void Interleaver<D,T>::
deinterleave(const D *itl, D *nat, const int frame_id, const int n_frames,
                         const bool frame_reordering) const
{
	this->_interleave(itl, nat, core.get_lut_inv(), frame_reordering, n_frames, frame_id);
}

template <typename D, typename T>
void Interleaver<D,T>::
_interleave(const D *in_vec, D *out_vec,
            const std::vector<T> &lookup_table,
            const bool frame_reordering,
            const int  n_frames,
            const int  frame_id) const
{
	if (frame_reordering)
	{
		if (!this->core.is_uniform())
		{
			// vectorized interleaving
			if (n_frames == mipp::nElReg<D>())
			{
				for (auto i = 0; i < this->core.get_size(); i++)
					mipp::store<D>(&out_vec[i * mipp::nElReg<D>()],
					               mipp::load<D>(&in_vec[lookup_table[i] * mipp::nElReg<D>()]));
			}
			else
			{
				for (auto i = 0; i < this->core.get_size(); i++)
				{
					const auto off1 =              i  * n_frames;
					const auto off2 = lookup_table[i] * n_frames;
					for (auto f = 0; f < n_frames; f++)
						out_vec[off1 +f] = in_vec[off2 +f];
				}
			}
		}
		else
		{
			auto cur_frame_id = frame_id % this->n_frames;
			for (auto f = 0; f < n_frames; f++)
			{
				const auto lut = lookup_table.data() + cur_frame_id * this->core.get_size();
				for (auto i = 0; i < this->core.get_size(); i++)
					out_vec[i * n_frames +f] = in_vec[lut[i] * n_frames +f];
				cur_frame_id = (cur_frame_id +1) % this->n_frames;
			}
		}
	}
	else
	{
		if (!this->core.is_uniform())
		{
			// TODO: vectorize this code with the new AVX gather instruction
			for (auto f = 0; f < n_frames; f++)
			{
				const auto off = f * this->core.get_size();
				for (auto i = 0; i < this->core.get_size(); i++)
					out_vec[off + i] = in_vec[off + lookup_table[i]];
			}
		}
		else
		{
			auto cur_frame_id = frame_id % this->n_frames;
			// TODO: vectorize this code with the new AVX gather instruction
			for (auto f = 0; f < n_frames; f++)
			{
				const auto lut = lookup_table.data() + cur_frame_id * this->core.get_size();
				const auto off = f * this->core.get_size();
				for (auto i = 0; i < this->core.get_size(); i++)
					out_vec[off + i] = in_vec[off + lut[i]];
				cur_frame_id = (cur_frame_id +1) % this->n_frames;
			}
		}
	}
}

}
}

