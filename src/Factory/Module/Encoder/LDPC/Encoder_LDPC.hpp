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

	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional
		std::string H_path = "";
		std::string G_path = "";

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Encoder_LDPC::prefix);
		virtual ~parameters();
		Encoder_LDPC::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_LDPC<B>* build(const tools::Sparse_matrix &G, const tools::Sparse_matrix &H) const;
	};

	template <typename B = int>
	static module::Encoder_LDPC<B>* build(const parameters &params, const tools::Sparse_matrix &G,
	                                                                const tools::Sparse_matrix &H);
};
}
}

#endif /* FACTORY_ENCODER_LDPC_HPP */
