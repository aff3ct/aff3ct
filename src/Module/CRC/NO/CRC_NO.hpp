#ifndef CRC_NO_HPP_
#define CRC_NO_HPP_

#include "../CRC.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class CRC_NO : public CRC<B>
{
public:
	CRC_NO(const int K, const int n_frames = 1, const std::string name = "CRC_NO");
	virtual ~CRC_NO();

	int  size (                                                   ) const;
	void build(      mipp::vector<B>& U_K                         );
	bool check(const mipp::vector<B>& V_K, const int n_frames = -1);
};
}
}

#endif /* CRC_NO_HPP_ */
