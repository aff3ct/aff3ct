#include <cstdlib>
#include <iostream>
#include <map>

#include "Tools/types.h"
#include "Tools/params.h"
#include "Tools/Arguments_reader.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Perf/MIPP/mipp.h"

#include "Launcher/Launcher.hpp"

#include "Launcher/BFER/Polar/Launcher_BFER_polar.hpp"
#include "Launcher/BFER/Turbo/Launcher_BFER_turbo.hpp"
#include "Launcher/BFER/LDPC/Launcher_BFER_LDPC.hpp"
#include "Launcher/BFER/RSC/Launcher_BFER_RSC.hpp"
#include "Launcher/BFER/RA/Launcher_BFER_RA.hpp"
#include "Launcher/BFER/Repetition/Launcher_BFER_repetition.hpp"
#include "Launcher/BFER/Uncoded/Launcher_BFER_uncoded.hpp"

#include "Launcher/BFERI/RSC/Launcher_BFERI_RSC.hpp"
#include "Launcher/BFERI/LDPC/Launcher_BFERI_LDPC.hpp"
#include "Launcher/BFERI/Uncoded/Launcher_BFERI_uncoded.hpp"

#include "Launcher/EXIT/Polar/Launcher_EXIT_polar.hpp"
#include "Launcher/EXIT/RSC/Launcher_EXIT_RSC.hpp"

#include "Launcher/GEN/Polar/Launcher_GEN_polar.hpp"

#include "main.h"

void print_version()
{
#if defined(_WIN64) || defined(_WIN32) || defined(WIN32) || defined(__MINGW32__)
	std::string os = "Windows";
#elif defined(__linux__) || defined(__linux)
	std::string os = "Linux";
#elif defined(__APPLE__)
	std::string os = "Mac OS X";
#else
	std::string os = "Unknown OS";
#endif

#if defined(__x86_64) || defined(__x86_64__) || defined(_WIN64) || defined(__aarch64__)
	std::string prec = " 64-bit";
#elif defined(__i386) || defined(__i386__) || defined(i386) || defined(_WIN32) || defined(__ARM_ARCH_7__)
	std::string prec = " 32-bit";
#else
	std::string prec = "";
#endif

#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
	std::string compiler = "icpc";
#if defined(__INTEL_COMPILER)
	std::string compiler_version = std::to_string(__INTEL_COMPILER);
#elif defined(__ICL)
	std::string compiler_version = std::to_string(__ICL);
#else 
	std::string compiler_version = std::to_string(__ICC);
#endif
	compiler_version = compiler_version.substr(0,2) + "." + compiler_version.substr(2,compiler_version.size());
#elif defined(__clang__) || defined(__llvm__)
	std::string compiler = "clang++";
	std::string compiler_version = std::to_string(__clang_major__);
	compiler_version += "." + std::to_string(__clang_minor__);
#elif defined(__GNUG__) || (defined(__GNUC__) && defined(__cplusplus))
	std::string compiler = "g++";
	std::string compiler_version = std::to_string(__GNUC__);
	compiler_version += "." + std::to_string(__GNUC_MINOR__);
#else
	std::string compiler = "Unknown compiler";
	std::string compiler_version = "";
#endif
	std::string affect_version = "1.0.0";

	std::cout << "aff3ct (" << os << prec << ", " << compiler << " " << compiler_version << ", " 
	          << mipp::IntructionsType << ") " << affect_version << std::endl;
	std::cout << "Copyright (c) 2016 - MIT license."                                           << std::endl;
	std::cout << "This is free software; see the source for copying conditions.  There is NO"  << std::endl;
	std::cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
	exit(EXIT_SUCCESS);
}

#ifdef MULTI_PREC
void read_arguments(const int argc, const char** argv, std::string &code_type, std::string &simu_type, int &prec)
#else
void read_arguments(const int argc, const char** argv, std::string &code_type, std::string &simu_type)
#endif
{
	using namespace std::chrono;

	std::map<std::vector<std::string>, std::vector<std::string>> req_args, opt_args;
	Arguments_reader ar(argc, argv);

	// ---------------------------------------------------------------------------------------------------- simulation
	opt_args[{"sim-type"}] =
			{"string",
			 "the type of simulation to run.",
			 "BFER, BFERI, EXIT, GEN"};
#ifdef MULTI_PREC
	opt_args[{"sim-prec", "p"}] =
		{"positive_int",
		 "the simulation precision in bit.",
		 "8, 16, 32, 64"};
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	req_args[{"cde-type"}] =
		{"string",
		 "the type of codes you want to simulate.",
		 "POLAR, TURBO, LDPC, REPETITION, RA, RSC, UNCODED"};

	// --------------------------------------------------------------------------------------------------------- other
	opt_args[{"version", "v"}] =
		{"",
		 "print informations about the version of the code."};
	opt_args[{"help", "h"}] =
		{"",
		 "print this help."};

	auto display_help = true;
	auto parsing = ar.parse_arguments(req_args, opt_args);

	if (ar.exist_arg({"version", "v"}))
		print_version();

	if (parsing)
	{
		// required parameters
		code_type = ar.get_arg({"cde-type"});

		// facultative parameters
		if(ar.exist_arg({"sim-type"})) simu_type = ar.get_arg({"sim-type"});
#ifdef MULTI_PREC
		if(ar.exist_arg({"sim-prec", "p"})) prec = ar.get_arg_int({"sim-prec", "p"});
#endif

		display_help = false;
	}

	if (display_help)
	{
		std::vector<std::vector<std::string>> arg_grp;
		arg_grp.push_back({"sim", "Simulation parameter(s)"});
		arg_grp.push_back({"cde", "Code parameter(s)"      });
		ar.print_usage(arg_grp);
		std::exit(EXIT_FAILURE);
	}

	if (!ar.check_arguments())
		std::exit(EXIT_FAILURE);
}

