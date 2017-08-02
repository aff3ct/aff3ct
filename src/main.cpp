#include <cstdlib>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <mipp.h>

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#ifdef SYSTEMC
#include <systemc>
#endif

#include "Tools/types.h"
#include "Tools/version.h"
#include "Tools/Arguments_reader.hpp"
#include "Tools/Display/bash_tools.h"

#include "Launcher/Launcher.hpp"
#include "Factory/Launcher/Launcher.hpp"

using namespace aff3ct;

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
	std::string affect_version = version() == "GIT-NOTFOUND" ? "Unknown version" : version();

	std::cout << "aff3ct (" << os << prec << ", " << compiler << " " << compiler_version << ", " 
	          << mipp::InstructionFullType << ") " << affect_version << std::endl;
	if (sha1() != "GITDIR-NOTFOUND")
		std::cout << "GIT SHA1: " << sha1() << std::endl;
	std::cout << "Copyright (c) 2016-2017 - MIT license."                                      << std::endl;
	std::cout << "This is free software; see the source for copying conditions.  There is NO"  << std::endl;
	std::cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
	exit(EXIT_SUCCESS);
}

#ifdef MULTI_PREC
void read_arguments(const int argc, const char** argv, factory::Launcher::parameters &params)
#else
void read_arguments(const int argc, const char** argv, factory::Launcher::parameters &params)
#endif
{
	tools::Arguments_reader ar(argc, argv);

	factory::arg_map req_args, opt_args;
	factory::arg_grp arg_group;

	std::vector<std::string> cmd_warn, cmd_error;

	factory::Launcher::build_args(req_args, opt_args, "sim");

	bool miss_arg = !ar.parse_arguments(req_args, opt_args, cmd_warn);
	bool error    = !ar.check_arguments(cmd_error);

	factory::Launcher::store_args(ar.get_args(), params, "sim");

	if (params.display_version)
		print_version();

	if (error || miss_arg)
	{
		arg_group.push_back({"sim", "Simulation parameter(s)"});
		ar.print_usage(arg_group);

		for (auto w = 0; w < (int)cmd_error.size(); w++)
			std::cerr << tools::format_error(cmd_error[w]) << std::endl;
		std::exit(EXIT_FAILURE);
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

	factory::Launcher::parameters params;
	read_arguments(argc, (const char**)argv, params);

	launcher::Launcher *launcher = nullptr;
	try
	{
#ifdef MULTI_PREC
		switch (params.sim_prec)
		{
			case 8 : launcher = factory::Launcher::build<B_8, R_8, Q_8, QD_8 >(params, argc, (const char**)argv); break;
			case 16: launcher = factory::Launcher::build<B_16,R_16,Q_16,QD_16>(params, argc, (const char**)argv); break;
			case 32: launcher = factory::Launcher::build<B_32,R_32,Q_32,QD_32>(params, argc, (const char**)argv); break;
#if defined(__x86_64) || defined(__x86_64__) || defined(_WIN64) || defined(__aarch64__)
			case 64: launcher = factory::Launcher::build<B_64,R_64,Q_64,QD_64>(params, argc, (const char**)argv); break;
#endif
			default: break;
		}
#else
		launcher = factory::Launcher::build<B,R,Q,QD>(params, argc, (const char**)argv);
#endif
		if (launcher != nullptr)
		{
			launcher->launch();
			delete launcher;
		}
	}
	catch(std::exception const& e)
	{
		std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
	}

#ifdef ENABLE_MPI
	MPI_Finalize();
#endif

	return EXIT_SUCCESS;
}
