#include "Tools/Exception/exception.hpp"

#include "Module/Interleaver/Random_column/Interleaver_random_column.hpp"
#include "Module/Interleaver/Row_column/Interleaver_row_column.hpp"
#include "Module/Interleaver/LTE/Interleaver_LTE.hpp"
#include "Module/Interleaver/CCSDS/Interleaver_CCSDS.hpp"
#include "Module/Interleaver/NO/Interleaver_NO.hpp"
#include "Module/Interleaver/Golden/Interleaver_golden.hpp"
#include "Module/Interleaver/Random/Interleaver_random.hpp"
#include "Module/Interleaver/User/Interleaver_user.hpp"

#include "Factory_interleaver.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename T>
Interleaver<T>* Factory_interleaver<T>
::build(const std::string type,
        const int         size,
        const std::string path,
        const bool        uniform,
        const int         n_cols,
        const int         seed,
        const int         n_frames)
{
	     if (type == "LTE"     ) return new Interleaver_LTE          <T>(size,                        n_frames);
	else if (type == "CCSDS"   ) return new Interleaver_CCSDS        <T>(size,                        n_frames);
	else if (type == "RANDOM"  ) return new Interleaver_random       <T>(size,         seed, uniform, n_frames);
	else if (type == "RAND_COL") return new Interleaver_random_column<T>(size, n_cols, seed, uniform, n_frames);
	else if (type == "ROW_COL" ) return new Interleaver_row_column   <T>(size, n_cols,                n_frames);
	else if (type == "GOLDEN"  ) return new Interleaver_golden       <T>(size,         seed, uniform, n_frames);
	else if (type == "USER"    ) return new Interleaver_user         <T>(size, path,                  n_frames);
	else if (type == "NO"      ) return new Interleaver_NO           <T>(size,                        n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation 
template struct aff3ct::tools::Factory_interleaver<int16_t>;
template struct aff3ct::tools::Factory_interleaver<int32_t>;
template struct aff3ct::tools::Factory_interleaver<int64_t>;
// ==================================================================================== explicit template instantiation
