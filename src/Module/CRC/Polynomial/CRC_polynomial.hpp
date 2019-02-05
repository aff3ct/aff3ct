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
	const static std::map<std::string, std::tuple<unsigned, int>> known_polynomials;
	std::vector<B> polynomial;
	unsigned       polynomial_packed;
	std::vector<B> buff_crc;

public:
	CRC_polynomial(const int K, std::string poly_key, const int size = 0, const int n_frames = 1);
	virtual ~CRC_polynomial() = default;

	static int         get_size (std::string poly_key);
	static std::string get_name (std::string poly_key);
	static unsigned    get_value(std::string poly_key);

protected:
	virtual void _build       (const B *U_K1, B *U_K2, const int frame_id);
	virtual void _extract     (const B *V_K1, B *V_K2, const int frame_id);
	virtual bool _check       (const B *V_K          , const int frame_id);
	virtual bool _check_packed(const B *V_K          , const int frame_id);

	void _generate(const B *U_in,
	                     B *U_out,
	               const int off_in,
	               const int off_out,
	               const int loop_size);
};

// database from here: https://en.wikipedia.org/wiki/Cyclic_redundancy_check#Commonly_used_and_standardized_CRCs
template <typename B>
const std::map<std::string, std::tuple<unsigned, int>> CRC_polynomial<B>::known_polynomials =
  {{"32-GZIP"        , std::make_tuple(0x04C11DB7, 32)},
   {"32-CASTAGNOLI"  , std::make_tuple(0x1EDC6F41, 32)},
   {"32-AIXM"        , std::make_tuple(0x814141AB, 32)},
   {"32-KOOPMAN"     , std::make_tuple(0x32583499, 32)},
   {"30-CDMA"        , std::make_tuple(0x2030B9C7, 30)},
   {"24-LTEA"        , std::make_tuple(0x864CFB  , 24)},
   {"24-RADIX-64"    , std::make_tuple(0x864CFB  , 24)},
   {"24-FLEXRAY"     , std::make_tuple(0x5D6DCB  , 24)},
   {"24-5GA"         , std::make_tuple(0x864CFB  , 24)},
   {"24-5GB"         , std::make_tuple(0x800063  , 24)},
   {"24-5GC"         , std::make_tuple(0xB2B117  , 24)},
   {"21-CAN"         , std::make_tuple(0x102899  , 21)},
   {"17-CAN"         , std::make_tuple(0x1685B   , 17)},
   {"16-IBM"         , std::make_tuple(0x8005    , 16)},
   {"16-CCITT"       , std::make_tuple(0x1021    , 16)},
   {"16-PROFIBUS"    , std::make_tuple(0x1DCF    , 16)},
   {"16-OPENSAFETY-B", std::make_tuple(0x755B    , 16)},
   {"16-OPENSAFETY-A", std::make_tuple(0x5935    , 16)},
   {"16-DNP"         , std::make_tuple(0x3D65    , 16)},
   {"16-T10-DIF"     , std::make_tuple(0x8BB7    , 16)},
   {"16-DECT"        , std::make_tuple(0x0589    , 16)},
   {"16-CDMA2000"    , std::make_tuple(0xC867    , 16)},
   {"16-ARINC"       , std::make_tuple(0xA02B    , 16)},
   {"16-5G"          , std::make_tuple(0x1023    , 16)},
   {"16-CHAKRAVARTY" , std::make_tuple(0x2F15    , 16)},
   {"15-MPT1327"     , std::make_tuple(0x6815    , 15)},
   {"15-CAN"         , std::make_tuple(0x4599    , 15)},
   {"14-DARC"        , std::make_tuple(0x0805    , 14)},
   {"13-BBC"         , std::make_tuple(0x1CF5    , 13)},
   {"12-CDMA2000"    , std::make_tuple(0xF13     , 12)},
   {"12-TELECOM"     , std::make_tuple(0x80F     , 12)},
   {"11-FLEXRAY"     , std::make_tuple(0x385     , 11)},
   {"11-5G"          , std::make_tuple(0x621     , 11)},
   {"10-CDMA2000"    , std::make_tuple(0x3D9     , 10)},
   {"10-ATM"         , std::make_tuple(0x233     , 10)},
   {"8-WCDMA"        , std::make_tuple(0x9B      ,  8)},
   {"8-SAE-J1850"    , std::make_tuple(0x1D      ,  8)},
   {"8-DARC"         , std::make_tuple(0x39      ,  8)},
   {"8-DALLAS"       , std::make_tuple(0x31      ,  8)},
   {"8-CCITT"        , std::make_tuple(0x07      ,  8)},
   {"8-AUTOSAR"      , std::make_tuple(0x2F      ,  8)},
   {"8-DVB-S2"       , std::make_tuple(0xD5      ,  8)},
   {"7-MVB"          , std::make_tuple(0x65      ,  7)},
   {"7-MMC"          , std::make_tuple(0x09      ,  7)},
   {"6-CDMA2000-A"   , std::make_tuple(0x27      ,  6)},
   {"6-CDMA2000-B"   , std::make_tuple(0x07      ,  6)},
   {"6-DARC"         , std::make_tuple(0x19      ,  6)},
   {"6-ITU"          , std::make_tuple(0x03      ,  6)},
   {"5-ITU"          , std::make_tuple(0x15      ,  5)},
   {"5-EPC"          , std::make_tuple(0x09      ,  5)},
   {"5-USB"          , std::make_tuple(0x05      ,  5)},
   {"4-ITU"          , std::make_tuple(0x3       ,  4)},
   {"1-PAR"          , std::make_tuple(0x1       ,  1)}};
}
}

#endif /* CRC_POLYNOMIAL_HPP_ */
