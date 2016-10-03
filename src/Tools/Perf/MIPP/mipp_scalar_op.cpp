#ifdef MIPP_NO_INTRINSICS

#include <cstdlib>
#include <iostream>

#include "mipp_scalar_op.h"

template <typename T>
T andb_s(const T val1, const T val2)
{
	return val1 & val2;
}

template <>
float andb_s(const float val1, const float val2)
{
	std::cerr << "andb_s is not available in floating point mode, exiting..." << std::endl;
	std::exit(-1);
	return 0.f;
}

template <>
double andb_s(const double val1, const double val2)
{
	std::cerr << "andb_s is not available in floating point mode, exiting..." << std::endl;
	std::exit(-1);
	return 0.f;
}

template <typename T>
T xorb_s(const T val1, const T val2)
{
	return val1 ^ val2;
}

template <>
float xorb_s(const float val1, const float val2)
{
	std::cerr << "xorb_s is not available in floating point mode, exiting..." << std::endl;
	std::exit(-1);
	return 0.f;
}

template <>
double xorb_s(const double val1, const double val2)
{
	std::cerr << "xorb_s is not available in floating point mode, exiting..." << std::endl;
	std::exit(-1);
	return 0.f;
}

template <typename T> T           div2_s(const T           val) { return val * 0.5; }
template <          > int         div2_s(const int         val) { return val >> 1;  }
template <          > short       div2_s(const short       val) { return val >> 1;  }
template <          > signed char div2_s(const signed char val) { return val >> 1;  }

template <typename T> T           div4_s(const T           val) { return val * 0.25; }
template <          > int         div4_s(const int         val) { return val >> 2;   }
template <          > short       div4_s(const short       val) { return val >> 2;   }
template <          > signed char div4_s(const signed char val) { return val >> 2;   }

// ==================================================================================== explicit template instantiation
template double      andb_s(const double     , const double     );
template float       andb_s(const float      , const float      );
template long long   andb_s(const long long  , const long long  );
template int         andb_s(const int        , const int        );
template short       andb_s(const short      , const short      );
template signed char andb_s(const signed char, const signed char);

template double      xorb_s(const double     , const double     );
template float       xorb_s(const float      , const float      );
template long long   xorb_s(const long long  , const long long  );
template int         xorb_s(const int        , const int        );
template short       xorb_s(const short      , const short      );
template signed char xorb_s(const signed char, const signed char);

template double      div2_s(const double     );
template float       div2_s(const float      );
template long long   div2_s(const long long  );
template int         div2_s(const int        );
template short       div2_s(const short      );
template signed char div2_s(const signed char);

template double      div4_s(const double     );
template float       div4_s(const float      );
template long long   div4_s(const long long  );
template int         div4_s(const int        );
template short       div4_s(const short      );
template signed char div4_s(const signed char);
// ==================================================================================== explicit template instantiation

#endif /* MIPP_NO_INTRINSICS */
