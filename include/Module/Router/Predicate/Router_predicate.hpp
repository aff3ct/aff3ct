/*!
 * \file
 * \brief Class module::Router_predicate.
 */
#ifndef ROUTER_PREDICATE_HPP_
#define ROUTER_PREDICATE_HPP_

#include <memory>

#include "Tools/Algo/Predicate.hpp"
#include "Module/Router/Router.hpp"

namespace aff3ct
{
namespace module
{
template <typename OUT = float>
class Router_predicate : public Router<OUT,OUT>
{
	std::shared_ptr<tools::Predicate> predicate;

public:
	Router_predicate(const tools::Predicate &predicate, const size_t n_elmts_out, const int n_frames = 1);
	virtual ~Router_predicate() = default;
	virtual Router_predicate<OUT>* clone() const;

	tools::Predicate& get_predicate();

protected:
	virtual void deep_copy(const Router_predicate<OUT> &m);

	virtual size_t _route(const OUT *in, const int frame_id);

	virtual size_t select_route_inter(const size_t a, const size_t b);
};
}
}

#endif /* ROUTER_CRC_HPP_ */
