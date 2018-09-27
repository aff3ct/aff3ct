#ifndef POST_PROCESSING_SISO_HPP
#define POST_PROCESSING_SISO_HPP

#include <mipp.h>

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Post_processing_SISO
{
public:
	virtual ~Post_processing_SISO() = default;

	virtual bool siso_n(const int ite,
	                    const mipp::vector<R>& sys,
	                          mipp::vector<R>& ext,
	                          mipp::vector<B>& s)
	{
		return false;
	}

	virtual bool siso_i(const int ite,
	                    const mipp::vector<R>& sys,
	                          mipp::vector<R>& ext)
	{
		return false;
	}

	virtual void end(const int n_ite)
	{
	}
};
}
}

#endif /* POST_PROCESSING_SISO_HPP */
