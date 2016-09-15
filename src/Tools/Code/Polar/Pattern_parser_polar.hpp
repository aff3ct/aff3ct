#ifndef PATTERN_PARSER_POLAR_HPP
#define PATTERN_PARSER_POLAR_HPP

#include <map>
#include <vector>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Module/Decoder/Polar/SC/Patterns/Pattern_SC_interface.hpp"

class Pattern_parser_polar
{
protected:
	const int N; // n bits input
	const int m; // graph depth

	const mipp::vector<int>& frozen_bits;

	const std::vector<Pattern_SC_interface*> &patterns;
	const Pattern_SC_interface &pattern_rate0;
	const Pattern_SC_interface &pattern_rate1;

	Binary_tree<Pattern_SC_interface> *polar_tree;

private:
	std::vector<char> pattern_types_per_id;

public:
	Pattern_parser_polar(const int& N,
	                     const mipp::vector<int>& frozen_bits,
	                     const std::vector<Pattern_SC_interface*> &patterns,
	                     const Pattern_SC_interface &pattern_rate0,
	                     const Pattern_SC_interface &pattern_rate1);
	virtual ~Pattern_parser_polar();

	std::vector<char> get_pattern_types_per_id() const;
	const Binary_tree<Pattern_SC_interface>* get_polar_tree() const;

private:
	void recursive_allocate_nodes_patterns  (      Binary_node<Pattern_SC_interface>* node_curr                 );
	void generate_nodes_indexes             (const Binary_node<Pattern_SC_interface>* node_curr, int& node_index);
	void recursive_deallocate_nodes_patterns(      Binary_node<Pattern_SC_interface>* node_curr                 );
};

#endif /* PATTERN_PARSER_POLAR_HPP */
