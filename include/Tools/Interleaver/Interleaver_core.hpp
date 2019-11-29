/*!
 * \file
 * \brief Class tools::Interleaver_core.
 */
#ifndef INTERLEAVER_CORE_HPP_
#define INTERLEAVER_CORE_HPP_

#include <cstdint>
#include <string>
#include <vector>

#include "Tools/Interface/Interface_set_seed.hpp"
#include "Tools/Interface/Interface_clone.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core : public Interface_set_seed, public Interface_clone
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

	virtual Interleaver_core<T>* clone() const = 0;

	const std::vector<T>& get_lut() const;

	const std::vector<T>& get_lut_inv() const;

	int get_size() const;

	int get_n_frames() const;

	bool is_uniform() const;

	std::string get_name() const;

	void refresh();

	virtual void set_seed(const int seed);

protected:
	bool is_initialized() const;

	void init();

	virtual void gen_lut(T *lut, const int frame_id) = 0;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Interleaver/Interleaver_core.hxx"
#endif

#endif /* INTERLEAVER_CORE_HPP_ */
