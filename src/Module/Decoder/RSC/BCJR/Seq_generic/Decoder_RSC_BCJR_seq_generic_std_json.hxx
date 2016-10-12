#include "Decoder_RSC_BCJR_seq_generic_std_json.hpp"

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,MAX1,MAX2>
::Decoder_RSC_BCJR_seq_generic_std_json(const int &K, 
                                        const std::vector<std::vector<int>> &trellis,
                                        const int n_ite,
                                        const bool buffered_encoding,
                                        std::ostream &stream,
                                        const std::string name)
: Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>(K, trellis, buffered_encoding, name),
  n_ite(n_ite), ite_counter(0), stream(stream)
{
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,MAX1,MAX2>
::~Decoder_RSC_BCJR_seq_generic_std_json()
{
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,MAX1,MAX2>
::compute_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
{
	// compute extrinsic values
	for (auto i = 0; i < this->K + this->n_ff; i++)
	{
		int transition0 = -1, transition1 = -1, transition = -1;

		RD max0 = -std::numeric_limits<RD>::max();
		RD max1 = -std::numeric_limits<RD>::max();

		RD prev_max0 = max0;
		RD prev_max1 = max1;

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[1][j] == +1)
			{
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

				transition0 = (prev_max0 != max0) ? this->trellis[6][j] : transition0;
				prev_max0 = max0;
			}
			else
			{
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

				transition1 = (prev_max1 != max1) ? this->trellis[8][j] : transition1;
				prev_max1 = max1;
			}

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[4][j] == +1)
			{
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

				transition0 = (prev_max0 != max0) ? this->trellis[6][j] : transition0;
				prev_max0 = max0;
			}
			else
			{
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

				transition1 = (prev_max1 != max1) ? this->trellis[8][j] : transition1;
				prev_max1 = max1;
			}

		if (i < this->K)
			ext[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - sys[i];

		transition = (max0 > max1) ? transition0 : transition1;

		stream << "\t\t\t\t\t\t\t" << std::setw(2) << +transition;
		if (i != (this->K + this->n_ff -1))
			stream << "," << std::endl;
	}
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,MAX1,MAX2>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
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
}
