/*!
 * \file
 * \brief Class factory::Encoder_LDPC.
 */
#ifndef FACTORY_ENCODER_LDPC_HPP
#define FACTORY_ENCODER_LDPC_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_LDPC_name;
extern const std::string Encoder_LDPC_prefix;
class Encoder_LDPC : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// matrices
	std::string H_path = "";
	std::string G_path = "";

	// optional parameters
	std::string H_reorder = "NONE";

	// G generator method
	std::string G_method    = "IDENTITY";
	std::string G_save_path = "";

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_LDPC(const std::string &p = Encoder_LDPC_prefix);
	virtual ~Encoder_LDPC() = default;
	Encoder_LDPC* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_LDPC<B>* build(const tools::Sparse_matrix &G,
	                               const tools::Sparse_matrix &H) const;
	template <typename B = int>
	module::Encoder_LDPC<B>* build(const tools::Sparse_matrix &G,
	                               const tools::Sparse_matrix &H,
	                               const tools::dvbs2_values  &dvbs2) const;
};
}
}

#endif /* FACTORY_ENCODER_LDPC_HPP */
