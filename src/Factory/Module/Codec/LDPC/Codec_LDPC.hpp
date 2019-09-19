#ifndef FACTORY_CODEC_LDPC_HPP
#define FACTORY_CODEC_LDPC_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Codec/LDPC/Codec_LDPC.hpp"
#include "Factory/Module/Codec/Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_LDPC_name;
extern const std::string Codec_LDPC_prefix;
struct Codec_LDPC : public Codec_SISO_SIHO
{
	class parameters : public Codec_SISO_SIHO::parameters
	{
	public:
		explicit parameters(const std::string &p = Codec_LDPC_prefix);
		virtual ~parameters() = default;
		Codec_LDPC::parameters* clone() const;
		void enable_puncturer();

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_LDPC<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_LDPC<B,Q>* build(const parameters &params, module::CRC<B>* crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_LDPC_HPP */
