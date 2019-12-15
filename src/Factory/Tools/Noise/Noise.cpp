#include <algorithm>
#include <sstream>
#include <utility>
#include <memory>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/general_utils.h"
#include "Tools/Noise/noise_utils.h"
#include "Tools/Documentation/documentation.h"
#include "Factory/Tools/Noise/Noise.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Noise_name   = "Noise";
const std::string aff3ct::factory::Noise_prefix = "sim";

Noise
::Noise(const std::string &prefix)
: Factory(Noise_name, Noise_name, prefix)
{
}

Noise* Noise
::clone() const
{
	return new Noise(*this);
}

void Noise
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Noise::";

	tools::add_arg(args, p, class_name+"p+noise-range,R",
		cli::Matlab_vector<float>(cli::Real(), std::make_tuple(cli::Length(1)), std::make_tuple(cli::Length(1,3))),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+noise-min,m",
		cli::Real(),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+noise-max,M",
		cli::Real(),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+noise-step,s",
		cli::Real(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+pdf-path",
		cli::File(cli::openmode::read));

	args.add_link({p+"-noise-range", "R"}, {p+"-noise-min", "m"});
	args.add_link({p+"-noise-range", "R"}, {p+"-noise-max", "M"});
	args.add_link({p+"-pdf-path"        }, {p+"-noise-range", "R"});
	args.add_link({p+"-pdf-path"        }, {p+"-noise-min",   "m"});
	args.add_link({p+"-pdf-path"        }, {p+"-noise-max",   "M"});

	tools::add_arg(args, p, class_name+"p+noise-type,E",
		cli::Text(cli::Including_set("ESN0", "EBN0", "ROP", "EP")));
}

void Noise
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if (vals.exist({p+"-pdf-path"}))
	{
		this->pdf_path = vals.to_file({p+"-pdf-path"});
		this->range = tools::Distributions<>(this->pdf_path).get_noise_range();

		if(vals.exist({p+"-noise-range", "R"}))
		{
			this->range = tools::generate_range(vals.to_list<std::vector<float>>({p+"-noise-range", "R"}), 0.1f);
		}
		else
		{
			if (vals.exist({p+"-noise-min",  "m"}))
			{
				auto it = std::lower_bound(this->range.begin(), this->range.end(), vals.to_float({p+"-noise-min",  "m"}));
				this->range.erase(this->range.begin(), it);
			}

			if (vals.exist({p+"-noise-max",  "M"}))
			{
				auto it = std::upper_bound(this->range.begin(), this->range.end(), vals.to_float({p+"-noise-max",  "M"}));
				this->range.erase(it, this->range.end());
			}

			if (vals.exist({p+"-noise-step", "s"}))
			{
				float step = vals.to_float({p+"-noise-step", "s"});

				auto it = this->range.begin();
				float start_val = *it++;

				while(it != this->range.end())
				{
					if ((start_val + step) > *it) // then original step is too short
						it = this->range.erase(it);
					else
						start_val = *it++; // step large enough, take this new val as new comparative point
				}
			}
		}
	}
	else
	{
		if(vals.exist({p+"-noise-range", "R"}))
		{
			this->range = tools::generate_range(vals.to_list<std::vector<float>>({p+"-noise-range", "R"}), 0.1f);
		}
		else if(vals.exist({p+"-noise-min",  "m"}) && vals.exist({p+"-noise-max",  "M"}))
		{
			float noise_min  = vals.to_float({p+"-noise-min",  "m"});
			float noise_max  = vals.to_float({p+"-noise-max",  "M"});
			float noise_step = 0.1f;

			if(vals.exist({p+"-noise-step", "s"})) noise_step = vals.to_float({p+"-noise-step", "s"});

			this->range = tools::generate_range({{noise_min, noise_max}}, noise_step);
		}
	}

	if(vals.exist({p+"-noise-type", "E"})) this->type = vals.at({p+"-noise-type", "E"});
}

void Noise
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	if (!this->range.empty())
	{
		std::stringstream range_str;
		range_str << this->range.front() << " -> " << this->range.back() << " dB";
		headers[p].push_back(std::make_pair("Noise range", range_str.str()));
	}

	headers[p].push_back(std::make_pair("Noise type (E)", this->type));

	if (!this->pdf_path.empty())
		headers[p].push_back(std::make_pair("PDF path", this->pdf_path));
}

template <typename R>
void Noise
::update(tools::Noise<R> &noise, R noise_val, R bit_rate, int bps, int upf) const
{
	auto new_noise = std::unique_ptr<tools::Noise<R>>(this->build(noise_val, bit_rate, bps, upf));
	noise.copy(*new_noise);
}

template <typename R>
tools::Noise<R>* Noise
::build(R noise_val, R bit_rate, int bps, int upf) const
{
	if (this->type == "EBN0" || this->type == "ESN0")
	{
		R esn0, ebn0;
		if (this->type == "EBN0")
		{
			ebn0 = noise_val;
			esn0 = tools::ebn0_to_esn0(ebn0, bit_rate, bps);
		}
		else // (this->type == "ESN0")
		{
			esn0 = noise_val;
			ebn0 = tools::esn0_to_ebn0(esn0, bit_rate, bps);
		}

		auto sigma = tools::esn0_to_sigma(esn0, upf);

		return new tools::Sigma<R>(sigma, ebn0, esn0);
	}

	if (this->type == "ROP") return new tools::Received_optical_power<R>(noise_val);
	if (this->type == "EP" ) return new tools::Event_probability     <R>(noise_val);

	std::stringstream message;
	message << "Unknown noise type ('noise_type' = " << this->type << ").";
	throw tools::cannot_allocate(__FILE__, __LINE__, __func__, message.str());
}

template <typename R>
tools::Noise<R>* Noise
::build() const
{
	if (this->type == "EBN0" || this->type == "ESN0") return new tools::Sigma                 <R>();
	if (this->type == "ROP"                         ) return new tools::Received_optical_power<R>();
	if (this->type == "EP"                          ) return new tools::Event_probability     <R>();

	std::stringstream message;
	message << "Unknown noise type ('noise_type' = " << this->type << ").";
	throw tools::cannot_allocate(__FILE__, __LINE__, __func__, message.str());
}

// ==================================================================================== explicit template instantiation
template aff3ct::tools::Noise<float >* aff3ct::factory::Noise::build<float >(float,  float,  int, int) const;
template aff3ct::tools::Noise<double>* aff3ct::factory::Noise::build<double>(double, double, int, int) const;
template aff3ct::tools::Noise<float >* aff3ct::factory::Noise::build<float >() const;
template aff3ct::tools::Noise<double>* aff3ct::factory::Noise::build<double>() const;
template void aff3ct::factory::Noise::update<float >(tools::Noise<float >&, float,  float,  int, int) const;
template void aff3ct::factory::Noise::update<double>(tools::Noise<double>&, double, double, int, int) const;
// ==================================================================================== explicit template instantiation
