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

void Source
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-info-bits", "K"}] =
		{"positive_int",
		 "number of generated bits (information bits)."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type"}] =
		{"string",
		 "method used to generate the codewords.",
		 "RAND, RAND_FAST, AZCW, USER"};

	opt_args[{p+"-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed source bits, to use with \"--src-type USER\"."};

	opt_args[{p+"-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Source
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-info-bits", "K"})) params.K        = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-fra",       "F"})) params.n_frames = std::stoi(vals.at({p+"-fra",       "F"}));
	if(exist(vals, {p+"-type"          })) params.type     =           vals.at({p+"-type"          });
	if(exist(vals, {p+"-path"          })) params.path     =           vals.at({p+"-path"          });
	if(exist(vals, {p+"-seed",      "S"})) params.seed     = std::stoi(vals.at({p+"-seed",      "S"}));
}

void Source
::make_header(params_list& head_src, const parameters& params, const bool full)
{
	head_src.push_back(std::make_pair("Type", params.type));
	head_src.push_back(std::make_pair("Info. bits (K_info)", std::to_string(params.K)));
	if (full) head_src.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	if (params.type == "USER")
		head_src.push_back(std::make_pair("Path", params.path));
	if ((params.type == "RAND" || params.type == "RAND_FAST") && full)
		head_src.push_back(std::make_pair("Seed", std::to_string(params.seed)));
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
