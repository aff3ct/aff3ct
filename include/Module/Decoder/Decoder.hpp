/*!
 * \file
 * \brief Class module::Decoder.
 */
#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <cstdint>
#include <cstddef>

#include "Tools/Interface/Interface_set_seed.hpp"
#include "Tools/Interface/Interface_reset.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace dec
	{
		enum class tsk : size_t { decode_hiho, decode_hiho_cw, decode_siho, decode_siho_cw, decode_siso, decode_siso_alt, SIZE };

		namespace sck
		{
			enum class decode_hiho    : size_t { Y_N,       CWD,  V_K, status };
			enum class decode_hiho_cw : size_t { Y_N,       CWD,  V_N, status };
			enum class decode_siho    : size_t { Y_N,       CWD,  V_K, status };
			enum class decode_siho_cw : size_t { Y_N,       CWD,  V_N, status };
			enum class decode_siso    : size_t { Y_N1,      CWD, Y_N2, status };
			enum class decode_siso_alt: size_t { sys,  par, CWD,  ext, status };
		}

		namespace tm
		{
			enum class decode_hiho    : size_t { load, decode, store       , SIZE };
			enum class decode_hiho_cw : size_t { load, decode, store       , SIZE };
			enum class decode_siho    : size_t { load, decode, store, total, SIZE };
			enum class decode_siho_cw : size_t { load, decode, store, total, SIZE };
		}
	}

class Decoder : public Module, public tools::Interface_set_seed, public tools::Interface_reset
{
protected:
	const int           K;          /*!< Number of information bits in one frame */
	const int           N;          /*!< Size of one frame (= number of bits in one frame) */
	      bool          auto_reset;
	const int           mask;
	std::vector<int8_t> CWD;

public:
	Decoder(const int K, const int N);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder() = default;

	virtual Decoder* clone() const;

	int get_K() const;

	int get_N() const;

	bool is_auto_reset() const;
	void set_auto_reset(const bool enable_auto_reset);

	void reset(const int frame_id);
	void reset();

	virtual void set_seed(const int seed);

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void _reset(const size_t frame_id);

	inline int compute_status(const std::vector<int>& status, const int frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Decoder.hxx"
#endif

#endif /* DECODER_HPP_ */
