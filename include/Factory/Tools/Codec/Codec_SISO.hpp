/*!
 * \file
 * \brief Class factory::Codec_SISO.
 */
#ifndef FACTORY_CODEC_SISO_HPP_
#define FACTORY_CODEC_SISO_HPP_

#include <string>

#include "Tools/Codec/Codec_SISO.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_SISO_name;
extern const std::string Codec_SISO_prefix;
class Codec_SISO : public Codec_SIHO
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Codec_SISO(const std::string &p = Codec_SISO_prefix);
	virtual ~Codec_SISO() {}
	// The following line is commented to prevent Visual C++ to incorrectly report ambiguity when covariance is
	// used with virtual inheritance.
	// See more about this MSVC bug:
	// - https://stackoverflow.com/questions/6933061/c-ambiguous-inheritance-error-in-vs-2010
	// - https://connect.microsoft.com/VisualStudio/feedback/details/590625/visual-c-incorrectly-reports-ambiguity-when-covariance-is-used-with-virtual-inheritance
	// - https://docs.microsoft.com/fr-fr/cpp/error-messages/compiler-errors-1/compiler-error-c2250
//		virtual Codec_SISO* clone() const = 0;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_SISO<B,Q>* build(const module::CRC<B>* crc = nullptr) const;

protected:
	Codec_SISO(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_CODEC_SISO_HPP_ */
