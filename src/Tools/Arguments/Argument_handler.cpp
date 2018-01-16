#include <iostream>
#include <sstream>
#include <algorithm>
#include <type_traits>

#include "Tools/Display/bash_tools.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"

#include "Argument_handler.hpp"

using namespace aff3ct::tools;

Argument_handler
::Argument_handler(const int argc, const char** argv)
{
	if (argc <= 0)
	{
		std::stringstream message;
		message << "'argc' has to be greater than 0 ('argc' = " << argc << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->m_program_name = argv[0];

	command.resize(argc-1);

	for (unsigned short i = 0; i < argc-1; ++i)
		this->command[i] = std::string(argv[i+1]);
}

Argument_handler
::~Argument_handler()
{
}

Argument_map_value Argument_handler
::parse_arguments(const Argument_map_info &req_args, const Argument_map_info &opt_args,
                  std::vector<std::string> &warnings, std::vector<std::string> &errors)
{
	Argument_map_value m_arg_v;
	std::vector<bool> command_found_pos(this->command.size(),false);

	auto req_found_pos = this->sub_parse_arguments(req_args, m_arg_v, command_found_pos, errors);
	auto opt_found_pos = this->sub_parse_arguments(opt_args, m_arg_v, command_found_pos, errors);


	for (unsigned i = 0; i < req_found_pos.size(); ++i)
	{
		if (req_found_pos[i])
			continue;

		auto it_arg = req_args.begin();
		std::advance(it_arg, i);
		std::string message = "The \"" + print_tag(it_arg->first) + "\" argument is required.";
		errors.push_back(message);
	}

	for (unsigned i = 0; i < command_found_pos.size(); ++i)
	{
		if (command_found_pos[i])
			continue;

		std::string message = "Unknown argument \"" + this->command[i] + "\" at the position "
		                     + std::to_string(i+1) + ".";
		warnings.push_back(message);
	}

	return m_arg_v;
}

std::vector<bool> Argument_handler
::sub_parse_arguments(const Argument_map_info &args, Argument_map_value& arg_v, std::vector<bool>& command_found_pos,
                      std::vector<std::string>& messages)
{
	std::map<Argument_tag, size_t> tag_found_pos;

	std::vector<bool> args_found_pos(args.size(), false);

	if (this->command.size() != command_found_pos.size())
		std::runtime_error("'command' and 'command_found_pos' vectors have to have the same length.");

	// unsigned found_arg_count = 0; // number of different arguments found in the command line

	for (auto it_arg_info = args.begin(); it_arg_info != args.end(); it_arg_info++)
	{
		std::string message;

		// try to find each tag version of each argument
		for (auto it_tag = it_arg_info->first.begin(); it_tag != it_arg_info->first.end(); it_tag++)
		{
			const auto cur_tag = this->print_tag(*it_tag);

			// find all occurances of this tag in the command line
			for (unsigned ix_arg_val = 0; ix_arg_val < this->command.size(); ++ix_arg_val)
			{
				if (cur_tag == this->command[ix_arg_val]) // the tag has been found
				{
					// check if the tag has already been found in the command
					auto it_prev_pos = tag_found_pos.find(it_arg_info->first);

					if (it_prev_pos == tag_found_pos.end())
						tag_found_pos[it_arg_info->first] = ix_arg_val; // not yet found then set the found position

					else if (it_prev_pos->second >= ix_arg_val) // the tag is set further in the command so ignore this one
						continue;
					// else the tag is set previously in the command so overwrite it


					if(it_arg_info->second->type->get_title() == "") // do not wait for a value after the tag
					{
						// auto it = arg_v.find(it_arg_info->first);
						// if (it == arg_v.end())
						// 	found_arg_count++;

						arg_v[it_arg_info->first] = std::make_pair("", it_arg_info->second);
						command_found_pos[ix_arg_val] = true;
						args_found_pos[std::distance(args.begin(), it_arg_info)] = true;

					}
					else // wait for a value with the tag
					{
						if(ix_arg_val != (this->command.size() -1))
						{
							// auto it = arg_v.find(it_arg_info->first);
							// if (it == arg_v.end())
							// 	found_arg_count++;

							arg_v[it_arg_info->first] = std::make_pair(this->command[ix_arg_val +1], it_arg_info->second);
							command_found_pos[ix_arg_val   ] = true;
							command_found_pos[ix_arg_val +1] = true;
							args_found_pos[std::distance(args.begin(), it_arg_info)] = true;

							// check the found argument
							try
							{
								it_arg_info->second->type->check(this->command[ix_arg_val +1]);
								message = "";
							}
							catch(std::exception& e)
							{
								message = "The \"" + print_tag(it_arg_info->first) + "\" argument ";
								message += e.what();
								message += "; given at the position " + std::to_string(ix_arg_val+1) + " '";
								message += this->command[ix_arg_val] + " " + this->command[ix_arg_val +1] + "'.";
							}
						}
					}
				}
			}
		}

		if (message != "")
			messages.push_back(message);
	}

	return args_found_pos;
}

void Argument_handler
::print_usage(const Argument_map_info &req_args) const
{
	std::cout << "Usage: " << this->m_program_name;

	std::vector<std::string> existing_flags;

	for (auto it = req_args.begin(); it != req_args.end(); ++it)
	{
		if (std::find(existing_flags.begin(), existing_flags.end(), it->first.back()) == existing_flags.end())
		{
			if(it->second->type->get_title() != "")
				std::cout << " " + print_tag(it->first.back()) << " <" << it->second->type->get_short_title() << ">";
			else
				std::cout << " " + print_tag(it->first.back());

			existing_flags.push_back(it->first.back());
		}
	}
	std::cout << " [optional args...]" << std::endl;
}

size_t Argument_handler
::find_longest_tags(const Argument_map_info &args) const
{
	size_t max_n_char_arg = 0;
	for (auto it = args.begin(); it != args.end(); ++it)
		max_n_char_arg = std::max(this->print_tag(it->first).size(), max_n_char_arg);

	return max_n_char_arg;
}

void Argument_handler
::print_help(const Argument_map_info &req_args, const Argument_map_info &opt_args) const
{
	this->print_usage(req_args);
	std::cout << std::endl;

	// found first the longest tag to align informations
	size_t max_n_char_arg = std::max(find_longest_tags(req_args), find_longest_tags(opt_args));

	// print arguments
	for (auto it = req_args.begin(); it != req_args.end(); ++it)
		this->print_help(it->first, *it->second, max_n_char_arg, true);

	for (auto it = opt_args.begin(); it != opt_args.end(); ++it)
		this->print_help(it->first, *it->second, max_n_char_arg, false);

	std::cout << std::endl;
}

std::string split_doc(const std::string& line, const std::string& start_line, const unsigned max_char)
{
	auto words = split(line);
	unsigned cur_char = start_line.length();
	std::string splitted_lines = start_line;
	for (auto &w : words)
	{
		if (cur_char + (unsigned)w.size() > max_char)
		{
			splitted_lines += "\n" + start_line;
			cur_char = start_line.length();
		}
		splitted_lines += w + " ";

		cur_char += w.length() +1;
	}

	return splitted_lines;
}

void Argument_handler
::print_help(const Argument_tag &tags, const Argument_info &info, const size_t max_n_char_arg, const bool required) const
{
	Format arg_format = 0;

	std::string tags_str = this->print_tag(tags);
	tags_str.append(max_n_char_arg - tags_str.size(), ' ');

	const std::string tab = "    ";
	std::string tabr = tab;
	if (required)
		tabr = format("{R} ", arg_format | Style::BOLD | FG::Color::ORANGE);

	std::cout << tabr << format(tags_str, arg_format | Style::BOLD);

	if (info.type->get_title().size())
		std::cout << format(" <" + info.type->get_title() + ">", arg_format | FG::GRAY);

	std::cout << std::endl;
	auto splitted_doc = split_doc(info.doc, tab + "  ", 80);
	std::cout << format(splitted_doc, arg_format) << std::endl;
}

void Argument_handler
::print_help_title(const std::string& title) const
{
	Format head_format = Style::BOLD | Style::ITALIC | FG::Color::MAGENTA | FG::INTENSE;

	std::cout << format(title + ":", head_format) << std::endl;
}

void Argument_handler
::print_help(const Argument_map_info &req_args, const Argument_map_info &opt_args, const Argument_map_group& arg_groups) const
{
	this->print_usage(req_args);
	std::cout << std::endl;

	// found first the longest tag to align informations
	size_t max_n_char_arg = std::max(find_longest_tags(req_args), find_longest_tags(opt_args));
	bool title_displayed = false;

	// the already displayed positions then it can't be displayed several times in different sub parameters
	std::vector<bool> req_args_print_pos(req_args.size(), false);
	std::vector<bool> opt_args_print_pos(opt_args.size(), false);

	// display each group
	for (auto it_grp = arg_groups.begin(); it_grp != arg_groups.end(); it_grp++)
	{
		title_displayed = false;

		auto& prefix = it_grp->first;

		// display first the required arguments of this group
		for (auto it_arg = req_args.begin(); it_arg != req_args.end(); it_arg++)
		{
			auto& tag  = it_arg->first.front();
			auto  dist = std::distance(req_args.begin(), it_arg);

			if (tag.find(prefix) == 0 && !req_args_print_pos[dist])
			{
				if (!title_displayed)
				{
					print_help_title(it_grp->second);
					title_displayed = true;
				}

				req_args_print_pos[dist] = true;

				this->print_help(it_arg->first, *it_arg->second, max_n_char_arg, true);
			}
		}

		// display then the optional arguments of this group
		for (auto it_arg = opt_args.begin(); it_arg != opt_args.end(); it_arg++)
		{
			auto& tag  = it_arg->first.front();
			auto  dist = std::distance(opt_args.begin(), it_arg);

			if (tag.find(prefix) == 0 && !opt_args_print_pos[dist])
			{
				if (!title_displayed)
				{
					print_help_title(it_grp->second);
					title_displayed = true;
				}

				opt_args_print_pos[dist] = true;

				this->print_help(it_arg->first, *it_arg->second, max_n_char_arg, false);
			}
		}

		if (title_displayed)
			std::cout << std::endl;
	}


	title_displayed = false;
	// display the other required parameters
	for (auto it_arg = req_args.begin(); it_arg != req_args.end(); it_arg++)
	{
		auto& tag = it_arg->first.front();
		bool found = false;

		if (req_args_print_pos[std::distance(req_args.begin(), it_arg)])
			continue; // already displayed

		for (auto it_grp = arg_groups.begin(); it_grp != arg_groups.end(); it_grp++)
		{
			auto& prefix = it_grp->first;

			if (tag.find(prefix) == 0)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (!title_displayed)
			{
				print_help_title("Other parameter(s)");
				title_displayed = true;
			}

			this->print_help(it_arg->first, *it_arg->second, max_n_char_arg, true);
		}
	}

	// display the other optional parameters
	for (auto it_arg = opt_args.begin(); it_arg != opt_args.end(); it_arg++)
	{
		auto& tag = it_arg->first.front();
		bool found = false;

		if (opt_args_print_pos[std::distance(opt_args.begin(), it_arg)])
			continue; // already displayed

		for (auto it_grp = arg_groups.begin(); it_grp != arg_groups.end(); it_grp++)
		{
			auto& prefix = it_grp->first;

			if (tag.find(prefix) == 0)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (!title_displayed)
			{
				print_help_title("Other parameter(s)");
				title_displayed = true;
			}

			this->print_help(it_arg->first, *it_arg->second, max_n_char_arg, false);
		}
	}
}

std::string Argument_handler
::print_tag(const std::string& tag)
{
	return ((tag.size() == 1) ? "-" : "--") + tag;
}

std::string Argument_handler
::print_tag(const Argument_tag &tags)
{
	std::string txt;
	for (unsigned i = 0; i < tags.size(); i++)
		txt += print_tag(tags[i]) + ((i < tags.size()-1)?", ":"");
	return txt;
}