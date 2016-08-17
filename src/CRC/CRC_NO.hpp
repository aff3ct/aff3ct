#ifndef CRC_NO_HPP_
#define CRC_NO_HPP_

#include "CRC.hpp"

template <typename B>
class CRC_NO : public CRC<B>
{
public:
	CRC_NO(const int n_frames = 1, const std::string name = "CRC_NO");
	virtual ~CRC_NO();

	int  size (                          ) const;
	void build(      mipp::vector<B>& U_K);
	bool check(const mipp::vector<B>& V_K);
};

#endif /* CRC_NO_HPP_ */