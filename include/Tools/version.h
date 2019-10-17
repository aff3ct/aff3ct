/*!
 * \file
 * \brief Functions for the code version.
 */
#ifndef VERSION_H_
#define VERSION_H_

#include <string>

namespace aff3ct
{
namespace tools
{
std::string sha1           ();
std::string version        ();
int         version_major  ();
int         version_minor  ();
int         version_release();
}
}

#endif
