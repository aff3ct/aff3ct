#include "Tools/Exception/exception.hpp"

#include "Module/Source/AZCW/Source_AZCW.hpp"
#include "Module/Source/Random/Source_random.hpp"
#include "Module/Source/Random/Source_random_fast.hpp"
#include "Module/Source/User/Source_user.hpp"

#include "Source.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Source::name   = "Source";
const std::string aff3ct::factory::Source::prefix = "src";

Source::parameters
::parameters(const std::string prefix)
: Factory::parameters(Source::name, Source::name, prefix)
{
}

Source::parameters
::~parameters()
{
}

Source::parameters* Source::parameters
::clone() const
{
	return new Source::parameters(*this);
}

void Source::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	auto p = this->get_prefix();

	req_args.add(
		{p+"-info-bits", "K"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"number of generated bits (information bits).");

	opt_args.add(
		{p+"-fra", "F"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the number of inter frame level to process.");

	opt_args.add(
		{p+"-type"},
		tools::Text(tools::Including_set("RAND", "RAND_FAST", "AZCW", "USER")),
		"method used to generate the codewords.");

	opt_args.add(
		{p+"-path"},
		tools::Text(),
		"path to a file containing one or a set of pre-computed source bits, to use with \"--src-type USER\".");

	opt_args.add(
		{p+"-seed", "S"},
		tools::Integer(tools::Positive()),
		"seed used to initialize the pseudo random generators.");
}

void Source::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K        = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames = vals.to_int({p+"-fra",       "F"});
	if(vals.exist({p+"-type"          })) this->type     = vals.at    ({p+"-type"          });
	if(vals.exist({p+"-path"          })) this->path     = vals.at    ({p+"-path"          });
	if(vals.exist({p+"-seed",      "S"})) this->seed     = vals.to_int({p+"-seed",      "S"});
}

void Source::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	headers[p].push_back(std::make_pair("Info. bits (K_info)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
	if (this->type == "USER")
		headers[p].push_back(std::make_pair("Path", this->path));
	if ((this->type == "RAND" || this->type == "RAND_FAST") && full)
		headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));
}

template <typename B>
module::Source<B>* Source::parameters
::build() const
{
	     if (this->type == "RAND"     ) return new module::Source_random     <B>(this->K, this->seed, this->n_frames);
	else if (this->type == "RAND_FAST") return new module::Source_random_fast<B>(this->K, this->seed, this->n_frames);
	else if (this->type == "AZCW"     ) return new module::Source_AZCW       <B>(this->K,             this->n_frames);
	else if (this->type == "USER"     ) return new module::Source_user       <B>(this->K, this->path, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Source<B>* Source
::build(const parameters& params)
{
	return params.template build<B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Source<B_8 >* aff3ct::factory::Source::parameters::build<B_8 >() const;
template aff3ct::module::Source<B_16>* aff3ct::factory::Source::parameters::build<B_16>() const;
template aff3ct::module::Source<B_32>* aff3ct::factory::Source::parameters::build<B_32>() const;
template aff3ct::module::Source<B_64>* aff3ct::factory::Source::parameters::build<B_64>() const;
template aff3ct::module::Source<B_8 >* aff3ct::factory::Source::build<B_8 >(const aff3ct::factory::Source::parameters&);
template aff3ct::module::Source<B_16>* aff3ct::factory::Source::build<B_16>(const aff3ct::factory::Source::parameters&);
template aff3ct::module::Source<B_32>* aff3ct::factory::Source::build<B_32>(const aff3ct::factory::Source::parameters&);
template aff3ct::module::Source<B_64>* aff3ct::factory::Source::build<B_64>(const aff3ct::factory::Source::parameters&);
#else
template aff3ct::module::Source<B>* aff3ct::factory::Source::parameters::build<B>() const;
template aff3ct::module::Source<B>* aff3ct::factory::Source::build<B>(const aff3ct::factory::Source::parameters&);
#endif
// ==================================================================================== explicit template instantiation
