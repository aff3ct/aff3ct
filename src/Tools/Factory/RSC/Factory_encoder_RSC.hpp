#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include <string>

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

#include "../Factory_encoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_RSC : public Factory_encoder_common<B>
{
	struct encoder_parameters_RSC : Factory_encoder_common<B>::encoder_parameters
	{
		virtual ~encoder_parameters_RSC() {}

		// ------- encoder
		bool buffered = true;

		// ------- code
		std::string      standard;
		std::vector<int> poly   = {013, 015};
		int         tail_length = 2 * 3;
	};

	static module::Encoder_RSC_sys<B>* build(const encoder_parameters_RSC &params, std::ostream &stream = std::cout);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, encoder_parameters_RSC &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, Header::params_list& head_cde,
	                   const encoder_parameters_RSC& params);
};
}
}

#endif /* FACTORY_ENCODER_RSC_HPP */
