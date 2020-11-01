#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Math/max.h"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding.hpp"
#include "Module/Decoder/LDPC/BP/Horizontal_layered/Decoder_LDPC_BP_horizontal_layered.hpp"
#include "Module/Decoder/LDPC/BP/Vertical_layered/Decoder_LDPC_BP_vertical_layered.hpp"
#include "Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA.hpp"
#include "Tools/Code/LDPC/Update_rule/LSPA/Update_rule_LSPA.hpp"
#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS.hpp"
#include "Tools/Code/LDPC/Update_rule/OMS/Update_rule_OMS.hpp"
#include "Tools/Code/LDPC/Update_rule/NMS/Update_rule_NMS.hpp"
#include "Tools/Code/LDPC/Update_rule/AMS/Update_rule_AMS.hpp"
#ifdef __cpp_aligned_new
#include "Module/Decoder/LDPC/BP/Horizontal_layered/Decoder_LDPC_BP_horizontal_layered_inter.hpp"
#include "Module/Decoder/LDPC/BP/Vertical_layered/Decoder_LDPC_BP_vertical_layered_inter.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding_inter.hpp"
#include "Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA_simd.hpp"
#include "Tools/Code/LDPC/Update_rule/LSPA/Update_rule_LSPA_simd.hpp"
#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS_simd.hpp"
#include "Tools/Code/LDPC/Update_rule/OMS/Update_rule_OMS_simd.hpp"
#include "Tools/Code/LDPC/Update_rule/NMS/Update_rule_NMS_simd.hpp"
#include "Tools/Code/LDPC/Update_rule/AMS/Update_rule_AMS_simd.hpp"
#endif
#include "Module/Decoder/LDPC/BP/Horizontal_layered/ONMS/Decoder_LDPC_BP_horizontal_layered_ONMS_inter.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_B.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_E.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/SPA/Decoder_LDPC_BP_flooding_SPA.hpp"
#include "Module/Decoder/LDPC/BP/Peeling/Decoder_LDPC_BP_peeling.hpp"
#include "Module/Decoder/LDPC/BF/OMWBF/Decoder_LDPC_bit_flipping_OMWBF.hpp"
#include "Module/Decoder/LDPC/BF/PPBF/Decoder_LDPC_probabilistic_parallel_bit_flipping.hpp"
#include "Factory/Module/Decoder/LDPC/Decoder_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_LDPC_name   = "Decoder LDPC";
const std::string aff3ct::factory::Decoder_LDPC_prefix = "dec";

Decoder_LDPC
::Decoder_LDPC(const std::string &prefix)
: Decoder(Decoder_LDPC_name, prefix)
{
	this->type   = "BP_FLOODING";
	this->implem = "SPA";
}

Decoder_LDPC* Decoder_LDPC
::clone() const
{
	return new Decoder_LDPC(*this);
}

struct Real_splitter
{
	static std::vector<std::string> split(const std::string& val)
	{
		const std::string head      = "{([";
		const std::string queue     = "})]";
		const std::string separator = ",";

		return cli::Splitter::split(val, head, queue, separator);
	}
};

