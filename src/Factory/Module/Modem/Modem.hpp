#ifndef FACTORY_MODEM_HPP
#define FACTORY_MODEM_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
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
struct Modem : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         N          = 0;

		// optional parameters
		// ------- modulator parameters
		std::string type       = "BPSK";    // modulation type (PAM, QAM, ...)
		std::string implem     = "STD";
		std::string const_path = "";        // PATH to constellation file (CSV file)
		std::string codebook   = "";        // PATH to codebook file
		bool        complex    = true;      // true if the modulated signal is complex
		int         bps        = 1;         // bits per symbol

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
		float       noise        = -1.f;   // noise value
		int         rop_est_bits = 0;      // The number of bits known by the Modem_OOK_optical_rop_estimate demodulator
		                                   // to estimate the ROP

		// ------- common parameters
		int         n_frames     = 1;
		std::string channel_type = "AWGN"; // the channel type used to build correct OOK modulation

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Modem_prefix);
		virtual ~parameters() = default;
		Modem::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float, typename Q = R>
		module::Modem<B,R,Q>* build() const;
		template <typename B = int, typename R = float, typename Q = R>
		module::Modem<B,R,Q>* build(const tools::Distributions<R>& dist) const;

	private:
		template <typename B = int, typename R = float, typename Q = R, tools::proto_max<Q> MAX>
		inline module::Modem<B,R,Q>* _build() const;

		template <typename B = int, typename R = float, typename Q = R>
		inline module::Modem<B,R,Q>* _build_scma() const;

		template <typename R = float>
		tools::Constellation<R>* build_constellation() const;
	};

	template <typename B = int, typename R = float, typename Q = R>
	static module::Modem<B,R,Q>* build(const parameters &params);

	template <typename B = int, typename R = float, typename Q = R>
	static module::Modem<B,R,Q>* build(const parameters &params, const tools::Distributions<R>& dist);


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
};
}
}

#endif /* FACTORY_MODEM_HPP */
