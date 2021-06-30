/*!
 * \file
 * \brief Class module::Modem.
 */
#ifndef MODEM_HPP_
#define MODEM_HPP_

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

#include "Tools/Noise/Noise.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace mdm
	{
		enum class tsk : size_t { modulate, tmodulate, filter, demodulate, tdemodulate, demodulate_wg, tdemodulate_wg, SIZE };

		namespace sck
		{
			enum class modulate       : size_t {          X_N1, X_N2      , status };
			enum class tmodulate      : size_t {          X_N1, X_N2      , status };
			enum class filter         : size_t { CP,      Y_N1, Y_N2      , status };
			enum class demodulate     : size_t { CP,      Y_N1, Y_N2      , status };
			enum class tdemodulate    : size_t { CP,      Y_N1, Y_N2, Y_N3, status };
			enum class demodulate_wg  : size_t { CP, H_N, Y_N1, Y_N2      , status };
			enum class tdemodulate_wg : size_t { CP, H_N, Y_N1, Y_N2, Y_N3, status };
		}
	}

/*!
 * \class Modem
 *
 * \brief Modulates, filters and/or demodulates a signal.
 *
 * \tparam B: type of the bits or symbols to modulate.
 * \tparam R: type of the reals (floating-point representation) in the modulation and in the filtering process.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the demodulation.
 *
 * Please use Modem for inheritance (instead of Modem)
 */
template <typename B = int, typename R = float, typename Q = R>
class Modem : public Module
{
public:
	inline Task&   operator[](const mdm::tsk                 t);
	inline Socket& operator[](const mdm::sck::modulate       s);
	inline Socket& operator[](const mdm::sck::tmodulate      s);
	inline Socket& operator[](const mdm::sck::filter         s);
	inline Socket& operator[](const mdm::sck::demodulate     s);
	inline Socket& operator[](const mdm::sck::tdemodulate    s);
	inline Socket& operator[](const mdm::sck::demodulate_wg  s);
	inline Socket& operator[](const mdm::sck::tdemodulate_wg s);

protected:
	const int N;     /*!< Size of one frame (= number of bits in one frame) */
	const int N_mod; /*!< Number of transmitted elements after the modulation (could be smaller, bigger or equal to N) */
	const int N_fil; /*!< Number of transmitted elements after the filtering process */

	bool  enable_filter;
	bool  enable_demodulator;
	float last_channel_param;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:     size of one frame (= number of bits in one frame).
	 * \param N_mod: number of transmitted elements after the modulation (could be smaller, bigger or equal to N).
	 * \param N_fil: number of transmitted elements after the filtering process.
	 */
	Modem(const int N, const int N_mod, const int N_fil);

	/*!
	 * \brief Constructor (assumes that nothing is done in the filtering process).
	 *
	 * \param N:     size of one frame (= number of bits in one frame).
	 * \param N_mod: number of transmitted elements after the modulation (could be smaller, bigger or equal to N).
	 */
	Modem(const int N, const int N_mod);

	/*!
	 * \brief Constructor (assumes that nothing is done in the filtering process).
	 *
	 * \param N: size of one frame (= number of bits in one frame).
	 */
	Modem(const int N);

	virtual Modem<B,R,Q>* clone() const;

	void init_processes();

	/*!
	 * \brief Destructor.
	 */
	virtual ~Modem() = default;

	int get_N() const;

	int get_N_mod() const;

	int get_N_fil() const;

	bool is_filter() const;

	bool is_demodulator() const;

	/*!
	 * \brief Task method that modulates a vector of bits or symbols.
	 *
	 * \param X_N1: a vector of bits or symbols.
	 * \param X_N2: a vector of modulated bits or symbols.
	 */
	template <class AB = std::allocator<B>, class AR = std::allocator<R>>
	void modulate(const std::vector<B,AB>& X_N1, std::vector<R,AR>& X_N2, const int frame_id = -1,
	              const bool managed_memory = true);

