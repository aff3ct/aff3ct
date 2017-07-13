#ifndef FACTORY_MODEM_HPP
#define FACTORY_MODEM_HPP

#include <string>

#include "Tools/Math/max.h"
#include "Tools/Arguments_reader.hpp"

#include "Module/Modem/Modem.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_modem : public Factory
{
	struct parameters
	{
		int         N;
		int         n_frames;

		// ------- modulator parameters
		std::string type              = "BPSK";    // modulation type (PAM, QAM, ...)
		std::string const_path        = "";        // PATH to constellation file (CSV file)
		bool        complex           = true;      // true if the modulated signal is complex
		int         bps               = 1;         // bits per symbol
		int         upf               = 1;         // samples per symbol

		std::string cpm_std           = "";        // the selection of a default cpm standard hardly implemented (GSM)
		std::string mapping           = "NATURAL"; // symbol mapping layout (natural, gray)
		std::string wave_shape        = "GMSK";    // wave shape (gmsk, rcos, rec)
		int         cpm_L             = 2;         // cpm pulse width or cpm memory
		int         cpm_k             = 1;         // modulation index numerator
		int         cpm_p             = 2;         // modulation index denumerator
		int         N_mod             = 0;         // frame size at the output of the modulator

		// ------- demodulator parameters
		std::string max               = "MAXSS";   // max to use in the demodulation (MAX = max, MAXL = max_linear, MAXS = max_star)
		std::string psi               = "PSI0";    // psi function to use in the SCMA demodulation (PSI0, PSI1, PSI2, PSI3)
		bool        no_sig2           = false;     // do not divide by (sig^2) / 2 in the demodulation
		int         n_ite             = 1;         // number of demodulations/decoding sessions to perform in the BFERI simulations
		int         N_fil             = 0;         // frame size at the output of the filter
	};


	template <typename B = int, typename R = float, typename Q = R>
	static module::Modem<B,R,Q>* build(const parameters &params, const float sigma);

	static int get_buffer_size_after_modulation(const std::string type,
	                                            const int         N,
	                                            const int         bps   = 1,
	                                            const int         upf   = 5,
	                                            const int         cpm_L = 3);

	static int get_buffer_size_after_filtering(const std::string type,
	                                           const int         N,
	                                           const int         bps   = 1,
	                                           const int         cpm_L = 3,
	                                           const int         cpm_p = 2);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_mod, params_list& head_demod, const parameters& params);

private:
	template <typename B = int, typename R = float, typename Q = R, proto_max<Q> MAX>
	static module::Modem<B,R,Q>* _build(const parameters &params, const float sigma);

	template <typename B = int, typename R = float, typename Q = R>
	static module::Modem<B,R,Q>* _build_scma(const parameters &params, const float sigma);
};
}
}

#endif /* FACTORY_MODEM_HPP */
