#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Interleaver/ARP/Interleaver_core_ARP_DVB_RCS2.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_ARP_DVB_RCS2<T>
::Interleaver_core_ARP_DVB_RCS2(const int size)
: Interleaver_core<T>(size, "DVB_RCS2", false)
{
	switch (size)
	{
	// ======== // DVB-RCS2 Reference Waveforms (ETSI EN 301 545-2 V1.3.1 (2020-07), Annex A, Table A-1):
	case 152:	// wfm 1, wfm 3
		p  = 17;
		q0 = 9;
		q1 = 5;
		q2 = 14;
		q3 = 1;
		break;
	case 56:	// wfm 2
		p  = 9;
		q0 = 2;
		q1 = 2;
		q2 = 8;
		q3 = 0;
		break;
	case 236:	// wfm 4, wfm 40, wfm 41
		p  = 23;
		q0 = 10;
		q1 = 2;
		q2 = 11;
		q3 = 1;
		break;
	case 340:	// wfm 5
		p  = 23;
		q0 = 6;
		q1 = 13;
		q2 = 10;
		q3 = 0;
		break;
	case 384:	// wfm 6
		p  = 25;
		q0 = 1;
		q1 = 2;
		q2 = 0;
		q3 = 1;
		break;
	case 432:	// wfm 7
		p  = 29;
		q0 = 1;
		q1 = 4;
		q2 = 1;
		q3 = 1;
		break;
	case 460:	// wfm 8
		p  = 29;
		q0 = 6;
		q1 = 5;
		q2 = 0;
		q3 = 0;
		break;
	case 520:	// wfm 9
		p  = 31;
		q0 = 0;
		q1 = 1;
		q2 = 2;
		q3 = 0;
		break;
	case 576:	// wfm 10
		p  = 31;
		q0 = 0;
		q1 = 0;
		q2 = 0;
		q3 = 0;
		break;
	case 700:	// wfm 11
		p  = 37;
		q0 = 0;
		q1 = 2;
		q2 = 0;
		q3 = 2;
		break;
	case 776:	// wfm 12
		p  = 39;
		q0 = 7;
		q1 = 0;
		q2 = 0;
		q3 = 0;
		break;
	case 492:	// wfm 13, wfm 42
		p  = 31;
		q0 = 0;
		q1 = 3;
		q2 = 1;
		q3 = 0;
		break;
	case 752:	// wfm 14, wfm 43
		p  = 37;
		q0 = 1;
		q1 = 3;
		q2 = 4;
		q3 = 2;
		break;
	case 1056:	// wfm 15
		p  = 43;
		q0 = 0;
		q1 = 0;
		q2 = 6;
		q3 = 2;
		break;
	case 1192:	// wfm 16
		p  = 49;
		q0 = 0;
		q1 = 3;
		q2 = 5;
		q3 = 0;
		break;
	case 1332:	// wfm 17
		p  = 49;
		q0 = 0;
		q1 = 5;
		q2 = 0;
		q3 = 5;
		break;
	case 1420:	// wfm 18
		p  = 53;
		q0 = 1;
		q1 = 4;
		q2 = 6;
		q3 = 2;
		break;
	case 1600:	// wfm 19
		p  = 53;
		q0 = 1;
		q1 = 10;
		q2 = 7;
		q3 = 1;
		break;
	case 1776:	// wfm 20
		p  = 59;
		q0 = 3;
		q1 = 8;
		q2 = 5;
		q3 = 1;
		break;
	case 2156:	// wfm 21
		p  = 65;
		q0 = 0;
		q1 = 3;
		q2 = 7;
		q3 = 0;
		break;
	case 2396:	// wfm 22
		p  = 81;
		q0 = 1;
		q1 = 2;
		q2 = 5;
		q3 = 2;
		break;
	case 400:	// wfm 32, wfm 33
		p  = 23;
		q0 = 10;
		q1 = 8;
		q2 = 2;
		q3 = 1;
		break;
	case 680:	// wfm 34, wfm 35, wfm 36
		p  = 33;
		q0 = 9;
		q1 = 15;
		q2 = 3;
		q3 = 1;
		break;
	case 1752:	// wfm 37, wfm 38, wfm 39
		p  = 59;
		q0 = 1;
		q1 = 1;
		q2 = 2;
		q3 = 1;
		break;
	case 204:	// wfm 44
		p  = 23;
		q0 = 10;
		q1 = 5;
		q2 = 0;
		q3 = 0;
		break;
	case 220:	// wfm 45
		p  = 23;
		q0 = 6;
		q1 = 10;
		q2 = 4;
		q3 = 0;
		break;
	case 248:	// wfm 46
		p  = 23;
		q0 = 6;
		q1 = 8;
		q2 = 1;
		q3 = 1;
		break;
	case 276:	// wfm 47
		p  = 25;
		q0 = 1;
		q1 = 1;
		q2 = 2;
		q3 = 0;
		break;
	case 336:	// wfm 48
		p  = 23;
		q0 = 8;
		q1 = 1;
		q2 = 4;
		q3 = 1;
		break;
	case 372:	// wfm 49
		p  = 25;
		q0 = 1;
		q1 = 7;
		q2 = 2;
		q3 = 1;
		break;
	// ======== // Old waveforms (source unknown), kept for compatibility:
	case 64:
		p  = 19;
		q0 = 1;
		q1 = 3;
		q2 = 8;
		q3 = 1;
		break;
	case 228:
		p  = 23;
		q0 = 1;
		q1 = 11;
		q2 = 2;
		q3 = 3;
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
	case 864:
		p  = 67;
		q0 = 2;
		q1 = 4;
		q2 = 13;
		q3 = 5;
		break;
	case 1504:
		p  = 53;
		q0 = 2;
		q1 = 1;
		q2 = 2;
		q3 = 2;
		break;
	default:
		std::stringstream message;
		message << "There are no DVB-RCS2 parameters for 'size' = " << size << " (supported sizes are 56, 64, 152, "
					"204, 220, 228, 236, 248, 276, 336, 340, 372, 384, 400, 432, 440, 460, 492, 520, 576, 652, 680, "
					"700, 752, 776, 864, 1056, 1192, 1332, 1420, 1504, 1600, 1752, 1776, 2156 and 2396).";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		break;
	}

	this->init();
}

template <typename T>
Interleaver_core_ARP_DVB_RCS2<T>* Interleaver_core_ARP_DVB_RCS2<T>
::clone() const
{
	auto t = new Interleaver_core_ARP_DVB_RCS2(*this);
	return t;
}

template <typename T>
void Interleaver_core_ARP_DVB_RCS2<T>
::gen_lut(T *lut, const size_t frame_id)
{
	int q = 0;
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

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_ARP_DVB_RCS2<uint8_t >;
template class aff3ct::tools::Interleaver_core_ARP_DVB_RCS2<uint16_t>;
template class aff3ct::tools::Interleaver_core_ARP_DVB_RCS2<uint32_t>;
template class aff3ct::tools::Interleaver_core_ARP_DVB_RCS2<uint64_t>;
// ==================================================================================== explicit template instantiation
