#ifndef CRC_POLYNOMIAL_HPP_
#define CRC_POLYNOMIAL_HPP_

#include <map>

#include "../CRC.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class CRC_polynomial : public CRC<B>
{
protected:
	const static std::map<std::string, mipp::vector<B>> polynomials;
//const int insert_pos;
	mipp::vector<B> polynomial;
	mipp::vector<B> buff_crc;

public:
	CRC_polynomial(const int K, std::string poly_key, const int n_frames = 1, const std::string name = "CRC_polynomial");
	virtual ~CRC_polynomial(){};

	static  int  size (      std::string poly_key                         );
	virtual int  size (                                                   ) const;
	virtual void build(      mipp::vector<B>& U_K                         );
	virtual bool check(const mipp::vector<B>& V_K, const int n_frames = -1);

protected:
	void _generate(const mipp::vector<B>& U_in, 
	                     mipp::vector<B>& U_out, 
	               const int off_in, 
	               const int off_out, 
	               const int loop_size);
};

template<typename B>
const std::map<std::string, mipp::vector<B>> CRC_polynomial<B>::polynomials = {
  {"32-GZIP", {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1}},
  {"24-LTEA", {                        1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1}},
  {"16-IBM",  {                                                1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1}},
  {"16-CCITT",{                                                1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}},
  {"8-DVB-S2",{                                                                        1, 1, 1, 0, 1, 0, 1, 0, 1}},
  {"4-ITU",   {                                                                                    1, 0, 0, 1, 1}},
  {"3-0x3",   {                                                                                       1, 0, 1, 1}},
  {"2-0x1",   {                                                                                          1, 0, 1}},
  {"1-0x1",   {                                                                                             1, 1}}};
}
}

#endif /* CRC_POLYNOMIAL_HPP_ */
