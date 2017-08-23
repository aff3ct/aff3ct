#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Module/Encoder/LDPC/From_H/Encoder_LDPC_from_H.hpp"
#include "Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2.hpp"

#include "Encoder_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_LDPC::name   = "Encoder LDPC";
const std::string aff3ct::factory::Encoder_LDPC::prefix = "enc";

template <typename B>
module::Encoder_LDPC<B>* Encoder_LDPC::parameters
::build(const tools::Sparse_matrix &G, const tools::Sparse_matrix &H) const
{
	     if (this->type == "LDPC"      ) return new module::Encoder_LDPC       <B>(this->K, this->N_cw, G, this->n_frames);
	else if (this->type == "LDPC_H"    ) return new module::Encoder_LDPC_from_H<B>(this->K, this->N_cw, H, this->n_frames);
	else if (this->type == "LDPC_DVBS2") return new module::Encoder_LDPC_DVBS2 <B>(this->K, this->N_cw,    this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_LDPC<B>* Encoder_LDPC
::build(const parameters           &params,
        const tools::Sparse_matrix &G,
        const tools::Sparse_matrix &H)
{
	return params.template build<B>(G, H);
}

void Encoder_LDPC
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args, p);

	opt_args[{p+"-type"}][2] += ", LDPC, LDPC_H, LDPC_DVBS2";

	opt_args[{p+"-h-path"}] =
		{"string",
		 "path to the H matrix (AList formated file, required by the \"LDPC_H\" encoder)."};

	opt_args[{p+"-g-path"}] =
		{"string",
		 "path to the G matrix (AList formated file, required by the \"LDPC\" encoder)."};
}

void Encoder_LDPC
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "AZCW";

	Encoder::store_args(vals, params, p);

	if(exist(vals, {p+"-h-path"})) params.H_alist_path = vals.at({p+"-h-path"});
	if(exist(vals, {p+"-g-path"})) params.G_alist_path = vals.at({p+"-g-path"});
}

void Encoder_LDPC
::make_header(params_list& head_enc, const parameters& params, const bool full)
{
	Encoder::make_header(head_enc, params, full);

	if (params.type == "LDPC")
		head_enc.push_back(std::make_pair("G matrix path", params.G_alist_path));
	if (params.type == "LDPC_H")
		head_enc.push_back(std::make_pair("H matrix path", params.H_alist_path));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_LDPC<B_8 >* aff3ct::factory::Encoder_LDPC::parameters::build<B_8 >(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;
template aff3ct::module::Encoder_LDPC<B_16>* aff3ct::factory::Encoder_LDPC::parameters::build<B_16>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;
template aff3ct::module::Encoder_LDPC<B_32>* aff3ct::factory::Encoder_LDPC::parameters::build<B_32>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;
template aff3ct::module::Encoder_LDPC<B_64>* aff3ct::factory::Encoder_LDPC::parameters::build<B_64>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;
template aff3ct::module::Encoder_LDPC<B_8 >* aff3ct::factory::Encoder_LDPC::build<B_8 >(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&);
template aff3ct::module::Encoder_LDPC<B_16>* aff3ct::factory::Encoder_LDPC::build<B_16>(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&);
template aff3ct::module::Encoder_LDPC<B_32>* aff3ct::factory::Encoder_LDPC::build<B_32>(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&);
template aff3ct::module::Encoder_LDPC<B_64>* aff3ct::factory::Encoder_LDPC::build<B_64>(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&);
#else
template aff3ct::module::Encoder_LDPC<B>* aff3ct::factory::Encoder_LDPC::parameters::build<B>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;
template aff3ct::module::Encoder_LDPC<B>* aff3ct::factory::Encoder_LDPC::build<B>(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&);
#endif
// ==================================================================================== explicit template instantiation
