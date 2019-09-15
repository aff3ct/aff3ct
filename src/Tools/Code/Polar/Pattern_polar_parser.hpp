/*!
 * \file
 * \brief Parses a polar code (represented as a tree) and returns a simplified tree with specialized nodes and tree
 *        cuts when possible.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef PATTERN_POLAR_PARSER_HPP
#define PATTERN_POLAR_PARSER_HPP

#include <utility>
#include <vector>
#include <memory>

#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Pattern_polar_parser
 * \brief Parses a polar code (represented as a tree) and returns a simplified tree with specialized nodes and tree
 *        cuts when possible.
 */
class Pattern_polar_parser
{
protected:
	const int                                                   N;             /*!< Codeword size. */
	const int                                                   m;             /*!< Tree depth. */
	const std::vector<bool>                                    &frozen_bits;   /*!< Vector of frozen bits (true if frozen, false otherwise). */
	const std::vector<std::unique_ptr<tools::Pattern_polar_i>>  patterns;      /*!< Vector of patterns. */
	const std::unique_ptr<tools::Pattern_polar_i>&              pattern_rate0; /*!< Terminal pattern when the bit is frozen. */
	const std::unique_ptr<tools::Pattern_polar_i>&              pattern_rate1; /*!< Terminal pattern when the bit is an information bit. */
	      std::unique_ptr<Binary_tree<Pattern_polar_i>>         polar_tree;    /*!< Tree of patterns. */
	      std::vector<unsigned char>                            pattern_types; /*!< Tree of patterns represented with a vector of pattern IDs. */
	      std::vector<std::pair<unsigned char, int>>            leaves_pattern_types;

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
	Pattern_polar_parser(const int& N,
	                     const std::vector<bool> &frozen_bits,
	                     std::vector<std::unique_ptr<tools::Pattern_polar_i>> &&patterns,
	                     const std::unique_ptr<tools::Pattern_polar_i> &pattern_rate0,
	                     const std::unique_ptr<tools::Pattern_polar_i> &pattern_rate1);

	/*!
	 * \brief Constructor.
	 *
	 * \param N:                codeword size.
	 * \param frozen_bits:      vector of frozen bits (true if frozen, false otherwise).
	 * \param patterns:         vector of patterns.
	 * \param pattern_rate0_id: id of the terminal pattern when the bit is frozen (id in the patterns vector).
	 * \param pattern_rate1_id: id of the terminal pattern when the bit is an info. bit (id in the patterns vector).
	 */
	Pattern_polar_parser(const int& N,
	                     const std::vector<bool>& frozen_bits,
	                     std::vector<std::unique_ptr<tools::Pattern_polar_i>> &&patterns,
	                     const int pattern_rate0_id,
	                     const int pattern_rate1_id);

	/*!
	 * \brief Constructor.
	 *
	 * \param N:             codeword size.
	 * \param frozen_bits:   vector of frozen bits (true if frozen, false otherwise).
	 * \param patterns:      vector of patterns.
	 * \param pattern_rate0: terminal pattern when the bit is frozen.
	 * \param pattern_rate1: terminal pattern when the bit is an information bit.
	 */
	Pattern_polar_parser(const int& N,
	                     const std::vector<bool> &frozen_bits,
	                     const std::vector<tools::Pattern_polar_i*> patterns,
	                     const std::unique_ptr<tools::Pattern_polar_i> &pattern_rate0,
	                     const std::unique_ptr<tools::Pattern_polar_i> &pattern_rate1);

	/*!
	 * \brief Constructor.
	 *
	 * \param N:                codeword size.
	 * \param frozen_bits:      vector of frozen bits (true if frozen, false otherwise).
	 * \param patterns:         vector of patterns.
	 * \param pattern_rate0_id: id of the terminal pattern when the bit is frozen (id in the patterns vector).
	 * \param pattern_rate1_id: id of the terminal pattern when the bit is an info. bit (id in the patterns vector).
	 */
	Pattern_polar_parser(const int& N,
	                     const std::vector<bool>& frozen_bits,
	                     const std::vector<tools::Pattern_polar_i*> patterns,
	                     const int pattern_rate0_id,
	                     const int pattern_rate1_id);

	/*!
	 * \brief Destructor. call release_patterns()
	 */
	virtual ~Pattern_polar_parser();

	virtual void notify_frozenbits_update();

	/*!
	 * \brief Gets a binary tree of patterns.
	 *
	 * \return a binary tree of patterns.
	 */
	const Binary_tree<Pattern_polar_i>& get_polar_tree() const;

	/*!
	 * \brief Gets a vector of pattern IDs.
	 *
	 * The vector of patterns IDs is a compressed version of the binary tree of patterns provided by the
	 * Pattern_parser_polar::get_polar_tree method.
	 *
	 * \return a vector of pattern IDs.
	 */
	std::vector<unsigned char> get_pattern_types() const;

	const std::vector<std::pair<unsigned char, int>>& get_leaves_pattern_types() const;

	/*!
	 * \brief Gets a node pattern type from the id of the node.
	 *
	 * \param node_id: id of the node
	 *
	 * \return the type of the node.
	 */
	inline polar_node_t get_node_type(const int node_id) const;

	/*!
	 * \brief Check if a node type exists in the the tree.
	 *
	 * \param node_type: the type of the requested node
	 * \param rev_depth: check for the given reversed depth
	 *
	 * \return true if the node type exists, false otherwise.
	 */
	inline bool exist_node_type(const polar_node_t node_type, const int rev_depth = -1) const;

private:
	void recursive_allocate_nodes_patterns  (      Binary_node<Pattern_polar_i>* node_curr);
	void generate_nodes_indexes             (const Binary_node<Pattern_polar_i>* node_curr);
	void recursive_deallocate_nodes_patterns(      Binary_node<Pattern_polar_i>* node_curr);

	void operator=(Pattern_polar_parser&) = delete;
	Pattern_polar_parser(Pattern_polar_parser&) = delete;

	/*!
	 * \brief Release the polar patterns given in the constructor.
	 */
	void release_patterns() const;
};
}
}

#include "Tools/Code/Polar/Pattern_polar_parser.hxx"

#endif /* PATTERN_POLAR_PARSER_HPP */
