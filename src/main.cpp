#include <cstdlib>
#include <iostream>
#include <exception>
#include <map>
#include <string>
#include <mipp.h>

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#ifdef SYSTEMC
#include <systemc>
#endif

#include "Tools/git_sha1.h"
#include "Tools/types.h"
#include "Tools/Arguments_reader.hpp"
#include "Tools/Display/bash_tools.h"

#include "Launcher/Launcher.hpp"
#include "Launcher/Simulation_chain/Launcher_BFER_std.hpp"
#include "Launcher/Simulation_chain/Launcher_BFER_ite.hpp"
#include "Launcher/BCH/Launcher_BCH.hpp"
#include "Launcher/LDPC/Launcher_LDPC.hpp"
#include "Launcher/Polar/Launcher_polar.hpp"
#include "Launcher/RA/Launcher_RA.hpp"
#include "Launcher/RSC/Launcher_RSC.hpp"
#include "Launcher/Turbo/Launcher_turbo.hpp"
#include "Launcher/Repetition/Launcher_repetition.hpp"
#include "Launcher/Uncoded/Launcher_NO.hpp"


//#include "Launcher/BFERI/Polar/Launcher_BFERI_polar.hpp"
//#include "Launcher/BFERI/RSC/Launcher_BFERI_RSC.hpp"
//#include "Launcher/BFERI/LDPC/Launcher_BFERI_LDPC.hpp"
//#include "Launcher/BFERI/Uncoded/Launcher_BFERI_uncoded.hpp"
//
//#include "Launcher/EXIT/Polar/Launcher_EXIT_polar.hpp"
//#include "Launcher/EXIT/RSC/Launcher_EXIT_RSC.hpp"
//
//#include "Launcher/GEN/Polar/Launcher_GEN_polar.hpp"

#include "Tools/Factory/Simulation/Factory_simulation_main.hpp"
#include "Tools/params.h"

using namespace aff3ct::launcher;
using namespace aff3ct::tools;

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
#elif defined(_MSC_VER)
	std::string compiler = "MSVC++";
	std::string compiler_version = std::to_string(_MSC_VER);
#else
	std::string compiler = "Unknown compiler";
	std::string compiler_version = "";
#endif
	std::string affect_version = "1.1.0";

	std::string git_sha1 = g_GIT_SHA1;

	std::cout << "aff3ct (" << os << prec << ", " << compiler << " " << compiler_version << ", " 
	          << mipp::IntructionsType << ") " << affect_version << std::endl;
	if (git_sha1 != "GITDIR-NOTFOUND")
		std::cout << "GIT SHA1: " << git_sha1 << std::endl;
	std::cout << "Copyright (c) 2016-2017 - MIT license."                                      << std::endl;
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
	Arguments_reader ar(argc, argv);

	Arguments_reader::arg_map req_args, opt_args;
	Arguments_reader::arg_grp arg_group;

	std::vector<std::string> cmd_warn, cmd_error;
	Factory_simulation_main::simu_parameters_main params;

	Factory_simulation_main::build_args(req_args, opt_args);

	bool miss_arg = !ar.parse_arguments(req_args, opt_args, cmd_warn);
	bool error    = !ar.check_arguments(cmd_error);

	Factory_simulation_main::store_args(ar, params);

	if (params.display_version)
		print_version();

	if (error || miss_arg)
	{
		Factory_simulation_main::group_args(arg_group);
		ar.print_usage(arg_group);

		for (auto w = 0; w < (int)cmd_error.size(); w++)
			std::cerr << format_error(cmd_error[w]) << std::endl;
		std::exit(EXIT_FAILURE);
	}

	code_type = params.cde_type;
	simu_type = params.sim_type;

#ifdef MULTI_PREC
	prec = params.sim_prec;
