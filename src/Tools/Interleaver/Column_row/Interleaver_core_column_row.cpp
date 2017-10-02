#include "Interleaver_core_column_row.hpp"

#include <sstream>

#include "Tools/Exception/exception.hpp"


using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_column_row<T>
::Interleaver_core_column_row(const int size, const int n_cols, const int n_frames)
: Interleaver_core<T>(size, "row_column", false, n_frames), n_cols(n_cols), n_rows(size / n_cols)
{
	if (n_rows * n_cols != size)
	{
		std::stringstream message;
		message << "'size' has to be equal to 'n_rows' * 'n_cols' ('size' = " << size
		        << ", 'n_rows' = " << n_rows << ", 'n_cols' = " << n_cols << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
Interleaver_core_column_row<T>
::~Interleaver_core_column_row()
{
}

template <typename T>
void Interleaver_core_column_row<T>
::gen_lut(T *lut, const int frame_id)
{
	for (auto i = 0; i < n_cols; i++)
		for (auto j = 0; j < n_rows; j++)
			lut[j * n_cols +i] = i * n_rows +j;
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_column_row<uint8_t >;
template class aff3ct::tools::Interleaver_core_column_row<uint16_t>;
template class aff3ct::tools::Interleaver_core_column_row<uint32_t>;
template class aff3ct::tools::Interleaver_core_column_row<uint64_t>;
// ==================================================================================== explicit template instantiation
