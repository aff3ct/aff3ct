#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "Frame_dumper.hpp"

using namespace aff3ct::tools;

template <typename B, typename R>
Frame_dumper<B,R>
::Frame_dumper(const int K_info, const int K, const int N, const int N_mod, const int n_frames)
: U_K(nullptr), X_N(nullptr), Y_N(nullptr), I_N(nullptr), K_info(K_info), K(K), N(N), N_mod(N_mod), n_frames(n_frames)
{
	this->checks();
}

template <typename B, typename R>
Frame_dumper<B,R>
::Frame_dumper(const int K_info, const int K, const int N, const int n_frames)
: U_K(nullptr), X_N(nullptr), Y_N(nullptr), I_N(nullptr), K_info(K_info), K(K), N(N), N_mod(N), n_frames(n_frames)
{
	this->checks();
}

template <typename B, typename R>
Frame_dumper<B,R>
::Frame_dumper(const int K, const int N, const int n_frames)
: U_K(nullptr), X_N(nullptr), Y_N(nullptr), I_N(nullptr), K_info(K), K(K), N(N), N_mod(N), n_frames(n_frames)
{
	this->checks();
}

template <typename B, typename R>
Frame_dumper<B,R>
::Frame_dumper(const int K_info, const int n_frames)
: U_K(nullptr), X_N(nullptr), Y_N(nullptr), I_N(nullptr), K_info(K_info), K(K_info), N(K_info), N_mod(K_info),
  n_frames(n_frames)
{
	this->checks();
}

template <typename B, typename R>
void Frame_dumper<B,R>
::checks()
{
	if (K_info <= 0)
		throw std::invalid_argument("aff3ct::tools::Frame_dumper: \"K_info\" has to be greater than 0.");
	if (K < K_info)
		throw std::invalid_argument("aff3ct::tools::Frame_dumper: \"K\" has to be greater or equal to \"K_info\".");
	if (N < K)
		throw std::invalid_argument("aff3ct::tools::Frame_dumper: \"N\" has to be greater or equal to \"K\".");
	if (N_mod <= 0)
		throw std::invalid_argument("aff3ct::tools::Frame_dumper: \"N_mod\" has to be greater than 0.");
	if (n_frames <= 0)
		throw std::invalid_argument("aff3ct::tools::Frame_dumper: \"n_frames\" has to be greater than 0.");
}

template <typename B, typename R>
Frame_dumper<B,R>
::~Frame_dumper()
{
}

template <typename B, typename R>
void Frame_dumper<B,R>
::set_U_K(const B *U_K)
{
	this->U_K = U_K;
}

template <typename B, typename R>
void Frame_dumper<B,R>
::set_X_N(const B *X_N)
{
	this->X_N = X_N;
}

template <typename B, typename R>
void Frame_dumper<B,R>
::set_Y_N(const R *Y_N)
{
	this->Y_N = Y_N;
}

template <typename B, typename R>
void Frame_dumper<B,R>
::set_I_N(const int *I_N)
{
	this->I_N = I_N;
}

template <typename B, typename R>
int Frame_dumper<B,R>
::get_K_info()
{
	return K_info;
}

template <typename B, typename R>
int Frame_dumper<B,R>
::get_K()
{
	return K;
}

template <typename B, typename R>
int Frame_dumper<B,R>
::get_N()
{
	return N;
}

template <typename B, typename R>
int Frame_dumper<B,R>
::get_N_mod()
{
	return N_mod;
}

template <typename B, typename R>
int Frame_dumper<B,R>
::get_n_frames()
{
	return n_frames;
}

template <typename B, typename R>
const std::vector<std::vector<B>>& Frame_dumper<B,R>
::get_buff_src()
{
	return buff_src;
}

template <typename B, typename R>
const std::vector<std::vector<B>>& Frame_dumper<B,R>
::get_buff_enc()
{
	return buff_enc;
}

template <typename B, typename R>
const std::vector<std::vector<R>>& Frame_dumper<B,R>
::get_buff_chn()
{
	return buff_chn;
}

template <typename B, typename R>
const std::vector<std::vector<int>>& Frame_dumper<B,R>
::get_buff_itl()
{
	return buff_itl;
}