#endif
}
//
//template <typename B, typename R, typename Q, typename QD>
//Launcher<B,R,Q>* create_exit_simu(const int argc, const char **argv, std::string code_type, std::string simu_type)
//{
//	return nullptr;
//}
//
//#if defined(MULTI_PREC) || defined(PREC_32_BIT)
//template <>
//Launcher<int,float,float>* create_exit_simu<int, float, float, float>(const int     argc,
//                                                                      const char  **argv,
//                                                                      std::string   code_type,
//                                                                      std::string   simu_type)
//{
//	if (code_type == "POLAR")
//		if (simu_type == "EXIT")
//			return new Launcher_EXIT_polar<int,float,float>(argc, argv);
//
//	if (code_type == "RSC")
//		if (simu_type == "EXIT")
//			return new Launcher_EXIT_RSC<int,float,float,float>(argc, argv);
//
//	return nullptr;
//}
//#endif
//
//#if defined(MULTI_PREC) || defined(PREC_64_BIT)
//template <>
//Launcher<long long,double,double>* create_exit_simu<long long, double, double, double>(const int     argc,
//                                                                                       const char  **argv,
//                                                                                       std::string   code_type,
//                                                                                       std::string   simu_type)
//{
//	if (code_type == "POLAR")
//		if (simu_type == "EXIT")
//			return new Launcher_EXIT_polar<long long,double,double>(argc, argv);
//
//	if (code_type == "RSC")
//		if (simu_type == "EXIT")
//			return new Launcher_EXIT_RSC<long long,double,double,double>(argc, argv);
//
//	return nullptr;
//}
//#endif

template <typename B, typename R, typename Q, typename QD>
void start_simu(const int argc, const char **argv, std::string code_type, std::string simu_type)
{
	try
	{
		Launcher<B,R,Q> *launcher = nullptr;

		if (code_type == "POLAR")
		{
			if (simu_type == "BFER")
				launcher = new Launcher_polar<Launcher_BFER_std<B,R,Q>,B,R,Q>(argc, argv);
//			else if (simu_type == "BFERI")
//				launcher = new Launcher_BFERI_polar<B,R,Q>(argc, argv);
//			else if (simu_type == "GEN")
//				launcher = new Launcher_GEN_polar<B,R,Q>(argc, argv);
		}

		if (code_type == "RSC")
		{
			if (simu_type == "BFER")
				launcher = new Launcher_RSC<Launcher_BFER_std<B,R,Q>,B,R,Q,QD>(argc, argv);
//			else if (simu_type == "BFERI")
//				launcher = new Launcher_BFERI_RSC<B,R,Q,QD>(argc, argv);
		}

		if (code_type == "TURBO")
		{
			if (simu_type == "BFER")
				launcher = new Launcher_turbo<Launcher_BFER_std<B,R,Q>,B,R,Q,QD>(argc, argv);
		}

		if (code_type == "REPETITION")
		{
			if (simu_type == "BFER")
				launcher = new Launcher_repetition<Launcher_BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		}

		if (code_type == "BCH")
		{
			if (simu_type == "BFER")
				launcher = new Launcher_BCH<Launcher_BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		}

		if (code_type == "RA")
		{
			if (simu_type == "BFER")
				launcher = new Launcher_RA<Launcher_BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		}

		if (code_type == "LDPC")
		{
			if (simu_type == "BFER")
				launcher = new Launcher_LDPC<Launcher_BFER_std<B,R,Q>,B,R,Q>(argc, argv);
//			else if (simu_type == "BFERI")
//				launcher = new Launcher_BFERI_LDPC<B,R,Q>(argc, argv);
		}

		if (code_type == "UNCODED")
		{
			if (simu_type == "BFER")
				launcher = new Launcher_NO<Launcher_BFER_std<B,R,Q>,B,R,Q>(argc, argv);
//			else if (simu_type == "BFERI")
//				launcher = new Launcher_BFERI_uncoded<B,R,Q>(argc, argv);
		}

		if (launcher == nullptr)
		{
//			launcher = create_exit_simu<B,R,Q,QD>(argc, argv, code_type, simu_type);

			if (launcher == nullptr)
			{
				std::cerr << format_error("Unsupported code/simulation pair.") << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		launcher->launch();
		delete launcher;
	}
	catch (std::exception const& e)
	{
		std::cerr << format_error("An issue was encountered when running the launcher.") << std::endl
		          << format_error(e.what()) << std::endl;
	}
}

#ifndef SYSTEMC
int main(int argc, char **argv)
#else
int sc_main(int argc, char **argv)
#endif
{
#ifdef ENABLE_MPI
	MPI_Init(nullptr, nullptr);
#endif

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
			std::cerr << format_error("Unsupported bit precision.") << std::endl;
			return EXIT_FAILURE;
			break;
	}
#else
	read_arguments(argc, (const char**)argv, code_type, simu_type);
	start_simu<B,R,Q,QD>(argc, (const char**)argv, code_type, simu_type);
#endif

#ifdef ENABLE_MPI
	MPI_Finalize();
#endif

	return EXIT_SUCCESS;
}
