/*!
 * \file
 * \brief Class factory::Codec_HIHO.
 */
#ifndef FACTORY_CODEC_HIHO_HPP_
#define FACTORY_CODEC_HIHO_HPP_

#include <string>

#include "Tools/Codec/Codec_HIHO.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Tools/Codec/Codec.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_HIHO_name;
extern const std::string Codec_HIHO_prefix;
class Codec_HIHO : public Codec
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Codec_HIHO(const std::string &p = Codec_HIHO_prefix);
	virtual ~Codec_HIHO() = default;
	// The following line is commented to prevent Visual C++ to incorrectly report ambiguity when covariance is
	// used with virtual inheritance.
	// See more about this MSVC bug:
	// - https://stackoverflow.com/questions/6933061/c-ambiguous-inheritance-error-in-vs-2010
	// - https://connect.microsoft.com/VisualStudio/feedback/details/590625/visual-c-incorrectly-reports-ambiguity-when-covariance-is-used-with-virtual-inheritance
	// - https://docs.microsoft.com/fr-fr/cpp/error-messages/compiler-errors-1/compiler-error-c2250
//		virtual Codec_HIHO* clone() const = 0;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_HIHO<B,Q>* build(const module::CRC<B>* crc = nullptr) const;

protected:
	Codec_HIHO(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_CODEC_HIHO_HPP_ */
