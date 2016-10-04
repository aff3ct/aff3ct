#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

#include "Generator_polar_SC_sys.hpp"

Generator_polar_SC_sys
::Generator_polar_SC_sys(const int& K,
                         const int& N,
                         const float& snr,
                         const mipp::vector<int>& frozen_bits,
                         const std::vector<Pattern_SC_interface*> &patterns,
                         const Pattern_SC_interface &pattern_rate0,
                         const Pattern_SC_interface &pattern_rate1,
                         ostream &dec_stream,
                         ostream &short_dec_stream,
                         ostream &graph_stream,
                         ostream &short_graph_stream)
: K(K),
  N(N),
  m((int)log2(N)),
  snr(snr),
  frozen_bits(frozen_bits),
  patterns(patterns),
  pattern_rate0(pattern_rate0),
  pattern_rate1(pattern_rate1),
  parser(N, frozen_bits, patterns, pattern_rate0, pattern_rate1),
  dec_stream(dec_stream),
  short_dec_stream(short_dec_stream),
  graph_stream(graph_stream),
  short_graph_stream(short_graph_stream),
  tab("\t"),
  inlining_level(3),
  stats(m +1),
  n_nodes_before_compression(0),
  n_nodes_after_compression(0)
{
	for (unsigned i = 0; i < stats.size(); i++)
		stats[i].resize(patterns.size());
}

Generator_polar_SC_sys
::~Generator_polar_SC_sys()
{
}

