#include "Terminal.hpp"

using namespace aff3ct::factory;

const std::string aff3ct::factory::Terminal::name   = "Terminal";
const std::string aff3ct::factory::Terminal::prefix = "ter";

Terminal::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Terminal::name, prefix)
{
}

Terminal::parameters
::~parameters()
{
}

Terminal::parameters* Terminal::parameters
::clone() const
{
	return new Terminal::parameters(*this);
}

void Terminal::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	opt_args[{p+"-no"}] =
		{"",
		 "disable reporting for each iteration."};

	opt_args[{p+"-freq"}] =
		{"positive_int",
		 "display frequency in ms (refresh time step for each iteration, 0 = disable display refresh)."};
}

void Terminal::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-no"  })) this->disabled  = true;
	if(exist(vals, {p+"-freq"})) this->frequency = std::chrono::milliseconds(std::stoi(vals.at({p+"-freq"})));
}

void Terminal::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Enabled", this->disabled ? "no" : "yes"));
	headers[p].push_back(std::make_pair("Frequency (ms)", std::to_string(this->frequency.count())));
}
