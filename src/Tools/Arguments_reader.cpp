#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <type_traits>

#include "Arguments_reader.hpp"
#include "Tools/Display/bash_tools.h"

using namespace aff3ct::tools;

using color_function = std::add_pointer<std::string(std::string)>::type;

Arguments_reader
::Arguments_reader(const int argc, const char** argv)
: m_argv(argc), max_n_char_arg(0)
{
	if (argc <= 0)
		throw std::invalid_argument("aff3ct::tools::Arguments_reader: \"argc\" has to be greater than 0.");

	this->m_program_name = argv[0];

	for (unsigned short i = 0; i < argc; ++i)
		this->m_argv[i] = std::string(argv[i]);
}

Arguments_reader
::~Arguments_reader()
{
}

bool Arguments_reader
::parse_arguments(const std::map<std::vector<std::string>, std::vector<std::string>> &required_args,
                  const std::map<std::vector<std::string>, std::vector<std::string>> &optional_args,
                  const bool display_warnings)
{
	std::vector<std::string> warns;
	const bool result = parse_arguments(required_args, optional_args, warns);

	if (display_warnings)
		for (auto w = 0; w < (int)warns.size(); w++)
			std::clog << warns[w] << std::endl;

	return result;
}

bool Arguments_reader
::parse_arguments(const std::map<std::vector<std::string>, std::vector<std::string>> &required_args,
                  const std::map<std::vector<std::string>, std::vector<std::string>> &optional_args,
                        std::vector<std::string>                                     &warnings)
{
	unsigned short int n_req_arg = 0;

	this->clear_arguments();

	this->m_required_args = required_args;
	this->m_optional_args = optional_args;

	for (unsigned short i = 0; i < this->m_argv.size(); ++i)
	{
		bool valid_arg = false;
		if (this->sub_parse_arguments(this->m_required_args, i))
		{
			valid_arg = true;
			n_req_arg++;
		}
		valid_arg = this->sub_parse_arguments(this->m_optional_args, i) || valid_arg;

		// do not display warning when negative value
		try { std::stoi(this->m_argv[i]); if (!valid_arg) valid_arg = true; }
		catch (std::exception const&) {}

		if (!valid_arg && this->m_argv[i][0] == '-')
			warnings.push_back("Unknown argument \"" + this->m_argv[i] + "\".");
	}

	return n_req_arg >= required_args.size();
}

bool Arguments_reader
::sub_parse_arguments(std::map<std::vector<std::string>, std::vector<std::string>> &args, unsigned short pos_arg)
{
	if (pos_arg >= this->m_argv.size())
		throw std::invalid_argument("aff3ct::tools::Arguments_reader: \"pos_arg\" has to be smaller than "
		                            "\"this->m_argv.size()\".");

	auto is_found = false;
	for (auto it = args.begin(); it != args.end(); ++it)
	{
		if (it->first.size() <= 0)
			throw std::runtime_error("aff3ct::tools::Arguments_reader: \"it->first.size()\" has to be greater "
			                         "than 0.");
		if (it->second.size() <= 0)
			throw std::runtime_error("aff3ct::tools::Arguments_reader: \"it->second.size()\" has to be greater "
			                         "than 0.");

		// remember the biggest argument length to display the doc after
		const std::string delimiter = ", ";
		unsigned total_length = 0;
		for (auto i = 0; i < (int)it->first.size() -1; i++)
			total_length += unsigned((it->first[i].length() == 1 ? 1 : 2) + it->first[i].length() + delimiter.length());
		const auto last = it->first.size() -1;
		total_length += unsigned((it->first[last].length() == 1 ? 1 : 2) + it->first[last].length());

		this->max_n_char_arg = std::max(this->max_n_char_arg, total_length);

		auto i = 0;
		do
		{
			const auto cur_arg = ((it->first[i].length() == 1) ? "-" : "--") + it->first[i];
			if (cur_arg == this->m_argv[pos_arg])
			{
				if(it->second[0] != "")
				{
					if(pos_arg != (this->m_argv.size() -1))
					{
						this->m_args[it->first] = this->m_argv[pos_arg +1];
						is_found = true;
					}
				}
				else
				{
					this->m_args[it->first] = "";
					is_found = true;
				}
			}

			i++;
		}
		while( !is_found && i < (int)it->first.size());
	}

	return is_found;
}

bool Arguments_reader
::exist_argument(const std::vector<std::string> &tags)
{
	return (this->m_args.find(tags) != this->m_args.end());
}

std::string Arguments_reader
::get_argument(const std::vector<std::string> &tags)
{
	return this->m_args[tags];
}

