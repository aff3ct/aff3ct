#ifndef FACTORY_NOISE_HPP
#define FACTORY_NOISE_HPP

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Noise_name;
extern const std::string Noise_prefix;
struct Noise : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		std::vector<float> range;

		// optional parameters
		std::string type     = "EBN0";
		std::string pdf_path = "";

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Noise_prefix);
		virtual ~parameters() = default;
		Noise::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename R = float>
		tools::Noise<R>* build(R noise_val, R bit_rate = 1., int bps = 1, int upf = 1) const;
	};

	template <typename R = float>
	static tools::Noise<R>* build(const parameters& params, R noise_val, R bit_rate = 1., int bps = 1, int upf = 1);
};
}
}

#endif /* FACTORY_NOISE_HPP */
