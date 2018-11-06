#ifndef INTERLEAVER_CORE_RANDOM_COLUMN_HPP
#define INTERLEAVER_CORE_RANDOM_COLUMN_HPP

#include <random>

#include "../Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_random_column : public Interleaver_core<T>
{
private:
	std::mt19937 rd_engine;

	const int n_cols;
	const int col_size;

public:
	Interleaver_core_random_column(const int size, const int n_cols, const int seed = 0, const bool uniform = false,
	                               const int n_frames = 1);
	virtual ~Interleaver_core_random_column() = default;

protected:
	void gen_lut(T *lut, const int frame_id);
};
}
}

#endif	/* INTERLEAVER_CORE_RANDOM_COLUMN_HPP */
