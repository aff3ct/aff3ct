#ifndef GENERATOR_POLAR_SC_SYS_
#define GENERATOR_POLAR_SC_SYS_

#include <map>
#include <vector>

#include "Tools/MIPP/mipp.h"
#include "Tools/Tree/Binary_tree.hpp"
#include "Tools/Polar/Pattern_parser_polar.hpp"

#include "Module/Decoder/Polar/SC/Patterns/Pattern_SC_interface.hpp"

#include "../../Generator.hpp"

class Generator_polar_SC_sys : public Generator
{
protected:
	const int K; // k bits input
	const int N; // n bits input
	const int m; // graph depth
	const float snr;

	const mipp::vector<int>& frozen_bits;

	const std::vector<Pattern_SC_interface*> &patterns;
	const Pattern_SC_interface &pattern_rate0;
	const Pattern_SC_interface &pattern_rate1;

	Pattern_parser_polar parser;

	std::ostream &dec_stream;
	std::ostream &short_dec_stream;
	std::ostream &graph_stream;
	std::ostream &short_graph_stream;

private:
	std::string tab;
	const int inlining_level;
	std::vector<std::vector<int>> stats;
	std::map<std::string, int> subtree_occurences;
	std::map<std::string, int> subtree_occurences_cpy;
	unsigned n_nodes_before_compression;
	unsigned n_nodes_after_compression;

public:
	Generator_polar_SC_sys(const int& K,
	                       const int& N,
	                       const float& snr,
	                       const mipp::vector<int>& frozen_bits,
	                       const std::vector<Pattern_SC_interface*> &patterns,
	                       const Pattern_SC_interface &pattern_rate0,
	                       const Pattern_SC_interface &pattern_rate1,
	                       std::ostream &dec_stream = std::cout,
	                       std::ostream &short_dec_stream = std::cout,
	                       std::ostream &graph_stream = std::cout,
	                       std::ostream &short_graph_stream = std::cout);
	virtual ~Generator_polar_SC_sys();

	void generate();

	unsigned long get_n_generated_nodes           (                          int graph_depth = -1) const;
	unsigned long get_n_generated_nodes_by_pattern(std::size_t pattern_hash, int graph_depth = -1) const;

private:
	void recursive_generate_decoder            (const Binary_node<Pattern_SC_interface>* node_curr, std::ostream &stream);
	void recursive_generate_short_decoder_funcs(const Binary_node<Pattern_SC_interface>* node_curr, std::ostream &stream);
	void recursive_generate_short_decoder      (const Binary_node<Pattern_SC_interface>* node_curr, std::ostream &stream);
	void recursive_generate_graph              (const Binary_node<Pattern_SC_interface>* node_curr, std::ostream &stream);
	void recursive_generate_short_graph        (const Binary_node<Pattern_SC_interface>* node_curr, std::ostream &stream);
};

#endif /* GENERATOR_POLAR_SC_SYS_ */