void Arguments_reader
::print_usage()
{
	std::cout << "Usage: " << this->m_program_name;

	for (auto it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
		if (it->second[0] != "")
			std::cout << ((it->first[0].size() == 1) ? " -" : " --") << it->first[0] << " <" << it->second[0] << ">";
		else
			std::cout << ((it->first[0].size() == 1) ? " -" : " --") << it->first[0];
	std::cout << " [optional args...]" << std::endl << std::endl;

	for (auto it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
		this->print_usage(it->first, it->second, true);

	for (auto it = this->m_optional_args.begin(); it != this->m_optional_args.end(); ++it)
		this->print_usage(it->first, it->second, false);

	std::cout << std::endl;
}

void Arguments_reader
::print_usage(std::vector<std::vector<std::string>> arg_groups)
{
	color_function head_color = tools::bold_yellow;

	std::cout << "Usage: " << this->m_program_name;

	for (auto it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
	{
		const auto last = it->first.size() -1;
		if(it->second[0] != "")
			std::cout << ((it->first[last].size() == 1) ? " -" : " --") << it->first[last] << " <" << it->second[0] << ">";
		else
			std::cout << ((it->first[last].size() == 1) ? " -" : " --") << it->first[last];
	}
	std::cout << " [optional args...]" << std::endl << std::endl;

	auto req_args_cpy = this->m_required_args;
	auto opt_args_cpy = this->m_optional_args;

	for (auto i = 0; i < (int)arg_groups.size(); i++)
	{
		if (arg_groups[i].size() <= 1)
			throw std::runtime_error("aff3ct::tools::Arguments_reader: \"arg_groups[i].size()\" has to be greater "
			                         "than 1.");

		// detect if there is at least one argument of this group
		auto display = false;
		for (auto it = req_args_cpy.begin(); it != req_args_cpy.end(); ++it)
		{
			const auto res = std::mismatch(arg_groups[i][0].begin(), arg_groups[i][0].end(), it->first[0].begin());
			if (res.first == arg_groups[i][0].end())
			{
				display = true;
				break;
			}
		}
		if (!display)
		{
			for (auto it = opt_args_cpy.begin(); it != opt_args_cpy.end(); ++it)
			{
				const auto res = std::mismatch(arg_groups[i][0].begin(), arg_groups[i][0].end(), it->first[0].begin());
				if (res.first == arg_groups[i][0].end())
				{
					display = true;
					break;
				}
			}
		}

		if (display)
		{
			std::cout << head_color(arg_groups[i][1] + ": ") << std::endl;
			if (arg_groups[i].size() > 2)
				std::cout << arg_groups[i][2] << std::endl;

			const auto req_args_cpy2 = req_args_cpy;
			for (auto it = req_args_cpy2.begin(); it != req_args_cpy2.end(); ++it)
			{
				const auto res = std::mismatch(arg_groups[i][0].begin(), arg_groups[i][0].end(), it->first[0].begin());
				if (res.first == arg_groups[i][0].end())
				{
					// gr->first is a prefix of it->first[0].
					this->print_usage(it->first, it->second, true);
					req_args_cpy.erase(it->first);
				}
			}

			const auto opt_args_cpy2 = opt_args_cpy;
			for (auto it = opt_args_cpy2.begin(); it != opt_args_cpy2.end(); ++it)
			{
				const auto res = std::mismatch(arg_groups[i][0].begin(), arg_groups[i][0].end(), it->first[0].begin());
				if (res.first == arg_groups[i][0].end())
				{
					// gr->first is a prefix of it->first[0].
					this->print_usage(it->first, it->second, false);
					opt_args_cpy.erase(it->first);
				}
			}

			std::cout << std::endl;
		}
	}

	if (!req_args_cpy.empty() || !opt_args_cpy.empty())
	{
		std::cout << head_color("Other parameter(s): ") << std::endl;
		for (auto it = req_args_cpy.begin(); it != req_args_cpy.end(); )
		{
			// gr->first is a prefix of it->first[0].
			this->print_usage(it->first, it->second, true);
			it = req_args_cpy.erase(it);
		}

		for (auto it = opt_args_cpy.begin(); it != opt_args_cpy.end();)
		{
			// gr->first is a prefix of it->first[0].
			this->print_usage(it->first, it->second, false);
			it = opt_args_cpy.erase(it);
		}

		std::cout << std::endl;
	}
}

void Arguments_reader
::print_usage(const std::vector<std::string> &tags, const std::vector<std::string> &values, const bool required)
{
	color_function arg_color;
	if (required)
		arg_color = tools::red;
	else
		arg_color = tools::blue;

	if (values.size() >= 2 && !values[1].empty())
	{
		const auto tab = "    ";
		const std::string delimiter = ", ";
		auto total_length = 0;
		std::cout << tab;
		for (auto i = 0; i < (int)tags.size() -1; i++)
		{
			std::cout << arg_color(((tags[i].length() == 1) ? "-" : "--") + tags[i] + delimiter);
			total_length += unsigned((tags[i].length() == 1 ? 1 : 2) + tags[i].length() + delimiter.length());
		}
		const auto last = tags.size() -1;
		std::cout << arg_color(((tags[last].length() == 1) ? "-" : "--") + tags[last]);
		total_length += unsigned((tags[last].length() == 1 ? 1 : 2) + tags[last].length());

		for (unsigned i = 0; i < this->max_n_char_arg - total_length; i++) std::cout << arg_color(" ");
		if (values.size() < 3)
		{
			if (!values[0].empty())
				std::cout << arg_color(" <" + values[0] + ">");
		}
		else
		{
			auto entries = Arguments_reader::split(values[2]);
			std::string set;
			for (auto i = 0; i < (int)entries.size() -1; i++)
				set += entries[i] + "|";
			set += entries[entries.size() -1];

			std::cout << arg_color(" <" + values[0] + "=" + set + ">");
		}
		if (required)
			std::cout << arg_color(" {REQUIRED}");
		std::cout << std::endl;
		std::cout << arg_color(tab + values[1]) << std::endl;
	}
}

bool Arguments_reader
::check_arguments(std::string &error)
{
	auto success = true;
	for (auto it = this->m_args.begin(); it != this->m_args.end(); ++it)
	{
		if (this->m_required_args.find(it->first) != this->m_required_args.end())
			success = this->check_argument(it->first, this->m_required_args, error);
		else if (this->m_optional_args.find(it->first) != this->m_optional_args.end())
			success = this->check_argument(it->first, this->m_optional_args, error);
		else
			success = false;

		if (!success)
			break;
	}

	return success;
}

bool Arguments_reader
::check_argument(const std::vector<std::string> &tags, std::map<std::vector<std::string>, std::vector<std::string>> &args, std::string &error)
{
	// check if the input is positive
	if (args[tags][0] == "positive_int")
	{
		const auto int_num = std::stoi(this->m_args[tags]);
		if (int_num < 0)
		{
			error = "The \"" + ((tags[0].length() == 1) ? std::string("-") : std::string("--")) + tags[0] +
			        "\" argument have to be positive.";
			return false;
		}
	}

	// check if the input is positive
	if (args[tags][0] == "positive_float")
	{
		const auto float_num = std::stof(this->m_args[tags]);
		if (float_num < 0.f)
		{
			error = "The \"" + ((tags[0].length() == 1) ? std::string("-") : std::string("--")) + tags[0] +
			        "\" argument have to be positive.";
			return false;
		}
	}

	// check if the input is in the list
	if (args[tags].size() >= 3)
	{
		auto entries = Arguments_reader::split(args[tags][2]);
		auto cur_entry = this->m_args[tags];
		auto found_entry = false;
		auto i = 0;
		do
		{
			if (entries[i] == cur_entry)
				found_entry = true;
			i++;
		}
		while(!found_entry && i < (int)entries.size());

		if (!found_entry)
		{
			std::string set = "<";
			for (auto i = 0; i < (int)entries.size() -1; i++)
				set += entries[i] + "|";
			set += entries[entries.size() -1] + ">";

			error = "The \"" + ((tags[0].length() == 1) ? std::string("-") : std::string("--")) + tags[0] +
			        "\" argument have to be in the " + set + " set.";

			return false;
		}
	}

	error = "";
	return true;
}

std::vector<std::string> Arguments_reader
::split(std::string str)
{
	std::vector<std::string> str_splited;

	if (!str.empty())
	{
		// remove white spaces
		str.erase(remove(str.begin(), str.end(), ' '), str.end());

		// specify a delimiter
		std::string delimiter = ",";

		// extract each entry from "str"
		size_t pos = 0;
		while ((pos = str.find(delimiter)) != std::string::npos)
		{
			auto token = str.substr(0, pos);
			str_splited.push_back(token);
			str.erase(0, pos + delimiter.length());
		}
		str_splited.push_back(str);
	}

	return str_splited;
}

void Arguments_reader
::clear_arguments()
{
	this->m_required_args.clear();
	this->m_optional_args.clear();
	this->m_args.clear();
}
