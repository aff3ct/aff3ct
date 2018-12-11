#include <utility>
#include <cmath>

#include "Constellation.hpp"

using namespace aff3ct;
using namespace tools;

Constellation::
Constellation(const unsigned n_bps, const std::string& name)
: n_bps(n_bps),
  n_symbs(1 << n_bps),
  name(std::to_string(n_symbs) + name)
{

}
