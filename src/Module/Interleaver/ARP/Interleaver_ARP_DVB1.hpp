#ifndef INTERLEAVER_ARP_DVB1_HPP
#define	INTERLEAVER_ARP_DVB1_HPP

#include <algorithm>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_ARP_DVB1 : public Interleaver<T>
{
private:
	int p0, p1, p2, p3;
	std::vector<std::vector<int>> parameters = {{11, 24,  0, 24},
	                                            { 7, 34, 32,  2},
	                                            {13,106,108,  2},
	                                            {23,112,  4,116},
	                                            {17,116, 72,188},
	                                            {11,  6,  8,  2},
	                                            {13,  0,  4,  8},
	                                            {13, 10,  4,  2},
	                                            {19,  2, 16,  6},
	                                            {19,428,224,652},
	                                            {19,  2, 16,  6},
	                                            {19,376,224,600}};

public:
	Interleaver_ARP_DVB1(const int size, const int n_frames = 1, const std::string name = "Interleaver_ARP_DVB1")
	: Interleaver<T>(size, false, n_frames, name)
	{
		switch (size / 2)
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
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
				break;
		}
	}

protected:
	void gen_lut(T *lut, const int frame_id)
	{
		int p;
		int size_d2 = this->get_size()/2;
		std::vector<int> symbols_lut(size_d2);
		
		for (auto i = 0; i < size_d2; i++)
		{
			switch(i%4)
			{
				case 0:
					p = 0;
					break;
				case 1:
					p = size_d2/2 + p1;
					break;
				case 2:
					p = p2;
					break;
				case 3:
					p = size_d2/2 + p3;
					break;
			}
			symbols_lut[i] = (p0*i + p + 1) % size_d2;
		}

		std::vector<int> io(this->get_size());
		std::iota(io.begin(), io.end(), 0);

		for (auto i = 0; i < this->get_size(); i+=2)
		{
			lut[i  ] = io[2*symbols_lut[i/2]  ];
			lut[i+1] = io[2*symbols_lut[i/2]+1];
		}
	}
};
}
}

#endif	/* INTERLEAVER_ARP_DVB1_HPP */
