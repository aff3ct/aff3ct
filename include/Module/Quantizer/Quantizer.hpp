/*!
 * \file
 * \brief Class module::Quantizer.
 */
#ifndef QUANTIZER_HPP_
#define QUANTIZER_HPP_

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
	namespace qnt
	{
		enum class tsk : size_t { process, SIZE };

		namespace sck
		{
			enum class process : size_t { Y_N1, Y_N2, SIZE };
		}
	}

/*!
 * \class Quantizer
 *
 * \brief Quantizes floating-point data to fixed-point representation.
 *
 * \tparam R: type of the reals (floating-point representation) in the Quantizer.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the Quantizer.
 *
 * Please use Quantizer for inheritance (instead of Quantizer).
 * If Q is a floating-point representation then the Quantizer does nothing more than a cast.
 */
template <typename R = float, typename Q = int>
class Quantizer : public Module
{
public:
	inline Task&   operator[](const qnt::tsk          t);
	inline Socket& operator[](const qnt::sck::process s);

protected:
	const int N; /*!< Size of one frame (= number of bits in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Decoder.
	 */
	Quantizer(const int N, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Quantizer() = default;

	int get_N() const;

	/*!
	 * \brief Quantizes the data if Q is a fixed-point representation, does nothing else.
	 *
	 * \param Y_N1: a vector of floating-point data.
	 * \param Y_N2: a vector of quantized data (fixed-point representation).
	 */
	template <class AR = std::allocator<R>, class AQ = std::allocator<Q>>
	void process(const std::vector<R,AR>& Y_N1, std::vector<Q,AQ>& Y_N2, const int frame_id = -1);

	virtual void process(const R *Y_N1, Q *Y_N2, const int frame_id = -1);

protected:
	virtual void _process(const R *Y_N1, Q *Y_N2, const int frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Quantizer/Quantizer.hxx"
#endif

#endif /* QUANTIZER_HPP_ */
