#ifndef CRC_POLYNOMIAL_HPP_
#define CRC_POLYNOMIAL_HPP_

#include <map>

#include "../CRC.hpp"

template <typename B>
class CRC_polynomial : public CRC<B>
{
protected:
	const static std::map<std::string, std::tuple<unsigned, int>> known_polynomials;
	mipp::vector<B> polynomial;
	unsigned        polynomial_packed;
	int             poly_size;
	mipp::vector<B> buff_crc;

public:
	CRC_polynomial(const int K, std::string poly_key, const int size = 0, const int n_frames = 1,
	               const std::string name = "CRC_polynomial");
	virtual ~CRC_polynomial(){};

	static int         size (std::string poly_key);
	static std::string name (std::string poly_key);
	static unsigned    value(std::string poly_key);

	virtual int  size        (                                                   ) const;
	virtual void build       (      mipp::vector<B>& U_K                         );
	virtual bool check       (const mipp::vector<B>& V_K, const int n_frames = -1);
	virtual bool check_packed(const mipp::vector<B>& V_K, const int n_frames = -1);

protected:
	void _generate(const mipp::vector<B>& U_in, 
	                     mipp::vector<B>& U_out, 
	               const int off_in, 
	               const int off_out, 
	               const int loop_size);
};

template<typename B>
const std::map<std::string, std::tuple<unsigned, int>> CRC_polynomial<B>::known_polynomials =
  {{"32-GZIP" , std::make_tuple(0x04C11DB7, 32)},
   {"24-LTEA" , std::make_tuple(0x864CFB  , 24)},
   {"16-IBM"  , std::make_tuple(0x8005    , 16)},
   {"16-CCITT", std::make_tuple(0x1021    , 16)},
   {"8-DVB-S2", std::make_tuple(0xD5      ,  8)},
   {"5-ITU"   , std::make_tuple(0x15      ,  5)}};

#endif /* CRC_POLYNOMIAL_HPP_ */
