/*!
 * \file
 * \brief It all starts here!
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MAIN_H_
#define MAIN_H_

#include <string>

#ifdef SYSTEMC
#include <systemc>
#endif

/*!
 * \brief Print the version of the binary (code version, compiler version, operating system, type of SIMD, ...).
 */
void print_version();

/*!
 * \brief Read arguments from command line and fill code_type, simu_type and prec.
 *
 * \param argc     : number of arguments from the command line.
 * \param argv     : array of arguments from the command line.
 * \param code_type: the type of the code (POLAR, TURBO, LDPC, etc.).
 * \param simu_type: the type of the simulation (BFER, BFERI, EXIT, GEN, etc.).
 * \param prec     : the precision of the simulation to run (8, 16, 32 or 64 bits).
 */
#ifdef MULTI_PREC
void read_arguments(const int argc, const char** argv, std::string &code_type, std::string &simu_type, int &prec);
#else
void read_arguments(const int argc, const char** argv, std::string &code_type, std::string &simu_type);
#endif

/*!
 * \brief Start the simulation by calling a Launcher (the Launcher depends on the code_type and the simu_type).
 *
 * \tparam B : type of the bits in the simulation.
 * \tparam R : type of the reals (floating-point representation) in the simulation.
 * \tparam Q : type of the quantified reals (fixed-point representation) in the simulation.
 * \tparam QD: type of the quantified reals (fixed-point representation) in the decoder (can be used or not depending
 *             on the code and the simulation type).
 *
 * \param argc     : number of arguments from the command line.
 * \param argv     : array of arguments from the command line.
 * \param code_type: the type of the code (POLAR, TURBO, LDPC, etc.).
 * \param simu_type: the type of the simulation (BFER, BFERI, EXIT, GEN, etc.).
 */
template <typename B, typename R, typename Q, typename QD>
void start_simu(const int argc, const char **argv, std::string code_type, std::string simu_type);

/**
 * \brief The first function called in the program!
 *
 * \param argc: number of arguments from the command line.
 * \param argv: array of arguments from the command line.
 *
 * \return EXIT_SUCCESS if no errors.
 */
#ifndef SYSTEMC
int main(int argc, char **argv);
#else
int sc_main(int argc, char **argv);
#endif

#endif /* MAIN_H_ */
