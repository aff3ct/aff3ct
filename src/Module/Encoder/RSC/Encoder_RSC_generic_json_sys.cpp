#include <cmath>
#include <sstream>
#include <iomanip>
#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_RSC_generic_json_sys<B>
::Encoder_RSC_generic_json_sys(const int& K, const int& N, const bool buffered_encoding, const std::vector<int> poly,
                               std::ostream &stream)
: Encoder_RSC_generic_sys<B>(K, N, buffered_encoding, poly),
  stream(stream), bit_counter(0), natural_domain(true), poly(poly)
{
	const std::string name = "Encoder_RSC_generic_json_sys";
	this->set_name(name);
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

template <typename B>
void Encoder_RSC_generic_json_sys<B>
::set_n_frames(const size_t n_frames)
{
	if (n_frames != 1)
	{
		std::stringstream message;
		message << "'n_frames' has to be equal to 1 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
		Encoder_RSC_generic_sys<B>::set_n_frames(n_frames);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_RSC_generic_json_sys<B_8>;
template class aff3ct::module::Encoder_RSC_generic_json_sys<B_16>;
template class aff3ct::module::Encoder_RSC_generic_json_sys<B_32>;
template class aff3ct::module::Encoder_RSC_generic_json_sys<B_64>;
#else
template class aff3ct::module::Encoder_RSC_generic_json_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
