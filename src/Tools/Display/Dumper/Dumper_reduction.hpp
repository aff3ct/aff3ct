#ifndef DUMPER_REDUCTION_HPP_
#define DUMPER_REDUCTION_HPP_

#include <memory>

#include "Dumper.hpp"

namespace aff3ct
{
namespace tools
{
class Dumper_reduction : Dumper
{
protected:
	std::vector<std::unique_ptr<Dumper>>& dumpers;

public:
	explicit Dumper_reduction(std::vector<std::unique_ptr<Dumper>> &dumpers);
	virtual ~Dumper_reduction() = default;

	virtual void dump (const std::string& base_path);
	virtual void add  (const int frame_id = 0      );
	virtual void clear(                            );

private:
	void checks();
};
}
}

#endif /* DUMPER_REDUCTION_HPP_ */
