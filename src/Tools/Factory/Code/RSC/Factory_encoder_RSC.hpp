#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include <string>

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

#include "../Factory_encoder.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_encoder_RSC : public Factory_encoder
{
	struct parameters : Factory_encoder::parameters
	{
		virtual ~parameters() {}

		// ------- encoder
		bool buffered = true;

		// ------- code
		std::string      standard;
		std::vector<int> poly        = {013, 015};
		int              tail_length = 2 * 3;
	};

	template <typename B = int>
	static module::Encoder_RSC_sys<B>* build(const parameters &params, std::ostream &stream = std::cout);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, Header::params_list& head_cde,
	                   const parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_RSC_HPP */
