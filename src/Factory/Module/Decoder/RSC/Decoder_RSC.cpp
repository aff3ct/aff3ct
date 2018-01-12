#include <type_traits>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_std.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_scan.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_very_fast.hpp"

#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std.hpp"
#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std_json.hpp"

#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_std.hpp"
#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_SSE.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_AVX.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x4_AVX.hpp"

#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_std.hpp"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_very_fast.hpp"

#include "Decoder_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RSC::name   = "Decoder RSC";
const std::string aff3ct::factory::Decoder_RSC::prefix = "dec";

Decoder_RSC::parameters
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_RSC::name, prefix)
{
	this->type   = "BCJR";
	this->implem = "STD";
}

Decoder_RSC::parameters
::~parameters()
{
}

Decoder_RSC::parameters* Decoder_RSC::parameters
::clone() const
{
	return new Decoder_RSC::parameters(*this);
}

void Decoder_RSC::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Decoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-cw-size", "N"});

	opt_args[{p+"-type", "D"}].push_back("BCJR");

	opt_args[{p+"-implem"}].push_back("GENERIC, STD, FAST, VERY_FAST");

	opt_args[{p+"-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};

	opt_args[{p+"-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXL, MAXS"};

	opt_args[{p+"-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};

	opt_args[{p+"-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{p+"-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};
}

void Decoder_RSC::parameters
::store(const arg_val_map &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-simd"   })) this->simd_strategy = vals.at({p+"-simd"});
	if(exist(vals, {p+"-max"    })) this->max           = vals.at({p+"-max" });
	if(exist(vals, {p+"-std"    })) this->standard      = vals.at({p+"-std" });
	if(exist(vals, {p+"-no-buff"})) this->buffered      = false;

	if (this->standard == "LTE" && !exist(vals, {p+"-poly"}))
		this->poly = {013, 015};

	if (this->standard == "CCSDS" && !exist(vals, {p+"-poly"}))
		this->poly = {023, 033};

	if (exist(vals, {p+"-poly"}))
	{
		auto poly_str = vals.at({p+"-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &this->poly[0], &this->poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &this->poly[0], &this->poly[1]);
#endif
	}

	if (this->poly[0] != 013 || this->poly[1] != 015)
		this->implem = "GENERIC";

	this->tail_length = (int)(2 * std::floor(std::log2((float)std::max(this->poly[0], this->poly[1]))));
	this->N_cw        = 2 * this->K + this->tail_length;
	this->R           = (float)this->K / (float)this->N_cw;
}

void Decoder_RSC::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->tail_length && full)
		headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));

	if (full) headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));

	if (!this->standard.empty())
		headers[p].push_back(std::make_pair("Standard", this->standard));

	std::stringstream poly;
	poly << "{0" << std::oct << this->poly[0] << ",0" << std::oct << this->poly[1] << "}";
	headers[p].push_back(std::make_pair(std::string("Polynomials"), poly.str()));

	if (!this->simd_strategy.empty())
		headers[p].push_back(std::make_pair(std::string("SIMD strategy"), this->simd_strategy));

	headers[p].push_back(std::make_pair(std::string("Max type"), this->max));
}

template <typename B, typename Q, typename QD, tools::proto_max<Q> MAX1, tools::proto_max<QD> MAX2>
module::Decoder_SISO_SIHO<B,Q>* Decoder_RSC::parameters
::_build_seq(const std::vector<std::vector<int>> &trellis,
                   std::ostream                  &stream,
             const int                            n_ite) const
{
	if (this->type == "BCJR")
	{
		     if (this->implem == "STD"         ) return new module::Decoder_RSC_BCJR_seq_std             <B,Q,QD,MAX1,MAX2>(this->K, trellis,        this->buffered,         this->n_frames);
		else if (this->implem == "GENERIC"     ) return new module::Decoder_RSC_BCJR_seq_generic_std     <B,Q,QD,MAX1,MAX2>(this->K, trellis,        this->buffered,         this->n_frames);
		else if (this->implem == "GENERIC_JSON") return new module::Decoder_RSC_BCJR_seq_generic_std_json<B,Q,QD,MAX1,MAX2>(this->K, trellis, n_ite, this->buffered, stream, this->n_frames);
		else if (this->implem == "FAST"        ) return new module::Decoder_RSC_BCJR_seq_fast            <B,Q,QD,MAX1,MAX2>(this->K, trellis,        this->buffered,         this->n_frames);
		else if (this->implem == "VERY_FAST"   ) return new module::Decoder_RSC_BCJR_seq_very_fast       <B,Q,QD,MAX1,MAX2>(this->K, trellis,        this->buffered,         this->n_frames);
		else if (this->implem == "SCAN"        ) return new module::Decoder_RSC_BCJR_seq_scan            <B,Q,QD          >(this->K, trellis,        this->buffered,         this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, typename QD, tools::proto_max_i<Q> MAX>
module::Decoder_SISO_SIHO<B,Q>* Decoder_RSC::parameters
::_build_simd(const std::vector<std::vector<int>> &trellis) const
{
	if (this->type == "BCJR" && this->simd_strategy == "INTER")
	{
		     if (this->implem == "STD"      ) return new module::Decoder_RSC_BCJR_inter_std      <B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames);
		else if (this->implem == "FAST"     ) return new module::Decoder_RSC_BCJR_inter_fast     <B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames);
		else if (this->implem == "VERY_FAST") return new module::Decoder_RSC_BCJR_inter_very_fast<B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames);
	}

	if (this->type == "BCJR" && this->simd_strategy == "INTRA")
	{
		if (this->implem == "STD")
		{
			switch (mipp::nElReg<Q>())
			{
				case 8: return new module::Decoder_RSC_BCJR_intra_std<B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames);
				default:
					break;
			}
		}
		else if (this->implem == "FAST")
		{
#ifdef __AVX__
			switch (mipp::nElReg<Q>())
			{
				case 8:  return new module::Decoder_RSC_BCJR_intra_fast             <B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames); break;
				case 16: return new module::Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames); break;
				case 32: return new module::Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames); break;
				default:
					break;
			}
