#include <stdexcept>
#include <utility>
#include <tuple>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"
#include "Factory/Tools/Code/Polar_MK/Polar_code.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Polar_code_name   = "Polar code";
const std::string aff3ct::factory::Polar_code_prefix = "pc";

Polar_code
::Polar_code(const std::string &prefix)
: Factory(Polar_code_name, Polar_code_name, prefix)
{
}

Polar_code* Polar_code
::clone() const
{
	return new Polar_code(*this);
}

struct sub_same_length
{
	template <typename T>
	static void check(const std::vector<T>& v)
	{
		for(unsigned i = 0; i < v.size(); i++)
			if (v[i].size() != v[0].size())
				throw std::runtime_error("all elements do not have the same length");
	}
};

void Polar_code
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Polar_code::";

	tools::add_arg(args, p, class_name+"p+cw-size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+kernel",
		cli::List2D<bool>(cli::Boolean(),
		                  std::make_tuple(cli::Length(1), cli::Function<sub_same_length>("elements of same length")),
		                  std::make_tuple(cli::Length(1))));

	tools::add_arg(args, p, class_name+"p+path",
		cli::File(cli::openmode::read));
}

void Polar_code
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-cw-size", "N"})) this->N_cw          = vals.to_int                    ({p+"-cw-size", "N"});
	if(vals.exist({p+"-kernel"      })) this->kernel_matrix = vals.to_list<std::vector<bool>>({p+"-kernel"      });
	if(vals.exist({p+"-path"        })) this->code_path     = vals.to_file                   ({p+"-path"        });
}

void Polar_code
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	if (this->code_path.empty())
	{
		headers[p].push_back(std::make_pair(std::string("Kernel"), tools::display_kernel(this->kernel_matrix)));
	}
	else
	{
		std::vector<std::vector<std::vector<bool>>> kernel_matrices;
		std::vector<uint32_t> stages;
		tools::read_polar_MK_code(this->code_path, kernel_matrices, stages);
		if (kernel_matrices.size() > 0)
		{
			std::string kernels_str = "{" + tools::display_kernel(kernel_matrices[0]);
			for (size_t k = 1; k < kernel_matrices.size(); k++)
				kernels_str += "," + tools::display_kernel(kernel_matrices[k]);
			kernels_str += "}";
			headers[p].push_back(std::make_pair(std::string("Kernels"), kernels_str));
		}
		if (stages.size() > 0)
		{
			std::string stages_str = "{" + std::to_string(stages[0]);
			for (size_t s = 1; s < stages.size(); s++)
				stages_str += "," + std::to_string(stages[s]);
			stages_str += "}";
			headers[p].push_back(std::make_pair(std::string("Stages"), stages_str));
		}
	}
}

tools::Polar_code* Polar_code
::build() const
{
	if (this->code_path.empty()) return new tools::Polar_code(this->N_cw, this->kernel_matrix);
	else                         return new tools::Polar_code(this->code_path);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}
