#ifndef SYSTEM_FUNCTIONS_H_
#define SYSTEM_FUNCTIONS_H_

#include <string>

namespace aff3ct
{
namespace tools
{

/*!
 * \brief Get the back trace in the stack.
 *
 * \param first_call: number of ignored called functions in the stack (1 means ignore call of this function)
 * \return a string with the back trace with one function per line
 */
std::string get_back_trace(int first_call = 1);

/*!
 * \brief run the given system command (this function add "2>&1" at the end of the command)
 *
 * \param cmd is the command to run on the system
 * \return the standard and error output mixed
 */
std::string run_system_command(std::string cmd);

/*!
 * \brief transform in the backtrace the addresses into function name and line index
 *
 * \param backtrace is the backtrace got with get_back_trace(int)
 * \return the backtrace with function name and line index
 */
std::string addr_to_line(const std::string& backtrace);

}
}

#endif // SYSTEM_FUNCTIONS_H_