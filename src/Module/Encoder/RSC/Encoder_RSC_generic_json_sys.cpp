#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_RSC_generic_json_sys.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_RSC_generic_json_sys<B>
::Encoder_RSC_generic_json_sys(const int& K, const int& N, const int& n_frames, const bool buffered_encoding,
                               const std::vector<int> poly, std::ostream &stream, const std::string name)
: Encoder_RSC_generic_sys<B>(K, N, n_frames, buffered_encoding, poly, name),
  stream(stream), bit_counter(0), natural_domain(true), poly(poly)
{
}

template <typename B>
std::vector<std::vector<int>> Encoder_RSC_generic_json_sys<B>
::get_trellis()
{
	std::vector<std::vector<int>> trellis(10, std::vector<int>(this->n_states));

	std::vector<bool> occurrence(this->n_states, false);

	for (auto i = 0; i < this->n_states; i++)
	{
		// sys = 0
		auto state   = i;
		auto bit_sys = 0;
		auto bit_par = Encoder_RSC_generic_sys<B>::inner_encode(bit_sys, state);

		trellis[0 + (occurrence[state] ? 3 : 0)][state] = i;                 // initial state
		trellis[1 + (occurrence[state] ? 3 : 0)][state] = +1;                // gamma coeff
		trellis[2 + (occurrence[state] ? 3 : 0)][state] = bit_sys ^ bit_par; // gamma
		trellis[6                              ][i    ] = state;             // final state, bit syst = 0
		trellis[7                              ][i    ] = bit_sys ^ bit_par; // gamma      , bit syst = 0

		occurrence[state] = true;

		// sys = 1
		state   = i;
		bit_sys = 1;
		bit_par = Encoder_RSC_generic_sys<B>::inner_encode(bit_sys, state);

		trellis[0 + (occurrence[state] ? 3 : 0)][state] = i;                 // initial state
		trellis[1 + (occurrence[state] ? 3 : 0)][state] = -1;                // gamma coeff
		trellis[2 + (occurrence[state] ? 3 : 0)][state] = bit_sys ^ bit_par; // gamma
		trellis[8                              ][i    ] = state;             // initial state, bit syst = 1
		trellis[9                              ][i    ] = bit_sys ^ bit_par; // gamma        , bit syst = 1

		occurrence[state] = true;
	}

	return trellis;
}

template <typename B>
int Encoder_RSC_generic_json_sys<B>
::inner_encode(const int bit_sys, int &state)
{
	const auto bit_par = Encoder_RSC_generic_sys<B>::inner_encode(bit_sys, state);

	if (bit_counter == 0)
	{
		if (natural_domain)
		{
			assert(this->n_frames == 1);
			const auto trellis = this->get_trellis();

			stream << "\t" << "[{" << std::endl;

			stream << "\t\t" << "\"stage\": \"encoder\"," << std::endl
			       << "\t\t" << "\"K\": " << this->K << "," << std::endl
			       << "\t\t" << "\"poly\": \"{0" << std::oct << poly[0] << ",0" << std::oct << poly[1] << "}\"," << std::endl
			       << "\t\t" << "\"R\": \"1/3\", " << std::endl
			       << "\t\t" << "\"ff\": " << std::dec << this->n_ff << "," << std::endl
			       << "\t\t" << "\"states\": " << this->n_states << "," << std::endl;

			stream << "\t\t" << "\"trellis\": [" << std::endl;
			for (auto i = 0; i < (int)trellis.size(); i++)
			{
				stream << "\t\t\t" << "[";
				for (auto j = 0; j < (int)trellis[i].size() -1; j++)
					stream << std::setw(3) << trellis[i][j] << ",";
				stream << std::setw(3) << trellis[i][(int)trellis[i].size() -1] << "]";
				if (i != (int)trellis.size() -1)
					stream << "," << std::endl;
			}
			stream << "]," << std::endl;

			stream << "\t\t" << "\"transitions\": {" << std::endl
			       << "\t\t\t" << "\"natural\": [" << std::endl;
		}
		else
			stream << "\t\t\t" << "\"interleaved\": [" << std::endl;
	}

	stream << "\t\t\t\t" << std::setw(2) << state;

	if (bit_counter != (this->K + this->n_ff -1))
	{
		stream << "," << std::endl;
		bit_counter++;
	}
	else
	{
		if (natural_domain)
			stream << "]," << std::endl;
		else
			stream << "]}"  << std::endl
			       << "\t" << "},{" << std::endl;

		bit_counter = 0;
		natural_domain = !natural_domain;
	}

	return bit_par;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_RSC_generic_json_sys<B_8>;
template class Encoder_RSC_generic_json_sys<B_16>;
template class Encoder_RSC_generic_json_sys<B_32>;
template class Encoder_RSC_generic_json_sys<B_64>;
#else
template class Encoder_RSC_generic_json_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
