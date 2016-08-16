#include <cstdlib>
#include <string>
#include <iostream>
#include <map>

#ifdef SYSTEMC
#include <systemc>
#endif

#include "Tools/types.h"

#include "Launcher/Launcher.hpp"

#include "Launcher/BFER/Launcher_BFER_polar.hpp"
#include "Launcher/BFER/Launcher_BFER_turbo.hpp"
#include "Launcher/BFER/Launcher_BFER_LDPC.hpp"
#include "Launcher/BFER/Launcher_BFER_RSC.hpp"
#include "Launcher/BFER/Launcher_BFER_RA.hpp"
#include "Launcher/BFER/Launcher_BFER_repetition.hpp"
#include "Launcher/BFER/Launcher_BFER_uncoded.hpp"

#include "Launcher/BFERI/Launcher_BFERI_RSC.hpp"
#include "Launcher/BFERI/Launcher_BFERI_LDPC.hpp"
#include "Launcher/BFERI/Launcher_BFERI_uncoded.hpp"

#include "Launcher/EXIT/Launcher_EXIT_polar.hpp"
#include "Launcher/EXIT/Launcher_EXIT_RSC.hpp"

#include "Launcher/GEN/Launcher_GEN_polar.hpp"

#include "Tools/params.h"
#include "Tools/bash_tools.h"
#include "Tools/MIPP/mipp.h"
#include "Tools/Arguments_reader.hpp"

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

	std::map<std::string, std::string> req_args, opt_args, doc_args;
	Arguments_reader ar(argc, argv);

	req_args["code-type"] = "code_type";
	doc_args["code-type"] = "the type of codes you want to simulate (POLAR, TURBO, REPETITION, RA or RSC).";
	
	opt_args["simu-type"] = "simu_type";
	doc_args["simu-type"] = "the type of simulation to run (BFER [default], EXIT or GEN).";

	opt_args["v"        ] = "";
	doc_args["v"        ] = "print informations about the version of the code.";
	opt_args["version"  ] = "";
	doc_args["version"  ] = "print informations about the version of the code.";

	opt_args["h"        ] = "";
	doc_args["h"        ] = "print this help.";
	opt_args["help"     ] = "";
	doc_args["help"     ] = "print this help.";

#ifdef MULTI_PREC
	opt_args["prec"] = "prec";
	doc_args["prec"] = "the simulation precision in bit (ex: 8, 16, 32 or 64).";
#endif

	auto display_help = true;
	auto parsing = ar.parse_arguments(req_args, opt_args);

	if (ar.exist_arg("v") || ar.exist_arg("version")) 
		print_version();

	if (parsing)
	{
		// required parameters
		code_type = ar.get_arg("code-type");

		// facultative parameters
		if(ar.exist_arg("simu-type")) simu_type = ar.get_arg("simu-type");
#ifdef MULTI_PREC
		if(ar.exist_arg("prec")) prec = stoi(ar.get_arg("prec"));
#endif

		display_help = false;
	}

	if (display_help)
	{
		if (ar.parse_doc_args(doc_args))
			ar.print_usage();
		else
			std::cerr << bold_red("(EE) A problem was encountered when parsing arguments documentation, exiting.") 
			          << std::endl;
		exit(EXIT_FAILURE);
	}
}

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

#ifndef SYSTEMC
int main(int argc, char **argv)
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
#else
int sc_main(int argc, char **argv)
{
	sc_core::sc_start(); // Start simulation

	return EXIT_SUCCESS;
}
#endif
