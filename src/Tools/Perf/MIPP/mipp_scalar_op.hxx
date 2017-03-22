#include <cstdlib>
#include <iostream>

#include "mipp_scalar_op.h"

namespace mipp_scop
{
template <typename T> inline T      andb(const T      val1, const T      val2) { return                                            val1  &                        val2;   }
template <          > inline float  andb(const float  val1, const float  val2) { return static_cast<float >(static_cast<int      >(val1) & static_cast<int      >(val2)); }
template <          > inline double andb(const double val1, const double val2) { return static_cast<double>(static_cast<long long>(val1) & static_cast<long long>(val2)); }

template <typename T> inline T      xorb(const T      val1, const T      val2) { return                                            val1  ^                        val2;   }
template <          > inline float  xorb(const float  val1, const float  val2) { return static_cast<float >(static_cast<int      >(val1) ^ static_cast<int      >(val2)); }
template <          > inline double xorb(const double val1, const double val2) { return static_cast<double>(static_cast<long long>(val1) ^ static_cast<long long>(val2)); }

template <typename T> inline T           div2(const T           val) { return val * (T)0.5; }
template <          > inline int         div2(const int         val) { return val >> 1;     }
template <          > inline short       div2(const short       val) { return val >> 1;     }
template <          > inline signed char div2(const signed char val) { return val >> 1;     }

template <typename T> inline T           div4(const T           val) { return val * (T)0.25; }
template <          > inline int         div4(const int         val) { return val >> 2;      }
template <          > inline short       div4(const short       val) { return val >> 2;      }
template <          > inline signed char div4(const signed char val) { return val >> 2;      }

template <typename T> inline T           lshift(const T           val, const int n) { return                                                          val  << n;  }
template <          > inline double      lshift(const double      val, const int n) { return static_cast<double     >(static_cast<unsigned long long>(val) << n); }
template <          > inline float       lshift(const float       val, const int n) { return static_cast<float      >(static_cast<unsigned          >(val) << n); }
template <          > inline long long   lshift(const long long   val, const int n) { return static_cast<long long  >(static_cast<unsigned long long>(val) << n); }
template <          > inline int         lshift(const int         val, const int n) { return static_cast<int        >(static_cast<unsigned          >(val) << n); }
template <          > inline short       lshift(const short       val, const int n) { return static_cast<short      >(static_cast<unsigned short    >(val) << n); }
template <          > inline signed char lshift(const signed char val, const int n) { return static_cast<signed char>(static_cast<unsigned char     >(val) << n); }

template <typename T> inline T           rshift(const T           val, const int n) { return                                                          val  >> n;  }
template <          > inline double      rshift(const double      val, const int n) { return static_cast<double     >(static_cast<unsigned long long>(val) >> n); }
template <          > inline float       rshift(const float       val, const int n) { return static_cast<float      >(static_cast<unsigned          >(val) >> n); }
template <          > inline long long   rshift(const long long   val, const int n) { return static_cast<long long  >(static_cast<unsigned long long>(val) >> n); }
template <          > inline int         rshift(const int         val, const int n) { return static_cast<int        >(static_cast<unsigned          >(val) >> n); }
template <          > inline short       rshift(const short       val, const int n) { return static_cast<short      >(static_cast<unsigned short    >(val) >> n); }
template <          > inline signed char rshift(const signed char val, const int n) { return static_cast<signed char>(static_cast<unsigned char     >(val) >> n); }
}
