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

	int get_size() const;

	void build       (const B *U_K1, B *U_K2               ); using CRC<B>::build;
	void extract     (const B *V_K1, B *V_K2               ); using CRC<B>::extract;
	bool check       (const B *V_K, const int n_frames = -1); using CRC<B>::check;
	bool check_packed(const B *V_K, const int n_frames = -1); using CRC<B>::check_packed;
};
}
}

#endif /* CRC_NO_HPP_ */
