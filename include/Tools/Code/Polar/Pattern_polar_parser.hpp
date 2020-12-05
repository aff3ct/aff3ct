/*!
 * \file
 * \brief Class tools::Pattern_polar_parser.
 */
#ifndef PATTERN_POLAR_PARSER_HPP
#define PATTERN_POLAR_PARSER_HPP

#include <utility>
#include <vector>
#include <memory>

#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"
#include "Tools/Algo/Tree/Binary/Binary_tree.hpp"
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
class Pattern_polar_parser : public Interface_get_set_frozen_bits
{
protected:
	const int                                                   N;                /*!< Codeword size. */
	const int                                                   m;                /*!< Tree depth. */
	      std::vector<bool>                                     frozen_bits;      /*!< Vector of frozen bits (true if frozen, false otherwise). */
	      std::vector<std::shared_ptr<tools::Pattern_polar_i>>  patterns;         /*!< Vector of patterns. */
	const size_t                                                pattern_rate0_id; /*!< Terminal pattern when the bit is frozen. */
	const size_t                                                pattern_rate1_id; /*!< Terminal pattern when the bit is an information bit. */
	      Binary_tree<Pattern_polar_i>                          polar_tree;       /*!< Tree of patterns. */
	      std::vector<unsigned char>                            pattern_types;    /*!< Tree of patterns represented with a vector of pattern IDs. */
	      std::vector<std::pair<unsigned char, int>>            leaves_pattern_types;

public:
 	/*!
	 * \brief Constructor.
	 *
	 * \param frozen_bits:   vector of frozen bits (true if frozen, false otherwise).
	 * \param patterns:      vector of patterns.
	 * \param pattern_rate0_id: id of the terminal pattern when the bit is frozen (id in the patterns vector).
	 * \param pattern_rate1_id: id of the terminal pattern when the bit is an info. bit (id in the patterns vector).
	 * \param delete_input_patterns: if true, delete the patterns in the input patterns vector.
	 */
	Pattern_polar_parser(const std::vector<bool> &frozen_bits,
	                     const std::vector<tools::Pattern_polar_i*> &patterns,
	                     const size_t pattern_rate0_id,
	                     const size_t pattern_rate1_id,
	                     const bool delete_input_patterns = false);

	Pattern_polar_parser(const Pattern_polar_parser &ppp);

	/*!
	 * \brief Destructor. call release_patterns()
	 */
	virtual ~Pattern_polar_parser();

	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual const std::vector<bool>& get_frozen_bits() const;

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
	void deep_copy          (const Pattern_polar_parser& t);
	void recursive_deep_copy(const tools::Binary_node<Pattern_polar_i> *nref,
	                               tools::Binary_node<Pattern_polar_i> *nclone);

	void recursive_allocate_nodes_patterns  (      Binary_node<Pattern_polar_i>* node_curr);
	void generate_nodes_indexes             (const Binary_node<Pattern_polar_i>* node_curr);
	void recursive_deallocate_nodes_patterns(      Binary_node<Pattern_polar_i>* node_curr);

	Pattern_polar_parser& operator=(const Pattern_polar_parser&) = delete;

	/*!
	 * \brief Release the polar patterns given in the constructor.
	 */
	void release_patterns() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/Pattern_polar_parser.hxx"
#endif

#endif /* PATTERN_POLAR_PARSER_HPP */
