#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Module/Encoder/LDPC/From_H/Encoder_LDPC_from_H.hpp"
#include "Module/Encoder/LDPC/From_QC/Encoder_LDPC_from_QC.hpp"
#include "Module/Encoder/LDPC/From_IRA/Encoder_LDPC_from_IRA.hpp"
#include "Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2.hpp"
#include "Factory/Module/Encoder/LDPC/Encoder_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_LDPC_name   = "Encoder LDPC";
const std::string aff3ct::factory::Encoder_LDPC_prefix = "enc";

Encoder_LDPC
::Encoder_LDPC(const std::string &prefix)
: Encoder(Encoder_LDPC_name, prefix)
{
	this->type = "AZCW";
}

Encoder_LDPC* Encoder_LDPC
::clone() const
{
	return new Encoder_LDPC(*this);
}

void Encoder_LDPC
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_LDPC::";

	cli::add_options(args.at({p+"-type"}), 0, "LDPC", "LDPC_H", "LDPC_DVBS2", "LDPC_QC", "LDPC_IRA");

	tools::add_arg(args, p, class_name+"p+h-path",
		cli::File(cli::openmode::read));

	args.add_link({p+"-h-path"}, {p+"-cw-size",   "N"}); // N_cw is H width
	args.add_link({p+"-h-path"}, {p+"-info-bits", "K"}); // if there is no K, then H is considered regular,
	                                                     // so K is the N - H's height

	tools::add_arg(args, p, class_name+"p+g-path",
		cli::File(cli::openmode::read));

	args.add_link({p+"-g-path"}, {p+"-info-bits", "K"});
	args.add_link({p+"-g-path"}, {p+"-cw-size",   "N"});

	tools::add_arg(args, p, class_name+"p+h-reorder",
		cli::Text(cli::Including_set("NONE", "ASC", "DSC")));

	tools::add_arg(args, p, class_name+"p+g-method",
		cli::Text(cli::Including_set("IDENTITY", "LU_DEC")));

	tools::add_arg(args, p, class_name+"p+g-save-path",
		cli::File(cli::openmode::write));
}

void Encoder_LDPC
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-h-path"     })) this->H_path      = vals.to_file({p+"-h-path"     });
	if(vals.exist({p+"-g-path"     })) this->G_path      = vals.to_file({p+"-g-path"     });
	if(vals.exist({p+"-h-reorder"  })) this->H_reorder   = vals.at     ({p+"-h-reorder"  });
	if(vals.exist({p+"-g-method"   })) this->G_method    = vals.at     ({p+"-g-method"   });
	if(vals.exist({p+"-g-save-path"})) this->G_save_path = vals.at     ({p+"-g-save-path"});

	if (!this->G_path.empty())
	{
		tools::LDPC_matrix_handler::read_matrix_size(this->G_path, this->K, this->N_cw);
	}
	else if (!this->H_path.empty())
	{
		int M;
		tools::LDPC_matrix_handler::read_matrix_size(this->H_path, M, this->N_cw);
		this->K = this->N_cw - M; // considered as regular so M = N - K
	}

	Encoder::store(vals);
}

void Encoder_LDPC
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->type == "LDPC")
		headers[p].push_back(std::make_pair("G matrix path", this->G_path));

	if (this->type == "LDPC_H" || this->type == "LDPC_QC")
	{
		headers[p].push_back(std::make_pair("H matrix path", this->H_path));
		headers[p].push_back(std::make_pair("H matrix reordering", this->H_reorder));
	}

	if (this->type == "LDPC_H")
	{
		headers[p].push_back(std::make_pair("G build method", this->G_method));
		if (this->G_save_path != "")
		headers[p].push_back(std::make_pair("G save path", this->G_save_path));
	}
}

template <typename B>
module::Encoder_LDPC<B>* Encoder_LDPC
::build(const tools::Sparse_matrix &G, const tools::Sparse_matrix &H) const
{
	if (this->type == "LDPC"    ) return new module::Encoder_LDPC         <B>(this->K, this->N_cw, G);
	if (this->type == "LDPC_H"  ) return new module::Encoder_LDPC_from_H  <B>(this->K, this->N_cw, H, this->G_method, this->G_save_path, true);
	if (this->type == "LDPC_QC" ) return new module::Encoder_LDPC_from_QC <B>(this->K, this->N_cw, H);
	if (this->type == "LDPC_IRA") return new module::Encoder_LDPC_from_IRA<B>(this->K, this->N_cw, H);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_LDPC<B>* Encoder_LDPC
::build(const tools::Sparse_matrix &G, const tools::Sparse_matrix &H, const tools::dvbs2_values& dvbs2) const
{
	if (this->type == "LDPC_DVBS2") return new module::Encoder_LDPC_DVBS2<B>(dvbs2);

	return build<B>(G, H);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_LDPC<B_8 >* aff3ct::factory::Encoder_LDPC::build<B_8 >(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;
template aff3ct::module::Encoder_LDPC<B_16>* aff3ct::factory::Encoder_LDPC::build<B_16>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;
template aff3ct::module::Encoder_LDPC<B_32>* aff3ct::factory::Encoder_LDPC::build<B_32>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;
template aff3ct::module::Encoder_LDPC<B_64>* aff3ct::factory::Encoder_LDPC::build<B_64>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;

template aff3ct::module::Encoder_LDPC<B_8 >* aff3ct::factory::Encoder_LDPC::build<B_8 >(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values&) const;
template aff3ct::module::Encoder_LDPC<B_16>* aff3ct::factory::Encoder_LDPC::build<B_16>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values&) const;
template aff3ct::module::Encoder_LDPC<B_32>* aff3ct::factory::Encoder_LDPC::build<B_32>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values&) const;
template aff3ct::module::Encoder_LDPC<B_64>* aff3ct::factory::Encoder_LDPC::build<B_64>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values&) const;
#else
template aff3ct::module::Encoder_LDPC<B>* aff3ct::factory::Encoder_LDPC::build<B>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&) const;

template aff3ct::module::Encoder_LDPC<B>* aff3ct::factory::Encoder_LDPC::build<B>(const aff3ct::tools::Sparse_matrix&, const aff3ct::tools::Sparse_matrix&, const tools::dvbs2_values&) const;
#endif
// ==================================================================================== explicit template instantiation
