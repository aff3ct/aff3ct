
#include "Reporter_stream.hpp"


const char        aff3ct::tools::Reporter_stream::line_separator        = '-';
const std::string aff3ct::tools::Reporter_stream::col_separator         = "|";
const std::string aff3ct::tools::Reporter_stream::group_separator       = "||";
const std::string aff3ct::tools::Reporter_stream::spaced_scol_separator = " |";
const std::string aff3ct::tools::Reporter_stream::spaced_dcol_separator = " ||";
const std::string aff3ct::tools::Reporter_stream::data_tag              = "  ";
const rang::style aff3ct::tools::Reporter_stream::legend_style          = rang::style::bold;
const rang::style aff3ct::tools::Reporter_stream::report_style          = rang::style::bold;

#ifdef _WIN32
const int aff3ct::tools::Reporter_stream::column_width = 11;
#else
const int aff3ct::tools::Reporter_stream::column_width = 10;
#endif
