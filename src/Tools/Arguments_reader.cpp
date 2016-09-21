#include <iostream>
#include <cassert>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Arguments_reader.hpp"

using namespace std;

Arguments_reader
::Arguments_reader(const int argc, const char** argv)
: m_argv(argc), max_n_char_arg(0)
{
	assert(argc > 0);

	this->m_program_name = argv[0];

	for(unsigned short i = 0; i < argc; ++i)
		this->m_argv[i] = string(argv[i]);
}

Arguments_reader
::~Arguments_reader()
{
}

bool Arguments_reader
::parse_arguments(const map<vector<string>, vector<string>> &required_args,
                  const map<vector<string>, vector<string>> &optional_args)
{
	unsigned short int n_req_arg = 0;

	this->clear_arguments();

	this->m_required_args = required_args;
	this->m_optional_args = optional_args;

	for(unsigned short i = 0; i < this->m_argv.size(); ++i)
	{
		if(this->sub_parse_arguments(this->m_required_args, i))
			n_req_arg++;
		this->sub_parse_arguments(this->m_optional_args, i);
	}

	return n_req_arg >= required_args.size();
}

bool Arguments_reader
::sub_parse_arguments(map<vector<string>, vector<string>> &args, unsigned short pos_arg)
{
	assert(pos_arg < this->m_argv.size());

	auto is_found = false;
	for (auto it = args.begin(); it != args.end(); ++it)
	{
		assert(it->first .size() > 0);
		assert(it->second.size() > 0);

		// remember the biggest argument length to display the doc after
		const string delimiter = ", ";
		unsigned total_length = 0;
		for (auto i = 0; i < (int)it->first.size() -1; i++)
			total_length += (it->first[i].length() == 1 ? 1 : 2) + it->first[i].length() + delimiter.length();
		const auto last = it->first.size() -1;
		total_length += (it->first[last].length() == 1 ? 1 : 2) + it->first[last].length();

		this->max_n_char_arg = max(this->max_n_char_arg, total_length);

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
		while(!is_found && i < (int)it->first.size());
	}

	return is_found;
}

bool Arguments_reader
::exist_argument(const vector<string> &tags)
{
	return (this->m_args.find(tags) != this->m_args.end());
}

string Arguments_reader
::get_argument(const vector<string> &tags)
{
	return this->m_args[tags];
}

