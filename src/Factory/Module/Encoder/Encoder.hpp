#ifndef FACTORY_ENCODER_HPP
#define FACTORY_ENCODER_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder : public Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K           = 0;
		int         N_cw        = 0;

		// optional parameters
		std::string type        = "NO";
		std::string path        = "";
		bool        systematic  = true;
		int         n_frames    = 1;
		int         seed        = 0;
		int         tail_length = 0;

		// deduced parameters
		float       R           = -1.f;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Encoder::prefix);
		virtual ~parameters();
		virtual Encoder::parameters* clone() const;

		// parameters construction
		virtual void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		virtual void store          (const arg_val_map &vals                                           );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder<B>* build() const;

	protected:
		parameters(const std::string n, const std::string p);
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters &params);
};
}
}

#endif /* FACTORY_ENCODER_HPP */
