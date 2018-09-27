#include "Length.hpp"

aff3ct::tools::Length_range* aff3ct::tools::Length(size_t min_size, size_t max_size)
{
	return new Length_range(min_size, max_size);
}