void Decoder_LDPC
::get_description(cli::Argument_map_info &args) const
{
	Decoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_LDPC::";

	tools::add_arg(args, p, class_name+"p+h-path",
		cli::File(cli::openmode::read),
		cli::arg_rank::REQ);

	args.add_link({p+"-h-path"}, {p+"-cw-size",   "N"}); // N_cw is H width
	args.add_link({p+"-h-path"}, {p+"-info-bits", "K"}); // if there is no K, then H is considered regular,
	                                                     // so K is the N - H's height

	cli::add_options(args.at({p+"-type", "D"}), 0, "BP_FLOODING", "BP_HORIZONTAL_LAYERED", "BP_VERTICAL_LAYERED", "BP_PEELING", "BIT_FLIPPING");
#ifdef __cpp_aligned_new
	cli::add_options(args.at({p+"-type", "D"}), 0, "BP_HORIZONTAL_LAYERED_LEGACY");
#endif
	cli::add_options(args.at({p+"-implem"   }), 0, "SPA", "LSPA", "MS", "OMS", "NMS", "AMS", "GALA", "GALB", "GALE", "WBF", "MWBF",  "PPBF");

	tools::add_arg(args, p, class_name+"p+ite,i",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+off",
		cli::Real());

	tools::add_arg(args, p, class_name+"p+mwbf-factor",
		cli::Real());

	tools::add_arg(args, p, class_name+"p+norm",
		cli::Real(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+no-synd",
		cli::None());

	tools::add_arg(args, p, class_name+"p+synd-depth",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+simd",
		cli::Text(cli::Including_set("INTER", "INTRA")));

	tools::add_arg(args, p, class_name+"p+min",
		cli::Text(cli::Including_set("MIN", "MINL", "MINS")));

	tools::add_arg(args, p, class_name+"p+h-reorder",
		cli::Text(cli::Including_set("NONE", "ASC", "DSC")));

	tools::add_arg(args, p, class_name+"p+ppbf-proba",
		cli::List<float,Real_splitter>(cli::Real(), cli::Length(1)));
}

void Decoder_LDPC
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-h-path"     })) this->H_path          = vals.to_file ({p+"-h-path"     });
	if(vals.exist({p+"-h-reorder"  })) this->H_reorder       = vals.at      ({p+"-h-reorder"  });
	if(vals.exist({p+"-simd"       })) this->simd_strategy   = vals.at      ({p+"-simd"       });
	if(vals.exist({p+"-min"        })) this->min             = vals.at      ({p+"-min"        });
	if(vals.exist({p+"-ite",    "i"})) this->n_ite           = vals.to_int  ({p+"-ite",    "i"});
	if(vals.exist({p+"-synd-depth" })) this->syndrome_depth  = vals.to_int  ({p+"-synd-depth" });
	if(vals.exist({p+"-off"        })) this->offset          = vals.to_float({p+"-off"        });
	if(vals.exist({p+"-mwbf-factor"})) this->mwbf_factor     = vals.to_float({p+"-mwbf-factor"});
	if(vals.exist({p+"-norm"       })) this->norm_factor     = vals.to_float({p+"-norm"       });
	if(vals.exist({p+"-ppbf-proba" })) this->ppbf_proba      = vals.to_list<float>({p+"-ppbf-proba"});
	if(vals.exist({p+"-no-synd"    })) this->enable_syndrome = false;

	if (!this->H_path.empty())
	{
		int M;
		tools::LDPC_matrix_handler::read_matrix_size(this->H_path, M, this->N_cw);
		this->K = this->N_cw - M; // considered as regular so M = N - K
	}

	Decoder::store(vals);
}

void Decoder_LDPC
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Decoder::get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();

		if (!this->H_path.empty())
		{
			headers[p].push_back(std::make_pair("H matrix path", this->H_path));
			headers[p].push_back(std::make_pair("H matrix reordering", this->H_reorder));
		}

		if (!this->simd_strategy.empty())
			headers[p].push_back(std::make_pair("SIMD strategy", this->simd_strategy));

		headers[p].push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->n_ite)));

		if (this->implem == "NMS")
			headers[p].push_back(std::make_pair("Normalize factor", std::to_string(this->norm_factor)));

		if (this->implem == "OMS")
			headers[p].push_back(std::make_pair("Offset", std::to_string(this->offset)));

		std::string syndrome = this->enable_syndrome ? "on" : "off";
		headers[p].push_back(std::make_pair("Stop criterion (syndrome)", syndrome));

		if (this->enable_syndrome)
			headers[p].push_back(std::make_pair("Stop criterion depth", std::to_string(this->syndrome_depth)));

		if (this->implem == "AMS")
			headers[p].push_back(std::make_pair("Min type", this->min));

		if (this->implem == "PPBF")
		{
			std::stringstream bern_str;
			bern_str << "{";
			for (unsigned i = 0; i < this->ppbf_proba.size(); i++)
				bern_str << this->ppbf_proba[i] << (i == this->ppbf_proba.size()-1 ?"":", ");
			bern_str << "}";

			headers[p].push_back(std::make_pair("Bernouilli probas", bern_str.str()));
		}

		if (this->implem == "MWBF")
			headers[p].push_back(std::make_pair("Weighting factor", std::to_string(this->mwbf_factor)));
	}
}

