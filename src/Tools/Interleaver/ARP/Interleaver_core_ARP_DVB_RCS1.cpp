#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Interleaver/ARP/Interleaver_core_ARP_DVB_RCS1.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_ARP_DVB_RCS1<T>
::Interleaver_core_ARP_DVB_RCS1(const int size)
: Interleaver_core<T>(size, "DVB_RCS1", false)
{
	switch (size)
	{
		case 48:
			p0 = parameters[0][0];
			p1 = parameters[0][1];
			p2 = parameters[0][2];
			p3 = parameters[0][3];
			break;
		case 64:
			p0 = parameters[1][0];
			p1 = parameters[1][1];
			p2 = parameters[1][2];
			p3 = parameters[1][3];
			break;
		case 212:
			p0 = parameters[2][0];
			p1 = parameters[2][1];
			p2 = parameters[2][2];
			p3 = parameters[2][3];
			break;
		case 220:
			p0 = parameters[3][0];
			p1 = parameters[3][1];
			p2 = parameters[3][2];
			p3 = parameters[3][3];
			break;
		case 228:
			p0 = parameters[4][0];
			p1 = parameters[4][1];
			p2 = parameters[4][2];
			p3 = parameters[4][3];
			break;
		case 424:
			p0 = parameters[5][0];
			p1 = parameters[5][1];
			p2 = parameters[5][2];
			p3 = parameters[5][3];
			break;
		case 432:
			p0 = parameters[6][0];
			p1 = parameters[6][1];
			p2 = parameters[6][2];
			p3 = parameters[6][3];
			break;
		case 440:
			p0 = parameters[7][0];
			p1 = parameters[7][1];
			p2 = parameters[7][2];
			p3 = parameters[7][3];
			break;
		case 752:
			p0 = parameters[11][0];
			p1 = parameters[11][1];
			p2 = parameters[11][2];
			p3 = parameters[11][3];
			break;
		case 848:
			p0 = parameters[8][0];
			p1 = parameters[8][1];
			p2 = parameters[8][2];
			p3 = parameters[8][3];
			break;
		case 856:
			p0 = parameters[9][0];
			p1 = parameters[9][1];
			p2 = parameters[9][2];
			p3 = parameters[9][3];
			break;
		case 864:
			p0 = parameters[10][0];
			p1 = parameters[10][1];
			p2 = parameters[10][2];
			p3 = parameters[10][3];
			break;
		default:
			std::stringstream message;
			message << "There is no ARP parameters for 'size' = " << size
			        << " (supported sizes are 48, 64, 212, 220, 228, 424, 432, 440, 752, 848, 856, and 864).";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			break;
	}

	this->init();
}

template <typename T>
Interleaver_core_ARP_DVB_RCS1<T>* Interleaver_core_ARP_DVB_RCS1<T>
::clone() const
{
	auto t = new Interleaver_core_ARP_DVB_RCS1(*this);
	return t;
}

template <typename T>
void Interleaver_core_ARP_DVB_RCS1<T>
::gen_lut(T *lut, const size_t frame_id)
{
	int p = 0;
	int size = this->get_size();
	for (auto i = 0; i < size; i++)
	{
		switch(i%4)
		{
			case 0:
				p = 0;
				break;
			case 1:
				p = size/2 + p1;
				break;
			case 2:
				p = p2;
				break;
			case 3:
				p = size/2 + p3;
				break;
		}
		lut[i] = (p0*i + p + 1) % size;
	}
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_ARP_DVB_RCS1<uint8_t >;
template class aff3ct::tools::Interleaver_core_ARP_DVB_RCS1<uint16_t>;
template class aff3ct::tools::Interleaver_core_ARP_DVB_RCS1<uint32_t>;
template class aff3ct::tools::Interleaver_core_ARP_DVB_RCS1<uint64_t>;
// ==================================================================================== explicit template instantiation

