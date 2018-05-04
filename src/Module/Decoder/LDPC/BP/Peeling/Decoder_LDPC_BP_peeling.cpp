#include "Decoder_LDPC_BP_peeling.hpp"

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Noise/Erased_value.hpp"
#include "Tools/Math/utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

template<typename B, typename R>
Decoder_LDPC_BP_peeling<B, R>::Decoder_LDPC_BP_peeling(const int K, const int N, const int n_ite,
                                                       const tools::Sparse_matrix &_H,
                                                       const std::vector<unsigned> &info_bits_pos,
                                                       const bool enable_syndrome, const int syndrome_depth,
                                                       const int n_frames)
: Decoder                (K, N,                                             n_frames, 1),
  Decoder_LDPC_BP<B,R>   (K, N, n_ite, _H, enable_syndrome, syndrome_depth, n_frames, 1),
  info_bits_pos          (info_bits_pos                                                ),
  var_nodes              (n_frames, std::vector<B>(N                   )               ),
  check_nodes            (n_frames, std::vector<B>(this->H.get_n_cols())               )
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
		if (Y_N[i] <= tools::Erased_value<R>::llr && Y_N[i] >= -tools::Erased_value<R>::llr)
			var_nodes[frame_id][i] = tools::Erased_value<B>::symbol;
}

template <typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>
::_decode(const int frame_id)
{
	auto links = this->H;

	std::fill(this->check_nodes[frame_id].begin(), this->check_nodes[frame_id].end(), (B)0);

	// std::cout << "(L) var_nodes : " << std::endl;
	// for (unsigned i = 0; i < this->var_nodes[frame_id].size(); i++)
	// 	std::cout << this->var_nodes[frame_id][i] << " ";
	// std::cout << std::endl;
	// std::cout << "(L) check_nodes : " << std::endl;
	// for (unsigned i = 0; i < this->check_nodes[frame_id].size(); i++)
	// 	std::cout << this->check_nodes[frame_id][i] << " ";
	// std::cout << std::endl;

	// std::cout << "(L) links : " << std::endl;
	// links.print(true);

	// first forward known values
	for (unsigned i = 0; i < links.get_n_rows(); i++)
	{
		auto cur_state = this->var_nodes[frame_id][i];
		if (cur_state != tools::Erased_value<B>::symbol)
		{
			auto& cn_list = links.get_cols_from_row(i);
			while (cn_list.size())
			{
				auto& cn_pos = cn_list.front();

				this->check_nodes[frame_id][cn_pos] ^= cur_state;
				links.rm_connection(i, cn_pos);
			}
		}
	}

	// std::cout << "(I) var_nodes : " << std::endl;
	// for (unsigned i = 0; i < this->var_nodes[frame_id].size(); i++)
	// 	std::cout << this->var_nodes[frame_id][i] << " ";
	// std::cout << std::endl;
	// std::cout << "(I) check_nodes : " << std::endl;
	// for (unsigned i = 0; i < this->check_nodes[frame_id].size(); i++)
	// 	std::cout << this->check_nodes[frame_id][i] << " ";
	// std::cout << std::endl;

	// std::cout << "(I) links : " << std::endl;
	// links.print(true);

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		bool all_check_nodes_done = true, no_modification = true;

		// find degree-1 check nodes
		for (unsigned i = 0; i < links.get_n_cols(); i++)
		{
			if (links.get_rows_from_col(i).size() == 1)
			{
				no_modification = false;

				// then forward the belief
				auto& vn_pos = links.get_rows_from_col(i).front();
				auto cur_state = this->check_nodes[frame_id][i];
				this->var_nodes  [frame_id][vn_pos] = cur_state;
				this->check_nodes[frame_id][     i] = 0;
				links.rm_connection(vn_pos, i);

				// and propagate it
				auto& cn_list = links.get_cols_from_row(vn_pos);
				while (cn_list.size())
				{
					auto& cn_pos = cn_list.front();

					this->check_nodes[frame_id][cn_pos] ^= cur_state;
					links.rm_connection(vn_pos, cn_pos);
				}
			}
			else
				all_check_nodes_done &= links.get_rows_from_col(i).size() == 0;
		}

		// std::cout << "(" << ite << ") var_nodes : " << std::endl;
		// for (unsigned i = 0; i < this->var_nodes[frame_id].size(); i++)
		// 	std::cout << this->var_nodes[frame_id][i] << " ";
		// std::cout << std::endl;
		// std::cout << "(" << ite << ") check_nodes : " << std::endl;
		// for (unsigned i = 0; i < this->check_nodes[frame_id].size(); i++)
		// 	std::cout << this->check_nodes[frame_id][i] << " ";
		// std::cout << std::endl;

		if (this->enable_syndrome && (all_check_nodes_done || no_modification))
		{
			this->cur_syndrome_depth++;
			if (this->cur_syndrome_depth == this->syndrome_depth)
				break;
		}
		else
			this->cur_syndrome_depth = 0;
	}
};

template <typename B, typename R>
void Decoder_LDPC_BP_peeling<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now();  // ---------------------------------------------------------- LOAD
	std::copy(Y_N, Y_N + this->N, var_nodes[frame_id].data());
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store(V_K, frame_id);
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
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store_cw(V_N, frame_id);
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
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store(V_K, frame_id);
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
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	_store_cw(V_N, frame_id);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

template<typename B, typename R>
void Decoder_LDPC_BP_peeling<B, R>::_store(B *V_K, const int frame_id)
{
	for (auto i = 0; i < this->K; i++)
		V_K[i] = this->var_nodes[frame_id][this->info_bits_pos[i]];
}

template<typename B, typename R>
void Decoder_LDPC_BP_peeling<B, R>::_store_cw(B *V_N, const int frame_id)
{
	std::copy(this->var_nodes[frame_id].begin(), this->var_nodes[frame_id].end(), V_N);
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
