#include "Decoder_LDPC_BP_peeling.hpp"

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Noise/noise_utils.h"
#include "Tools/Math/utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

template<typename B, typename R>
Decoder_LDPC_BP_peeling<B,R>::Decoder_LDPC_BP_peeling(const int K, const int N, const int n_ite,
                                                      const tools::Sparse_matrix &_H,
                                                      const std::vector<unsigned> &info_bits_pos,
                                                      Encoder<B> &encoder,
                                                      const bool enable_syndrome, const int syndrome_depth,
                                                      const int n_frames)
: Decoder                (K, N,                                             n_frames, 1),
  Decoder_LDPC_BP<B,R>   (K, N, n_ite, _H, enable_syndrome, syndrome_depth, n_frames, 1),

  info_bits_pos          (info_bits_pos),
  encoder                (encoder),
  var_nodes              (n_frames, std::vector<B>(N)),
  check_nodes            (this->H.get_n_cols())
{
	const std::string name = "Decoder_LDPC_BP_peeling";
	this->set_name(name);
}

template <typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>
::_load(const R *Y_N, const int frame_id)
{
	tools::hard_decide(Y_N, var_nodes[frame_id].data(), this->N);
	for (auto i = 0; i < this->N; i++)
		if (Y_N[i] <= tools::erased_llr_val<R>() && Y_N[i] >= -tools::erased_llr_val<R>())
			var_nodes[frame_id][i] = tools::erased_symbol_val<B>();
}

template <typename B, typename R>
bool Decoder_LDPC_BP_peeling<B,R>
::_decode(const int frame_id)
{
	auto links = this->H;

	auto& CN = this->check_nodes;
	auto& VN = this->var_nodes[frame_id];

	std::fill(CN.begin(), CN.end(), (B)0);
//
//	unsigned display_max = 32;
//
//	std::cout << "(L) var_nodes : " << std::endl;
//	for (unsigned i = 0; i < VN.size() && i < display_max; i++)
//		std::cout << VN[i] << " ";
//	std::cout << std::endl;
//	std::cout << "(L) check_nodes : " << std::endl;
//	for (unsigned i = 0; i < CN.size() && i < display_max; i++)
//		std::cout << CN[i] << " ";
//	std::cout << std::endl;

	//	 std::cout << "(L) links : " << std::endl;
	//	 links.print(true);

	// first forward known values
	for (unsigned i = 0; i < links.get_n_rows(); i++)
	{
		auto cur_state = VN[i];
		if (cur_state != tools::erased_symbol_val<B>())
		{
			auto& cn_list = links.get_cols_from_row(i);
			while (cn_list.size())
			{
				auto& cn_pos = cn_list.front();

				CN[cn_pos] ^= cur_state;
				links.rm_connection(i, cn_pos);
			}
		}
	}

//	 std::cout << "(I) var_nodes : " << std::endl;
//	 for (unsigned i = 0; i < VN.size() && i < display_max; i++)
//	    std::cout << VN[i] << " ";
//	 std::cout << std::endl;
//	 std::cout << "(I) check_nodes : " << std::endl;
//	 for (unsigned i = 0; i < CN.size() && i < display_max; i++)
//	    std::cout << CN[i] << " ";
//	 std::cout << std::endl;

	//	 std::cout << "(I) links : " << std::endl;
	//	 links.print(true);

	bool all_check_nodes_done = false;

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		bool no_modification = true;
		all_check_nodes_done = true;

		// find degree-1 check nodes
		for (unsigned i = 0; i < links.get_n_cols(); i++)
		{
			auto& vn_list = links.get_rows_from_col(i);
			if (vn_list.size() == 1)
			{
				no_modification = false;

				// then forward the belief
				auto& vn_pos = vn_list.front();
				auto cur_state = CN[i];
				VN[vn_pos] = cur_state;
				CN[     i] = 0;
				links.rm_connection(vn_pos, i);

				// and propagate it
				auto& cn_list = links.get_cols_from_row(vn_pos);
				while (cn_list.size())
				{
					auto& cn_pos = cn_list.front();

					CN[cn_pos] ^= cur_state;
					links.rm_connection(vn_pos, cn_pos);
				}
			}
			else
				all_check_nodes_done &= vn_list.size() == 0;
		}

//		 std::cout << "(" << ite << ") var_nodes : " << std::endl;
//		for (unsigned i = 0; i < VN.size() && i < display_max; i++)
//		{
//			std::cout << VN[i] << " ";
//		}
//		std::cout << std::endl;
//		std::cout << "(" << ite << ") check_nodes : " << std::endl;
//		for (unsigned i = 0; i < CN.size() && i < display_max; i++)
//		{
//			std::cout << CN[i] << " ";
//		}
//		std::cout << std::endl;
//		std::cout << "all_check_nodes_done : " << all_check_nodes_done << std::endl;
//		std::cout << "no_modification      : " << no_modification << std::endl;

		if (this->enable_syndrome && (all_check_nodes_done || no_modification))
		{
			this->cur_syndrome_depth++;
			if (this->cur_syndrome_depth == this->syndrome_depth)
				break;
		}
		else
			this->cur_syndrome_depth = 0;
	}

	return all_check_nodes_done;
};

template <typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, var_nodes[frame_id].data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto syndrome = this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store(V_K, syndrome, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho].update_timer(dec::tm::decode_hiho::store,  d_store);
}

template <typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, var_nodes[frame_id].data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto syndrome = this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store_cw(V_N, syndrome, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_hiho_cw].update_timer(dec::tm::decode_hiho_cw::store,  d_store);
}

template <typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto syndrome = this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store(V_K, syndrome, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto syndrome = this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store_cw(V_N, syndrome, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

template<typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>::_store(B *V_K, bool syndrome, const int frame_id)
{
	auto& VN = this->var_nodes[frame_id];

	if (syndrome)
		for (auto i = 0; i < this->K; i++)
			V_K[i] = VN[this->info_bits_pos[i]];
	else
	{
		auto& X_N = encoder.get_X_N(frame_id);
		for (auto i = 0; i < this->K; i++)
		{
			auto ibp = this->info_bits_pos[i];
			V_K[i] = VN[ibp] == tools::erased_symbol_val<B>() ? !X_N[ibp] : VN[ibp];
		}
	}
}

template<typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>::_store_cw(B *V_N, bool syndrome, const int frame_id)
{
	auto& VN = this->var_nodes[frame_id];

	if (syndrome)
		std::copy(VN.begin(), VN.end(), V_N);
	else
	{
		auto& X_N = encoder.get_X_N(frame_id);
		for (auto i = 0; i < this->N; i++)
			V_N[i] = VN[i] == tools::erased_symbol_val<B>() ? !X_N[i] : VN[i];
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_peeling<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_peeling<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_peeling<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_peeling<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_peeling<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