template <typename B, typename Q>
module::Decoder_SISO<B,Q>* Decoder_LDPC
::build_siso(const tools::Sparse_matrix &H, const std::vector<unsigned> &info_bits_pos,
             module::Encoder<B> *encoder) const
{
	if (this->type == "BP_FLOODING" && this->simd_strategy.empty())
	{
		const auto max_CN_degree = (unsigned int)H.get_cols_max_degree();

		if (this->implem == "MS"  )  return new module::Decoder_LDPC_BP_flooding<B,Q,tools::Update_rule_MS  <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_MS  <Q                           >(                 ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "OMS" )  return new module::Decoder_LDPC_BP_flooding<B,Q,tools::Update_rule_OMS <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_OMS <Q                           >((Q)this->offset  ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "NMS" )  return new module::Decoder_LDPC_BP_flooding<B,Q,tools::Update_rule_NMS <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS <Q                           >(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "SPA" )  return new module::Decoder_LDPC_BP_flooding<B,Q,tools::Update_rule_SPA <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_SPA <Q                           >(max_CN_degree    ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "LSPA")  return new module::Decoder_LDPC_BP_flooding<B,Q,tools::Update_rule_LSPA<Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_LSPA<Q                           >(max_CN_degree    ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "AMS" )
		{
			if (this->min == "MIN" ) return new module::Decoder_LDPC_BP_flooding<B,Q,tools::Update_rule_AMS <Q,tools::min             <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min             <Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINL") return new module::Decoder_LDPC_BP_flooding<B,Q,tools::Update_rule_AMS <Q,tools::min_star_linear2<Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min_star_linear2<Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINS") return new module::Decoder_LDPC_BP_flooding<B,Q,tools::Update_rule_AMS <Q,tools::min_star        <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min_star        <Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
		}
	}
	else if (this->type == "BP_HORIZONTAL_LAYERED" && this->simd_strategy.empty())
	{
		const auto max_CN_degree = (unsigned int)H.get_cols_max_degree();

		if (this->implem == "MS"  )  return new module::Decoder_LDPC_BP_horizontal_layered<B,Q,tools::Update_rule_MS  <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_MS  <Q                           >(                 ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "OMS" )  return new module::Decoder_LDPC_BP_horizontal_layered<B,Q,tools::Update_rule_OMS <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_OMS <Q                           >((Q)this->offset  ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "NMS" )  return new module::Decoder_LDPC_BP_horizontal_layered<B,Q,tools::Update_rule_NMS <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS <Q                           >(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "SPA" )  return new module::Decoder_LDPC_BP_horizontal_layered<B,Q,tools::Update_rule_SPA <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_SPA <Q                           >(max_CN_degree    ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "LSPA")  return new module::Decoder_LDPC_BP_horizontal_layered<B,Q,tools::Update_rule_LSPA<Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_LSPA<Q                           >(max_CN_degree    ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "AMS" )
		{
			if (this->min == "MIN" ) return new module::Decoder_LDPC_BP_horizontal_layered<B,Q,tools::Update_rule_AMS <Q,tools::min             <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min             <Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINL") return new module::Decoder_LDPC_BP_horizontal_layered<B,Q,tools::Update_rule_AMS <Q,tools::min_star_linear2<Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min_star_linear2<Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINS") return new module::Decoder_LDPC_BP_horizontal_layered<B,Q,tools::Update_rule_AMS <Q,tools::min_star        <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min_star        <Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
		}
	}
	else if (this->type == "BP_VERTICAL_LAYERED" && this->simd_strategy.empty())
	{
		const auto max_CN_degree = (unsigned int)H.get_cols_max_degree();

		if (this->implem == "MS"  )  return new module::Decoder_LDPC_BP_vertical_layered<B,Q,tools::Update_rule_MS  <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_MS  <Q                           >(                 ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "OMS" )  return new module::Decoder_LDPC_BP_vertical_layered<B,Q,tools::Update_rule_OMS <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_OMS <Q                           >((Q)this->offset  ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "NMS" )  return new module::Decoder_LDPC_BP_vertical_layered<B,Q,tools::Update_rule_NMS <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS <Q                           >(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "SPA" )  return new module::Decoder_LDPC_BP_vertical_layered<B,Q,tools::Update_rule_SPA <Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_SPA <Q                           >(max_CN_degree    ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "LSPA")  return new module::Decoder_LDPC_BP_vertical_layered<B,Q,tools::Update_rule_LSPA<Q                           >>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_LSPA<Q                           >(max_CN_degree    ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "AMS" )
		{
			if (this->min == "MIN" ) return new module::Decoder_LDPC_BP_vertical_layered<B,Q,tools::Update_rule_AMS <Q,tools::min             <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min             <Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINL") return new module::Decoder_LDPC_BP_vertical_layered<B,Q,tools::Update_rule_AMS <Q,tools::min_star_linear2<Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min_star_linear2<Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINS") return new module::Decoder_LDPC_BP_vertical_layered<B,Q,tools::Update_rule_AMS <Q,tools::min_star        <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS <Q,tools::min_star        <Q>>(                 ), this->enable_syndrome, this->syndrome_depth);
		}
	}
	else if (this->type == "BIT_FLIPPING")
	{
		if (this->implem == "WBF" ) return new module::Decoder_LDPC_bit_flipping_OMWBF<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, (Q)0.               , this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "MWBF") return new module::Decoder_LDPC_bit_flipping_OMWBF<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, (Q)this->mwbf_factor, this->enable_syndrome, this->syndrome_depth);
	}