void Generator_polar_SC_sys
::generate()
{
	n_nodes_before_compression = 0;
	n_nodes_after_compression = 0;
	for (unsigned i = 0; i < stats.size(); i++)
		std::fill(stats[i].begin(), stats[i].end(), 0);

	string class_name = "Decoder_polar_SC_fast_sys_N"   + to_string(N) +
	                                             "_K"   + to_string(K) +
	                                           "_SNR" + to_string((int)(snr*10));

	string pragma_name = "DECODER_POLAR_SC_FAST_SYS_N"   + to_string(N) +
	                                              "_K"   + to_string(K) +
	                                            "_SNR" + to_string((int)(snr*10)) + "_HPP_";

	const auto n_lines = (int)ceil((float)frozen_bits.size() / 32.f);

	string fbits_name = "Frozen_bits_" + to_string(N) +
	                               "_" + to_string(K) +
	                               "_" + to_string((int)(snr*10));

	stringstream fbits;
	auto i = 0;
	for (auto j = 0; j < n_lines; j++)
	{
		auto k = 0;
		while (k < 32 && i < (int)frozen_bits.size())
		{
			fbits << frozen_bits[i];
			if (i < (int)frozen_bits.size() -1) fbits << ", ";
			i++; k++;
		}
		if (i < (int)frozen_bits.size()) fbits << endl;
	}

	// decoder generation
	stringstream dec_common1, dec_common2, dec_common3, dec, short_dec1, short_dec2;
	dec_common1 << "#ifndef " << pragma_name                                                                  << endl;
	dec_common1 << "#define " << pragma_name                                                                  << endl;
	dec_common1                                                                                               << endl;
	dec_common1 << "#include <cassert>"                                                                       << endl;
	dec_common1                                                                                               << endl;
	dec_common1 << "#include \"../Decoder_polar_SC_fast_sys.hpp\""                                            << endl;
	dec_common1                                                                                               << endl;
	dec_common1 << "static const char " << fbits_name << "[" << N << "] = {"                                  << endl;
	dec_common1 << fbits.str() << "};"                                                                        << endl;
	dec_common1                                                                                               << endl;
	dec_common1 << "template <typename B, typename R, class API_polar>"                                       << endl;
	dec_common1 << "class " << class_name << " : public Decoder_polar_SC_fast_sys<B, R, API_polar>"           << endl;
	dec_common1 << "{"                                                                                        << endl;
	dec_common1 << "public:"                                                                                  << endl;
	dec_common1 << tab << class_name << "(const int& K, const int& N, const mipp::vector<B>& frozen_bits)"    << endl;
	dec_common1 << tab << ": Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)"                   << endl;
	dec_common1 << tab << "{"                                                                                 << endl;
	dec_common1 << tab << tab << "assert(N == " << N << ");"                                                  << endl;
	dec_common1 << tab << tab << "assert(K == " << K << ");"                                                  << endl;
	dec_common1 << tab << tab                                                                                 << endl;
	dec_common1 << tab << tab << "auto i = 0;"                                                                << endl;
	dec_common1 << tab << tab << "while (i < " << N << " && " << fbits_name << "[i] == frozen_bits[i]) i++;"  << endl;
	dec_common1 << tab << tab << "assert(i == " << N << ");"                                                  << endl;
	dec_common1 << tab << "}"                                                                                 << endl;
	dec_common1                                                                                               << endl;
	dec_common1 << tab << "virtual ~" << class_name << "()"                                                   << endl;
	dec_common1 << tab << "{"                                                                                 << endl;
	dec_common1 << tab << "}"                                                                                 << endl;
	dec_common1                                                                                               << endl;
	dec_common2 << tab << "void decode()"                                                                     << endl;
	dec_common2 << tab << "{"                                                                                 << endl;
	dec_common2 << tab << tab << "auto &l = this->l;"                                                         << endl;
	dec_common2 << tab << tab << "auto &s = this->s;"                                                         << endl;
	dec_common2                                                                                               << endl;
	this->recursive_generate_decoder(parser.get_polar_tree()->get_root(), dec);
	dec_common3 << tab << "}"                                                                                 << endl;
	dec_common3 << "};" << ""                                                                                 << endl;
	dec_common3 << "#endif"                                                                                   << endl;

	dec_stream << dec_common1.str();
	dec_stream << dec_common2.str();
	dec_stream << dec.str();
	dec_stream << dec_common3.str();

	// graph generation
	stringstream graph, short_graph;
	this->recursive_generate_graph(parser.get_polar_tree()->get_root(), graph);
	this->subtree_occurences_cpy = this->subtree_occurences;
	this->recursive_generate_short_graph(parser.get_polar_tree()->get_root(), short_graph);
	stringstream graph_common1;
	graph_common1 << "digraph " << class_name << "{" << endl;
	graph_common1 << tab << "subgraph cluster_0 {" << endl;
	graph_common1 << tab << tab << "label=\"Legend\";" << endl;
	graph_common1 << tab << tab << "rankdir=LR;" << endl;
	graph_common1 << tab << tab << "rank=same;" << endl;
	for (unsigned p = 0; p < patterns.size(); p++)
	{
		graph_common1 << tab << tab;
		graph_common1 << "\"" << patterns[p]->name() << "\"["
		                      << "style=filled, "
		                      << "fillcolor=\"" << patterns[p]->fill_color() << "\","
		                      << "fontcolor=\"" << patterns[p]->font_color() << "\""
		                      << "];" << endl;
	}
	for (unsigned p = 0; p < patterns.size() -1; p++)
	{
		graph_common1 << tab << tab;
		graph_common1 << "\"" << patterns[p]->name() << "\" -> " << "\"" << patterns[p +1]->name() << "\";" << endl;
	}

	float compression_rate = (float)n_nodes_before_compression / (float)n_nodes_after_compression;
	graph_common1 << tab << "}" << endl;
	graph_common1 << tab << "subgraph cluster_1 {" << endl;
	graph_common1 << tab << tab
	              << "label=\"Decoder SC systematic - "
	              << "N="                                        << N                     << ", "
	              << "K="                                        << K                     << ", "
	              << "R="                << std::setprecision(3) << ((float)K / (float)N) << ", "
	              << "SNR="              << std::setprecision(2) << snr                   << " - "
	              << "Compression rate=" << std::setprecision(3) << compression_rate      << "\";" << endl;

	stringstream graph_common2;
	graph_common2 << tab << "}" << endl;
	graph_common2 << "}" << endl;

	graph_stream << graph_common1.str();
	graph_stream << graph.str();
	graph_stream << graph_common2.str();

	short_graph_stream << graph_common1.str();
	short_graph_stream << short_graph.str();
	short_graph_stream << graph_common2.str();

	// short decoder generation
	short_dec_stream << dec_common1.str();
	this->subtree_occurences_cpy = this->subtree_occurences;
	this->recursive_generate_short_decoder_funcs(parser.get_polar_tree()->get_root(), short_dec1);
	short_dec_stream << short_dec1.str();
	short_dec_stream << dec_common2.str();
	this->subtree_occurences_cpy = this->subtree_occurences;
	this->recursive_generate_short_decoder(parser.get_polar_tree()->get_root(), short_dec2);
	short_dec_stream << short_dec2.str();
	short_dec_stream << dec_common3.str();
}

void Generator_polar_SC_sys
::recursive_generate_decoder(const Binary_node<Pattern_SC_interface>* node_curr, ostream &stream)
{
	n_nodes_before_compression++;

	if (!node_curr->is_leaf()) // stop condition
	{
		if (!node_curr->get_c()->apply_f().empty())
			stream << tab << tab << node_curr->get_c()->apply_f();

		this->recursive_generate_decoder(node_curr->get_left(), stream); // recursive call

		if (!node_curr->get_c()->apply_g().empty())
			stream << tab << tab << node_curr->get_c()->apply_g();

		this->recursive_generate_decoder(node_curr->get_right(), stream); // recursive call
	}

	if (!node_curr->get_c()->apply_h().empty())
		stream << tab << tab << node_curr->get_c()->apply_h();
}

