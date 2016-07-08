#include "mipp.h"

template <typename T>
class Reg;

template <typename T> inline Reg<T> add(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> sub(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> mul(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> div(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> min(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> max(const Reg<T> v1, const Reg<T> v2);

template <typename T>
class Regx2;

template <typename T>
class Reg
{
public:
#ifndef MIPP_NO_INTRINSICS
	reg r;
#else
	T r;
#endif

#ifndef MIPP_NO_INTRINSICS
	Reg(              )                          {}
	Reg(const reg r   ) : r(r)                   {}
	Reg(const T   val ) : r(mipp::set1 <T>(val)) {}

#ifdef MIPP_ALIGNED_LOADS
	Reg(const T  *data) : r(mipp::load <T>(data)){}
#else
	Reg(const T  *data) : r(mipp::loadu<T>(data)){}
#endif

	Reg(const std::initializer_list<T> &l)
	{
		if (l.size() >= (unsigned)mipp::nElReg<T>())
			r = mipp::loadu<T>(l.begin());
		else
		{
			auto vec = mipp::vector<T>(mipp::nElReg<T>());
			vec = l;
			r = mipp::load<T>(vec.data());
		}
	}

	~Reg() {}
#else
	Reg(             )              {}
	Reg(const T  val ) : r(val)     {}
	Reg(const T *data) : r(data[0]) {}

	Reg(const std::initializer_list<T> &l)
	{
		auto vec = mipp::vector<T>(mipp::nElReg<T>());
		vec = l;
		r = vec[0];
	}

	~Reg() {}
#endif

#ifndef MIPP_NO_INTRINSICS
	static inline Reg<T> cmask (const int mask[nElReg<T>()  ]) { return mipp::cmask <T>(mask); }
	static inline Reg<T> cmask2(const int mask[nElReg<T>()/2]) { return mipp::cmask2<T>(mask); }
#else
	static inline Reg<T> cmask (const int mask[nElReg<T>()  ]) { return Reg<T>((T)0);          }
	static inline Reg<T> cmask2(const int mask[nElReg<T>()/2]) { return Reg<T>((T)0);          }
#endif

	static inline void transpose(Reg<T> regs[nElReg<T>()])
	{
#ifndef MIPP_NO_INTRINSICS
		reg rs[nElReg<T>()];
		for (auto i = 0; i < nElReg<T>(); i++) rs[i] = regs[i].r;
		mipp::transpose<T>(rs);
		for (auto i = 0; i < nElReg<T>(); i++) regs[i].r = rs[i];
#endif
	}

	static inline void transpose8x8(Reg<T> regs[8])
	{
#ifndef MIPP_NO_INTRINSICS
		reg rs[8];
		for (auto i = 0; i < 8; i++) rs[i] = regs[i].r;
		mipp::transpose8x8<T>(rs);
		for (auto i = 0; i < 8; i++) regs[i].r = rs[i];
#else
		std::cerr << "\"transpose8x8\" static method is a non-sense in sequential mode, exiting." << std::endl;
		exit(EXIT_FAILURE);
#endif
	}

	static inline void transpose2(Reg<T> regs[nElReg<T>()/2])
	{
#ifndef MIPP_NO_INTRINSICS
		reg rs[nElReg<T>()/2];
		for (auto i = 0; i < nElReg<T>()/2; i++) rs[i] = regs[i].r;
		mipp::transpose2<T>(rs);
		for (auto i = 0; i < nElReg<T>()/2; i++) regs[i].r = rs[i];
#else
		std::cerr << "\"transpose2\" static method is a non-sense in sequential mode, exiting." << std::endl;
		exit(EXIT_FAILURE);
#endif
	}

	static inline void transpose28x8(Reg<T> regs[8])
	{
#ifndef MIPP_NO_INTRINSICS
		reg rs[8];
		for (auto i = 0; i < 8; i++) rs[i] = regs[i].r;
		mipp::transpose28x8<T>(rs);
		for (auto i = 0; i < 8; i++) regs[i].r = rs[i];
#else
		std::cerr << "\"transpose28x8\" static method is a non-sense in sequential mode, exiting." << std::endl;
		exit(EXIT_FAILURE);
#endif
	}

#ifndef MIPP_NO_INTRINSICS
	inline void     set0         (               )                        { r = mipp::set0<T>();                          }
	inline void     set1         (const T val    )                        { r = mipp::set1<T>(val);                       }
	inline void     load         (const T* data  )                        { r = mipp::load<T>(data);                      }
	inline void     loadu        (const T* data  )                        { r = mipp::loadu<T>(data);                     }
	inline void     store        (T* data        )                  const { mipp::store<T>(data, r);                      }
	inline void     storeu       (T* data        )                  const { mipp::storeu<T>(data, r);                     }
#else
	inline void     set0         (               )                        { r = 0;                                        }
	inline void     set1         (const T val    )                        { r = val;                                      }
	inline void     load         (const T* data  )                        { r = data[0];                                  }
	inline void     loadu        (const T* data  )                        { r = data[0];                                  }
	inline void     store        (T* data        )                  const { data[0] = r;                                  }
	inline void     storeu       (T* data        )                  const { data[0] = r;                                  }
#endif

#ifndef MIPP_NO_INTRINSICS
	inline Reg<T>   shuff        (const Reg<T> v_shu)               const { return mipp::shuff        <T>(r, v_shu.r);    }
	inline Reg<T>   shuff2       (const Reg<T> v_shu)               const { return mipp::shuff2       <T>(r, v_shu.r);    }
	inline Reg<T>   interleavelo (const Reg<T> v)                   const { return mipp::interleavelo <T>(r, v.r);        }
	inline Reg<T>   interleavehi (const Reg<T> v)                   const { return mipp::interleavehi <T>(r, v.r);        }
	inline Reg<T>   interleavelo2(const Reg<T> v)                   const { return mipp::interleavelo2<T>(r, v.r);        }
	inline Reg<T>   interleavehi2(const Reg<T> v)                   const { return mipp::interleavehi2<T>(r, v.r);        }
	inline Regx2<T> interleave   (const Reg<T> v)                   const { return mipp::interleave   <T>(r, v.r);        }
	inline Regx2<T> interleave2  (const Reg<T> v)                   const { return mipp::interleave2  <T>(r, v.r);        }
	inline Reg<T>   interleave   ()                                 const { return mipp::interleave   <T>(r);             }
	inline Regx2<T> interleavex2 (const Reg<T> v)                   const { return mipp::interleavex2 <T>(r, v.r);        }
	inline Reg<T>   interleavex4 ()                                 const { return mipp::interleavex4 <T>(r);             }
	inline Reg<T>   interleavex16()                                 const { return mipp::interleavex16<T>(r);             }
	inline Reg<T>   andb         (const Reg<T> v)                   const { return mipp::andb         <T>(r, v.r);        }
	inline Reg<T>   andnb        (const Reg<T> v)                   const { return mipp::andnb        <T>(r, v.r);        }
	inline Reg<T>   notb         ()                                 const { return mipp::notb         <T>(r);             }
	inline Reg<T>   orb          (const Reg<T> v)                   const { return mipp::orb          <T>(r, v.r);        }
	inline Reg<T>   xorb         (const Reg<T> v)                   const { return mipp::xorb         <T>(r, v.r);        }
	inline Reg<T>   lshift       (const int n)                      const { return mipp::lshift       <T>(r, n);          }
	inline Reg<T>   rshift       (const int n)                      const { return mipp::rshift       <T>(r, n);          }
	inline Reg<T>   cmpeq        (const Reg<T> v)                   const { return mipp::cmpeq        <T>(r, v.r);        }
	inline Reg<T>   cmpneq       (const Reg<T> v)                   const { return mipp::cmpneq       <T>(r, v.r);        }
	inline Reg<T>   cmplt        (const Reg<T> v)                   const { return mipp::cmplt        <T>(r, v.r);        }
	inline Reg<T>   cmple        (const Reg<T> v)                   const { return mipp::cmple        <T>(r, v.r);        }
	inline Reg<T>   cmpgt        (const Reg<T> v)                   const { return mipp::cmpgt        <T>(r, v.r);        }
	inline Reg<T>   cmpge        (const Reg<T> v)                   const { return mipp::cmpge        <T>(r, v.r);        }
	inline Reg<T>   add          (const Reg<T> v)                   const { return mipp::add          <T>(r, v.r);        }
	inline Reg<T>   sub          (const Reg<T> v)                   const { return mipp::sub          <T>(r, v.r);        }
	inline Reg<T>   mul          (const Reg<T> v)                   const { return mipp::mul          <T>(r, v.r);        }
	inline Reg<T>   div          (const Reg<T> v)                   const { return mipp::div          <T>(r, v.r);        }
	inline Reg<T>   min          (const Reg<T> v)                   const { return mipp::min          <T>(r, v.r);        }
	inline Reg<T>   max          (const Reg<T> v)                   const { return mipp::max          <T>(r, v.r);        }
	inline Reg<T>   sign         ()                                 const { return mipp::sign         <T>(r);             }
	inline Reg<T>   sign         (const Reg<T> v)                   const { return mipp::sign         <T>(r, v.r);        }
	inline Reg<T>   neg          (const Reg<T> v)                   const { return mipp::neg          <T>(r, v.r);        }
	inline Reg<T>   abs          ()                                 const { return mipp::abs          <T>(r);             }
	inline Reg<T>   sqrt         ()                                 const { return mipp::sqrt         <T>(r);             }
	inline Reg<T>   rsqrt        ()                                 const { return mipp::rsqrt        <T>(r);             }
	inline Reg<T>   log          ()                                 const { return mipp::log          <T>(r);             }
	inline Reg<T>   exp          ()                                 const { return mipp::exp          <T>(r);             }
	inline Reg<T>   sin          ()                                 const { return mipp::sin          <T>(r);             }
	inline Reg<T>   cos          ()                                 const { return mipp::cos          <T>(r);             }
	inline void     sincos       (      Reg<T> &s,       Reg<T> &c) const { return mipp::sincos       <T>(r,  s.r,  c.r); }
	inline Reg<T>   fmadd        (const Reg<T> v1, const Reg<T> v2) const { return mipp::fmadd        <T>(r, v1.r, v2.r); }
	inline Reg<T>   fnmadd       (const Reg<T> v1, const Reg<T> v2) const { return mipp::fnmadd       <T>(r, v1.r, v2.r); }
	inline Reg<T>   fmsub        (const Reg<T> v1, const Reg<T> v2) const { return mipp::fmsub        <T>(r, v1.r, v2.r); }
	inline Reg<T>   rot          ()                                 const { return mipp::rot          <T>(r);             }
	inline Reg<T>   rotr         ()                                 const { return mipp::rotr         <T>(r);             }
	inline Reg<T>   div2         ()                                 const { return mipp::div2         <T>(r);             }
	inline Reg<T>   div4         ()                                 const { return mipp::div4         <T>(r);             }
	inline Reg<T>   sat          (T min, T max)                     const { return mipp::sat          <T>(r, min, max);   }
	inline Reg<T>   round        ()                                 const { return mipp::round        <T>(r);             }
#else
	inline Reg<T>   shuff        (const Reg<T> v_shu)               const { return *this;                                 }
	inline Reg<T>   shuff2       (const Reg<T> v_shu)               const { return *this;                                 }
	inline Reg<T>   interleavelo (const Reg<T> v)                   const { return *this;                                 }
	inline Reg<T>   interleavehi (const Reg<T> v)                   const { return *this;                                 }
	inline Reg<T>   interleavelo2(const Reg<T> v)                   const { return *this;                                 }
	inline Reg<T>   interleavehi2(const Reg<T> v)                   const { return *this;                                 }
	inline Regx2<T> interleave   (const Reg<T> v)                   const { return Regx2<T>(*this, v);                    }
	inline Regx2<T> interleave2  (const Reg<T> v)                   const { return Regx2<T>(*this, v);                    }
	inline Reg<T>   interleave   ()                                 const { return *this;                                 }
	inline Regx2<T> interleavex2 (const Reg<T> v)                   const { return Regx2<T>(*this, v);                    }
	inline Reg<T>   interleavex4 ()                                 const { return *this;                                 }
	inline Reg<T>   interleavex16()                                 const { return *this;                                 }
	inline Reg<T>   andb         (const Reg<T> v)                   const { return   r  &  v.r;                           }
	inline Reg<T>   andnb        (const Reg<T> v)                   const { return andb(~r);                              }
	inline Reg<T>   notb         ()                                 const { return  ~r;                                   }
	inline Reg<T>   orb          (const Reg<T> v)                   const { return   r  |  v.r;                           }
	inline Reg<T>   xorb         (const Reg<T> v)                   const { return   r  ^  v.r;                           }
	inline Reg<T>   lshift       (const int n)                      const { return   r  << n;                             }
	inline Reg<T>   rshift       (const int n)                      const { return   r  >> n ;                            }
	inline Reg<T>   cmpeq        (const Reg<T> v)                   const { return   r  == v.r;                           }
	inline Reg<T>   cmpneq       (const Reg<T> v)                   const { return   r  != v.r;                           }
	inline Reg<T>   cmplt        (const Reg<T> v)                   const { return   r  <  v.r;                           }
	inline Reg<T>   cmple        (const Reg<T> v)                   const { return   r  <= v.r;                           }
	inline Reg<T>   cmpgt        (const Reg<T> v)                   const { return   r  >  v.r;                           }
	inline Reg<T>   cmpge        (const Reg<T> v)                   const { return   r  >= v.r;                           }
	inline Reg<T>   add          (const Reg<T> v)                   const { return   r  +  v.r;                           }
	inline Reg<T>   sub          (const Reg<T> v)                   const { return   r  -  v.r;                           }
	inline Reg<T>   mul          (const Reg<T> v)                   const { return   r  *  v.r;                           }
	inline Reg<T>   div          (const Reg<T> v)                   const { return   r  /  v.r;                           }
	inline Reg<T>   min          (const Reg<T> v)                   const { return std::min<T>(r, v.r);                   }
	inline Reg<T>   max          (const Reg<T> v)                   const { return std::max<T>(r, v.r);                   }
	// (1 = positive, -1 = negative, 0 = 0)
	inline Reg<T>   sign         ()                                 const { return (T(0) < r) - (r < T(0));               }
	inline Reg<T>   sign         (const Reg<T> v)                   const { return sign(Reg<T>(r ^ v.r));                 }
	inline Reg<T>   neg          (const Reg<T> v)                   const { return v.r >= 0 ? Reg<T>(r) : Reg<T>(-r);     }
	inline Reg<T>   abs          ()                                 const { return std::abs<T>(r);                        }
	inline Reg<T>   sqrt         ()                                 const { return std::sqrt<T>(r);                       }
	inline Reg<T>   rsqrt        ()                                 const { return 1 / std::sqrt<T>(r);                   }
	inline Reg<T>   log          ()                                 const { return std::log<T>(r);                        }
	inline Reg<T>   exp          ()                                 const { return std::exp<T>(r);                        }
	inline Reg<T>   sin          ()                                 const { return std::sin<T>(r);                        }
	inline Reg<T>   cos          ()                                 const { return std::cos<T>(r);                        }
	inline void     sincos       (      Reg<T> &s,       Reg<T> &c) const { s = std::sin<T>(r); c = std::cos<T>(r);       }
	inline Reg<T>   fmadd        (const Reg<T> v1, const Reg<T> v2) const { return   r * v1.r + v2.r;                     }
	inline Reg<T>   fnmadd       (const Reg<T> v1, const Reg<T> v2) const { return -(r * v1.r + v2.r);                    }
	inline Reg<T>   fmsub        (const Reg<T> v1, const Reg<T> v2) const { return   r * v1.r - v2.r;                     }
	inline Reg<T>   rot          ()                                 const { return r;                                     }
	inline Reg<T>   rotr         ()                                 const { return r;                                     }
	inline Reg<T>   div2         ()                                 const { return r * (T)0.50;                           }
	inline Reg<T>   div4         ()                                 const { return r * (T)0.25;                           }
	inline Reg<T>   sat          (T min, T max)                     const { return std::min(std::max(r, min), max);       }
	inline Reg<T>   round        ()                                 const { return std::round(r);                         }
#endif

#ifndef MIPP_NO_INTRINSICS
	template <typename T2> inline Reg<T2> cvt ()               const { return mipp::cvt<T,T2>(r);       }
	template <typename T2> inline Reg<T2> pack(const Reg<T> v) const { return mipp::pack<T,T2>(r, v.r); }
#else
	template <typename T2> inline Reg<T2> cvt ()               const { return (T2)r; }
	template <typename T2> inline Reg<T2> pack(const Reg<T> v) const
	{
		std::cerr << "\"pack\" method is a non-sense in sequential mode, exiting." << std::endl;
		exit(EXIT_FAILURE);
	}
#endif

	inline Reg<T>  operator~  (               )       { return this->notb();                    }

	inline Reg<T>& operator+= (const Reg<T> &v)       { r =    this->add(v).r;    return *this; }
	inline Reg<T>  operator+  (const Reg<T>  v) const { return this->add(v);                    }

	inline Reg<T>& operator-= (const Reg<T> &v)       { r =    this->sub(v).r;    return *this; }
	inline Reg<T>  operator-  (const Reg<T>  v) const { return this->sub(v);                    }

	inline Reg<T>& operator*= (const Reg<T> &v)       { r =    this->mul(v).r;    return *this; }
	inline Reg<T>  operator*  (const Reg<T>  v) const { return this->mul(v);                    }

	inline Reg<T>& operator/= (const Reg<T> &v)       { r =    this->div(v).r;    return *this; }
	inline Reg<T>  operator/  (const Reg<T>  v) const { return this->div(v);                    }

	inline Reg<T>& operator^= (const Reg<T> &v)       { r =    this->xorb(v).r;   return *this; }
	inline Reg<T>  operator^  (const Reg<T>  v) const { return this->xorb(v);                   }

	inline Reg<T>& operator|= (const Reg<T> &v)       { r =    this->orb(v).r;    return *this; }
	inline Reg<T>  operator|  (const Reg<T>  v) const { return this->orb(v);                    }

	inline Reg<T>& operator&= (const Reg<T> &v)       { r =    this->andb(v).r;   return *this; }
	inline Reg<T>  operator&  (const Reg<T>  v) const { return this->andb(v);                   }

	inline Reg<T>& operator<<=(const int n    )       { r =    this->lshift(n).r; return *this; }
	inline Reg<T>  operator<< (const int n    ) const { return this->lshift(n);                 }

	inline Reg<T>& operator>>=(const int n    )       { r =    this->rshift(n).r; return *this; }
	inline Reg<T>  operator>> (const int n    ) const { return this->rshift(n);                 }

	inline Reg<T>  operator== (      Reg<T>  v) const { return this->cmpeq (v);                 }
	inline Reg<T>  operator!= (      Reg<T>  v) const { return this->cmpneq(v);                 }
	inline Reg<T>  operator<  (      Reg<T>  v) const { return this->cmplt (v);                 }
	inline Reg<T>  operator<= (      Reg<T>  v) const { return this->cmple (v);                 }
	inline Reg<T>  operator>  (      Reg<T>  v) const { return this->cmpgt (v);                 }
	inline Reg<T>  operator>= (      Reg<T>  v) const { return this->cmpge (v);                 }


	// ------------------------------------------------------------------------------------------------------ reduction
#ifndef MIPP_NO_INTRINSICS
	inline Reg<T> sum () const { return Reduction<T,mipp::add>::apply(*this); }
	inline Reg<T> hadd() const { return Reduction<T,mipp::add>::apply(*this); }
	inline Reg<T> hsub() const { return Reduction<T,mipp::sub>::apply(*this); }
	inline Reg<T> hmul() const { return Reduction<T,mipp::mul>::apply(*this); }
	inline Reg<T> hdiv() const { return Reduction<T,mipp::div>::apply(*this); }
	inline Reg<T> hmin() const { return Reduction<T,mipp::min>::apply(*this); }
	inline Reg<T> hmax() const { return Reduction<T,mipp::max>::apply(*this); }
#else
	inline Reg<T> sum () const { return *this; }
	inline Reg<T> hadd() const { return *this; }
	inline Reg<T> hsub() const { return *this; }
	inline Reg<T> hmul() const { return *this; }
	inline Reg<T> hdiv() const { return *this; }
	inline Reg<T> hmin() const { return *this; }
	inline Reg<T> hmax() const { return *this; }
#endif
};

template <typename T>
class Regx2 
{
public:
	Reg<T> val[2];

	Regx2(                    )                                             {}
	Regx2(Reg<T> r1, Reg<T> r2) : val({r1, r2})                             {}
#ifndef MIPP_NO_INTRINSICS
	Regx2(regx2 r2            ) : val{Reg<T>(r2.val[0]), Reg<T>(r2.val[1])} {}
#endif

	~Regx2() {}
};

#ifndef MIPP_NO_INTRINSICS
template <typename T>
std::ostream& operator<<(std::ostream& os, const Reg<T>& r)
{
	dump<T>(r.r, os); return os;
}
#else
template <> Reg<int>         Reg<int        >::div2() const { return r >> 1; }
template <> Reg<short>       Reg<short      >::div2() const { return r >> 1; }
template <> Reg<signed char> Reg<signed char>::div2() const { return r >> 1; }

template <> Reg<int>         Reg<int        >::div4() const { return r >> 2; }
template <> Reg<short>       Reg<short      >::div4() const { return r >> 2; }
template <> Reg<signed char> Reg<signed char>::div4() const { return r >> 2; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const Reg<T>& r)
{
	os << +r.r;
	return os;
}
#endif

template <typename T> inline Reg<T>   shuff        (const Reg<T> v1, const Reg<T> v2)                  { return v1.shuff(v2);         }
template <typename T> inline Reg<T>   shuff2       (const Reg<T> v1, const Reg<T> v2)                  { return v1.shuff2(v2);        }
template <typename T> inline Reg<T>   interleavelo (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavelo(v2);  }
template <typename T> inline Reg<T>   interleavehi (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavehi(v2);  }
template <typename T> inline Reg<T>   interleavelo2(const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavelo2(v2); }
template <typename T> inline Reg<T>   interleavehi2(const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavehi2(v2); }
template <typename T> inline Regx2<T> interleave   (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleave(v2);    }
template <typename T> inline Regx2<T> interleave2  (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleave2(v2);   }
template <typename T> inline Reg<T>   interleave   (const Reg<T> v)                                    { return v.interleave();       }
template <typename T> inline Regx2<T> interleavex2 (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavex2(v2);  }
template <typename T> inline Reg<T>   interleavex4 (const Reg<T> v)                                    { return v.interleavex4();     }
template <typename T> inline Reg<T>   interleavex16(const Reg<T> v)                                    { return v.interleavex16();    }
template <typename T> inline Reg<T>   andb         (const Reg<T> v1, const Reg<T>v2)                   { return v1.andb(v2);          }
template <typename T> inline Reg<T>   andnb        (const Reg<T> v1, const Reg<T>v2)                   { return v1.andnb(v2);         }
template <typename T> inline Reg<T>   notb         (const Reg<T> v)                                    { return v.notb();             }
template <typename T> inline Reg<T>   orb          (const Reg<T> v1, const Reg<T> v2)                  { return v1.orb(v2);           }
template <typename T> inline Reg<T>   xorb         (const Reg<T> v1, const Reg<T> v2)                  { return v1.xorb(v2);          }
template <typename T> inline Reg<T>   lshift       (const Reg<T> v,  const int n)                      { return v.lshift(n);          }
template <typename T> inline Reg<T>   rshift       (const Reg<T> v,  const int n)                      { return v.rshift(n);          }
template <typename T> inline Reg<T>   cmpeq        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmpeq(v2);         }
template <typename T> inline Reg<T>   cmpneq       (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmpneq(v2);        }
template <typename T> inline Reg<T>   cmplt        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmplt(v2);         }
template <typename T> inline Reg<T>   cmple        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmple(v2);         }
template <typename T> inline Reg<T>   cmpgt        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmpgt(v2);         }
template <typename T> inline Reg<T>   cmpge        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmpge(v2);         }
template <typename T> inline Reg<T>   add          (const Reg<T> v1, const Reg<T> v2)                  { return v1.add(v2);           }
template <typename T> inline Reg<T>   sub          (const Reg<T> v1, const Reg<T> v2)                  { return v1.sub(v2);           }
template <typename T> inline Reg<T>   mul          (const Reg<T> v1, const Reg<T> v2)                  { return v1.mul(v2);           }
template <typename T> inline Reg<T>   div          (const Reg<T> v1, const Reg<T> v2)                  { return v1.div(v2);           }
template <typename T> inline Reg<T>   min          (const Reg<T> v1, const Reg<T> v2)                  { return v1.min(v2);           }
template <typename T> inline Reg<T>   max          (const Reg<T> v1, const Reg<T> v2)                  { return v1.max(v2);           }
template <typename T> inline Reg<T>   sign         (const Reg<T> v)                                    { return v.sign();             }
template <typename T> inline Reg<T>   sign         (const Reg<T> v1, const Reg<T> v2)                  { return v1.sign(v2);          }
template <typename T> inline Reg<T>   neg          (const Reg<T> v1, const Reg<T> v2)                  { return v1.andb(v2);          }
template <typename T> inline Reg<T>   abs          (const Reg<T> v)                                    { return v.abs();              }
template <typename T> inline Reg<T>   sqrt         (const Reg<T> v)                                    { return v.sqrt();             }
template <typename T> inline Reg<T>   rsqrt        (const Reg<T> v)                                    { return v.rsqrt();            }
template <typename T> inline Reg<T>   log          (const Reg<T> v)                                    { return v.log();              }
template <typename T> inline Reg<T>   exp          (const Reg<T> v)                                    { return v.exp();              }
template <typename T> inline Reg<T>   sin          (const Reg<T> v)                                    { return v.sin();              }
template <typename T> inline Reg<T>   cos          (const Reg<T> v)                                    { return v.cos();              }
template <typename T> inline void     sincos       (const Reg<T> x,        Reg<T> &s,       Reg<T> &c) { return x.sincos(s,c);        }
template <typename T> inline Reg<T>   fmadd        (const Reg<T> v1, const Reg<T> v2, const Reg<T> v3) { return v1.fmadd(v2, v3);     }
template <typename T> inline Reg<T>   fnmadd       (const Reg<T> v1, const Reg<T> v2, const Reg<T> v3) { return v1.fnmadd(v2, v3);    }
template <typename T> inline Reg<T>   fmsub        (const Reg<T> v1, const Reg<T> v2, const Reg<T> v3) { return v1.fmsub(v2, v3);     }
template <typename T> inline Reg<T>   rot          (const Reg<T> v)                                    { return v.rot();              }
template <typename T> inline Reg<T>   rotr         (const Reg<T> v)                                    { return v.rotr();             }
template <typename T> inline Reg<T>   div2         (const Reg<T> v)                                    { return v.div2();             }
template <typename T> inline Reg<T>   div4         (const Reg<T> v)                                    { return v.div4();             }
template <typename T> inline Reg<T>   sat          (const Reg<T> v, T min, T max)                      { return v.sat(min, max);      }
template <typename T> inline Reg<T>   round        (const Reg<T> v)                                    { return v.round();            }
template <typename T> inline Reg<T>   sum          (const Reg<T> v)                                    { return v.sum();              }
template <typename T> inline Reg<T>   hadd         (const Reg<T> v)                                    { return v.hadd();             }
template <typename T> inline Reg<T>   hmin         (const Reg<T> v)                                    { return v.hmin();             }
template <typename T> inline Reg<T>   hmax         (const Reg<T> v)                                    { return v.hmax();             }

template <typename T1, typename T2> 
inline Reg<T2> cvt(const Reg<T1> v) {
	return v.template cvt<T2>();
}

template <typename T1, typename T2>
inline Reg<T2> pack(const Reg<T1> v1, const Reg<T1> v2) {
	return v1.template pack<T2>(v2);
}