#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include <vector>

class Generator
{
public:
	Generator() {};
	virtual ~Generator() {};

	virtual void generate() = 0;

	virtual unsigned long get_n_generated_nodes           (                          int graph_depth = -1) const = 0;
	virtual unsigned long get_n_generated_nodes_by_pattern(std::size_t pattern_hash, int graph_depth = -1) const = 0;
};

#endif /* GENERATOR_HPP_ */