void Generator_polar_SC_sys
::recursive_generate_graph(const Binary_node<Pattern_SC_interface>* node_curr, ostream &stream)
{
	string key = "";

	// generate graphviz tree
	stream << tab << tab;
	stream << "N" << node_curr->get_c()->get_id() << "["
	       << "style=filled,"
	       << "fillcolor=\"" << node_curr->get_c()->fill_color() << "\","
	       << "fontcolor=\"" << node_curr->get_c()->font_color() << "\""
	       << "];" << endl;

	if (!node_curr->is_leaf()) // stop condition
	{
		stream << tab << tab;
		stream << "N" << node_curr->get_c()->get_id() << " -> N" << node_curr->get_left()->get_c()->get_id()
		       << "[label=\"" << node_curr->get_c()->f() << ((!node_curr->get_c()->f().empty()) ? "()" : "")
		       << "\"];" << endl;

		this->recursive_generate_graph(node_curr->get_left(), stream); // recursive call
		key += node_curr->get_left()->get_c()->get_key();

		stream << tab << tab;
		stream << "N" << node_curr->get_c()->get_id() << " -> N" << node_curr->get_right()->get_c()->get_id()
		       << "[label=\"" << node_curr->get_c()->g() << ((!node_curr->get_c()->g().empty()) ? "()" : "")
		       << "\"];" << endl;

		this->recursive_generate_graph(node_curr->get_right(), stream); // recursive call
		key += node_curr->get_right()->get_c()->get_key();
	}
	else
	{
		key += node_curr->get_c()->short_name() + to_string(node_curr->get_depth());
	}

	if (!node_curr->get_c()->h().empty())
	{
		stream << tab << tab;
		stream << "N" << node_curr->get_c()->get_id() << " -> N" << node_curr->get_c()->get_id()
		       << "[label=\"" << node_curr->get_c()->h() << "()\"];" << endl;
	}

	// statistics
	int pattern_id = 0;
	Pattern_SC_interface* pattern = node_curr->get_c();
	for (unsigned i = 0; i < patterns.size(); i++)
	{
		auto cur_pattern = patterns[i];
		if (typeid(*cur_pattern) == typeid(*pattern))
		{
			pattern_id = i;
			break;
		}
	}
	stats[node_curr->get_depth()][pattern_id]++;

	node_curr->get_c()->set_key(key);
	if (subtree_occurences.find(key) != subtree_occurences.end())
		subtree_occurences[key]++;
	else
		subtree_occurences[key] = 1;
}

void Generator_polar_SC_sys
::recursive_generate_short_graph(const Binary_node<Pattern_SC_interface>* node_curr, ostream &stream)
{
	if (subtree_occurences_cpy[node_curr->get_c()->get_key()])
	{
		n_nodes_after_compression++;
		subtree_occurences_cpy[node_curr->get_c()->get_key()] = 0;

		// generate graphviz tree
		stream << tab << tab;
		stream << "N" << node_curr->get_c()->get_id() << "["
		       << "style=filled,"
		       << "fillcolor=\"" << node_curr->get_c()->fill_color() << "\","
		       << "fontcolor=\"" << node_curr->get_c()->font_color() << "\""
		       << "];" << endl;

		if (!node_curr->is_leaf()) // stop condition
		{
			auto occurences = subtree_occurences_cpy[node_curr->get_left()->get_c()->get_key()];
			if (occurences)
			{
				stream << tab << tab;
				stream << "N" << node_curr->get_c()->get_id() << " -> "
				       << "N" << node_curr->get_left()->get_c()->get_id()
				       << "[label=\"" << occurences << "\"];" << endl;

				this->recursive_generate_short_graph(node_curr->get_left(), stream); // recursive call
			}

			occurences = subtree_occurences_cpy[node_curr->get_right()->get_c()->get_key()];
			if (occurences)
			{
				stream << tab << tab;
				stream << "N" << node_curr->get_c()->get_id() << " -> "
				       << "N" << node_curr->get_right()->get_c()->get_id()
				       << "[label=\"" << occurences << "\"];" << endl;

				this->recursive_generate_short_graph(node_curr->get_right(), stream); // recursive call
			}
		}
	}
}

