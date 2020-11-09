#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Reporter/Probe/Reporter_probe_decstat.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Reporter_probe_decstat
::Reporter_probe_decstat(const std::string &group_name, const std::string &group_description, const int n_frames)
: Reporter_probe(group_name, group_description, n_frames)
{
	if ((size_t)n_frames >= sizeof(int32_t) * 8)
	{
		std::stringstream message;
		message << "'n_frames' has to be smaller than 32. ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Reporter_probe_decstat
::Reporter_probe_decstat(const std::string &group_name, const int n_frames)
: Reporter_probe_decstat(group_name, "", n_frames)
{
}

void Reporter_probe_decstat
::probe(const std::string &name, const void *data, const size_t frame_id)
{
	const int col = this->name_to_col[name];
	if (this->datatypes[col] == typeid(int32_t))
	{
		if (frame_id == 0)
		{
			int32_t packed_statuses = ((int32_t*)data)[0];
			std::vector<int> statuses(n_frames);
			for (auto f = n_frames -1; f >= 0; f--)
			{
				statuses[f] = packed_statuses & 1;
				packed_statuses >>= 1;
			}
			for (auto s : statuses)
				this->push<int32_t>(col, &s);
		}
	}
	else
	{
		std::stringstream message;
		message << "Unsupported type.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}
