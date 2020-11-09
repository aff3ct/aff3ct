#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Interleaver/Row_column/Interleaver_core_row_column.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
static typename Interleaver_core_row_column<T>::READ_ORDER read_order_cvt(const std::string& read_order)
{
	if (read_order == "TOP_LEFT")
		return Interleaver_core_row_column<T>::READ_ORDER::TOP_LEFT;
	if (read_order == "TOP_RIGHT")
		return Interleaver_core_row_column<T>::READ_ORDER::TOP_RIGHT;
	if (read_order == "BOTTOM_LEFT")
		return Interleaver_core_row_column<T>::READ_ORDER::BOTTOM_LEFT;
	if (read_order == "BOTTOM_RIGHT")
		return Interleaver_core_row_column<T>::READ_ORDER::BOTTOM_RIGHT;

	std::stringstream message;
	message << "Unknown 'read_order' ( = " << read_order << ").";
	throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
}

template <typename T>
Interleaver_core_row_column<T>
::Interleaver_core_row_column(const int size, const int n_cols, const std::string& read_order)
: Interleaver_core_row_column<T>(size, n_cols, read_order_cvt<T>(read_order))
{
}

template <typename T>
Interleaver_core_row_column<T>
::Interleaver_core_row_column(const int size, const int n_cols, const READ_ORDER read_order)
: Interleaver_core<T>(size, "row_column", false), n_cols(n_cols), n_rows(size / n_cols), read_order(read_order)
{
	if (n_rows * n_cols != size)
	{
		std::stringstream message;
		message << "'size' has to be equal to 'n_rows' * 'n_cols' ('size' = " << size
		        << ", 'n_rows' = " << n_rows << ", 'n_cols' = " << n_cols << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->init();
}

template <typename T>
Interleaver_core_row_column<T>* Interleaver_core_row_column<T>
::clone() const
{
	auto t = new Interleaver_core_row_column(*this);
	return t;
}

template <typename T>
void Interleaver_core_row_column<T>
::gen_lut(T *lut, const size_t frame_id)
{
	T idx = 0;
	switch (read_order)
	{
		case READ_ORDER::TOP_LEFT:
			for (auto i = 0; i < n_cols; i++)
				for (auto j = 0; j < n_rows; j++)
					lut[idx++] = j * n_cols +i;
		break;
		case READ_ORDER::TOP_RIGHT:
			for (auto i = n_cols - 1; i >= 0; i--)
				for (auto j = 0; j < n_rows; j++)
					lut[idx++] = j * n_cols +i;
		break;
		case READ_ORDER::BOTTOM_LEFT:
			for (auto i = 0; i < n_cols; i++)
				for (auto j = n_rows -1; j >= 0; j--)
					lut[idx++] = j * n_cols +i;
		break;
		case READ_ORDER::BOTTOM_RIGHT:
			for (auto i = n_cols - 1; i >= 0; i--)
				for (auto j = n_rows -1; j >= 0; j--)
					lut[idx++] = j * n_cols +i;
		break;
	};
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_row_column<uint8_t >;
template class aff3ct::tools::Interleaver_core_row_column<uint16_t>;
template class aff3ct::tools::Interleaver_core_row_column<uint32_t>;
template class aff3ct::tools::Interleaver_core_row_column<uint64_t>;
// ==================================================================================== explicit template instantiation
