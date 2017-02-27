#include "Module/Interleaver/LTE/Interleaver_LTE.hpp"
#include "Module/Interleaver/CCSDS/Interleaver_CCSDS.hpp"
#include "Module/Interleaver/NO/Interleaver_NO.hpp"
#include "Module/Interleaver/Columns/Interleaver_columns.hpp"
#include "Module/Interleaver/Golden/Interleaver_golden.hpp"
#include "Module/Interleaver/Random/Interleaver_random.hpp"
#include "Module/Interleaver/Uniform/Interleaver_uniform.hpp"
#include "Module/Interleaver/User/Interleaver_user.hpp"

#include "Factory_interleaver.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename T>
Interleaver<T>* Factory_interleaver<T>
::build(const parameters &params, const int &size, const int seed)
{
	Interleaver<T> *interleaver = nullptr;

	// build the interleaver
	if (params.interleaver.type == "LTE")
		interleaver = new Interleaver_LTE<T>(size);
	else if (params.interleaver.type == "CCSDS")
		interleaver = new Interleaver_CCSDS<T>(size);
	else if (params.interleaver.type == "RANDOM")
		interleaver = new Interleaver_random<T>(size, seed);
	else if (params.interleaver.type == "UNIFORM")
		interleaver = new Interleaver_uniform<T>(size, seed);
	else if (params.interleaver.type == "COLUMNS")
		interleaver = new Interleaver_columns<T>(params.code.K, params.code.M);
	else if (params.interleaver.type == "GOLDEN")
		interleaver = new Interleaver_golden<T>(size);
	else if (params.interleaver.type == "USER")
		interleaver = new Interleaver_user<T>(size, params.interleaver.path);
	else if (params.interleaver.type == "NO")
		interleaver = new Interleaver_NO<T>(size);

	return interleaver;
}

// ==================================================================================== explicit template instantiation 
template struct Factory_interleaver<short>;
template struct Factory_interleaver<int>;
template struct Factory_interleaver<long long>;
// ==================================================================================== explicit template instantiation
