#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

#include "Generator_polar.hpp"

using namespace aff3ct::generator;
using namespace aff3ct::tools;

Generator_polar
::Generator_polar(const int& K,
                  const int& N,
                  const float& snr,
                  const mipp::vector<int>& frozen_bits,
                  const std::vector<Pattern_polar_i*> &patterns,
                  const Pattern_polar_i &pattern_rate0,
                  const Pattern_polar_i &pattern_rate1,
                  string mother_class_name,
                  string MOTHER_CLASS_NAME,
                  ostream &dec_stream,
                  ostream &short_dec_stream,
                  ostream &graph_stream,
                  ostream &short_graph_stream,
                  const bool enable_short_decoder)
: K                         (K                                                       ),
  N                         (N                                                       ),
  m                         ((int)log2(N)                                            ),
  snr                       (snr                                                     ),
  frozen_bits               (frozen_bits                                             ),
  patterns                  (patterns                                                ),
  pattern_rate0             (pattern_rate0                                           ),
  pattern_rate1             (pattern_rate1                                           ),
  parser                    (N, frozen_bits, patterns, &pattern_rate0, &pattern_rate1),
  mother_class_name         (mother_class_name                                       ),
  MOTHER_CLASS_NAME         (MOTHER_CLASS_NAME                                       ),
  dec_stream                (dec_stream                                              ),
  short_dec_stream          (short_dec_stream                                        ),
  graph_stream              (graph_stream                                            ),
  short_graph_stream        (short_graph_stream                                      ),
  tab                       ("\t"                                                    ),
  inlining_level            (3                                                       ),
  stats                     (m +1                                                    ),
  n_nodes_before_compression(0                                                       ),
  n_nodes_after_compression (0                                                       ),
  enable_short_decoder      (enable_short_decoder                                    )
{
	for (unsigned i = 0; i < stats.size(); i++)
		stats[i].resize(patterns.size());
}

Generator_polar
::~Generator_polar()
{
}

std::string  Generator_polar
::get_class_name()
{
	return mother_class_name + "_N" + to_string(N) +
	                           "_K" + to_string(K) +
	                         "_SNR" + to_string((int)(snr*10));
}

