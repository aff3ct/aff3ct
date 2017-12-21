#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo/Puncturer_turbo.hpp"

#include "Puncturer_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_turbo::name   = "Puncturer Turbo";
const std::string aff3ct::factory::Puncturer_turbo::prefix = "pct";

Puncturer_turbo::parameters
::parameters(const std::string prefix)
: Puncturer::parameters(Puncturer_turbo::name, prefix)
{
	this->type = "TURBO";
}

Puncturer_turbo::parameters
::~parameters()
{
}

Puncturer_turbo::parameters* Puncturer_turbo::parameters
::clone() const
{
	return new Puncturer_turbo::parameters(*this);
}

void Puncturer_turbo::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Puncturer::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-fra-size", "N"});

	tools::add_options<std::string>(opt_args.at({p+"-type"}), {"TURBO"}, 0);

	opt_args.add(
		{p+"-pattern"},
		new tools::List<>(new tools::Text<>()),
		"puncturing pattern for the turbo encoder (ex: \"11,10,01\").");

	// opt_args.add(
	// 	{p+"-pattern"},
	// 	new tools::List<>(new tools::Text<>({new tools::Including_set<std::string>({"00", "01", "10", "11"})})),
	// 	"puncturing pattern for the turbo encoder (ex: \"11,10,01\").");

	// opt_args.add(
	// 	{p+"-pattern"},
	// 	new tools::List<>(new tools::List<>(new tools::Boolean<>())),
	// 	"puncturing pattern for the turbo encoder (ex: \"11,10,01\").");

	// opt_args.add(
	// 	{p+"-pattern"},
	// 	new tools::Text<>(),
	// 	"puncturing pattern for the turbo encoder (ex: \"11,10,01\").");

	opt_args.add(
		{p+"-tail-length"},
		new tools::Integer<>({new tools::Positive<int>()}),
		"total number of tail bits at the end of the frame.");

	opt_args.add(
		{p+"-no-buff"},
		new tools::None(),
		"does not suppose a buffered encoding.");
}

template <typename T>
std::string display_pattern(std::vector<T> pattern)
{
	std::string m;

	for(auto &v : pattern)
		m += std::to_string(v) + ",";

	m.erase(m.size() -1);

	return m;
}

std::string display_pattern(std::vector<std::string> pattern)
{
	std::string m;

	for(auto &v : pattern)
		m += v + ",";

	m.erase(m.size() -1);

	return m;
}

int compute_N(const int K, const int tail_bits,  const std::vector<std::string> pattern)
{
	std::vector<std::vector<bool>> pattern_bits(3);

	if (tail_bits < 0)
	{
		std::stringstream message;
		message << "'tail_bits' has to be positive ('tail_bits' = " << tail_bits << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// auto str_array = tools::split(pattern, ',');

	if (pattern.size() != 3)
	{
		std::stringstream message;
		message << "'pattern' should give 3 different sets ('pattern' = " << display_pattern(pattern)
		        << ", 'pattern.size()' = " << pattern.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (pattern[0].size() != pattern[1].size() || pattern[0].size() != pattern[2].size())
	{
		std::stringstream message;
		message << "'pattern' sets have to contains an equal number of bits ('pattern' = " << display_pattern(pattern)
		        << ", 'pattern[0].size()' = " << pattern[0].size()
		        << ", 'pattern[1].size()' = " << pattern[1].size()
		        << ", 'pattern[2].size()' = " << pattern[2].size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto all_one = true;
	for (auto i = 0; i < 3; i++)
		for (auto j = 0; j < (int)pattern[i].size(); j++)
			all_one = pattern[i][j] != '1' ? false : all_one;

	if (all_one)
		return 3 * K + tail_bits;

	auto period = (int)pattern[0].size();

	if (K % period)
	{
		std::stringstream message;
		message << "'period' has to be a multiple of 'K' ('period' = " << period << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	pattern_bits[0].resize(period);
	pattern_bits[1].resize(period);
	pattern_bits[2].resize(period);

	for (auto i = 0; i < 3; i++)
		for (auto j = 0; j < period; j++)
		{
			char c[2] = {pattern[i][j], '\0'};
			pattern_bits[i][j] = std::stoi(std::string(c)) ? true : false;
		}

	auto bit_sys_count = 0; for (auto j = 0; j < period; j++) bit_sys_count += pattern_bits[0][j] ? 1 : 0;
	auto bit_pa1_count = 0; for (auto j = 0; j < period; j++) bit_pa1_count += pattern_bits[1][j] ? 1 : 0;
	auto bit_pa2_count = 0; for (auto j = 0; j < period; j++) bit_pa2_count += pattern_bits[2][j] ? 1 : 0;

	auto bit_count = bit_sys_count + bit_pa1_count + bit_pa2_count;

	auto N = (K / period) * bit_count + tail_bits;

	return N;
}

int compute_N(const int K, const int tail_bits,  const std::string pattern)
{
	return compute_N(K, tail_bits, tools::List<>::get_list(pattern));
}

void Puncturer_turbo::parameters
::store(const tools::Argument_map_value &vals)
{
	Puncturer::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-pattern"    })) this->pattern     = vals.at     ({p+"-pattern"    });
	if(vals.exist({p+"-pattern"    })) this->pattern2    = vals.to_list<tools::Generic_splitter, std::string>({p+"-pattern"    });
	if(vals.exist({p+"-tail-length"})) this->tail_length = vals.to_int({p+"-tail-length"});
	if(vals.exist({p+"-no-buff"    })) this->buffered    = false;

	this->N_cw = 3 * this->K + this->tail_length;
	this->N    = compute_N(this->K, this->tail_length, this->pattern2);

	if (this->N == this->N_cw)
		this->type = "NO";
}

void Puncturer_turbo::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Puncturer::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->type != "NO")
	{
		headers[p].push_back(std::make_pair(std::string("Pattern"), std::string("{" + this->pattern + "}")));
		headers[p].push_back(std::make_pair(std::string("Pattern list"), std::string("{" + display_pattern(this->pattern2) + "}")));
		if (full) headers[p].push_back(std::make_pair(std::string("Tail length"), std::to_string(this->tail_length)));
		if (full) headers[p].push_back(std::make_pair(std::string("Buffered"), this->buffered ? "on" : "off"));
	}
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo::parameters
::build() const
{
	if (this->type == "TURBO") return new module::Puncturer_turbo<B,Q>(this->K, this->N, this->tail_length, display_pattern(this->pattern2), this->buffered, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo
::build(const parameters &params)
{
	return params.template build<B,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer_turbo::parameters&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo::build<B_16,Q_16>(const aff3ct::factory::Puncturer_turbo::parameters&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo::build<B_32,Q_32>(const aff3ct::factory::Puncturer_turbo::parameters&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo::build<B_64,Q_64>(const aff3ct::factory::Puncturer_turbo::parameters&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo::parameters::build<B,Q>() const;
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo::build<B,Q>(const aff3ct::factory::Puncturer_turbo::parameters&);
#endif
// ==================================================================================== explicit template instantiation
