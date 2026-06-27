#include <algorithm>
#include <memory>
#include <streampu.hpp>
#include <utility>

#include "Factory/Module/Decoder/Polar_PAC/Decoder_polar_PAC.hpp"
#include "Module/Decoder/Polar_PAC/Fano/Decoder_polar_PAC_Fano_naive.hpp"
#include "Module/Decoder/Polar_PAC/SCL/Decoder_polar_PAC_SCL_naive.hpp"
#include "Tools/Documentation/documentation.h"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_polar_PAC_name = "Decoder Polar PAC";
const std::string aff3ct::factory::Decoder_polar_PAC_prefix = "dec";

Decoder_polar_PAC ::Decoder_polar_PAC(const std::string& prefix)
  : Decoder(Decoder_polar_PAC_name, prefix)
{
    this->type = "SCL";
    this->implem = "NAIVE";
}

Decoder_polar_PAC ::Decoder_polar_PAC(const std::string& name, const std::string& prefix)
  : Decoder(name, prefix)
{
    this->type = "SCL";
    this->implem = "NAIVE";
}

Decoder_polar_PAC*
Decoder_polar_PAC ::clone() const
{
    return new Decoder_polar_PAC(*this);
}

void
Decoder_polar_PAC ::get_description(cli::Argument_map_info& args) const
{
    Decoder::get_description(args);

    auto p = this->get_prefix();
    const std::string class_name = "factory::Decoder_polar_PAC::";

    cli::add_options(args.at({ p + "-type", "D" }), 0, "SCL", "FANO");

    args.at({ p + "-implem" })->change_type(cli::Text(cli::Example_set("NAIVE")));

    tools::add_arg(args, p, class_name + "p+lists,L", cli::Integer(cli::Positive(), cli::Non_zero()));
    tools::add_arg(args, p, class_name + "p+delta", cli::Real());
    tools::add_arg(args, p, class_name + "p+perr-snr", cli::Real());
}

void
Decoder_polar_PAC ::store(const cli::Argument_map_value& vals)
{
    Decoder::store(vals);

    auto p = this->get_prefix();
    if (this->type == "SCL")
        if (vals.exist({ p + "-lists", "L" })) this->L = vals.to_int({ p + "-lists", "L" });

    if (this->type == "FANO")
    {
        if (vals.exist({ p + "-delta" })) this->DELTA = vals.to_float({ p + "-delta" });
        if (vals.exist({ p + "-perr-snr" })) this->p_err_snr = vals.to_float({ p + "-perr-snr" });
    }
}

void
Decoder_polar_PAC ::get_headers(std::map<std::string, tools::header_list>& headers, const bool full) const
{
    Decoder::get_headers(headers, full);

    if (this->type != "ML" && this->type != "CHASE")
    {
        auto p = this->get_prefix();

        if (this->type == "SCL") headers[p].push_back(std::make_pair("Num. of lists (L)", std::to_string(this->L)));
        if (this->type == "FANO")
        {
            headers[p].push_back(std::make_pair("Delta", std::to_string(this->DELTA)));
            headers[p].push_back(std::make_pair("Error Probablity SNR", std::to_string(this->p_err_snr)));
        }
    }
}

template<typename B, typename Q>
module::Decoder_SIHO<B, Q>*
Decoder_polar_PAC ::build(const std::vector<bool>& frozen_bits,
                          const std::vector<double>& Perr,
                          const module::CRC<B>* crc,
                          module::Encoder<B>* encoder) const
{
    if (this->type == "SCL" && this->implem == "NAIVE")
        return new module::Decoder_polar_PAC_SCL_naive<B, Q, tools::f_LLR<Q>, tools::g_LLR<B, Q>>(
          this->K, this->N_cw, this->L, frozen_bits, this->conv);

    if (this->type == "FANO" && this->implem == "NAIVE")
        return new module::Decoder_polar_PAC_Fano_naive<B, Q, tools::f_LLR<Q>, tools::g_LLR<B, Q>>(
          this->K, this->N_cw, frozen_bits, this->conv, this->DELTA, Perr);

    throw spu::tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8, Q_8>*
aff3ct::factory::Decoder_polar_PAC::build<B_8, Q_8>(const std::vector<bool>&,
                                                    const std::vector<double>&,
                                                    const module::CRC<B_8>*,
                                                    module::Encoder<B_8>*) const;
template aff3ct::module::Decoder_SIHO<B_16, Q_16>*
aff3ct::factory::Decoder_polar_PAC::build<B_16, Q_16>(const std::vector<bool>&,
                                                      const std::vector<double>&,
                                                      const module::CRC<B_16>*,
                                                      module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32, Q_32>*
aff3ct::factory::Decoder_polar_PAC::build<B_32, Q_32>(const std::vector<bool>&,
                                                      const std::vector<double>&,
                                                      const module::CRC<B_32>*,
                                                      module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64, Q_64>*
aff3ct::factory::Decoder_polar_PAC::build<B_64, Q_64>(const std::vector<bool>&,
                                                      const std::vector<double>&,
                                                      const module::CRC<B_64>*,
                                                      module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B, Q>*
aff3ct::factory::Decoder_polar_PAC::build<B, Q>(const std::vector<bool>&,
                                                const module::CRC<B>*,
                                                module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