void Generator_polar_SC_sys
::recursive_generate_short_decoder_funcs(const Binary_node<Pattern_SC_interface>* node_curr, ostream &stream)
{
	if (!node_curr->is_leaf()) // stop condition
	{
		this->recursive_generate_short_decoder_funcs(node_curr->get_left(),  stream); // recursive call
		this->recursive_generate_short_decoder_funcs(node_curr->get_right(), stream); // recursive call
	}

	if (subtree_occurences_cpy[node_curr->get_c()->get_key()] > 1)
	{
		stream << tab << "// depth = " << node_curr->get_depth() << ", "
		              << "reverse depth = " << (this->m - node_curr->get_depth()) << ", "
		              << "size = " << node_curr->get_c()->get_size() << ", "
		              << "calls = " << subtree_occurences_cpy[node_curr->get_c()->get_key()] << endl;
		if ((this->m - node_curr->get_depth()) <= this->inlining_level)
			stream << tab << "__attribute__((always_inline))" << endl;
		stream << tab << "inline void " << node_curr->get_c()->get_key() << "(const int off_l, "
		                                                                 << "const int off_s)" << endl;
		stream << tab << "{" << endl;

		subtree_occurences_cpy[node_curr->get_c()->get_key()] = 0;

		if (node_curr->is_leaf())
		{
			std::string h = node_curr->get_c()->apply_h("off_l", "off_s");
			if (!h.empty())
			{
				stream << tab << tab << "auto &l = this->l;" << endl;
				stream << tab << tab << "auto &s = this->s;" << endl;
				stream                                      << endl;
				stream << tab << tab << h;
			}
		}
		else
		{
			stream << tab << tab << "auto &l = this->l;" << endl;
			stream << tab << tab << "auto &s = this->s;" << endl;
			stream                                      << endl;

			if (!node_curr->get_c()->apply_f("off_l", "off_s").empty())
				stream << tab << tab << node_curr->get_c()->apply_f("off_l", "off_s");
			if(node_curr->get_left() != nullptr)
			{
				stream << tab << tab;
				stream << node_curr->get_left()->get_c()->get_key() << "("
				       << "off_l" << "+" << node_curr->get_c()->get_size() << ", "
				       << "off_s" << "+" << 0 << ");" << endl;
			}

			if (!node_curr->get_c()->apply_g("off_l", "off_s").empty())
				stream << tab << tab << node_curr->get_c()->apply_g("off_l", "off_s");
			if(node_curr->get_right() != nullptr)
			{
				stream << tab << tab;
				stream << node_curr->get_right()->get_c()->get_key() << "("
				       << "off_l" << "+" << node_curr->get_c()->get_size() << ", "
				       << "off_s" << "+" << node_curr->get_c()->get_si_2() << ");" << endl;
			}

			if (!node_curr->get_c()->apply_h("off_l", "off_s").empty())
				stream << tab << tab << node_curr->get_c()->apply_h("off_l", "off_s");
		}
		stream << tab << "}" << endl << endl;
	}
}

void Generator_polar_SC_sys
::recursive_generate_short_decoder(const Binary_node<Pattern_SC_interface>* node_curr, ostream &stream)
{
	if (subtree_occurences_cpy[node_curr->get_c()->get_key()] == 1)
	{
		if (!node_curr->is_leaf()) // stop condition
		{
			if (!node_curr->get_c()->apply_f().empty())
				stream << tab << tab << node_curr->get_c()->apply_f();
			this->recursive_generate_short_decoder(node_curr->get_left(), stream); // recursive call
			if (!node_curr->get_c()->apply_g().empty())
				stream << tab << tab << node_curr->get_c()->apply_g();
			this->recursive_generate_short_decoder(node_curr->get_right(), stream); // recursive call
		}
		if (!node_curr->get_c()->apply_h().empty())
			stream << tab << tab << node_curr->get_c()->apply_h();
	}
	else
	{
		stream << tab << tab;
		stream << node_curr->get_c()->get_key()
		       << "(" << node_curr->get_c()->get_off_l() << ", " << node_curr->get_c()->get_off_s() << ");" << endl;
	}
}

unsigned long Generator_polar_SC_sys
::get_n_generated_nodes(int graph_depth) const
{
	assert(graph_depth < m +1);

	unsigned long sum_nodes = 0;

	if (graph_depth < 0)
		for (auto d = 0; d < m +1; d++)
			for (unsigned p = 0; p < patterns.size(); p++)
				sum_nodes += stats[d][p];
	else
		for (unsigned p = 0; p < patterns.size(); p++)
			sum_nodes += stats[graph_depth][p];

	return sum_nodes;
}

unsigned long Generator_polar_SC_sys
::get_n_generated_nodes_by_pattern(std::size_t pattern_hash, int graph_depth) const
{
	assert(graph_depth < m +1);

	unsigned long sum_nodes = 0;

	if (graph_depth < 0)
		for (auto d = 0; d < m +1; d++)
			for (unsigned p = 0; p < patterns.size(); p++)
			{
				auto cur_pattern = patterns[p];
				if (typeid(*cur_pattern).hash_code() == pattern_hash)
				{
					sum_nodes += stats[d][p];
					break;
				}
			}
	else
		for (unsigned p = 0; p < patterns.size(); p++)
		{
			auto cur_pattern = patterns[p];
			if (typeid(*cur_pattern).hash_code() == pattern_hash)
			{
				sum_nodes += stats[graph_depth][p];
				break;
			}
		}

	return sum_nodes;
}
