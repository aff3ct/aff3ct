#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Module/Encoder/LDPC/From_H/Encoder_LDPC_from_H.hpp"
#include "Module/Encoder/LDPC/From_QC/Encoder_LDPC_from_QC.hpp"
#include "Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2.hpp"

#include "Encoder_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_LDPC_name   = "Encoder LDPC";
const std::string aff3ct::factory::Encoder_LDPC_prefix = "enc";

Encoder_LDPC::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_LDPC_name, prefix)
{
	this->type = "AZCW";
}

Encoder_LDPC::parameters
::~parameters()
{
}

Encoder_LDPC::parameters* Encoder_LDPC::parameters
::clone() const
{
	return new Encoder_LDPC::parameters(*this);
}

void Encoder_LDPC::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Encoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	tools::add_options(opt_args.at({p+"-type"}), 0, "LDPC", "LDPC_H", "LDPC_DVBS2", "LDPC_QC");

	opt_args.add(
		{p+"-h-path"},
		tools::File(tools::openmode::read),
		"path to the H matrix (AList formated file, required by the \"LDPC_H\" encoder).");

	opt_args.add(
		{p+"-g-path"},
		tools::File(tools::openmode::read),
		"path to the G matrix (AList formated file, required by the \"LDPC\" encoder).");

	opt_args.add(
		{p+"-h-reorder"},
		tools::Text(tools::Including_set("NONE", "ASC", "DSC")),
		"specify if the check nodes (CNs) from H have to be reordered, 'NONE': do nothing (default), 'ASC': from the "
		"smallest to the biggest CNs, 'DSC': from the biggest to the smallest CNs.");
}

void Encoder_LDPC::parameters
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-h-path"   })) this->H_path    = vals.at({p+"-h-path"   });
	if(vals.exist({p+"-g-path"   })) this->G_path    = vals.at({p+"-g-path"   });
	if(vals.exist({p+"-h-reorder"})) this->H_reorder = vals.at({p+"-h-reorder"});
}

void Encoder_LDPC::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->type == "LDPC")
		headers[p].push_back(std::make_pair("G matrix path", this->G_path));
	if (this->type == "LDPC_H" || this->type == "LDPC_QC")
	{
		headers[p].push_back(std::make_pair("H matrix path", this->H_path));
		headers[p].push_back(std::make_pair("H matrix reordering", this->H_reorder));
	}
}

template <typename B>
module::Encoder_LDPC<B>* Encoder_LDPC::parameters
::build(const tools::Sparse_matrix &G, const tools::Sparse_matrix &H, const tools::dvbs2_values* dvbs2) const
{
	     if (this->type == "LDPC"      ) return new module::Encoder_LDPC        <B>(this->K, this->N_cw, G, this->n_frames);
	else if (this->type == "LDPC_H"    ) return new module::Encoder_LDPC_from_H <B>(this->K, this->N_cw, H, this->n_frames);
	else if (this->type == "LDPC_QC"   ) return new module::Encoder_LDPC_from_QC<B>(this->K, this->N_cw, H, this->n_frames);
	else if (this->type == "LDPC_DVBS2" && dvbs2 != nullptr)
		return new module::Encoder_LDPC_DVBS2  <B>(*dvbs2, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_LDPC<B>* Encoder_LDPC
::build(const parameters           &params,
        const tools::Sparse_matrix &G,
        const tools::Sparse_matrix &H,
        const tools::dvbs2_values* dvbs2)
{
	return params.template build<B>(G, H, dvbs2);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_LDPC<B_8 >* aff3ct::factory::Encoder_LDPC::parameters::build<B_8 >(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*) const;
template aff3ct::module::Encoder_LDPC<B_16>* aff3ct::factory::Encoder_LDPC::parameters::build<B_16>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*) const;
template aff3ct::module::Encoder_LDPC<B_32>* aff3ct::factory::Encoder_LDPC::parameters::build<B_32>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*) const;
template aff3ct::module::Encoder_LDPC<B_64>* aff3ct::factory::Encoder_LDPC::parameters::build<B_64>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*) const;
template aff3ct::module::Encoder_LDPC<B_8 >* aff3ct::factory::Encoder_LDPC::build<B_8 >(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*);
template aff3ct::module::Encoder_LDPC<B_16>* aff3ct::factory::Encoder_LDPC::build<B_16>(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*);
template aff3ct::module::Encoder_LDPC<B_32>* aff3ct::factory::Encoder_LDPC::build<B_32>(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*);
template aff3ct::module::Encoder_LDPC<B_64>* aff3ct::factory::Encoder_LDPC::build<B_64>(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*);
#else
template aff3ct::module::Encoder_LDPC<B>* aff3ct::factory::Encoder_LDPC::parameters::build<B>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*) const;
template aff3ct::module::Encoder_LDPC<B>* aff3ct::factory::Encoder_LDPC::build<B>(const aff3ct::factory::Encoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values*);
#endif
// ==================================================================================== explicit template instantiation
