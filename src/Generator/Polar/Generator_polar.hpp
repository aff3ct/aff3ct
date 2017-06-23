#ifndef GENERATOR_POLAR_SYS_
#define GENERATOR_POLAR_SYS_

#include <map>
#include <vector>
#include <mipp.h>

#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"
#include "Tools/Code/Polar/Pattern_polar_parser.hpp"

#include "../Generator.hpp"

namespace aff3ct
{
namespace generator
{
class Generator_polar : public Generator
{
protected:
	const int K; // k bits input
	const int N; // n bits input
	const int m; // graph depth
	const float snr;

	const mipp::vector<int>& frozen_bits;

	const std::vector<tools::Pattern_polar_i*> &patterns;
	const tools::Pattern_polar_i &pattern_rate0;
	const tools::Pattern_polar_i &pattern_rate1;

	tools::Pattern_polar_parser<int> parser;

	std::string mother_class_name;
	std::string MOTHER_CLASS_NAME;

	std::ostream &dec_stream;
	std::ostream &short_dec_stream;
	std::ostream &graph_stream;
	std::ostream &short_graph_stream;

	std::string tab;
	const int inlining_level;
	std::vector<std::vector<int>> stats;
	std::map<std::string, int> subtree_occurences;
	std::map<std::string, int> subtree_occurences_cpy;
	std::map<std::string, std::string> subtree_nodes;
	unsigned n_nodes_before_compression;
	unsigned n_nodes_after_compression;

	const bool enable_short_decoder;

public:
	Generator_polar(const int& K,
	                const int& N,
	                const float& snr,
	                const mipp::vector<int>& frozen_bits,
	                const std::vector<tools::Pattern_polar_i*> &patterns,
	                const tools::Pattern_polar_i &pattern_rate0,
	                const tools::Pattern_polar_i &pattern_rate1,
	                std::string mother_class_name,
	                std::string MOTHER_CLASS_NAME,
	                std::ostream &dec_stream         = std::cout,
	                std::ostream &short_dec_stream   = std::cout,
	                std::ostream &graph_stream       = std::cout,
	                std::ostream &short_graph_stream = std::cout,
	                const bool enable_short_decoder  = true);
	virtual ~Generator_polar();

	void generate();

	std::string get_class_name();

	unsigned long get_n_generated_nodes           (                          int graph_depth = -1) const;
	unsigned long get_n_generated_nodes_by_pattern(std::size_t pattern_hash, int graph_depth = -1) const;

protected:
	virtual void generate_class_header(const std::string class_name,
	                                   const std::string fbits_name,
	                                         std::ostream &stream1,
	                                         std::ostream &stream2) = 0;
	virtual void generate_class_footer(      std::ostream &stream)  = 0;

	virtual void recursive_generate_decoder      (const tools::Binary_node<tools::Pattern_polar_i>* node_curr, std::ostream &stream) = 0;
	virtual void recursive_generate_short_decoder(const tools::Binary_node<tools::Pattern_polar_i>* node_curr, std::ostream &stream) = 0;

private:
	void recursive_generate_short_decoder_funcs(const tools::Binary_node<tools::Pattern_polar_i>* node_curr, std::ostream &stream);
	void recursive_generate_graph              (const tools::Binary_node<tools::Pattern_polar_i>* node_curr, std::ostream &stream);
	void recursive_generate_short_graph        (const tools::Binary_node<tools::Pattern_polar_i>* node_curr, std::ostream &stream);
};
}
}

#endif /* GENERATOR_POLAR_SYS_ */
