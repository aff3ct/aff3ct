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
		this->m_argv[i] = argv[i];
}

Arguments_reader
::~Arguments_reader()
{
}

bool Arguments_reader
::parse_arguments(map<string, string> required_args, map<string, string> optional_args)
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
::sub_parse_arguments(map<string, string> args, unsigned short pos_arg)
{
	assert(pos_arg < this->m_argv.size());

	bool is_found = false;

	map<string, string>::iterator it;
	for(it = args.begin(); it != args.end(); ++it)
	{
		this->max_n_char_arg = max(this->max_n_char_arg, (unsigned int)it->first.size());

		string cur_arg = ((it->first.size() == 1) ? "-" : "--") + it->first;
		if(cur_arg == this->m_argv[pos_arg])
		{
			if(it->second != "")
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
	}

	return is_found;
}

bool Arguments_reader
::exist_argument(std::string tag)
{
	return (this->m_args.find(tag) != this->m_args.end());
}

string Arguments_reader
::get_argument(string tag)
{
	return this->m_args[tag];
}

bool Arguments_reader
::parse_doc_args(std::map<std::string, std::string> doc_args)
{
	bool re_val = true;

	if(doc_args.empty())
		re_val = false;

	map<string, string>::iterator it;
	for(it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
		if(!(doc_args.find(it->first) != doc_args.end()))
			re_val = false;
		else
			this->m_doc_args[it->first] = doc_args[it->first];

	for(it = this->m_optional_args.begin(); it != this->m_optional_args.end(); ++it)
		if(!(doc_args.find(it->first) != doc_args.end()))
			re_val = false;
		else
			this->m_doc_args[it->first] = doc_args[it->first];

	return re_val;
}

void Arguments_reader
::print_usage()
{
	cout << "Usage: " << this->m_program_name;

	map<string, string>::iterator it;
	for(it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
		if(it->second != "")
			cout << ((it->first.size() == 1) ? " -" : " --") << it->first << " " << it->second;
		else
			cout << ((it->first.size() == 1) ? " -" : " --") << it->first;

	for(it = this->m_optional_args.begin(); it != this->m_optional_args.end(); ++it)
		if(it->second != "")
			cout << ((it->first.size() == 1) ? " [-" : " [--") << it->first << " " << it->second << "]";
		else
			cout << ((it->first.size() == 1) ? " [-" : " [--") << it->first << "]";

	cout << endl;

	if(!this->m_doc_args.empty())
	{
		cout << endl;
		if (this->m_required_args.size())
			cout << "Required argument(s): " << endl
			     << "--------------------- " << endl << endl;
		for(it = this->m_required_args.begin(); it != this->m_required_args.end(); ++it)
			if(this->m_doc_args.find(it->first) != this->m_doc_args.end())
			{
				cout << "\t" << ((it->first.size() == 1) ? " -" : " --") << it->first;
				for(unsigned i = 0; i < this->max_n_char_arg - (unsigned)it->first.size(); i++) cout << " ";
				cout << "\t" << this->m_doc_args.find(it->first)->second << endl;
			}
		if (this->m_required_args.size()) cout << endl << endl;

		if (this->m_optional_args.size())
			cout << "Optional argument(s): " << endl
			     << "--------------------- " << endl << endl;
		for(it = this->m_optional_args.begin(); it != this->m_optional_args.end(); ++it)
			if(this->m_doc_args.find(it->first) != this->m_doc_args.end())
			{
				cout << "\t" << ((it->first.size() == 1) ? " -" : " --") << it->first;
				for(unsigned i = 0; i < this->max_n_char_arg - (unsigned)it->first.size(); i++) cout << " ";
				cout << "\t" << this->m_doc_args.find(it->first)->second << endl;
			}
	}
}

void Arguments_reader
::clear_arguments()
{
	this->m_required_args.clear();
	this->m_optional_args.clear();
	this->m_args.clear();
	this->m_doc_args.clear();
}
