#ifndef INTERLEAVER_CORE_HPP_
#define INTERLEAVER_CORE_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core
{
protected:
	const int size;
	const std::string name;
	const int n_frames;
	      bool uniform;
	      bool initialized;
	std::vector<T> pi;     /*!< Lookup table for the interleaving process :
	                            the interleaving output position i can be found in the source at the position 'pi[i]' */
	std::vector<T> pi_inv; /*!< Lookup table for the deinterleaving process */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param size:     number of the data to interleave or to deinterleave.
	 * \param n_frames: number of frames to process in the Interleaver.
	 * \param name:     Interleaver's name.
	 */
	Interleaver_core(const int size, const std::string &name, const bool uniform = false, const int n_frames = 1);

	virtual ~Interleaver_core() = default;

	const std::vector<T>& get_lut() const;

	const std::vector<T>& get_lut_inv() const;

	int get_size() const;

	int get_n_frames() const;

	bool is_uniform() const;

	bool is_initialized() const;

	std::string get_name() const;

	void init();

	void refresh();

protected:
	virtual void gen_lut(T *lut, const int frame_id) = 0;
};
}
}

#include "Tools/Interleaver/Interleaver_core.hxx"

#endif /* INTERLEAVER_CORE_HPP_ */