	void modulate(const B *X_N1, R *X_N2, const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Task method that modulates softly a vector of LLRs.
	 *
	 * \param X_N1: a vector of LLRs.
	 * \param X_N2: a vector of soft symbols.
	 */
	template <class AQ = std::allocator<Q>, class AR = std::allocator<R>>
	void tmodulate(const std::vector<Q,AQ>& X_N1, std::vector<R,AR>& X_N2, const int frame_id = -1,
	               const bool managed_memory = true);

	void tmodulate(const Q *X_N1, R *X_N2, const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Task method that filters a vector of noised and modulated bits/symbols.
	 *
	 * By default this method does nothing.
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a filtered vector.
	 */
	template <class A = std::allocator<R>>
	void filter(const std::vector<float,A>& CP, const std::vector<R,A>& Y_N1, std::vector<R,A>& Y_N2,
	            const int frame_id = -1, const bool managed_memory = true);

	void filter(const float *CP, const R *Y_N1, R *Y_N2, const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Task method that demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a demodulated vector.
	 */
	template <class A = std::allocator<Q>>
	void demodulate(const std::vector<float,A>& CP, const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2,
	                const int frame_id = -1, const bool managed_memory = true);

	void demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const int frame_id = -1,
	                const bool managed_memory = true);

	/*!
	 * \brief Task method that demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * \param H_N:  channel gains.
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a demodulated vector.
	 */
	template <class AQ = std::allocator<Q>, class AR = std::allocator<R>>
	void demodulate_wg(const std::vector<float,AR>& CP, const std::vector<R,AR>& H_N, const std::vector<Q,AQ>& Y_N1,
	                   std::vector<Q,AQ>& Y_N2, const int frame_id = -1, const bool managed_memory = true);

	void demodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id = -1,
	                   const bool managed_memory = true);

	/*!
	 * \brief Task method that demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * Used for the iterative turbo demodulation technique, this type of demodulation takes the decoder information
	 * into account.
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a vector of extrinsic information coming from a SISO decoder (used in the iterative turbo
	 *              demodulation technique).
	 * \param Y_N3: a demodulated vector.
	 */
	template <class A = std::allocator<Q>>
	void tdemodulate(const std::vector<float,A>& CP, const std::vector<Q,A>& Y_N1, const std::vector<Q,A>& Y_N2,
	                 std::vector<Q,A>& Y_N3, const int frame_id = -1, const bool managed_memory = true);

	void tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id = -1,
	                 const bool managed_memory = true);

	/*!
	 * \brief Task that demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * Used for the iterative turbo demodulation technique, this type of demodulation takes the decoder information
	 * into account.
	 *
	 * \param H_N:  channel gains.
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a vector of extrinsic information coming from a SISO decoder (used in the iterative turbo
	 *              demodulation technique).
	 * \param Y_N3: a demodulated vector.
	 */
	template <class AQ = std::allocator<Q>, class AR = std::allocator<R>>
	void tdemodulate_wg(const std::vector<float,AR>& CP, const std::vector<R,AR>& H_N, const std::vector<Q,AQ>& Y_N1,
	                    const std::vector<Q,AQ>& Y_N2, std::vector<Q,AQ>& Y_N3, const int frame_id = -1,
	                    const bool managed_memory = true);

	void tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3,
	                    const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Gets the vector size after the modulation (considering a given frame size).
	 *
	 * \param N        : a frame size.
	 * \param n_b_per_s: number of bits per symbols
	 * \param tl       : tail length of the modulation (in number of symbols)
	 * \param s_factor : the sampling factor (number of samples per symbol)
	 * \param complex  : if true, the samples are in a complex form (so the size is twice longer)
	 *
	 * \return the vector size after the modulation.
	 */
	static int get_buffer_size_after_modulation(const int N, const int n_b_per_s, const int tl,
	                                            const int s_factor, const bool complex);

	/*!
	 * \brief Gets the vector size after the filtering process
	 *
	 * \param N        : a frame size.
	 * \param n_b_per_s: number of bits per symbols
	 * \param tl       : tail length of the modulation (in number of symbols)
	 * \param max_wa_id: the maximum number of possible wave forms
	 * \param complex  : if true, the samples are in a complex form (so the size is twice longer)
	 *
	 * \return the vector size after the modulation.
	 */
	static int get_buffer_size_after_filtering(const int N, const int n_b_per_s, const int tl, const int max_wa_id,
	                                           const bool complex);

protected:
	virtual void _modulate(const B *X_N1, R *X_N2, const size_t frame_id);

	virtual void _tmodulate(const Q *X_N1, R *X_N2, const size_t frame_id);

	virtual void _filter(const float *CP, const R *Y_N1, R *Y_N2, const size_t frame_id);

	virtual void _demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id);

	virtual void _demodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id);

	virtual void _tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id);

	virtual void _tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3,
	                             const size_t frame_id);

	void set_filter(const bool filter);

	void set_demodulator(const bool demodulator);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Modem/Modem.hxx"
#endif

#endif /* MODEM_HPP_ */
