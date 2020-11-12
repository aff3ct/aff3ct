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
Task& Interleaver<D,T>
::operator[](const itl::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename D, typename T>
Socket& Interleaver<D,T>
::operator[](const itl::sck::interleave s)
{
	return Module::operator[]((size_t)itl::tsk::interleave)[(size_t)s];
}

template <typename D, typename T>
Socket& Interleaver<D,T>
::operator[](const itl::sck::interleave_reordering s)
{
	return Module::operator[]((size_t)itl::tsk::interleave_reordering)[(size_t)s];
}

template <typename D, typename T>
Socket& Interleaver<D,T>
::operator[](const itl::sck::deinterleave s)
{
	return Module::operator[]((size_t)itl::tsk::deinterleave)[(size_t)s];
}

template <typename D, typename T>
Socket& Interleaver<D,T>
::operator[](const itl::sck::deinterleave_reordering s)
{
	return Module::operator[]((size_t)itl::tsk::deinterleave_reordering)[(size_t)s];
}

template <typename D, typename T>
Interleaver<D,T>
::Interleaver(const tools::Interleaver_core<T> &core)
: Module(),
  core(core)
{
	const std::string name = "Interleaver_" + core.get_name();
	this->set_name(name);
	const std::string short_name = "Interleaver";
	this->set_short_name(short_name);

	auto &p1 = this->create_task("interleave");
	auto p1s_nat = this->template create_socket_in <D>(p1, "nat", this->core.get_size());
	auto p1s_itl = this->template create_socket_out<D>(p1, "itl", this->core.get_size());
	this->create_codelet(p1, [p1s_nat, p1s_itl](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &itl = static_cast<Interleaver<D,T>&>(m);

		itl._interleave(static_cast<D*>(t[p1s_nat].get_dataptr()),
		                static_cast<D*>(t[p1s_itl].get_dataptr()),
		                itl.core.get_lut(),
		                frame_id);

		return status_t::SUCCESS;
	});

	auto &p2 = this->create_task("interleave_reordering");
	auto p2s_nat = this->template create_socket_in <D>(p2, "nat", this->core.get_size());
	auto p2s_itl = this->template create_socket_out<D>(p2, "itl", this->core.get_size());
	this->create_codelet(p2, [p2s_nat, p2s_itl](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &itl = static_cast<Interleaver<D,T>&>(m);

		itl._interleave_reordering(static_cast<D*>(t[p2s_nat].get_dataptr()),
		                           static_cast<D*>(t[p2s_itl].get_dataptr()),
		                           itl.core.get_lut(),
		                           frame_id);

		return status_t::SUCCESS;
	});

	auto &p3 = this->create_task("deinterleave");
	auto p3s_itl = this->template create_socket_in <D>(p3, "itl", this->core.get_size());
	auto p3s_nat = this->template create_socket_out<D>(p3, "nat", this->core.get_size());
	this->create_codelet(p3, [p3s_itl, p3s_nat](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &itl = static_cast<Interleaver<D,T>&>(m);

		itl._interleave(static_cast<D*>(t[p3s_itl].get_dataptr()),
		                static_cast<D*>(t[p3s_nat].get_dataptr()),
		                itl.core.get_lut_inv(),
		                frame_id);

		return status_t::SUCCESS;
	});

	auto &p4 = this->create_task("deinterleave_reordering");
	auto p4s_itl = this->template create_socket_in <D>(p4, "itl", this->core.get_size());
	auto p4s_nat = this->template create_socket_out<D>(p4, "nat", this->core.get_size());
	this->create_codelet(p4, [p4s_itl, p4s_nat](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &itl = static_cast<Interleaver<D,T>&>(m);

		itl._interleave_reordering(static_cast<D*>(t[p4s_itl].get_dataptr()),
		                           static_cast<D*>(t[p4s_nat].get_dataptr()),
		                           itl.core.get_lut_inv(),
		                           frame_id);

		return status_t::SUCCESS;
	});

	this->set_n_frames(core.get_n_frames());
}

template <typename D, typename T>
Interleaver<D,T>* Interleaver<D,T>
::clone() const
{
	auto m = new Interleaver(*this);
	m->deep_copy(*this);
	return m;
}

template <typename D, typename T>
const tools::Interleaver_core<T>& Interleaver<D,T>
::get_core() const
{
	return this->core;
}

template <typename D, typename T>
template <class A>
void Interleaver<D,T>
::interleave(const std::vector<D,A> &nat, std::vector<D,A> &itl, const int frame_id, const bool managed_memory)
{
	(*this)[itl::sck::interleave::nat].bind(nat);
	(*this)[itl::sck::interleave::itl].bind(itl);
	(*this)[itl::tsk::interleave].exec(frame_id, managed_memory);
}

template <typename D, typename T>
void Interleaver<D,T>
::interleave(const D *nat, D *itl, const int frame_id, const bool managed_memory)
{
	(*this)[itl::sck::interleave::nat].bind(nat);
	(*this)[itl::sck::interleave::itl].bind(itl);
	(*this)[itl::tsk::interleave].exec(frame_id, managed_memory);
}

template <typename D, typename T>
template <class A>
void Interleaver<D,T>
::interleave_reordering(const std::vector<D,A> &nat, std::vector<D,A> &itl, const int frame_id, const bool managed_memory)
{
	(*this)[itl::sck::interleave_reordering::nat].bind(nat);
	(*this)[itl::sck::interleave_reordering::itl].bind(itl);
	(*this)[itl::tsk::interleave_reordering].exec(frame_id, managed_memory);
}

template <typename D, typename T>
void Interleaver<D,T>
::interleave_reordering(const D *nat, D *itl, const int frame_id, const bool managed_memory)
{
	(*this)[itl::sck::interleave_reordering::nat].bind(nat);
	(*this)[itl::sck::interleave_reordering::itl].bind(itl);
	(*this)[itl::tsk::interleave_reordering].exec(frame_id, managed_memory);
}

template <typename D, typename T>
template <class A>
void Interleaver<D,T>
::deinterleave(const std::vector<D,A> &itl, std::vector<D,A> &nat, const int frame_id, const bool managed_memory)
{
	(*this)[itl::sck::deinterleave::itl].bind(itl);
	(*this)[itl::sck::deinterleave::nat].bind(nat);
	(*this)[itl::tsk::deinterleave].exec(frame_id, managed_memory);
}

template <typename D, typename T>
void Interleaver<D,T>
::deinterleave(const D *itl, D *nat, const int frame_id, const bool managed_memory)
{
	(*this)[itl::sck::deinterleave::itl].bind(itl);
	(*this)[itl::sck::deinterleave::nat].bind(nat);
	(*this)[itl::tsk::deinterleave].exec(frame_id, managed_memory);
}

template <typename D, typename T>
template <class A>
void Interleaver<D,T>
::deinterleave_reordering(const std::vector<D,A> &itl, std::vector<D,A> &nat, const int frame_id, const bool managed_memory)
{
	(*this)[itl::sck::deinterleave_reordering::itl].bind(itl);
	(*this)[itl::sck::deinterleave_reordering::nat].bind(nat);
	(*this)[itl::tsk::deinterleave_reordering].exec(frame_id, managed_memory);
}

template <typename D, typename T>
void Interleaver<D,T>
::deinterleave_reordering(const D *itl, D *nat, const int frame_id, const bool managed_memory)
{
	(*this)[itl::sck::deinterleave_reordering::itl].bind(itl);
	(*this)[itl::sck::deinterleave_reordering::nat].bind(nat);
	(*this)[itl::tsk::deinterleave_reordering].exec(frame_id, managed_memory);
}

template <typename D, typename T>
void Interleaver<D,T>
::_interleave(const D *in_vec, D *out_vec,
              const std::vector<T> &lookup_table,
              const size_t frame_id) const
{
	if (!this->core.is_uniform())
	{
		// TODO: vectorize this code with the new AVX gather instruction
		for (size_t f = 0; f < this->get_n_frames_per_wave(); f++)
		{
			const auto off = f * this->core.get_size();
			for (auto i = 0; i < this->core.get_size(); i++)
				out_vec[off + i] = in_vec[off + lookup_table[i]];
		}
	}
	else
	{
		auto cur_frame_id = (frame_id % this->get_n_frames()) % this->core.get_n_frames();
		// TODO: vectorize this code with the new AVX gather instruction
		for (size_t f = 0; f < this->get_n_frames_per_wave(); f++)
		{
			const auto lut = lookup_table.data() + cur_frame_id * this->core.get_size();
			const auto off = f * this->core.get_size();
			for (auto i = 0; i < this->core.get_size(); i++)
				out_vec[off + i] = in_vec[off + lut[i]];
			cur_frame_id = ((cur_frame_id +1) % this->get_n_frames()) % this->core.get_n_frames();
		}
	}
}

template <typename D, typename T>
void Interleaver<D,T>
::_interleave_reordering(const D *in_vec, D *out_vec,
                         const std::vector<T> &lookup_table,
                         const size_t frame_id) const
{
	if (!this->core.is_uniform())
	{
		// vectorized interleaving
		if (this->get_n_frames_per_wave() == (size_t)mipp::nElReg<D>())
		{
			for (auto i = 0; i < this->core.get_size(); i++)
				mipp::store<D>(&out_vec[i * mipp::nElReg<D>()],
				               mipp::load<D>(&in_vec[lookup_table[i] * mipp::nElReg<D>()]));
		}
		else
		{
			for (auto i = 0; i < this->core.get_size(); i++)
			{
				const auto off1 =              i  * this->get_n_frames();
				const auto off2 = lookup_table[i] * this->get_n_frames();
				for (size_t f = 0; f < this->get_n_frames_per_wave(); f++)
					out_vec[off1 +f] = in_vec[off2 +f];
			}
		}
	}
	else
	{
		auto cur_frame_id = (frame_id % this->get_n_frames()) % this->core.get_n_frames();
		for (size_t f = 0; f < this->get_n_frames_per_wave(); f++)
		{
			const auto lut = lookup_table.data() + cur_frame_id * this->core.get_size();
			for (auto i = 0; i < this->core.get_size(); i++)
				out_vec[i * this->get_n_frames_per_wave() +f] = in_vec[lut[i] * this->get_n_frames() +f];
			cur_frame_id = ((cur_frame_id +1) % this->get_n_frames()) % this->core.get_n_frames();
		}
	}
}

template <typename D, typename T>
void Interleaver<D,T>
::set_n_frames_per_wave(const size_t n_frames_per_wave)
{
	const auto old_n_frames_per_wave = this->get_n_frames_per_wave();
	if (old_n_frames_per_wave != n_frames_per_wave)
		Module::set_n_frames_per_wave(n_frames_per_wave);
}

}
}
