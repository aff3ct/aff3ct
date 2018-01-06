#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/LDPC/Puncturer_LDPC.hpp"

#include "Puncturer_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_LDPC::name   = "Puncturer LDPC";
const std::string aff3ct::factory::Puncturer_LDPC::prefix = "pct";

Puncturer_LDPC::parameters
::parameters(const std::string prefix)
: Puncturer::parameters(Puncturer_LDPC::name, prefix)
{
	this->type = "LDPC";
}

Puncturer_LDPC::parameters
::~parameters()
{
}

Puncturer_LDPC::parameters* Puncturer_LDPC::parameters
::clone() const
{
	return new Puncturer_LDPC::parameters(*this);
}

void Puncturer_LDPC::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Puncturer::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.add(
		{p+"-cw-size", "N_cw"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"the codeword size.");

	tools::add_options<std::string>(opt_args.at({p+"-type"}), 0, "LDPC");

	opt_args.add(
		{p+"-pattern"},
		tools::Text(),
		"puncturing pattern for the LDPC encoder/decoder (size = N_Code/Z) (ex: \"1,1,1,0\").");
}

std::vector<bool> generate_punct_vector(const std::string pattern)
{
	std::vector<std::string> str_array = aff3ct::tools::split(pattern, ',');
	int N_pattern = (int)str_array.size();

	if (N_pattern == 0)
	{
		std::stringstream message;
		message << "'pattern' shouldn't be null and should be delimited by a comma ('pattern' = " << pattern
		        << ", 'str_array.size()' = " << str_array.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<bool> pattern_vector(N_pattern, true);

	for (auto j = 0; j < N_pattern; j++)
	{
		char c[2] = {str_array[j][0], '\0'};
		pattern_vector[j] = std::stoi(std::string(c)) ? true : false;
	}
	return pattern_vector;
}

void Puncturer_LDPC::parameters
::store(const tools::Argument_map_value &vals)
{
	auto save_N_cw = this->N_cw;
	Puncturer::parameters::store(vals);
	if (save_N_cw > 0)
		this->N_cw = save_N_cw;

	auto p = this->get_prefix();

	std::string str_pattern;
	if(vals.exist({p+"-pattern"}))
	{
		str_pattern = vals.at({p+"-pattern"});
		this->pattern = generate_punct_vector(str_pattern);
	}

	if(vals.exist({p+"-cw-size", "N_cw"})) this->N_cw = vals.to_int({p+"-cw-size",  "N_cw"});

	if (this->N == this->N_cw)
		this->type = "NO";
}

void Puncturer_LDPC::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Puncturer::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->type != "NO")
	{
		headers[p].push_back(std::make_pair(std::string("Pattern"), std::string("{" + std::string(this->pattern.begin(),this->pattern.end())) + "}"));
	}
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_LDPC::parameters
::build() const
{
	if (this->type == "LDPC") return new module::Puncturer_LDPC<B,Q>(this->K, this->N, this->N_cw, this->pattern, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_LDPC
::build(const parameters &params)
{
	return params.template build<B,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_LDPC::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_LDPC::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_LDPC::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_LDPC::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_LDPC::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer_LDPC::parameters&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_LDPC::build<B_16,Q_16>(const aff3ct::factory::Puncturer_LDPC::parameters&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_LDPC::build<B_32,Q_32>(const aff3ct::factory::Puncturer_LDPC::parameters&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_LDPC::build<B_64,Q_64>(const aff3ct::factory::Puncturer_LDPC::parameters&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_LDPC::parameters::build<B,Q>() const;
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_LDPC::build<B,Q>(const aff3ct::factory::Puncturer_LDPC::parameters&);
#endif
// ==================================================================================== explicit template instantiation