void Arguments_reader
::print_usage()
{
	cout << "Usage: " << this->m_program_name;

	for (auto it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
		if(it->second[0] != "")
			cout << ((it->first[0].size() == 1) ? " -" : " --") << it->first[0] << " <" << it->second[0] << ">";
		else
			cout << ((it->first[0].size() == 1) ? " -" : " --") << it->first[0];
	cout << " [optional args...]" << endl << endl;

	for (auto it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
		this->print_usage(it->first, it->second, true);

	for (auto it = this->m_optional_args.begin(); it != this->m_optional_args.end(); ++it)
		this->print_usage(it->first, it->second, false);

	cout << endl;
}

void Arguments_reader
::print_usage(std::vector<std::vector<std::string>> arg_groups)
{
	cout << "Usage: " << this->m_program_name;

	for (auto it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
	{
		const auto last = it->first.size() -1;
		if(it->second[0] != "")
			cout << ((it->first[last].size() == 1) ? " -" : " --") << it->first[last] << " <" << it->second[0] << ">";
		else
			cout << ((it->first[last].size() == 1) ? " -" : " --") << it->first[last];
	}
	cout << " [optional args...]" << endl << endl;

	auto req_args_cpy = this->m_required_args;
	auto opt_args_cpy = this->m_optional_args;

	for (auto i = 0; i < (int)arg_groups.size(); i++)
	{
		assert(arg_groups[i].size() > 1);

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
			cout << arg_groups[i][1] << ": " << endl;
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

			cout << endl;
		}
	}

	if (!req_args_cpy.empty() || !opt_args_cpy.empty())
	{
		cout << "Other parameter(s): " << endl;
		for (auto it = req_args_cpy.begin(); it != req_args_cpy.end(); ++it)
		{
			// gr->first is a prefix of it->first[0].
			this->print_usage(it->first, it->second, true);
			req_args_cpy.erase(it->first);
		}

		for (auto it = opt_args_cpy.begin(); it != opt_args_cpy.end(); ++it)
		{
			// gr->first is a prefix of it->first[0].
			this->print_usage(it->first, it->second, false);
			opt_args_cpy.erase(it->first);
		}

		cout << endl;
	}
}

void Arguments_reader
::print_usage(const vector<string> &tags, const vector<string> &values, const bool required)
{
	if (values.size() >= 2 && !values[1].empty())
	{
		const auto tab = "    ";
		const string delimiter = ", ";
		auto total_length = 0;
		cout << tab;
		for (auto i = 0; i < (int)tags.size() -1; i++)
		{
			cout << ((tags[i].length() == 1) ? "-" : "--") << tags[i] << delimiter;
			total_length += (tags[i].length() == 1 ? 1 : 2) + tags[i].length() + delimiter.length();
		}
		const auto last = tags.size() -1;
		cout << ((tags[last].length() == 1) ? "-" : "--") << tags[last];
		total_length += (tags[last].length() == 1 ? 1 : 2) + tags[last].length();

		for (unsigned i = 0; i < this->max_n_char_arg - total_length; i++) cout << " ";
		if (values.size() < 3)
		{
			if (!values[0].empty())
				cout << " <" << values[0] << ">";
		}
		else
		{
			auto entries = Arguments_reader::split(values[2]);
			string set;
			for (auto i = 0; i < (int)entries.size() -1; i++)
				set += entries[i] + "|";
			set += entries[entries.size() -1];

			cout << " <" << values[0] << "=" << set << ">";
		}
		if (required)
			cout << " {REQUIRED}";
		cout << endl;
		cout << tab << values[1] << endl;
	}
}

bool Arguments_reader
::check_arguments()
{
	auto success = true;
	for (auto it = this->m_args.begin(); it != this->m_args.end(); ++it)
	{
		if (this->m_required_args.find(it->first) != this->m_required_args.end())
			success = this->check_argument(it->first, this->m_required_args);
		else if (this->m_optional_args.find(it->first) != this->m_optional_args.end())
			success = this->check_argument(it->first, this->m_optional_args);
		else
			success = false;

		if (!success)
			break;
	}

	return success;
}

bool Arguments_reader
::check_argument(const vector<string> &tags, map<vector<string>, vector<string>> &args)
{
	// check if the input is positive
	if (args[tags][0] == "positive_int")
	{
		const auto int_num = std::stoi(this->m_args[tags]);
		if (int_num < 0)
		{
			cerr << bold_red("(EE) The \"") << ((tags[0].length() == 1) ? bold_red("-") : bold_red("--"))
			     << bold_red(tags[0]) << bold_red("\" argument have to be positive, exiting.") << endl;
			return false;
		}
	}

	// check if the input is positive
	if (args[tags][0] == "positive_float")
	{
		const auto float_num = std::stof(this->m_args[tags]);
		if (float_num < 0.f)
		{
			cerr << bold_red("(EE) The \"") << ((tags[0].length() == 1) ? bold_red("-") : bold_red("--"))
			     << bold_red(tags[0]) << bold_red("\" argument have to be positive, exiting.") << endl;
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
			string set = "<";
			for (auto i = 0; i < (int)entries.size() -1; i++)
				set += entries[i] + "|";
			set += entries[entries.size() -1] + ">";

			cerr << bold_red("(EE) The \"") << ((tags[0].length() == 1) ? bold_red("-") : bold_red("--"))
			     << bold_red(tags[0]) << bold_red("\" argument have to be in the ") << bold_red(set)
			     << bold_red(" set, exiting.") << endl;
			return false;
		}
	}

	return true;
}

vector<string> Arguments_reader
::split(string str)
{
	vector<string> str_splited;

	if (!str.empty())
	{
		// remove white spaces
		str.erase(remove(str.begin(), str.end(), ' '), str.end());

		// specify a delimiter
		string delimiter = ",";

		// extract each entry from "str"
		size_t pos = 0;
		while ((pos = str.find(delimiter)) != string::npos)
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
