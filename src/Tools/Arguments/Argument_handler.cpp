#include <algorithm>
#include <sstream>
#include <utility>
#include <map>
#include <rang.hpp>

#include "Tools/general_utils.h"
#include "Tools/Arguments/Argument_handler.hpp"

using namespace aff3ct::tools;

Argument_handler
::Argument_handler(const int argc, const char** argv, std::ostream& help_os)
: help_os(help_os), help_display_keys(false)
{
	if (argc <= 0)
	{
		std::stringstream message;
		message << "'argc' has to be greater than 0 ('argc' = " << argc << ").";
		throw std::invalid_argument(message.str());
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

void Argument_handler
::set_help_display_keys(const bool display)
{
	this->help_display_keys = display;
}

bool Argument_handler
::is_linked(const Argument_map_info &args, const Argument_map_value& arg_v, const Argument_tag &tag) const
{
	auto& links = args.get_links();

	size_t tag_link_pos = 0;
	while((tag_link_pos = links.find(tag, tag_link_pos)) < links.size())
	{
		const auto& link = links[tag_link_pos];
		auto other_tag = link.other_tag(tag);

		if (arg_v.exist(other_tag))
		{ // the other argument has been given
			if (link.callback == nullptr)
				return true;

			auto val = arg_v.at(other_tag);

			if (link.is_first_tag(tag))
				return link.callback(nullptr, (const void*)&val);
			else
				return link.callback((const void*)&val, nullptr);
		}

		tag_link_pos++;
	}

	return false; // no link found
}

Argument_map_value Argument_handler
::parse_arguments(const Argument_map_info &args,
                  std::vector<std::string> &warnings, std::vector<std::string> &errors)
{
	Argument_map_value m_arg_v;
	std::vector<bool> command_found_pos(this->command.size(),false);

	auto args_found_pos = this->sub_parse_arguments(args, m_arg_v, command_found_pos, errors);

	auto it_arg = args.begin();
	for (unsigned i = 0; i < args_found_pos.size(); i++, it_arg++)
	{
		if (!args_found_pos[i]
		  && it_arg->second->rank == arg_rank::REQ
		  && !is_linked(args, m_arg_v, it_arg->first)) // check if any linked arguments, has been given
		{
			std::string message = "The \"" + print_tag(it_arg->first) + "\" argument is required.";
			errors.push_back(message);
		}
	}

	for (unsigned i = 0; i < command_found_pos.size(); ++i)
	{
		if (!command_found_pos[i])
		{
			std::string message = "Unknown argument \"" + this->command[i] + "\" at the position "
			                     + std::to_string(i+1) + ".";
			warnings.push_back(message);
		}
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
		std::runtime_error("'command' and 'command_found_pos' vectors shall have the same length.");

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
::print_usage(const Argument_map_info &args) const
{
	help_os << "Usage: " << this->m_program_name;

	std::vector<std::string> existing_flags;

	for (auto it = args.begin(); it != args.end(); ++it)
	{
		if (it->second->rank == arg_rank::REQ
		 && std::find(existing_flags.begin(), existing_flags.end(), it->first.back()) == existing_flags.end())
		{
			if(it->second->type->get_title() != "")
				help_os << " " + print_tag(it->first.back()) << " <" << it->second->type->get_short_title() << ">";
			else
				help_os << " " + print_tag(it->first.back());

			existing_flags.push_back(it->first.back());
		}
	}
	help_os << " [optional args...]" << std::endl;
}

size_t Argument_handler
::find_longest_tags(const Argument_map_info &args) const
{
	size_t longest_tag = 0;
	for (auto it = args.begin(); it != args.end(); ++it)
		longest_tag = std::max(this->print_tag(it->first).size(), longest_tag);

	return longest_tag;
}

void Argument_handler
::print_help(const Argument_map_info &args, const bool print_advanced_args) const
{
	this->print_usage(args);
	help_os << std::endl;

	// found first the longest tag to align informations
	size_t longest_tag = find_longest_tags(args);

	// print first REQUIRED arguments
	for (auto it = args.begin(); it != args.end(); ++it)
		if (it->second->rank == arg_rank::REQ)
			this->print_help(it->first, *it->second, longest_tag);

	// then print others
	for (auto it = args.begin(); it != args.end(); ++it)
		if (it->second->rank == arg_rank::OPT
		 || (print_advanced_args && it->second->rank == arg_rank::ADV))
			this->print_help(it->first, *it->second, longest_tag);

	help_os << std::endl;
}

std::string split_doc(const std::string& line, const std::string& start_line, const unsigned max_char)
{
	auto words = split(line);
	unsigned cur_char = (unsigned)start_line.length();
	std::string splitted_lines = start_line;
	for (auto &w : words)
	{
		if (cur_char + (unsigned)w.size() > max_char)
		{
			splitted_lines += "\n" + start_line;
			cur_char = (unsigned)start_line.length();
		}
		splitted_lines += w + " ";

		cur_char += (unsigned)w.length() +1;
	}

	return splitted_lines;
}

void Argument_handler
::print_help(const Argument_tag &tags, const Argument_info &info, const size_t longest_tag) const
{
	const std::string tab = "    ";

	switch (info.rank)
	{
		case arg_rank::OPT :
			help_os << tab;
			break;

		case arg_rank::REQ :
			help_os << rang::style::bold << rang::fg::red << "{R} " << rang::style::reset;
			break;

		case arg_rank::ADV :
			help_os << rang::style::bold << rang::fg::blue << "{A} " << rang::style::reset;
			break;
	}

	std::string tags_str = this->print_tag(tags);
	tags_str.append(longest_tag - tags_str.size(), ' ');

	help_os << rang::style::bold << tags_str << rang::style::reset;

	if (info.type->get_title().size())
		help_os << rang::fg::gray << " <" << info.type->get_title() << ">" << rang::style::reset;
	help_os << std::endl;

	if (this->help_display_keys)
		help_os << rang::fg::gray << tab + "  " << "[" << info.key << "]" << rang::style::reset << std::endl;

	auto splitted_doc = split_doc(info.doc, tab + "  ", 80);
	help_os << splitted_doc << std::endl;
}

void Argument_handler
::print_help_title(const std::string& title) const
{
	help_os << rang::style::bold << rang::style::italic << rang::fg::magenta << title << ":" << rang::style::reset
	        << std::endl;
}

void Argument_handler
::print_help(const Argument_map_info &args, const Argument_map_group& arg_groups,
             const bool print_advanced_args) const
{
	this->print_usage(args);
	help_os << std::endl;

	// found first the longest tag to align informations
	size_t longest_tag = find_longest_tags(args);
	bool title_displayed;

	// the already displayed positions then it can't be displayed several times in different sub parameters
	std::vector<bool> args_print_pos(args.size(), false);

	// display each group
	for (auto it_grp = arg_groups.begin(); it_grp != arg_groups.end(); it_grp++)
	{
		title_displayed = false;

		auto& prefix = it_grp->first;

		// display first the REQUIRED arguments of this group
		for (auto it_arg = args.begin(); it_arg != args.end(); it_arg++)
		{
			if (it_arg->second->rank != arg_rank::REQ)
				continue;

			auto& tag  = it_arg->first.front();
			auto  dist = std::distance(args.begin(), it_arg);

			if (tag.find(prefix) == 0 && !args_print_pos[dist])
			{
				if (!title_displayed)
				{
					print_help_title(it_grp->second);
					title_displayed = true;
				}

				args_print_pos[dist] = true;

				this->print_help(it_arg->first, *it_arg->second, longest_tag);
			}
		}

		// display then the OPTIONAL and ADVANCED arguments of this group
		for (auto it_arg = args.begin(); it_arg != args.end(); it_arg++)
		{
			if (it_arg->second->rank == arg_rank::REQ
			 || (!print_advanced_args && it_arg->second->rank == arg_rank::ADV))
				continue;

			auto& tag  = it_arg->first.front();
			auto  dist = std::distance(args.begin(), it_arg);

			if (tag.find(prefix) == 0 && !args_print_pos[dist])
			{
				args_print_pos[dist] = true;

				if (it_arg->second->rank == arg_rank::ADV && !print_advanced_args)
					continue;

				if (!title_displayed)
				{
					print_help_title(it_grp->second);
					title_displayed = true;
				}

				this->print_help(it_arg->first, *it_arg->second, longest_tag);
			}
		}

		if (title_displayed)
			help_os << std::endl;
	}


	title_displayed = false;
	// display the other REQUIRED parameters
	for (auto it_arg = args.begin(); it_arg != args.end(); it_arg++)
	{
		if (it_arg->second->rank != arg_rank::REQ)
			continue;

		if (args_print_pos[std::distance(args.begin(), it_arg)])
			continue; // already displayed

		// bool found = false;
		// auto& tag = it_arg->first.front();
		// for (auto it_grp = arg_groups.begin(); it_grp != arg_groups.end(); it_grp++)
		// {
		// 	auto& prefix = it_grp->first;

		// 	if (tag.find(prefix) == 0)
		// 	{
		// 		found = true;
		// 		help_os << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		// 		break;
		// 	}
		// }

		// if (!found)
		// {
			if (!title_displayed)
			{
				print_help_title("Other parameter(s)");
				title_displayed = true;
			}

			this->print_help(it_arg->first, *it_arg->second, longest_tag);
		// }
	}

	// display the other OPTIONAL and ADVANCED parameters
	for (auto it_arg = args.begin(); it_arg != args.end(); it_arg++)
	{
		if (it_arg->second->rank == arg_rank::REQ
		 || (!print_advanced_args && it_arg->second->rank == arg_rank::ADV))
			continue;

		if (args_print_pos[std::distance(args.begin(), it_arg)])
			continue; // already displayed

		// bool found = false;
		// auto& tag = it_arg->first.front();
		// for (auto it_grp = arg_groups.begin(); it_grp != arg_groups.end(); it_grp++)
		// {
		// 	auto& prefix = it_grp->first;

		// 	if (tag.find(prefix) == 0)
		// 	{
		// 		found = true;
		// 		break;
		// 	}
		// }

		// if (!found)
		// {
			if (!title_displayed)
			{
				print_help_title("Other parameter(s)");
				title_displayed = true;
			}

			this->print_help(it_arg->first, *it_arg->second, longest_tag);
		// }
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