#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

#include "Generator_polar_SCL_sys.hpp"

using namespace aff3ct::generator;
using namespace aff3ct::tools;

Generator_polar_SCL_sys
::Generator_polar_SCL_sys(const int& K,
                          const int& N,
                          const float& snr,
                          const mipp::vector<int>& frozen_bits,
                          const std::vector<Pattern_polar_i*> &patterns,
                          const Pattern_polar_i &pattern_rate0,
                          const Pattern_polar_i &pattern_rate1,
                          ostream &dec_stream,
                          ostream &graph_stream)
: Generator_polar(K,
                  N,
                  snr,
                  frozen_bits,
                  patterns,
                  pattern_rate0,
                  pattern_rate1,
                  "Decoder_polar_SCL_fast_CA_sys",
                  "DECODER_POLAR_SCL_FAST_SYS_CA",
                  dec_stream,
                  dec_stream,
                  graph_stream,
                  graph_stream,
                  false)
{
}

Generator_polar_SCL_sys
::~Generator_polar_SCL_sys()
{
}

void Generator_polar_SCL_sys
::generate_class_header(const std::string   class_name,
                        const std::string   fbits_name,
                              std::ostream &stream1,
                              std::ostream &stream2)
{
	stream1 << "template <typename B, typename R, class API_polar>"                                       << endl;
	stream1 << "class " << class_name << " : public " << this->mother_class_name << "<B, R, API_polar>"   << endl;
	stream1 << "{"                                                                                        << endl;
	stream1 << "public:"                                                                                  << endl;
	stream1 << tab << class_name << "(const int& K, const int& N, const int& L,"
	               <<               " const mipp::vector<B>& frozen_bits, CRC<B>& crc,"
	               <<               " const int n_frames = 1)"                                            << endl;
	stream1 << tab << ": " << this->mother_class_name << "<B, R, API_polar>(K, N, L, frozen_bits, crc, "
	               <<                                                      "n_frames)"                    << endl;
	stream1 << tab << "{"                                                                                 << endl;
	stream1 << tab << tab << "assert(N == " << N << ");"                                                  << endl;
	stream1 << tab << tab << "assert(K == " << K << ");"                                                  << endl;
	stream1 << tab << tab                                                                                 << endl;
	stream1 << tab << tab << "auto i = 0;"                                                                << endl;
	stream1 << tab << tab << "while (i < " << N << " && " << fbits_name << "[i] == frozen_bits[i]) i++;"  << endl;
	stream1 << tab << tab << "assert(i == " << N << ");"                                                  << endl;
	stream1 << tab << "}"                                                                                 << endl;
	stream1                                                                                               << endl;
	stream1 << tab << "virtual ~" << class_name << "()"                                                   << endl;
	stream1 << tab << "{"                                                                                 << endl;
	stream1 << tab << "}"                                                                                 << endl;
	stream1                                                                                               << endl;

	stream2 << tab << "void hard_decode()"                                                                << endl;
	stream2 << tab << "{"                                                                                 << endl;
	stream2 << tab << tab << "using namespace tools;"                                                     << endl;
	stream2                                                                                               << endl;
	stream2 << tab << tab << "auto &y = this->Y_N;"                                                       << endl;
	stream2 << tab << tab << "auto &l = this->l;"                                                         << endl;
	stream2 << tab << tab << "auto &s = this->s;"                                                         << endl;
	stream2                                                                                               << endl;
}

void Generator_polar_SCL_sys
::generate_class_footer(std::ostream &stream)
{
	stream << tab << tab << "this->select_best_path();" << endl;
	stream << tab << "}"                                << endl;
	stream << "};" << ""                                << endl;
}

void Generator_polar_SCL_sys
::recursive_generate_decoder(const Binary_node<Pattern_polar_i>* node_curr, ostream &stream)
{
	n_nodes_before_compression++;

	if (!node_curr->is_leaf()) // stop condition
	{
		if (!node_curr->get_c()->apply_f().empty())
			stream << node_curr->get_c()->apply_f(tab + tab);

		this->recursive_generate_decoder(node_curr->get_left(), stream); // recursive call

		if (!node_curr->get_c()->apply_g().empty())
			stream << node_curr->get_c()->apply_g(tab + tab);

		this->recursive_generate_decoder(node_curr->get_right(), stream); // recursive call
	}

	if (!node_curr->get_c()->apply_h().empty())
		stream << node_curr->get_c()->apply_h(tab + tab);
}

void Generator_polar_SCL_sys
::recursive_generate_short_decoder(const Binary_node<Pattern_polar_i>* node_curr, ostream &stream)
{
	// TODO
}
