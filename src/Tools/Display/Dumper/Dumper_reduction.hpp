#ifndef DUMPER_REDUCTION_HPP_
#define DUMPER_REDUCTION_HPP_

#include "Dumper.hpp"

namespace aff3ct
{
namespace tools
{
class Dumper_reduction : Dumper
{
protected:
	std::vector<Dumper*> dumpers;

public:
	explicit Dumper_reduction(std::vector<Dumper*> &dumpers);
	virtual ~Dumper_reduction();

	virtual void dump (const std::string& base_path);
	virtual void add  (const int frame_id = 0      );
	virtual void clear(                            );

private:
	void checks();
};
}
}

#endif /* DUMPER_REDUCTION_HPP_ */
