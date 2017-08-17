#ifndef INTERLEAVER_ARP_DVB2_HPP
#define	INTERLEAVER_ARP_DVB2_HPP

#include <algorithm>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_ARP_DVB_RCS2 : public Interleaver<T>
{
private:
	int p, q0, q1, q2, q3;

public:
	Interleaver_ARP_DVB_RCS2(const int size, const int n_frames = 1,
	                         const std::string name = "Interleaver_ARP_DVB_RCS2")
	: Interleaver<T>(size, false, n_frames, name)
	{
		switch (size)
		{
			case 56:
				p  = 9;
				q0 = 2;
				q1 = 2;
				q2 = 8;
				q3 = 0;
				break;
			case 64:
				p  = 19;
				q0 = 1;
				q1 = 3;
				q2 = 8;
				q3 = 1;
				break;
			case 152:
				p  = 17;
				q0 = 9;
				q1 = 5;
				q2 = 14;
				q3 = 1;
				break;
			case 204:
				p  = 23;
				q0 = 10;
				q1 = 5;
				q2 = 0;
				q3 = 0;
				break;
			case 228:
				p  = 23;
				q0 = 1;
				q1 = 11;
				q2 = 2;
				q3 = 3;
				break;
			case 340:
				p  = 23;
				q0 = 6;
				q1 = 13;
				q2 = 10;
				q3 = 0;
				break;
			case 384:
				p  = 23;
				q0 = 6;
				q1 = 13;
				q2 = 10;
				q3 = 0;
				break;
			case 400:
				p  = 23;
				q0 = 10;
				q1 = 8;
				q2 = 2;
				q3 = 1;
				break;
			case 432:
				p  = 29;
				q0 = 1;
				q1 = 4;
				q2 = 1;
				q3 = 1;
				break;
			case 440:
				p  = 49;
				q0 = 1;
				q1 = 9;
				q2 = 8;
				q3 = 1;
				break;
			case 652:
				p  = 107;
				q0 = 2;
				q1 = 10;
				q2 = 15;
				q3 = 1;
				break;
			case 680:
				p  = 33;
				q0 = 9;
				q1 = 15;
				q2 = 3;
				q3 = 1;
				break;
			case 752:
				p  = 37;
				q0 = 1;
				q1 = 3;
				q2 = 4;
				q3 = 2;
				break;
			case 864:
				p  = 67;
				q0 = 2;
				q1 = 4;
				q2 = 13;
				q3 = 5;
				break;
			case 1192:
				p  = 49;
				q0 = 0;
				q1 = 3;
				q2 = 5;
				q3 = 0;
				break;
			case 1332:
				p  = 53;
				q0 = 1;
				q1 = 4;
				q2 = 6;
				q3 = 2;
				break;
			case 1504:
				p  = 53;
				q0 = 2;
				q1 = 1;
				q2 = 2;
				q3 = 2;
				break;
			case 1752:
				p  = 59;
				q0 = 1;
				q1 = 1;
				q2 = 2;
				q3 = 1;
				break;
			default:
				std::stringstream message;
				message << "There is no ARP parameters for 'size' = " << size << " (supported sizes are 56, 64, 152, "
				        << "204, 228, 340, 384, 400, 432, 440, 652, 680, 752, 864, 1192, 1332, 1504 and 1752).";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
				break;
		}
	}

protected:
	void gen_lut(T *lut, const int frame_id)
	{
		int q;
		int size = this->get_size();
		for (auto i = 0; i < size; i++)
		{
			switch(i%4)
			{
				case 0:
					q = 0;
					break;
				case 1:
					q = 4*q1;
					break;
				case 2:
					q = 4*q0*p + 4*q2;
					break;
				case 3:
					q = 4*q0*p + 4*q3;
					break;
			}
			lut[i] = (p*i + q + 3) % size;
		}
	}
};
}
}

#endif	/* INTERLEAVER_ARP_DVB2_HPP */
