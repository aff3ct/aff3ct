/*!
 * \file
 * \brief Class factory::Scaling_factor.
 */
#ifndef FACTORY_SCALING_FACTOR_HPP
#define FACTORY_SCALING_FACTOR_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Scaling_factor_name;
extern const std::string Scaling_factor_prefix;
class Scaling_factor : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	std::string         type        = "LTE_VEC";
	int                 n_ite       = 6;
	bool                enable      = false;
	float               cst         = 0.75f;
	mipp::vector<float> alpha_array = { 0.15f, 0.15f,   // ite 1
	                                    0.25f, 0.25f,   // ite 2
	                                    0.30f, 0.30f,   // ite 3
	                                    0.40f, 0.40f,   // ite 4
	                                    0.70f, 0.70f,   // ite 5
	                                    0.80f, 0.80f,   // ite 6
	                                    0.90f, 0.90f,   // ite 7
	                                    0.95f, 0.95f }; // ite 8

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Scaling_factor(const std::string &p = Scaling_factor_prefix);
	virtual ~Scaling_factor() = default;
	Scaling_factor* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Scaling_factor<B,Q>* build() const;
};
}
}

#endif /* FACTORY_SCALING_FACTOR_HPP */
