#include <algorithm>
#include <utility>
#include <sstream>
#include <cmath>

#include "Factory/Factory.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Factory/Header.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

void aff3ct::tools::Header::print_parameters(std::string grp_key, std::string grp_name, header_list header,
                                             int max_n_chars, std::ostream& stream)
{
	auto key = tools::split(grp_key, '-');

	if (key.size() == 1)
	{
		stream << rang::tag::comment << "* " << rang::style::bold << rang::style::underline << grp_name << rang::style::reset << " ";
		for (auto i = 0; i < 46 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;
	}
	else if (key.size() > 1)
	{
		stream << rang::tag::comment << "   " << rang::style::bold << rang::style::underline << grp_name << rang::style::reset << " ";
		for (auto i = 0; i < 45 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;
	}

	std::vector<std::string> dup;
	for (auto i = 0; i < (int)header.size(); i++)
	{
		if (std::find(dup.begin(), dup.end(), header[i].first + header[i].second) == dup.end())
		{
			stream << rang::tag::comment << "   ** " << rang::style::bold << header[i].first << rang::style::reset;
			for (auto j = 0; j < max_n_chars - (int)header[i].first.length(); j++) stream << " ";
			stream << " = " << header[i].second << std::endl;

			dup.push_back(header[i].first + header[i].second);
		}
	}
}

void aff3ct::tools::Header::print_parameters(const std::vector<factory::Factory*> &factories, const bool full,
                                             std::ostream& stream)
{
	int max_n_chars = 0;
	for (auto *f : factories)
	{
		std::map<std::string,header_list> headers;
		f->get_headers(headers, full);

		for (auto &h : headers)
			if (full || (!full && h.second.size() && (h.second[0].first != "Type" || h.second[0].second != "NO")))
				aff3ct::tools::Header::compute_max_n_chars(h.second, max_n_chars);
	}

	std::vector<header_list> dup_h;
	std::vector<std::string> dup_n;
	for (auto *f : factories)
	{
		std::map<std::string,header_list> headers;
		f->get_headers(headers, full);

		auto prefixes = f->get_prefixes();
		auto short_names = f->get_short_names();

		if (prefixes.size() != short_names.size())
		{
			std::stringstream message;
			message << "'prefixes.size()' has to be equal to 'short_names.size()' ('prefixes.size()' = "
			        << prefixes.size() << ", 'short_names.size()' = " << short_names.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		bool print_first_title = false;
		for (size_t i = 1; i < prefixes.size(); i++)
		{
			auto h = headers[prefixes[i]];
			auto key = tools::split(prefixes[i], '-');

			if (key[0] == prefixes[0] && h.size())
			{
				print_first_title = true;
				break;
			}
		}

		for (size_t i = 0; i < prefixes.size(); i++)
		{
			auto h = headers[prefixes[i]];
			auto print_head = (i == 0) ? print_first_title || h.size() : h.size();

			if (full || (!full && h.size() && (h[0].first != "Type" || h[0].second != "NO")))
			{
				auto n = short_names[i];
				if (print_head && (std::find(dup_h.begin(), dup_h.end(), h) == dup_h.end() ||
				                   std::find(dup_n.begin(), dup_n.end(), n) == dup_n.end()))
				{
					aff3ct::tools::Header::print_parameters(prefixes[i], n, h, max_n_chars);

					dup_h.push_back(h);
					dup_n.push_back(n);
				}
			}
		}
	}
}

void aff3ct::tools::Header::compute_max_n_chars(const header_list& header, int& max_n_chars)
{
	for (unsigned i = 0; i < header.size(); i++)
		max_n_chars = std::max(max_n_chars, (int)header[i].first.length());
}