void Generator_polar
::generate()
{
	n_nodes_before_compression = 0;
	n_nodes_after_compression = 0;
	for (unsigned i = 0; i < stats.size(); i++)
		std::fill(stats[i].begin(), stats[i].end(), 0);

	string class_name = get_class_name();
	string pragma_name = MOTHER_CLASS_NAME + "_N"   + to_string(N) +
	                                         "_K"   + to_string(K) +
	                                       "_SNR" + to_string((int)(snr*10)) + "_HPP_";

	const auto n_lines = (int)ceil((float)frozen_bits.size() / 32.f);

	string fbits_name = mother_class_name + "_fb_" + to_string(N) +
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
	dec_common1 << "#include \"../" << mother_class_name << ".hpp\""                                          << endl;
	dec_common1                                                                                               << endl;
	dec_common1 << "namespace aff3ct"                                                                         << endl;
	dec_common1 << "{"                                                                                        << endl;
	dec_common1 << "namespace module"                                                                         << endl;
	dec_common1 << "{"                                                                                        << endl;
	dec_common1 << "static const char " << fbits_name << "[" << N << "] = {"                                  << endl;
	dec_common1 << fbits.str() << "};"                                                                        << endl;
	dec_common1                                                                                               << endl;

	this->generate_class_header     (class_name, fbits_name, dec_common1, dec_common2);
	this->recursive_generate_decoder(parser.get_polar_tree()->get_root(), dec        );
	this->generate_class_footer     (dec_common3                                     );

	dec_common3 << "}"      << endl;
	dec_common3 << "}"      << endl;
	dec_common3 << "#endif" << endl;

	dec_stream << dec_common1.str();
	dec_stream << dec_common2.str();
	dec_stream << dec.str();
	dec_stream << dec_common3.str();

	// graph generation
	stringstream graph, short_graph;
	this->recursive_generate_graph(parser.get_polar_tree()->get_root(), graph);
	this->subtree_occurences_cpy = this->subtree_occurences;

	if (enable_short_decoder)
		this->recursive_generate_short_graph(parser.get_polar_tree()->get_root(), short_graph);

	stringstream graph_common1;
	graph_common1 << "digraph " << class_name << "{" << endl;
	graph_common1 << tab << "subgraph cluster_0 {" << endl;
	graph_common1 << tab << tab << "label=\"Legend\";" << endl;
	graph_common1 << tab << tab << "rankdir=LR;" << endl;
	graph_common1 << tab << tab << "rank=same;" << endl;
	for (unsigned p = 0; p < patterns.size(); p++)
	{
		std::string rng = patterns[p]->range().empty() ? "" : std::string("\n") + patterns[p]->range();
		graph_common1 << tab << tab;
		graph_common1 << "\"" << patterns[p]->name() << rng << "\"["
		                      << "style=filled, "
		                      << "fillcolor=\"" << patterns[p]->fill_color() << "\","
		                      << "fontcolor=\"" << patterns[p]->font_color() << "\""
		                      << "];" << endl;
	}
	for (unsigned p = 0; p < patterns.size() -1; p++)
	{
		std::string rng0 = patterns[p   ]->range().empty() ? "" : std::string("\n") + patterns[p   ]->range();
		std::string rng1 = patterns[p +1]->range().empty() ? "" : std::string("\n") + patterns[p +1]->range();
		graph_common1 << tab << tab;
		graph_common1 << "\"" << patterns[p   ]->name() << rng0 << "\" -> " << "\""
		                      << patterns[p +1]->name() << rng1 << "\";" << endl;
	}

	float compression_rate = 1.f;
	if (n_nodes_after_compression != 0)
		compression_rate = (float)n_nodes_before_compression / (float)n_nodes_after_compression;
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

	if (enable_short_decoder)
	{
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
}

void Generator_polar
::recursive_generate_graph(const Binary_node<Pattern_polar_i>* n_c, ostream &stream)
{
	string key = "";

	string ne_c = string("\n(") + to_string(n_c->get_c()->get_size()) + string(")");

	// generate graphviz tree
	stream << tab << tab;
	stream << "\"N" << n_c->get_c()->get_id() << ne_c << "\"["
	       << "style=filled,"
	       << "fillcolor=\"" << n_c->get_c()->fill_color() << "\","
	       << "fontcolor=\"" << n_c->get_c()->font_color() << "\""
	       << "];" << endl;

	if (!n_c->is_leaf()) // stop condition
	{
		string ne_l = string("\n(") + to_string(n_c->get_left ()->get_c()->get_size()) + string(")");
		string ne_r = string("\n(") + to_string(n_c->get_right()->get_c()->get_size()) + string(")");

		stream << tab << tab;
		stream << "\"N" << n_c            ->get_c()->get_id() << ne_c << "\" -> "
		       << "\"N" << n_c->get_left()->get_c()->get_id() << ne_l << "\""
		       << "[label=\"" << n_c->get_c()->f() << ((!n_c->get_c()->f().empty()) ? "()" : "")
		       << "\"];" << endl;

		this->recursive_generate_graph(n_c->get_left(), stream); // recursive call
		key += n_c->get_left()->get_c()->get_key();

		stream << tab << tab;
		stream << "\"N" << n_c             ->get_c()->get_id() << ne_c << "\" -> "
		       << "\"N" << n_c->get_right()->get_c()->get_id() << ne_r << "\""
		       << "[label=\"" << n_c->get_c()->g() << ((!n_c->get_c()->g().empty()) ? "()" : "")
		       << "\"];" << endl;

		this->recursive_generate_graph(n_c->get_right(), stream); // recursive call
		key += n_c->get_right()->get_c()->get_key();
	}
	else
	{
		key += n_c->get_c()->short_name() + to_string(n_c->get_depth());
	}

	if (!n_c->get_c()->h().empty())
	{
		stream << tab << tab;
		stream << "\"N" << n_c->get_c()->get_id() << ne_c << "\" -> "
		       << "\"N" << n_c->get_c()->get_id() << ne_c << "\""
		       << "[label=\"" << n_c->get_c()->h() << "()\"];" << endl;
	}

	// statistics
	int pattern_id = 0;
	Pattern_polar_i* pattern = n_c->get_c();
	for (unsigned i = 0; i < patterns.size(); i++)
	{
		auto cur_pattern = patterns[i];
		if (typeid(*cur_pattern) == typeid(*pattern))
		{
			pattern_id = i;
			break;
		}
	}
	stats[n_c->get_depth()][pattern_id]++;

	n_c->get_c()->set_key(key);
	if (subtree_occurences.find(key) != subtree_occurences.end())
		subtree_occurences[key]++;
	else
		subtree_occurences[key] = 1;
}

void Generator_polar
::recursive_generate_short_graph(const Binary_node<Pattern_polar_i>* n_c, ostream &stream)
{
	if (subtree_occurences_cpy[n_c->get_c()->get_key()])
	{
		string ne_c = string("\n(") + to_string(n_c->get_c()->get_size()) + string(")");

		n_nodes_after_compression++;
		subtree_occurences_cpy[n_c->get_c()->get_key()] = 0;
		subtree_nodes         [n_c->get_c()->get_key()] = string("\"N") + to_string(n_c->get_c()->get_id()) + ne_c +
		                                                  string("\"");

		// generate graphviz tree
		stream << tab << tab;
		stream << "\"N" << n_c->get_c()->get_id() << ne_c << "\"["
		       << "style=filled,"
		       << "fillcolor=\"" << n_c->get_c()->fill_color() << "\","
		       << "fontcolor=\"" << n_c->get_c()->font_color() << "\""
		       << "];" << endl;

		if (!n_c->is_leaf()) // stop condition
		{
			string ne_l = string("\n(") + to_string(n_c->get_left ()->get_c()->get_size()) + string(")");
			string ne_r = string("\n(") + to_string(n_c->get_right()->get_c()->get_size()) + string(")");

			auto occurences = subtree_occurences_cpy[n_c->get_left()->get_c()->get_key()];
			if (occurences)
			{
				stream << tab << tab;
				stream << "\"N" << n_c            ->get_c()->get_id() << ne_c << "\" -> "
				       << "\"N" << n_c->get_left()->get_c()->get_id() << ne_l << "\""
//				       << "[label=\"" << occurences << "\"];"
				       << endl;

				this->recursive_generate_short_graph(n_c->get_left(), stream); // recursive call
			}
			else
			{
				auto name = subtree_nodes[n_c->get_left()->get_c()->get_key()];
				stream << tab << tab;
				stream << "\"N" << n_c->get_c()->get_id() << ne_c << "\" -> " << name << endl;
			}

			occurences = subtree_occurences_cpy[n_c->get_right()->get_c()->get_key()];
			if (occurences)
			{
				stream << tab << tab;
				stream << "\"N" << n_c             ->get_c()->get_id() << ne_c << "\" -> "
				       << "\"N" << n_c->get_right()->get_c()->get_id() << ne_r << "\""
//				       << "[label=\"" << occurences << "\"];"
				       << endl;

				this->recursive_generate_short_graph(n_c->get_right(), stream); // recursive call
			}
			else
			{
				auto name = subtree_nodes[n_c->get_right()->get_c()->get_key()];
				stream << tab << tab;
				stream << "\"N" << n_c->get_c()->get_id() << ne_c << "\" -> " << name << endl;
			}
		}
	}
}

void Generator_polar
::recursive_generate_short_decoder_funcs(const Binary_node<Pattern_polar_i>* node_curr, ostream &stream)
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
			std::string h = node_curr->get_c()->apply_h("", "off_l", "off_s");
			if (!h.empty())
			{
				stream << tab << tab << "using namespace tools;" << endl;
				stream                                           << endl;
				stream << tab << tab << "auto &l = this->l;"     << endl;
				stream << tab << tab << "auto &s = this->s;"     << endl;
				stream                                           << endl;
				stream << tab << tab << h;
			}
		}
		else
		{
			stream << tab << tab << "using namespace tools;" << endl;
			stream                                           << endl;
			stream << tab << tab << "auto &l = this->l;"     << endl;
			stream << tab << tab << "auto &s = this->s;"     << endl;
			stream                                           << endl;

			if (!node_curr->get_c()->apply_f("", "off_l", "off_s").empty())
				stream << tab << tab << node_curr->get_c()->apply_f("", "off_l", "off_s");
			if(node_curr->get_left() != nullptr)
			{
				stream << tab << tab;
				stream << node_curr->get_left()->get_c()->get_key() << "("
				       << "off_l" << "+" << node_curr->get_c()->get_size() << ", "
				       << "off_s" << "+" << 0 << ");" << endl;
			}

			if (!node_curr->get_c()->apply_g("", "off_l", "off_s").empty())
				stream << tab << tab << node_curr->get_c()->apply_g("", "off_l", "off_s");
			if(node_curr->get_right() != nullptr)
			{
				stream << tab << tab;
				stream << node_curr->get_right()->get_c()->get_key() << "("
				       << "off_l" << "+" << node_curr->get_c()->get_size() << ", "
				       << "off_s" << "+" << node_curr->get_c()->get_si_2() << ");" << endl;
			}

			if (!node_curr->get_c()->apply_h("", "off_l", "off_s").empty())
				stream << tab << tab << node_curr->get_c()->apply_h("", "off_l", "off_s");
		}
		stream << tab << "}" << endl << endl;
	}
}

unsigned long Generator_polar
::get_n_generated_nodes(int graph_depth) const
{
	if (graph_depth >= m +1)
		throw std::runtime_error("aff3ct::generator::Generator_polar: \"graph_depth\" has to be smaller than "
		                         "\"m\" +1.");

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

unsigned long Generator_polar
::get_n_generated_nodes_by_pattern(std::size_t pattern_hash, int graph_depth) const
{
	if (graph_depth >= m +1)
		throw std::runtime_error("aff3ct::generator::Generator_polar: \"graph_depth\" has to be smaller than "
		                         "\"m\" +1.");

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
