#ifndef INTERLEAVER_CORE_ROW_COLUMN_HPP
#define INTERLEAVER_CORE_ROW_COLUMN_HPP

#include "Tools/Exception/exception.hpp"

#include "../Interleaver_core.hpp"

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
private:
	const int n_cols;
	const int n_rows;

public:
	Interleaver_core_row_column(const int size, const int n_cols, const int n_frames = 1);
	virtual ~Interleaver_core_row_column();

protected:
	void gen_lut(T *lut, const int frame_id);
};
}
}

#endif /* INTERLEAVER_CORE_ROW_COLUMN_HPP */