#ifdef __cpp_aligned_new
	else if (this->type == "BP_HORIZONTAL_LAYERED" && this->simd_strategy == "INTER")
	{
		const auto max_CN_degree = (unsigned int)H.get_cols_max_degree();

		if (this->implem == "SPA" ) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_SPA_simd <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_SPA_simd <Q>(max_CN_degree), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "LSPA") return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_LSPA_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_LSPA_simd<Q>(max_CN_degree), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "MS"  ) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_MS_simd  <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_MS_simd  <Q>(             ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "OMS" ) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_OMS_simd <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_OMS_simd <Q>(this->offset ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "NMS" )
		{
			if (typeid(Q) == typeid(int16_t) || typeid(Q) == typeid(int8_t))
			{
				if (this->norm_factor == 0.125f) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,1>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,1>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.250f) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,2>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,2>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.375f) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,3>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,3>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.500f) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,4>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,4>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.625f) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,5>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,5>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.750f) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,6>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,6>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.875f) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,7>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,7>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 1.000f) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,8>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,8>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);

				return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
			}
			else
				return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
		}
		else if (this->implem == "AMS" )
		{
			if (this->min == "MIN" ) return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_i             <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_i             <Q>>(), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINL") return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_star_linear2_i<Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_star_linear2_i<Q>>(), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINS") return new module::Decoder_LDPC_BP_horizontal_layered_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_star_i        <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_star_i        <Q>>(), this->enable_syndrome, this->syndrome_depth);
		}
	}
#endif
#ifdef __cpp_aligned_new
	else if (this->type == "BP_HORIZONTAL_LAYERED_LEGACY" && this->simd_strategy == "INTER")
#else
	else if (this->type == "BP_HORIZONTAL_LAYERED" && this->simd_strategy == "INTER")
#endif
	{
		if (this->implem == "MS" ) return new module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, 1.f              , (Q)0           , this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "NMS") return new module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, this->norm_factor, (Q)0           , this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "OMS") return new module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, 1.f              , (Q)this->offset, this->enable_syndrome, this->syndrome_depth);
	}
#ifdef __cpp_aligned_new
	else if (this->type == "BP_FLOODING" && this->simd_strategy == "INTER")
	{
		const auto max_CN_degree = (unsigned int)H.get_cols_max_degree();

		if (this->implem == "SPA" ) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_SPA_simd <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_SPA_simd <Q>(max_CN_degree), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "LSPA") return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_LSPA_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_LSPA_simd<Q>(max_CN_degree), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "MS"  ) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_MS_simd  <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_MS_simd  <Q>(             ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "OMS" ) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_OMS_simd <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_OMS_simd <Q>(this->offset ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "NMS" )
		{
			if (typeid(Q) == typeid(int16_t) || typeid(Q) == typeid(int8_t))
			{
				if (this->norm_factor == 0.125f) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q,1>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,1>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.250f) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q,2>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,2>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.375f) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q,3>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,3>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.500f) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q,4>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,4>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.625f) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q,5>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,5>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.750f) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q,6>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,6>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.875f) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q,7>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,7>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 1.000f) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q,8>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,8>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);

				return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
			}
			else
				return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_NMS_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
		}
		else if (this->implem == "AMS" )
		{
			if (this->min == "MIN" ) return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_i             <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_i             <Q>>(), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINL") return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_star_linear2_i<Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_star_linear2_i<Q>>(), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINS") return new module::Decoder_LDPC_BP_flooding_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_star_i        <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_star_i        <Q>>(), this->enable_syndrome, this->syndrome_depth);
		}
	}
