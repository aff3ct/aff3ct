#ifndef MODULATOR_GSM_HPP_
#define MODULATOR_GSM_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Math/max.h"

#include "../Modulator.hpp"
#include "BCJR/Modulator_GSM_BCJR.hpp"

template <typename B, typename R, typename Q, proto_max<Q> MAX>
class Modulator_GSM : public Modulator<B,R,Q>
{
private:
	// ------------------------------------------------------------------------
	// specs of the BCJR (used in the demodulation process) -------------------
	const int BCJR_n_states        = 8;
	const int BCJR_m_order         = 2;
	const int BCJR_n_bits_per_symb = 1;
	const int BCJR_n_prev_branches = 2;

	const static std::vector<int> BCJR_binary_symbols;
	const static std::vector<int> BCJR_trellis;
	const static std::vector<int> BCJR_anti_trellis;
	// ------------------------------------------------------------------------

protected:
	const R sigma;
	const bool disable_sig2;
	const int n_output_symbs   = 16;
	const int up_sample_factor = 5;

	const static mipp::vector<R> baseband;   // translation of base band vectors           (80 complex elmts)
	const static mipp::vector<R> projection; // translation of filtering generator familly (80 complex elmts)

	// modulate data
	mipp::vector<B> parity_enc;

	// demodulate data
	Modulator_GSM_BCJR<Q,MAX> BCJR;

public:
	Modulator_GSM(const int N, const R sigma, const mipp::vector<R> &H, const bool disable_sig2 = false,
	              const std::string name = "Modulator_GSM", const bool tbless = false);
	virtual ~Modulator_GSM();

	int get_buffer_size_after_modulation(const int N);
	int get_buffer_size_after_filtering (const int N);

	void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2);
	void     filter(const mipp::vector<R>& Y_N1,                              mipp::vector<R>& Y_N2);
	void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2);
	void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3);

	void set_n_frames(const int n_frames);

protected:
	void _modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2);
	void   _filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2);

};

#include "Modulator_GSM.hxx"

#endif /* MODULATOR_GSM_HPP_ */
