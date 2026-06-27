/*!
 * \file
 * \brief Class factory::Decoder_polar_PAC.
 */
#ifndef FACTORY_DECODER_POLAR_PAC_HPP
#define FACTORY_DECODER_POLAR_PAC_HPP

#include <cli.hpp>
#include <map>
#include <string>
#include <vector>

#include "Factory/Module/Decoder/Decoder.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Tools/Factory/Header.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_polar_PAC_name;
extern const std::string Decoder_polar_PAC_prefix;
class Decoder_polar_PAC : public Decoder
{
  public:
    // ----------------------------------------------------------------------------------------------------- PARAMETERS
    // optional parameters
    std::string conv = "NO";
    int L = 8;
    float DELTA = 1.0f;
    float p_err_snr = 2.35;

    // -------------------------------------------------------------------------------------------------------- METHODS
    explicit Decoder_polar_PAC(const std::string& p = Decoder_polar_PAC_prefix);
    virtual ~Decoder_polar_PAC() = default;
    Decoder_polar_PAC* clone() const;

    // parameters construction
    virtual void get_description(cli::Argument_map_info& args) const;
    virtual void store(const cli::Argument_map_value& vals);
    virtual void get_headers(std::map<std::string, tools::header_list>& headers, const bool full = true) const;

    // builder
    template<typename B = int, typename Q = float>
    module::Decoder_SIHO<B, Q>* build(const std::vector<bool>& frozen_bits,
                                      const std::vector<double>& Perr,
                                      const module::CRC<B>* crc = nullptr,
                                      module::Encoder<B>* encoder = nullptr) const;

  protected:
    Decoder_polar_PAC(const std::string& n, const std::string& p);
};
}
}

#endif /* FACTORY_DECODER_POLAR_PAC_HPP */
