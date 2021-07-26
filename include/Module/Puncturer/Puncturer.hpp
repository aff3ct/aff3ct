/*!
 * \file
 * \brief Class module::Puncturer.
 */
#ifndef PUNCTURER_HPP_
#define PUNCTURER_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace pct
	{
		enum class tsk : size_t { puncture, depuncture, SIZE };

		namespace sck
		{
			enum class puncture   : size_t { X_N1, X_N2, status };
			enum class depuncture : size_t { Y_N1, Y_N2, status };
		}
	}

/*!
 * \class Puncturer
 *
 * \brief Punctures a codeword to match a frame size.
 *
 * \tparam B: type of the bits in the frames.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the Puncturer.
 *
 * Please use Puncturer for inheritance (instead of Puncturer)
 */
template <typename B = int, typename Q = float>
class Puncturer : public Module
{
public:
	inline Task&   operator[](const pct::tsk             t);
	inline Socket& operator[](const pct::sck::puncture   s);
	inline Socket& operator[](const pct::sck::depuncture s);

protected:
	const int K;    /*!< Number of information bits in one frame */
	const int N;    /*!< Size of one frame (= number of bits in one frame) */
	const int N_cw; /*!< Real size of the codeword (Puncturer::N_cw >= Puncturer::N) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:    number of information bits in the frame.
	 * \param N:    size of one frame.
	 * \param N_cw: real size of the codeword (Puncturer::N_cw >= Puncturer::N).
	 */
	Puncturer(const int K, const int N, const int N_cw);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Puncturer() = default;

	virtual Puncturer<B,Q>* clone() const;

	int get_K() const;

	int get_N() const;

	int get_N_cw() const;

	/*!
	 * \brief Punctures a codeword.
	 *
	 * \param X_N1: a complete/valid codeword..
	 * \param X_N2: a punctured codeword (corresponding to the frame size).
	 */
	template <class A = std::allocator<B>>
	void puncture(const std::vector<B,A>& X_N1, std::vector<B,A>& X_N2, const int frame_id = -1,
	              const bool managed_memory = true);

	void puncture(const B *X_N1, B *X_N2, const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Depunctures a codeword.
	 *
	 * \param Y_N1: a noised and punctured codeword (corresponding to the frame size).
	 * \param Y_N2: a noised and complete/valid codeword.
	 */
	template <class A = std::allocator<Q>>
	void depuncture(const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2, const int frame_id = -1,
	                const bool managed_memory = true);

	void depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _puncture(const B *X_N1, B *X_N2, const size_t frame_id) const;

	virtual void _depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Puncturer/Puncturer.hxx"
#endif

#endif /* PUNCTURER_HPP_ */