#else /* NEON and SSE */
			switch (mipp::nElReg<Q>())
			{
				case 8:  return new module::Decoder_RSC_BCJR_intra_fast             <B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames); break;
				case 16: return new module::Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames); break;
				default:
					break;
			}
#endif
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_RSC::parameters
::build(const std::vector<std::vector<int>>   &trellis,
              std::ostream                    &stream,
        const int                              n_ite) const
{
	using QD = typename std::conditional<std::is_same<Q,int8_t>::value,int16_t,Q>::type;

	if (this->simd_strategy.empty())
	{
		     if (this->max == "MAX" ) return _build_seq<B,Q,QD,tools::max       <Q>,tools::max       <QD>>(trellis, stream, n_ite);
		else if (this->max == "MAXS") return _build_seq<B,Q,QD,tools::max_star  <Q>,tools::max_star  <QD>>(trellis, stream, n_ite);
		else if (this->max == "MAXL") return _build_seq<B,Q,QD,tools::max_linear<Q>,tools::max_linear<QD>>(trellis, stream, n_ite);
	}
	else
	{
		     if (this->max == "MAX" ) return _build_simd<B,Q,QD,tools::max_i       <Q>>(trellis);
		else if (this->max == "MAXS") return _build_simd<B,Q,QD,tools::max_star_i  <Q>>(trellis);
		else if (this->max == "MAXL") return _build_simd<B,Q,QD,tools::max_linear_i<Q>>(trellis);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_RSC
::build(const parameters                      &params,
        const std::vector<std::vector<int>>   &trellis,
              std::ostream                    &stream,
        const int                              n_ite)
{
	return params.template build<B,Q>(trellis, stream, n_ite);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RSC::parameters::build<B_8 ,Q_8 >(const std::vector<std::vector<int>>&, std::ostream&, const int) const;
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RSC::parameters::build<B_16,Q_16>(const std::vector<std::vector<int>>&, std::ostream&, const int) const;
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RSC::parameters::build<B_32,Q_32>(const std::vector<std::vector<int>>&, std::ostream&, const int) const;
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RSC::parameters::build<B_64,Q_64>(const std::vector<std::vector<int>>&, std::ostream&, const int) const;
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RSC::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RSC::build<B_16,Q_16>(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RSC::build<B_32,Q_32>(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RSC::build<B_64,Q_64>(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
#else
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_RSC::parameters::build<B,Q>(const std::vector<std::vector<int>>&, std::ostream&, const int) const;
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_RSC::build<B,Q>(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
#endif
// ==================================================================================== explicit template instantiation
