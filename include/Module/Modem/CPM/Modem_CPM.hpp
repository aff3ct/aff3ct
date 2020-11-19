/*!
 * \file
 * \brief Class module::Modem_CPM.
 */
#ifndef MODEM_CPM_HPP_
#define MODEM_CPM_HPP_

#include <string>
#include <vector>
#include <memory>

#include "Tools/Math/max.h"
#include "Tools/Code/CPM/CPM_parameters.hpp"
#include "Tools/Code/CPM/CPE/Encoder_CPE_Rimoldi.hpp"
#include "Tools/Code/CPM/BCJR/CPM_BCJR.hpp"
#include "Module/Modem/Modem.hpp"

namespace aff3ct
{
namespace module
{
// TODO: warning: working for Rimoldi decomposition only!
template <typename B = int, typename R = float, typename Q = R, tools::proto_max<Q> MAX = tools::max_star>
class Modem_CPM : public Modem<B,R,Q>
{
	using SIN  = B;
	using SOUT = B;

private:
	static const std::string mapping_default;
	static const std::string wave_shape_default;

protected:
	// inputs:
	const bool                                       no_sig2;    // no computation of sigma^2

	// modulation data:
	std::shared_ptr<tools::CPM_parameters<SIN,SOUT>> cpm;        // all CPM parameters
	R                                                cpm_h;      // modulation index = k/p
	R                                                T_samp;     // sample duration  = 1/s_factor
	std::vector<R>                                   baseband;   // translation of base band vectors
	std::vector<R>                                   projection; // translation of filtering generator family
	const int                                        n_sy;       // number of symbols for one frame after encoding without tail symbols
	const int                                        n_sy_tl;    // number of symbols to send for one frame after encoding with tail symbols
	tools::Encoder_CPE_Rimoldi<SIN,SOUT>             cpe;        // the continuous phase encoder

	tools::CPM_BCJR<SIN,SOUT,Q,MAX>                  bcjr;       // demodulator

public:
	Modem_CPM(const int  N,
	          const int  bits_per_symbol    = 1,
	          const int  sampling_factor    = 5,
	          const int  cpm_L              = 3,
	          const int  cpm_k              = 1,
	          const int  cpm_p              = 2,
	          const std::string &mapping    = mapping_default,
	          const std::string &wave_shape = wave_shape_default,
	          const bool no_sig2            = false);
	virtual ~Modem_CPM() = default;

	virtual Modem_CPM<B,R,Q,MAX>* clone() const;

	static bool is_complex_mod();
	static bool is_complex_fil();
	static int size_mod(const int N, const int bps, const int L, const int p, const int ups);
	static int size_fil(const int N, const int bps, const int L, const int p);

protected:
	void   _modulate (                 const B *X_N1,                R *X_N2, const size_t frame_id);
	void     _filter (const float *CP, const R *Y_N1,                R *Y_N2, const size_t frame_id);
	void _demodulate (const float *CP, const Q *Y_N1,                Q *Y_N2, const size_t frame_id);
	void _tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id);

private:
	void generate_baseband    (               );
	void generate_projection  (const R factor );
	R calculate_phase_response(const R t_stamp);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Modem/CPM/Modem_CPM.hxx"
#endif

#endif /* MODEM_CPM_HPP_ */
