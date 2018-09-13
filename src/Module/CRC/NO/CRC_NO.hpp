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
	CRC_NO(const int K, const int n_frames = 1);
	virtual ~CRC_NO() = default;

	int get_size() const;

protected:
	virtual void _build       (const B *U_K1, B *U_K2, const int frame_id);
	virtual void _extract     (const B *V_K1, B *V_K2, const int frame_id);
	virtual bool _check       (const B *V_K          , const int frame_id);
	virtual bool _check_packed(const B *V_K          , const int frame_id);
};
}
}

#endif /* CRC_NO_HPP_ */
