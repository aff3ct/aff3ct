#include <cstdlib>
#include <iostream>

#include "mipp_scalar_op.h"

template <typename T> T      mipp_scop::andb(const T      val1, const T      val2) { return                  val1  &           val2;   }
template <          > float  mipp_scop::andb(const float  val1, const float  val2) { return        float(int(val1) &       int(val2)); }
template <          > double mipp_scop::andb(const double val1, const double val2) { return double(long long(val1) & long long(val2)); }

template <typename T> T      mipp_scop::xorb(const T      val1, const T      val2) { return                  val1  ^           val2;   }
template <          > float  mipp_scop::xorb(const float  val1, const float  val2) { return        float(int(val1) ^       int(val2)); }
template <          > double mipp_scop::xorb(const double val1, const double val2) { return double(long long(val1) ^ long long(val2)); }

template <typename T> T           mipp_scop::div2(const T           val) { return val * (T)0.5; }
template <          > int         mipp_scop::div2(const int         val) { return val >> 1;     }
template <          > short       mipp_scop::div2(const short       val) { return val >> 1;     }
template <          > signed char mipp_scop::div2(const signed char val) { return val >> 1;     }

template <typename T> T           mipp_scop::div4(const T           val) { return val * (T)0.25; }
template <          > int         mipp_scop::div4(const int         val) { return val >> 2;      }
template <          > short       mipp_scop::div4(const short       val) { return val >> 2;      }
template <          > signed char mipp_scop::div4(const signed char val) { return val >> 2;      }

template <typename T> T           mipp_scop::lshift(const T           val, const int n) { return                              val  << n;  }
template <          > double      mipp_scop::lshift(const double      val, const int n) { return    double(unsigned long long(val) << n); }
template <          > float       mipp_scop::lshift(const float       val, const int n) { return               float(unsigned(val) << n); }
template <          > long long   mipp_scop::lshift(const long long   val, const int n) { return long long(unsigned long long(val) << n); }
template <          > int         mipp_scop::lshift(const int         val, const int n) { return                 int(unsigned(val) << n); }
template <          > short       mipp_scop::lshift(const short       val, const int n) { return         short(unsigned short(val) << n); }
template <          > signed char mipp_scop::lshift(const signed char val, const int n) { return    signed char(unsigned char(val) << n); }

template <typename T> T           mipp_scop::rshift(const T           val, const int n) { return                              val  >> n;  }
template <          > double      mipp_scop::rshift(const double      val, const int n) { return    double(unsigned long long(val) >> n); }
template <          > float       mipp_scop::rshift(const float       val, const int n) { return               float(unsigned(val) >> n); }
template <          > long long   mipp_scop::rshift(const long long   val, const int n) { return long long(unsigned long long(val) >> n); }
template <          > int         mipp_scop::rshift(const int         val, const int n) { return                 int(unsigned(val) >> n); }
template <          > short       mipp_scop::rshift(const short       val, const int n) { return         short(unsigned short(val) >> n); }
template <          > signed char mipp_scop::rshift(const signed char val, const int n) { return    signed char(unsigned char(val) >> n); }

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