#endif
#ifdef __cpp_aligned_new
	else if (this->type == "BP_VERTICAL_LAYERED" && this->simd_strategy == "INTER")
	{
		const auto max_CN_degree = (unsigned int)H.get_cols_max_degree();

		if (this->implem == "SPA" ) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_SPA_simd <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_SPA_simd <Q>(max_CN_degree), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "LSPA") return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_LSPA_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_LSPA_simd<Q>(max_CN_degree), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "MS"  ) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_MS_simd  <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_MS_simd  <Q>(             ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "OMS" ) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_OMS_simd <Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_OMS_simd <Q>(this->offset ), this->enable_syndrome, this->syndrome_depth);
		if (this->implem == "NMS" )
		{
			if (typeid(Q) == typeid(int16_t) || typeid(Q) == typeid(int8_t))
			{
				if (this->norm_factor == 0.125f) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,1>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,1>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.250f) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,2>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,2>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.375f) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,3>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,3>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.500f) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,4>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,4>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.625f) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,5>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,5>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.750f) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,6>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,6>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 0.875f) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,7>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,7>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
				if (this->norm_factor == 1.000f) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q,8>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q,8>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);

				return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
			}
			else
				return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_NMS_simd<Q>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_NMS_simd<Q>(this->norm_factor), this->enable_syndrome, this->syndrome_depth);
		}
		else if (this->implem == "AMS" )
		{
			if (this->min == "MIN" ) return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_i             <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_i             <Q>>(), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINL") return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_star_linear2_i<Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_star_linear2_i<Q>>(), this->enable_syndrome, this->syndrome_depth);
			if (this->min == "MINS") return new module::Decoder_LDPC_BP_vertical_layered_inter<B,Q,tools::Update_rule_AMS_simd<Q,tools::min_star_i        <Q>>>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, tools::Update_rule_AMS_simd<Q,tools::min_star_i        <Q>>(), this->enable_syndrome, this->syndrome_depth);
		}
	}
#endif
	else if (this->type == "BP_FLOODING" && this->simd_strategy == "INTRA")
	{
		if (this->implem == "SPA" ) return new module::Decoder_LDPC_BP_flooding_SPA<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, this->enable_syndrome, this->syndrome_depth);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_LDPC
::build(const tools::Sparse_matrix &H, const std::vector<unsigned> &info_bits_pos, module::Encoder<B> *encoder) const
{
	try
	{
		return Decoder::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if ((this->type == "BP" || this->type == "BP_FLOODING") && this->simd_strategy.empty())
		{
			if (this->implem == "GALA") return new module::Decoder_LDPC_BP_flooding_GALA<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, this->enable_syndrome, this->syndrome_depth);
			if (this->implem == "GALB") return new module::Decoder_LDPC_BP_flooding_GALB<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, this->enable_syndrome, this->syndrome_depth);
			if (this->implem == "GALE") return new module::Decoder_LDPC_BP_flooding_GALE<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, this->enable_syndrome, this->syndrome_depth);
		}
		else if (this->type == "BP_PEELING")
		{
			if (this->implem == "STD") return new module::Decoder_LDPC_BP_peeling<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, this->enable_syndrome, this->syndrome_depth);
		}
		else if (this->type == "BIT_FLIPPING")
		{
		    if (this->implem == "PPBF") return new module::Decoder_LDPC_PPBF<B,Q>(this->K, this->N_cw, this->n_ite, H, info_bits_pos, this->ppbf_proba,  this->enable_syndrome, this->syndrome_depth, this->seed);
		}
		return build_siso<B,Q>(H, info_bits_pos);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SISO<B_8 ,Q_8 >* aff3ct::factory::Decoder_LDPC::build_siso<B_8 ,Q_8 >(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SISO<B_16,Q_16>* aff3ct::factory::Decoder_LDPC::build_siso<B_16,Q_16>(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SISO<B_32,Q_32>* aff3ct::factory::Decoder_LDPC::build_siso<B_32,Q_32>(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SISO<B_64,Q_64>* aff3ct::factory::Decoder_LDPC::build_siso<B_64,Q_64>(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SISO<B,Q>* aff3ct::factory::Decoder_LDPC::build_siso<B,Q>(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B>*) const;
#endif

#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_LDPC::build<B_8 ,Q_8 >(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_LDPC::build<B_16,Q_16>(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_LDPC::build<B_32,Q_32>(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_LDPC::build<B_64,Q_64>(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_LDPC::build<B,Q>(const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&, module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