/**
 * \fn void start_simu(const int argc, const char **argv, std::string code_type, std::string simu_type)
 *
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
void start_simu(const int argc, const char **argv, std::string code_type, std::string simu_type)
{
	Launcher<B,R,Q> *launcher = nullptr;

	if (code_type == "POLAR")
	{
		if (simu_type == "BFER")
			launcher = new Launcher_BFER_polar<B,R,Q>(argc, argv);
		else if (simu_type == "EXIT" && (typeid(Q) == typeid(float) || typeid(Q) == typeid(double)))
			launcher = new Launcher_EXIT_polar<B,R,Q>(argc, argv);
		else if (simu_type == "GEN")
			launcher = new Launcher_GEN_polar<B,R,Q>(argc, argv);
	}

	if (code_type == "RSC")
	{
		if (simu_type == "BFER")
			launcher = new Launcher_BFER_RSC<B,R,Q,QD>(argc, argv);
		else if (simu_type == "BFERI")
			launcher = new Launcher_BFERI_RSC<B,R,Q,QD>(argc, argv);
		else if (simu_type == "EXIT" && (typeid(Q) == typeid(float) || typeid(Q) == typeid(double)))
			launcher = new Launcher_EXIT_RSC<B,R,Q,QD>(argc, argv);
	}

	if (code_type == "TURBO")
	{
		if (simu_type == "BFER")
			launcher = new Launcher_BFER_turbo<B,R,Q,QD>(argc, argv);
	}

	if (code_type == "REPETITION")
	{
		if (simu_type == "BFER")
			launcher = new Launcher_BFER_repetition<B,R,Q>(argc, argv);
	}

	if (code_type == "RA")
	{
		if (simu_type == "BFER")
			launcher = new Launcher_BFER_RA<B,R,Q>(argc, argv);
	}

	if (code_type == "LDPC")
	{
		if (simu_type == "BFER")
			launcher = new Launcher_BFER_LDPC<B,R,Q>(argc, argv);
		else if (simu_type == "BFERI")
			launcher = new Launcher_BFERI_LDPC<B,R,Q>(argc, argv);
	}

	if (code_type == "UNCODED")
	{
		if (simu_type == "BFER")
			launcher = new Launcher_BFER_uncoded<B,R,Q>(argc, argv);
		else if (simu_type == "BFERI")
			launcher = new Launcher_BFERI_uncoded<B,R,Q>(argc, argv);
	}

	if (launcher == nullptr)
	{
		std::cerr << bold_red("(EE) Unsupported type of codes/simulation.") << std::endl;
		exit(EXIT_FAILURE);
	}

	launcher->launch();
	delete launcher;
}

/**
 * \fn int main(int argc, char **argv)
 *
 * \brief The first function called in the program!
 *
 * \param argc: number of arguments from the command line.
 * \param argv: array of arguments from the command line.
 *
 * \return EXIT_SUCCESS if no errors.
 */
#ifndef SYSTEMC
int main(int argc, char **argv)
#else
int sc_main(int argc, char **argv)
#endif
{
	srand(0);

	std::string code_type, simu_type = "BFER";

#ifdef MULTI_PREC
	int prec = 32;
	read_arguments(argc, (const char**)argv, code_type, simu_type, prec);

	switch (prec)
	{
		case 8: 
			start_simu<B_8, R_8, Q_8, QD_8 >(argc, (const char**)argv, code_type, simu_type); break;
		case 16: 
			start_simu<B_16,R_16,Q_16,QD_16>(argc, (const char**)argv, code_type, simu_type); break;
		case 32: 
			start_simu<B_32,R_32,Q_32,QD_32>(argc, (const char**)argv, code_type, simu_type); break;
#if defined(__x86_64) || defined(__x86_64__) || defined(_WIN64) || defined(__aarch64__)
		case 64: 
			start_simu<B_64,R_64,Q_64,QD_64>(argc, (const char**)argv, code_type, simu_type); break;
#endif
		default:
			std::cerr << bold_red("(EE) Unsupported number of bits.") << std::endl;
			exit(EXIT_FAILURE);
			break;
	}
#else
	read_arguments(argc, (const char**)argv, code_type, simu_type);
	start_simu<B,R,Q,QD>(argc, (const char**)argv, code_type, simu_type);
#endif

	return EXIT_SUCCESS;
}
