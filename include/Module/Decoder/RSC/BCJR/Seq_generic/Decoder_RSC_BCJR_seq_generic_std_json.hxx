#include <algorithm>
#include <iomanip>
#include <string>
#include <limits>

#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std_json.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,MAX1,MAX2>
::Decoder_RSC_BCJR_seq_generic_std_json(const int &K,
                                        const std::vector<std::vector<int>> &trellis,
                                        const int n_ite,
                                        const bool buffered_encoding,
                                        std::ostream &stream)
: Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>(K, trellis, buffered_encoding),
  n_ite(n_ite), ite_counter(0), stream(stream)
{
	const std::string name = "Decoder_RSC_BCJR_seq_generic_std_json";
	this->set_name(name);
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,MAX1,MAX2>
::compute_ext(const R *sys, R *ext)
{
	// compute extrinsic values
	for (auto i = 0; i < this->K + this->n_ff; i++)
	{
		int transition_a0 = -1, transition_b0 = 1, transition_a1 = -1, transition_b1 = -1;
		int transition_a, transition_b;

		RD max0 = -std::numeric_limits<RD>::max();
		RD max1 = -std::numeric_limits<RD>::max();

		RD max0_json      = max0;
		RD max1_json      = max1;
		RD prev_max0_json = max0;
		RD prev_max1_json = max1;

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[1][j] == +1)
			{
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

				max0_json = std::max(max0_json, (RD)(this->alpha[                 j ][i   ] +
				                                     this->beta [this->trellis[6][j]][i +1] +
				                                     this->gamma[this->trellis[7][j]][i   ]));

				if (max0_json > prev_max0_json)
				{
					transition_a0 = j;
					transition_b0 = this->trellis[6][j];
					prev_max0_json = max0_json;
				}
			}
			else
			{
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

				max1_json = std::max(max1_json, (RD)(this->alpha[                 j ][i   ] +
				                                     this->beta [this->trellis[8][j]][i +1] -
				                                     this->gamma[this->trellis[9][j]][i   ]));

				if (max1_json > prev_max1_json)
				{
					transition_a1 = j;
					transition_b1 = this->trellis[8][j];
					prev_max1_json = max1_json;
				}
			}

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[4][j] == +1)
			{
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

				max0_json = std::max(max0_json, (RD)(this->alpha[                 j ][i   ] +
				                                     this->beta [this->trellis[6][j]][i +1] +
				                                     this->gamma[this->trellis[7][j]][i   ]));

				if (max0_json > prev_max0_json)
				{
					transition_a0 = j;
					transition_b0 = this->trellis[6][j];
					prev_max0_json = max0_json;
				}
			}
			else
			{
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

				max1_json = std::max(max1_json, (RD)(this->alpha[                 j ][i   ] +
				                                     this->beta [this->trellis[8][j]][i +1] -
				                                     this->gamma[this->trellis[9][j]][i   ]));

				if (max1_json > prev_max1_json)
				{
					transition_a1 = j;
					transition_b1 = this->trellis[8][j];
					prev_max1_json = max1_json;
				}
			}

		if (i < this->K)
			ext[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - sys[i];

		transition_a = (max0_json > max1_json) ? transition_a0 : transition_a1;
		transition_b = (max0_json > max1_json) ? transition_b0 : transition_b1;

		stream << "\t\t\t\t\t\t\t" << "["<< std::setw(2) << +transition_a << ", " << +transition_b << "]";
		if (i != (this->K + this->n_ff -1))
			stream << "," << std::endl;
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
int Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,MAX1,MAX2>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	if (ite_counter == 0)
	{
		stream << "\t\t" << "\"stage\": \"decoder\"," << std::endl
		       << "\t\t" << "\"n_ite\": " << this->n_ite << "," << std::endl
		       << "\t\t" << "\"data\": [" << std::endl;
	}

	if (!(ite_counter % 2))
		stream << "\t\t\t" << "[" << std::endl;

	stream << "\t\t\t\t" << "{" << std::endl
	       << "\t\t\t\t\t" << "\"comment\": \"iteration n°" << ((ite_counter >> 1) +1) << "\"," << std::endl
	       << "\t\t\t\t\t" << "\"domain\": " << ((ite_counter % 2) ? "\"interleaved\"" : "\"natural\"") << "," << std::endl;

	this->compute_gamma(sys, par);

	stream << "\t\t\t\t\t" << "\"gamma\": [" << std::endl;
	for (auto i = 0; i < this->K + this->n_ff; i++)
	{
		stream << "\t\t\t\t\t\t";
		for (auto j = 0; j < (int)this->gamma.size(); j++)
			stream << std::setw(8) << std::setprecision(3) << +this->gamma[j][i]
			       << ((j != (int)this->gamma.size() -1) ? ", " : "");

		stream << ((i != this->K + this->n_ff -1) ? ", " : "],") << std::endl;
	}

	this->compute_alpha();

	stream << "\t\t\t\t\t" << "\"alpha\": [" << std::endl;
	for (auto i = 0; i < this->K + this->n_ff +1; i++)
	{
		stream << "\t\t\t\t\t\t";
		for (auto j = 0; j < (int)this->alpha.size(); j++)
			stream << std::setw(8) << std::setprecision(3) << +this->alpha[j][i]
			       << ((j != (int)this->alpha.size() -1) ? ", " : "");

		stream << ((i != this->K + this->n_ff +1 -1) ? ", " : "],") << std::endl;
	}

	this->compute_beta();

	stream << "\t\t\t\t\t" << "\"beta\": [" << std::endl;
	for (auto i = 0; i < this->K + this->n_ff +1; i++)
	{
		stream << "\t\t\t\t\t\t";
		for (auto j = 0; j < (int)this->beta.size(); j++)
			stream << std::setw(8) << std::setprecision(3) << +this->beta[j][i]
			       << ((j != (int)this->beta.size() -1) ? ", " : "");

		stream << ((i != this->K + this->n_ff +1 -1) ? ", " : "],") << std::endl;
	}

	stream << "\t\t\t\t\t" << "\"transitions\": [" << std::endl;
	this->compute_ext(sys, ext);
	stream << "]" << std::endl;

	stream << "\t\t\t\t" << "}" << ((ite_counter % 2) ? "" : ",") << std::endl;

	if (ite_counter % 2)
		stream << "\t\t\t" << "]";

	ite_counter++;
	if ((ite_counter >> 1) == n_ite)
	{
		stream << std::endl
		       << "\t\t" << "]" << std::endl
		       << "\t" << "}]," << std::endl;
		ite_counter = 0;
	}
	else if (!(ite_counter % 2))
		stream << "," << std::endl;

	return 0;
}
}
}
