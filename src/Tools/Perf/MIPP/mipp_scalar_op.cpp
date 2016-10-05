#include <cstdlib>
#include <iostream>

#include "mipp_scalar_op.h"

namespace mipp_scop
{
template <typename T> T      andb(const T      val1, const T      val2) { return                                            val1  &                        val2;   }
template <          > float  andb(const float  val1, const float  val2) { return static_cast<float >(static_cast<int      >(val1) & static_cast<int      >(val2)); }
template <          > double andb(const double val1, const double val2) { return static_cast<double>(static_cast<long long>(val1) & static_cast<long long>(val2)); }

template <typename T> T      xorb(const T      val1, const T      val2) { return                                            val1  ^                        val2;   }
template <          > float  xorb(const float  val1, const float  val2) { return static_cast<float >(static_cast<int      >(val1) ^ static_cast<int      >(val2)); }
template <          > double xorb(const double val1, const double val2) { return static_cast<double>(static_cast<long long>(val1) ^ static_cast<long long>(val2)); }

template <typename T> T           div2(const T           val) { return val * (T)0.5; }
template <          > int         div2(const int         val) { return val >> 1;     }
template <          > short       div2(const short       val) { return val >> 1;     }
template <          > signed char div2(const signed char val) { return val >> 1;     }

template <typename T> T           div4(const T           val) { return val * (T)0.25; }
template <          > int         div4(const int         val) { return val >> 2;      }
template <          > short       div4(const short       val) { return val >> 2;      }
template <          > signed char div4(const signed char val) { return val >> 2;      }

template <typename T> T           lshift(const T           val, const int n) { return                                                          val  << n;  }
template <          > double      lshift(const double      val, const int n) { return static_cast<double     >(static_cast<unsigned long long>(val) << n); }
template <          > float       lshift(const float       val, const int n) { return static_cast<float      >(static_cast<unsigned          >(val) << n); }
template <          > long long   lshift(const long long   val, const int n) { return static_cast<long long  >(static_cast<unsigned long long>(val) << n); }
template <          > int         lshift(const int         val, const int n) { return static_cast<int        >(static_cast<unsigned          >(val) << n); }
template <          > short       lshift(const short       val, const int n) { return static_cast<short      >(static_cast<unsigned short    >(val) << n); }
template <          > signed char lshift(const signed char val, const int n) { return static_cast<signed char>(static_cast<unsigned char     >(val) << n); }

template <typename T> T           rshift(const T           val, const int n) { return                                                          val  >> n;  }
template <          > double      rshift(const double      val, const int n) { return static_cast<double     >(static_cast<unsigned long long>(val) >> n); }
template <          > float       rshift(const float       val, const int n) { return static_cast<float      >(static_cast<unsigned          >(val) >> n); }
template <          > long long   rshift(const long long   val, const int n) { return static_cast<long long  >(static_cast<unsigned long long>(val) >> n); }
template <          > int         rshift(const int         val, const int n) { return static_cast<int        >(static_cast<unsigned          >(val) >> n); }
template <          > short       rshift(const short       val, const int n) { return static_cast<short      >(static_cast<unsigned short    >(val) >> n); }
template <          > signed char rshift(const signed char val, const int n) { return static_cast<signed char>(static_cast<unsigned char     >(val) >> n); }
}

// ==================================================================================== explicit template instantiation
template double      mipp_scop::andb(const double     , const double     );
template float       mipp_scop::andb(const float      , const float      );
template long long   mipp_scop::andb(const long long  , const long long  );
template int         mipp_scop::andb(const int        , const int        );
template short       mipp_scop::andb(const short      , const short      );
template signed char mipp_scop::andb(const signed char, const signed char);

template double      mipp_scop::xorb(const double     , const double     );
template float       mipp_scop::xorb(const float      , const float      );
template long long   mipp_scop::xorb(const long long  , const long long  );
template int         mipp_scop::xorb(const int        , const int        );
template short       mipp_scop::xorb(const short      , const short      );
template signed char mipp_scop::xorb(const signed char, const signed char);

template double      mipp_scop::div2(const double     );
template float       mipp_scop::div2(const float      );
template long long   mipp_scop::div2(const long long  );
template int         mipp_scop::div2(const int        );
template short       mipp_scop::div2(const short      );
template signed char mipp_scop::div2(const signed char);

template double      mipp_scop::div4(const double     );
template float       mipp_scop::div4(const float      );
template long long   mipp_scop::div4(const long long  );
template int         mipp_scop::div4(const int        );
template short       mipp_scop::div4(const short      );
template signed char mipp_scop::div4(const signed char);

template double      mipp_scop::lshift(const double     , const int);
template float       mipp_scop::lshift(const float      , const int);
template long long   mipp_scop::lshift(const long long  , const int);
template int         mipp_scop::lshift(const int        , const int);
template short       mipp_scop::lshift(const short      , const int);
template signed char mipp_scop::lshift(const signed char, const int);

template double      mipp_scop::rshift(const double     , const int);
template float       mipp_scop::rshift(const float      , const int);
template long long   mipp_scop::rshift(const long long  , const int);
template int         mipp_scop::rshift(const int        , const int);
template short       mipp_scop::rshift(const short      , const int);
template signed char mipp_scop::rshift(const signed char, const int);
// ==================================================================================== explicit template instantiation
