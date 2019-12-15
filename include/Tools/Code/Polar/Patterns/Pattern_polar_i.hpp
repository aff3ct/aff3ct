/*!
 * \file
 * \brief Class tools::Pattern_polar_i.
 */
#ifndef PATTERN_POLAR_INTERFACE_HPP_
#define PATTERN_POLAR_INTERFACE_HPP_

#include <string>

#include "Tools/Algo/Tree/Binary/Binary_node.hpp"

namespace aff3ct
{
namespace tools
{
enum class polar_node_t : uint8_t
{
	STANDARD = 0,
	RATE_0_LEFT,
	RATE_0,
	RATE_1,
	REP_LEFT,
	REP,
	SPC,
	NB_PATTERNS
};

// interface
class Pattern_polar_i
{
protected:
	const int N;
	const int m;
	const int size;
	const int si_2;
	const Binary_node<Pattern_polar_i>* node;
	const int off_l;  // offset for lambda buffer (reals)
	const int off_s;  // offset for s buffer (bits)
	const int rev_depth;
	const int n_dig;  // number of digits in n
	const int n2_dig; // number of digits in n / 2
	const std::string tab;

	unsigned id;
	std::string key;

	int min_level;
	int max_level;

	inline Pattern_polar_i(const int &N, const Binary_node<Pattern_polar_i>* node,
	                       const int min_level = 0, const int max_level = -1);

public:
	inline Pattern_polar_i(const int min_level = 0, const int max_level = -1);

	virtual ~Pattern_polar_i() = default;

	inline void        set_id   (const unsigned id     );
	inline unsigned    get_id   (                      );
	inline void        set_key  (const std::string &key);
	inline std::string get_key  (                      );
	inline int         get_size (                      );
	inline int         get_si_2 (                      );
	inline int         get_off_l(                      );
	inline int         get_off_s(                      );

	inline int get_min_lvl();
	inline int get_max_lvl();

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const = 0;

	virtual polar_node_t type()       const = 0;
	virtual std::string  name()       const = 0;
	virtual std::string  short_name() const = 0;
	virtual std::string  fill_color() const = 0;
	virtual std::string  font_color() const = 0;

	virtual std::string f() const = 0;
	virtual std::string g() const = 0;
	virtual std::string h() const = 0;

	virtual inline std::string apply_f(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const;
	virtual inline std::string apply_g(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const;
	virtual inline std::string apply_h(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const;

	virtual inline std::string range() const;

	virtual inline int match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr);

	virtual int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const = 0;

	virtual bool is_terminal() const = 0;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hxx"
#endif

#endif /* PATTERN_POLAR_INTERFACE_HPP_ */
