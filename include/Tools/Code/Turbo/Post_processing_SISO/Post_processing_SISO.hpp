/*!
 * \file
 * \brief Class tools::Post_processing_SISO.
 */
#ifndef POST_PROCESSING_SISO_HPP
#define POST_PROCESSING_SISO_HPP

#include <mipp.h>

#include "Tools/Interface/Interface_clone.hpp"
#include "Tools/Interface/Interface_get_set_n_frames.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Post_processing_SISO : public Interface_clone, public Interface_get_set_n_frames
{
protected:
	size_t n_frames;

public:
	virtual ~Post_processing_SISO() = default;

	virtual Post_processing_SISO<B,R>* clone() const;

	virtual bool siso_n(const int ite,
	                    const mipp::vector<R>& sys,
	                          mipp::vector<R>& ext,
	                          mipp::vector<B>& s);

	virtual bool siso_i(const int ite,
	                    const mipp::vector<R>& sys,
	                          mipp::vector<R>& ext);

	virtual void end(const int n_ite);

	inline size_t get_n_frames() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	Post_processing_SISO();

	void deep_copy(const Post_processing_SISO<B,R>& t);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hxx"
#endif

#endif /* POST_PROCESSING_SISO_HPP */
