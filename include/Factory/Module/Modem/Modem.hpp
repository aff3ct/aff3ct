/*!
 * \file
 * \brief Class factory::Modem.
 */
#ifndef FACTORY_MODEM_HPP
#define FACTORY_MODEM_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Math/max.h"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/Constellation/Constellation.hpp"
#include "Module/Modem/Modem.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Modem_name;
extern const std::string Modem_prefix;
class Modem : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         N          = 0;

	// optional parameters
	// ------- modulator parameters
	std::string type          = "BPSK"; // modulation type (PAM, QAM, ...)
	std::string implem        = "STD";
	std::string const_path    = "";     // PATH to constellation file (CSV file)
	std::string codebook_path = "";     // PATH to codebook file
	bool        complex       = true;   // true if the modulated signal is complex
	int         bps           = 1;      // bits per symbol

	// -------- CPM parameters
	std::string cpm_std        = "";        // the selection of a default cpm standard hardly implemented (GSM)
	std::string cpm_mapping    = "NATURAL"; // symbol mapping layout (natural, gray)
	std::string cpm_wave_shape = "GMSK";    // wave shape (gmsk, rcos, rec)
	int         cpm_L          = 2;         // cpm pulse width or cpm memory
	int         cpm_k          = 1;         // modulation index numerator
	int         cpm_p          = 2;         // modulation index denumerator
	int         cpm_upf        = 1;         // samples per symbol
	int         N_mod          = 0;         // frame size at the output of the modulator

	// ------- demodulator parameters
	std::string max          = "MAX";  // max to use in the demodulation (MAX = max, MAXL = max_linear, MAXS = max_star)
	std::string psi          = "PSI0"; // psi function to use in the SCMA demodulation (PSI0, PSI1, PSI2, PSI3)
	bool        no_sig2      = false;  // do not divide by (sig^2) / 2 in the demodulation
	int         n_ite        = 1;      // number of demodulations/decoding sessions to perform in the BFERI simulations
	int         N_fil        = 0;      // frame size at the output of the filter
	int         rop_est_bits = 0;      // The number of bits known by the Modem_OOK_optical_rop_estimate demodulator
	                                   // to estimate the ROP

	// ------- common parameters
	std::string channel_type = "AWGN"; // the channel type used to build correct OOK modulation

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Modem(const std::string &p = Modem_prefix);
	virtual ~Modem() = default;
	Modem* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename R = float, typename Q = R>
	module::Modem<B,R,Q>* build(const tools::Constellation<R>* cstl = nullptr) const;
	template <typename B = int, typename R = float, typename Q = R>
	module::Modem<B,R,Q>* build(const tools::Distributions<R>& dist) const;

	template <typename R = float>
	tools::Constellation<R>* build_constellation() const;

	static bool has_constellation(const std::string &type);

	static bool is_complex_mod(const std::string &type, const int bps = 1, const tools::Constellation<float>* c = nullptr);
	static bool is_complex_fil(const std::string &type, const int bps = 1, const tools::Constellation<float>* c = nullptr);

	static int get_buffer_size_after_modulation(const std::string &type,
	                                            const int         N,
	                                            const int         bps     = 1,
	                                            const int         cpm_upf = 5,
	                                            const int         cpm_L   = 3,
	                                            const int         cpm_p   = 2,
	                                            const tools::Constellation<float>* c = nullptr);

	static int get_buffer_size_after_filtering(const std::string &type,
	                                           const int         N,
	                                           const int         bps   = 1,
	                                           const int         cpm_L = 3,
	                                           const int         cpm_p = 2,
	                                           const tools::Constellation<float>* c = nullptr);

private:
	template <typename B = int, typename R = float, typename Q = R, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
	inline module::Modem<B,R,Q>* _build(const tools::Constellation<R>* cstl) const;

	template <typename B = int, typename R = float, typename Q = R>
	inline module::Modem<B,R,Q>* _build_scma() const;
};
}
}

#endif /* FACTORY_MODEM_HPP */
