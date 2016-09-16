/*!
 * \file    Arguments_reader.cpp
 * \brief   Command line arguments management class.
 * \author  A. Cassagne
 * \date    2014
 *
 * \section LICENSE
 * This file is under CC BY-NC-ND license (http://creativecommons.org/licenses/by-nc-nd/4.0/legalcode).
 *
 * \section DESCRIPTION
 * This is the traditional entry file for the code execution.
 */
#include <iostream>
#include <cassert>
#include <algorithm>

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
::parse_arguments(map<vector<string>, vector<string>> required_args, map<vector<string>, vector<string>> optional_args)
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
::sub_parse_arguments(map<vector<string>, vector<string>> args, unsigned short pos_arg)
{
	assert(pos_arg < this->m_argv.size());

	auto is_found = false;
	for (auto it = args.begin(); it != args.end(); ++it)
	{
		assert(it->first .size() > 0);
		assert(it->second.size() > 0);

		auto i = 0;
		do
		{
			// remember the biggest argument length to display the doc after
			this->max_n_char_arg = max(this->max_n_char_arg, (unsigned int)it->first[i].length());

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
::exist_argument(vector<string> tags)
{
	return (this->m_args.find(tags) != this->m_args.end());
}

string Arguments_reader
::get_argument(vector<string> tags)
{
	return this->m_args[tags];
}

void Arguments_reader
::print_usage()
{
	cout << "Usage: " << this->m_program_name;

	for (auto it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
		if(it->second[0] != "")
			cout << ((it->first[0].size() == 1) ? " -" : " --") << it->first[0] << " " << it->second[0];
		else
			cout << ((it->first[0].size() == 1) ? " -" : " --") << it->first[0];

	for (auto it = this->m_optional_args.begin(); it != this->m_optional_args.end(); ++it)
		if(it->second[0] != "")
			cout << ((it->first[0].size() == 1) ? " [-" : " [--") << it->first[0] << " " << it->second[0] << "]";
		else
			cout << ((it->first[0].size() == 1) ? " [-" : " [--") << it->first[0] << "]";

	cout << endl << endl;

	if (this->m_required_args.size())
		cout << "Required argument(s): " << endl
			 << "--------------------- " << endl << endl;
	for (auto it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
		if (it->second.size() >= 2 && !it->second[1].empty())
		{
			cout << "\t" << ((it->first[0].length() == 1) ? " -" : " --") << it->first[0];
			for(unsigned i = 0; i < this->max_n_char_arg - (unsigned)it->first[0].length(); i++) cout << " ";
			cout << "\t" << it->second[1] << endl;
		}
	if (this->m_required_args.size()) cout << endl << endl;

	if (this->m_optional_args.size())
		cout << "Optional argument(s): " << endl
			 << "--------------------- " << endl << endl;
	for(auto it = this->m_optional_args.begin(); it != this->m_optional_args.end(); ++it)
		if (it->second.size() >= 2 && !it->second[1].empty())
		{
			cout << "\t" << ((it->first[0].length() == 1) ? " -" : " --") << it->first[0];
			for(unsigned i = 0; i < this->max_n_char_arg - (unsigned)it->first[0].length(); i++) cout << " ";
			cout << "\t" << it->second[1] << endl;
		}
}

void Arguments_reader
::clear_arguments()
{
	this->m_required_args.clear();
	this->m_optional_args.clear();
	this->m_args.clear();
}
