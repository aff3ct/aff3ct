#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/general_utils.h"
#include "Tools/Noise/noise_utils.h"

#include "Noise.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Noise_name   = "Noise";
const std::string aff3ct::factory::Noise_prefix = "sim";

Noise::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Noise_name, Noise_name, prefix)
{
}

Noise::parameters* Noise::parameters
::clone() const
{
	return new Noise::parameters(*this);
}

void Noise::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Noise::parameters::";

	// args.add(
	// 		{p+"-noise-range", "R"},
	// 		tools::Matlab_vector<float>(tools::Real(), std::make_tuple(tools::Length(1)), std::make_tuple(tools::Length(1,3))),
	// 		"noise energy range to run (Matlab style: \"0.5:2.5,2.55,2.6:0.05:3\" with a default step of 0.1).",
	// 		tools::arg_rank::REQ);
	add_arg(args, p, class_name+"p+noise-range,R",
		tools::Matlab_vector<float>(tools::Real(), std::make_tuple(tools::Length(1)), std::make_tuple(tools::Length(1,3))),
		tools::arg_rank::REQ);

	// args.add(
	// 		{p+"-noise-min", "m"},
	// 		tools::Real(),
	// 		"minimal noise energy to simulate.",
	// 		tools::arg_rank::REQ);
	add_arg(args, p, class_name+"p+noise-min,m",
		tools::Real(),
		tools::arg_rank::REQ);

	// args.add(
	// 		{p+"-noise-max", "M"},
	// 		tools::Real(),
	// 		"maximal noise energy to simulate.",
	// 		tools::arg_rank::REQ);
	add_arg(args, p, class_name+"p+noise-max,M",
		tools::Real(),
		tools::arg_rank::REQ);

	// args.add(
	// 		{p+"-noise-step", "s"},
	// 		tools::Real(tools::Positive(), tools::Non_zero()),
	// 		"noise energy step between each simulation iteration.");
	add_arg(args, p, class_name+"p+noise-step,s",
		tools::Real(tools::Positive(), tools::Non_zero()));

	// args.add(
	// 		{p+"-pdf-path"},
	// 		tools::File(tools::openmode::read),
	// 		"A file that contains PDF for different SNR. Set the SNR range from the given ones. "
	// 		"Overwritten by -R or limited by -m and -M with a minimum step of -s");
	add_arg(args, p, class_name+"p+pdf-path",
		tools::File(tools::openmode::read));


	args.add_link({p+"-noise-range", "R"}, {p+"-noise-min", "m"});
	args.add_link({p+"-noise-range", "R"}, {p+"-noise-max", "M"});
	args.add_link({p+"-pdf-path"        }, {p+"-noise-range", "R"});
	args.add_link({p+"-pdf-path"        }, {p+"-noise-min",   "m"});
	args.add_link({p+"-pdf-path"        }, {p+"-noise-max",   "M"});


	// args.add(
	// 		{p+"-noise-type", "E"},
	// 		tools::Text(tools::Including_set("ESN0", "EBN0", "ROP", "EP")),
	// 		"select the type of NOISE: SNR per Symbol / SNR per information Bit"
	// 		" / Received Optical Power / Erasure Probability.");
	add_arg(args, p, class_name+"p+noise-type,E",
		tools::Text(tools::Including_set("ESN0", "EBN0", "ROP", "EP")));

}

void Noise::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if (vals.exist({p+"-pdf-path"}))
	{
		this->pdf_path = vals.at({p+"-pdf-path"});
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

void Noise::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
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
tools::Noise<R>* Noise::parameters
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
::build(const aff3ct::factory::Noise::parameters& params, R noise_val, R bit_rate, int bps, int upf)
{
	return params.template build<R>(noise_val, bit_rate, bps, upf);
}

// ==================================================================================== explicit template instantiation
template aff3ct::tools::Noise<float>* aff3ct::factory::Noise::parameters::build<float>(float, float, int, int) const;
template aff3ct::tools::Noise<float>* aff3ct::factory::Noise::build<float>(const aff3ct::factory::Noise::parameters&, float, float, int, int);

template aff3ct::tools::Noise<double>* aff3ct::factory::Noise::parameters::build<double>(double, double, int, int) const;
template aff3ct::tools::Noise<double>* aff3ct::factory::Noise::build<double>(const aff3ct::factory::Noise::parameters&, double, double, int, int);
// ==================================================================================== explicit template instantiation
