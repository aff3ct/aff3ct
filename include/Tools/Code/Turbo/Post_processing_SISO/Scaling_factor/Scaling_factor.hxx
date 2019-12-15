#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, typename R>
Scaling_factor<B,R>::Scaling_factor(const int n_ite)
: n_ite(n_ite)
{
}

template <typename B, typename R>
Scaling_factor<B,R>* Scaling_factor<B,R>
::clone() const
{
	throw unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
