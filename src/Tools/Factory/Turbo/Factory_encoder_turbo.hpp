#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Encoder/Encoder_sys.hpp"

#include "../Factory_encoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_turbo : public Factory_encoder_common<B>
{
	struct encoder_parameters_turbo : Factory_encoder_common<B>::encoder_parameters
	{
		virtual ~encoder_parameters_turbo() {}

		bool buffered = true;
		std::string json_path = "";

		// ------- code
		std::string      standard;
		std::vector<int> poly        = {013, 015};
		int              tail_length = 4 * 3;
		int              N_pct;

		typename Factory_interleaver<int>::interleaver_parameters itl;
	};

	static module::Encoder<B>* build(const encoder_parameters_turbo &params,
	                                 const module::Interleaver<int> &itl,
	                                       module::Encoder_sys<B>   *enc_n,
	                                       module::Encoder_sys<B>   *enc_i = nullptr);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, encoder_parameters_turbo &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, Header::params_list& head_cde, const encoder_parameters_turbo& params);
};
}
}

#endif /* FACTORY_ENCODER_TURBO_HPP */
