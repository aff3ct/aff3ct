/*!
 * \file
 * \brief Parses a polar code (represented as a tree) and returns a simplified tree with specialized nodes and tree
 *        cuts when possible.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef PATTERN_PARSER_POLAR_HPP
#define PATTERN_PARSER_POLAR_HPP

#include <map>
#include <vector>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Module/Decoder/Polar/SC/Patterns/Pattern_SC_interface.hpp"

/*!
 * \class Pattern_parser_polar
 * \brief Parses a polar code (represented as a tree) and returns a simplified tree with specialized nodes and tree
 *        cuts when possible.
 */
template <typename B>
class Pattern_parser_polar
{
protected:
	const int N; /*!< Codeword size. */
	const int m; /*!< Tree depth. */

	const mipp::vector<B>& frozen_bits; /*!< Vector of frozen bits (true if frozen, false otherwise). */

	const std::vector<Pattern_SC_interface*> patterns; /*!< Vector of patterns. */
	const Pattern_SC_interface *pattern_rate0;         /*!< Terminal pattern when the bit is frozen. */
	const Pattern_SC_interface *pattern_rate1;         /*!< Terminal pattern when the bit is an information bit. */

	Binary_tree<Pattern_SC_interface> *polar_tree;     /*!< Tree of patterns. */

private:
	std::vector<unsigned char> pattern_types;          /*!< Tree of patterns represented with a vector of pattern IDs. */
	std::vector<unsigned char> pattern_types_sums;
	std::vector<unsigned char> leaves_depth;
	std::vector<unsigned char> leaves_rev_depth;
	std::vector<int          > leaf_to_node_id;
	std::vector<int          > leaf_to_node_id_sums;
	std::vector<int          > leaf_to_g_node_id;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:             codeword size.
	 * \param frozen_bits:   vector of frozen bits (true if frozen, false otherwise).
	 * \param patterns:      vector of patterns.
	 * \param pattern_rate0: terminal pattern when the bit is frozen.
	 * \param pattern_rate1: terminal pattern when the bit is an information bit.
	 */
	Pattern_parser_polar(const int& N,
	                     const mipp::vector<B>& frozen_bits,
	                     const std::vector<Pattern_SC_interface*> patterns,
	                     const Pattern_SC_interface *pattern_rate0,
	                     const Pattern_SC_interface *pattern_rate1);

	Pattern_parser_polar(const int& N,
	                     const mipp::vector<B>& frozen_bits,
	                     const std::vector<Pattern_SC_interface*> patterns,
	                     const int pattern_rate0_id,
	                     const int pattern_rate1_id);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Pattern_parser_polar();

	/*!
	 * \brief Gets a binary tree of patterns.
	 *
	 * \return a binary tree of patterns.
	 */
	const Binary_tree<Pattern_SC_interface>* get_polar_tree() const;

	/*!
	 * \brief Gets a vector of pattern IDs.
	 *
	 * The vector of patterns IDs is a compressed version of the binary tree of patterns provided by the
	 * Pattern_parser_polar::get_polar_tree method.
	 *
	 * \return a vector of pattern IDs.
	 */
	std::vector<unsigned char> get_pattern_types() const;

	inline pattern_SC_type get_node_type           (const int node_id) const;
	inline pattern_SC_type get_node_type_sums      (const int node_id) const;
	inline pattern_SC_type get_leaf_type           (const int leaf_id) const;
	inline unsigned char   get_leaf_depth          (const int leaf_id) const;
	inline unsigned char   get_leaf_rev_depth      (const int leaf_id) const;
	inline int             get_leaf_to_node_id     (const int leaf_id) const;
	inline int             get_leaf_to_node_id_sums(const int leaf_id) const;
	inline int             get_leaf_to_g_node_id   (const int leaf_id) const;
	inline int             get_n_leaves            (                 ) const;

	void release_patterns() const;

private:
	void recursive_allocate_nodes_patterns  (      Binary_node<Pattern_SC_interface>* node_curr                 );
	void recursive_compute_leaves_depth     (      Binary_node<Pattern_SC_interface>* node_curr                 );
	int  recursive_find_g_node_id           (const Binary_node<Pattern_SC_interface>* node_curr                 );
	void generate_nodes_indexes             (const Binary_node<Pattern_SC_interface>* node_curr, int& node_index);
	void generate_nodes_indexes_sums        (const Binary_node<Pattern_SC_interface>* node_curr, int& leaf_index);
	void recursive_deallocate_nodes_patterns(      Binary_node<Pattern_SC_interface>* node_curr                 );
};

#include "Pattern_parser_polar.hxx"

#endif /* PATTERN_PARSER_POLAR_HPP */