template <typename B, typename R>
void Frame_dumper<B,R>
::add(const int frame_id)
{
	if (frame_id < 0 || frame_id >= this->n_frames)
		throw std::invalid_argument("aff3ct::tools::Frame_dumper: \"frame_id\" is invalid.");

	if (U_K != nullptr)
	{
		std::vector<B> src(this->K_info);
		std::copy(U_K + this->K_info * (frame_id +0),
		          U_K + this->K_info * (frame_id +1),
		          src.begin());
		buff_src.push_back(src);
	}

	if (X_N != nullptr)
	{
		std::vector<B> enc(this->N);
		std::copy(X_N + this->N * (frame_id +0),
		          X_N + this->N * (frame_id +1),
		          enc.begin());
		buff_enc.push_back(enc);
	}

	if (Y_N != nullptr)
	{
		std::vector<R> chn(this->N_mod);
		std::copy(Y_N + this->N_mod * (frame_id +0),
		          Y_N + this->N_mod * (frame_id +1),
		          chn.begin());
		buff_chn.push_back(chn);
	}

	if (I_N != nullptr)
	{
		std::vector<int> itl(this->N);
		std::copy(I_N + this->N * (frame_id +0),
		          I_N + this->N * (frame_id +1),
		          itl.begin());
		buff_itl.push_back(itl);
	}
}

template <typename B, typename R>
void Frame_dumper<B,R>
::dump(const std::string& base_path)
{
	const auto n_fe = (unsigned long long)buff_src.size();

	if (buff_src.size())
	{
		const std::string path_src = base_path + ".src";
		std::ofstream file_src(path_src);
		file_src << n_fe         << std::endl << std::endl;
		file_src << this->K_info << std::endl << std::endl;
		this->write_text_body(file_src, this->buff_src);
		file_src.close();
	}

	if (buff_enc.size())
	{
		const std::string path_enc = base_path + ".enc";
		std::ofstream file_enc(path_enc);
		file_enc << n_fe    << std::endl << std::endl;
		file_enc << this->K << std::endl << std::endl;
		file_enc << this->N << std::endl << std::endl;
		this->write_text_body(file_enc, this->buff_enc);
		file_enc.close();
	}

	if (buff_chn.size())
	{
		const std::string path_chn = base_path + ".chn";
		std::ofstream file_chn(path_chn, std::ios_base::binary);
		file_chn.write((char*)&n_fe,        sizeof(n_fe ));
		file_chn.write((char*)&this->N_mod, sizeof(N_mod));
		this->write_binary_body(file_chn, this->buff_chn);
		file_chn.close();
	}

	if (buff_itl.size())
	{
		const std::string path_itl = base_path + ".itl";
		std::ofstream file_itl(path_itl);
		file_itl << n_fe    << std::endl << std::endl;
		file_itl << this->N << std::endl << std::endl;
		this->write_text_body_itl(file_itl, this->buff_itl);
		file_itl.close();
	}
}

template <typename B, typename R>
void Frame_dumper<B,R>
::clear()
{
	buff_src.clear();
	buff_enc.clear();
	buff_chn.clear();
	buff_itl.clear();
}

template <typename B, typename R>
void Frame_dumper<B,R>
::write_text_body(std::ofstream &file, const std::vector<std::vector<B>> &fra)
{
	for (auto f : fra)
	{
		for (auto word : f)
			file << word << " ";
		file << std::endl << std::endl;
	}
}

template <typename B, typename R>
void Frame_dumper<B,R>
::write_text_body_itl(std::ofstream &file, const std::vector<std::vector<int>> &fra)
{
	for (auto f : fra)
	{
		for (auto word : f)
			file << word << " ";
		file << std::endl << std::endl;
	}
}

template <typename B, typename R>
void Frame_dumper<B,R>
::write_binary_body(std::ofstream &file, const std::vector<std::vector<R>> &fra)
{
	for (auto f : fra)
		file.write(reinterpret_cast<char*>(f.data()), f.size() * sizeof(R));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Frame_dumper<B_8 ,R_8>;
template class aff3ct::tools::Frame_dumper<B_16,R_16>;
template class aff3ct::tools::Frame_dumper<B_32,R_32>;
template class aff3ct::tools::Frame_dumper<B_64,R_64>;
#else
template class aff3ct::tools::Frame_dumper<B,R>;
#endif
// ==================================================================================== explicit template instantiation
