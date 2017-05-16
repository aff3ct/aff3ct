#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "Frame_dumper_reduction.hpp"

using namespace aff3ct::tools;

template <typename B, typename R>
Frame_dumper_reduction<B,R>
::Frame_dumper_reduction(std::vector<Frame_dumper<B,R>*> dumpers,
                         const int K_info, const int K, const int N, const int N_mod, const int n_frames)
: Frame_dumper<B,R>(K_info, K, N, N_mod, n_frames), dumpers(dumpers)
{
	this->checks();
}

template <typename B, typename R>
Frame_dumper_reduction<B,R>
::Frame_dumper_reduction(std::vector<Frame_dumper<B,R>*> dumpers,
                         const int K_info, const int K, const int N, const int n_frames)
: Frame_dumper<B,R>(K_info, K, N, n_frames), dumpers(dumpers)
{
	this->checks();
}

template <typename B, typename R>
Frame_dumper_reduction<B,R>
::Frame_dumper_reduction(std::vector<Frame_dumper<B,R>*> dumpers,
                         const int K, const int N, const int n_frames)
: Frame_dumper<B,R>(K, N, n_frames), dumpers(dumpers)
{
	this->checks();
}

template <typename B, typename R>
Frame_dumper_reduction<B,R>
::Frame_dumper_reduction(std::vector<Frame_dumper<B,R>*> dumpers,
                         const int K_info, const int n_frames)
: Frame_dumper<B,R>(K_info, n_frames), dumpers(dumpers)
{
	this->checks();
}

template <typename B, typename R>
void Frame_dumper_reduction<B,R>
::checks()
{
	for (auto d : dumpers)
	{
		if (d == nullptr)
			throw std::runtime_error("aff3ct::tools::Frame_dumper_reduction: \"dumper\" should not but NULL.");
		if (d->get_K_info() != this->K_info)
			throw std::invalid_argument("aff3ct::tools::Frame_dumper_reduction: \"K_info\" is invalid.");
		if (d->get_K() != this->K)
			throw std::invalid_argument("aff3ct::tools::Frame_dumper_reduction: \"K\" is invalid.");
		if (d->get_N() != this->N)
			throw std::invalid_argument("aff3ct::tools::Frame_dumper_reduction: \"N\" is invalid.");
		if (d->get_N_mod() != this->N_mod)
			throw std::invalid_argument("aff3ct::tools::Frame_dumper_reduction: \"N_mod\" is invalid.");
		if (d->get_n_frames() != this->n_frames)
			throw std::invalid_argument("aff3ct::tools::Frame_dumper_reduction: \"n_frames\" is invalid.");
	}
}

template <typename B, typename R>
Frame_dumper_reduction<B,R>
::~Frame_dumper_reduction()
{
}

template <typename B, typename R>
void Frame_dumper_reduction<B,R>
::dump(const std::string& base_path)
{
	auto n_fe = (unsigned long long)this->buff_src.size();
	for (auto d : dumpers)
	{
		auto buff = d->get_buff_src();
		n_fe += buff.size();
	}

	bool is_src = this->buff_src.size();
	if (!is_src)
		for (auto d : dumpers)
		{
			auto buff = d->get_buff_src();
			if (buff.size())
			{
				is_src = true;
				break;
			}
		}
	if (is_src)
	{
		const std::string path_src = base_path + ".src";
		std::ofstream file_src(path_src);
		file_src << n_fe         << std::endl << std::endl;
		file_src << this->K_info << std::endl << std::endl;
		this->write_text_body(file_src, this->buff_src);
		for (auto d : dumpers)
			this->write_text_body(file_src, d->get_buff_src());
		file_src.close();
	}

	bool is_enc = this->buff_enc.size();
	if (!is_enc)
		for (auto d : dumpers)
		{
			auto buff = d->get_buff_enc();
			if (buff.size())
			{
				is_enc = true;
				break;
			}
		}
	if (is_enc)
	{
		const std::string path_enc = base_path + ".enc";
		std::ofstream file_enc(path_enc);
		file_enc << n_fe    << std::endl << std::endl;
		file_enc << this->K << std::endl << std::endl;
		file_enc << this->N << std::endl << std::endl;
		this->write_text_body(file_enc, this->buff_enc);
		for (auto d : dumpers)
			this->write_text_body(file_enc, d->get_buff_enc());
		file_enc.close();
	}

	bool is_chn = this->buff_chn.size();
	if (!is_chn)
		for (auto d : dumpers)
		{
			auto buff = d->get_buff_chn();
			if (buff.size())
			{
				is_chn = true;
				break;
			}
		}
	if (is_chn)
	{
		const std::string path_chn = base_path + ".chn";
		std::ofstream file_chn(path_chn, std::ios_base::binary);
		file_chn.write((char*)&n_fe,        sizeof(n_fe       ));
		file_chn.write((char*)&this->N_mod, sizeof(this->N_mod));
		this->write_binary_body(file_chn, this->buff_chn);
		for (auto d : dumpers)
			this->write_binary_body(file_chn, d->get_buff_chn());
		file_chn.close();
	}

	bool is_itl = this->buff_itl.size();
	if (!is_itl)
		for (auto d : dumpers)
		{
			auto buff = d->get_buff_itl();
			if (buff.size())
			{
				is_itl = true;
				break;
			}
		}
	if (is_itl)
	{
		const std::string path_itl = base_path + ".itl";
		std::ofstream file_itl(path_itl);
		file_itl << n_fe    << std::endl << std::endl;
		file_itl << this->N << std::endl << std::endl;
		this->write_text_body_itl(file_itl, this->buff_itl);
		for (auto d : dumpers)
			this->write_text_body_itl(file_itl, d->get_buff_itl());
		file_itl.close();
	}
}

template <typename B, typename R>
void Frame_dumper_reduction<B,R>
::clear()
{
	Frame_dumper<B,R>::clear();
	for (auto d : dumpers)
		if (d != nullptr)
			d->clear();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Frame_dumper_reduction<B_8 ,R_8>;
template class aff3ct::tools::Frame_dumper_reduction<B_16,R_16>;
template class aff3ct::tools::Frame_dumper_reduction<B_32,R_32>;
template class aff3ct::tools::Frame_dumper_reduction<B_64,R_64>;
#else
template class aff3ct::tools::Frame_dumper_reduction<B,R>;
#endif
// ==================================================================================== explicit template instantiation
