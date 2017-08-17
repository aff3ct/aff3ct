#ifndef FACTORY_ENCODER_LDPC_HPP
#define FACTORY_ENCODER_LDPC_HPP

#include <string>
#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_LDPC : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Encoder::parameters
	{
		virtual ~parameters() {}

		std::string H_alist_path = "";
		std::string G_alist_path = "";
	};

	template <typename B = int>
	static module::Encoder_LDPC<B>* build(const parameters &params, const tools::Sparse_matrix &G,
	                                                                const tools::Sparse_matrix &H);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_ENCODER_LDPC_HPP */
