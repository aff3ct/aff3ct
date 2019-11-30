/*!
 * \file
 * \brief Class module::Chain.
 */
#ifndef MODULE_CHAIN_HPP_
#define MODULE_CHAIN_HPP_

#include <memory>

#include "Tools/Chain/Chain.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
class Chain : public Module
{
	std::shared_ptr<tools::Chain> chain;

public:
	explicit Chain(const tools::Chain &chain);

	virtual ~Chain() = default;

	virtual Chain* clone() const;

protected:
	virtual void deep_copy(const Chain& m);
};
}
}

#endif /* MODULE_CHAIN_HPP_ */
