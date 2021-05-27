/*!
 * \file
 * \brief Class tools::Interleaver_core_row_column.
 */
#ifndef INTERLEAVER_CORE_ROW_COLUMN_HPP
#define INTERLEAVER_CORE_ROW_COLUMN_HPP

#include <cstdint>
#include <string>

#include "Tools/Exception/exception.hpp"

#include "Tools/Interleaver/Interleaver_core.hpp"

/*
 * This interleaver is such as a table that is written row by row and read column by column
 */
namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_row_column : public Interleaver_core<T>
{
public:
	enum class READ_ORDER : int8_t
	{
		TOP_LEFT,    // read from the top row by row from left to right
		TOP_RIGHT,   // read from the top row by row from right to left
		BOTTOM_LEFT, // read from the bottom row by row from left to right
		BOTTOM_RIGHT // read from the bottom row by row from right to left
	};

private:
	const int n_cols;
	const int n_rows;
	const READ_ORDER read_order;

public:
	Interleaver_core_row_column(const int size, const int n_cols, const std::string& read_order);
	Interleaver_core_row_column(const int size, const int n_cols, const aff3ct::tools::Interleaver_core_row_column<T>::READ_ORDER read_order);
	virtual ~Interleaver_core_row_column() = default;

	virtual Interleaver_core_row_column<T>* clone() const;

protected:
	void gen_lut(T *lut, const size_t frame_id);
};
}
}

#endif /* INTERLEAVER_CORE_ROW_COLUMN_HPP */
