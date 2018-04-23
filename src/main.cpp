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
#include "Tools/Arguments/Argument_handler.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/system_functions.h"

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
	std::string affect_version = version() == "GIT-NOTFOUND" ? "" : version();

	std::cout << "aff3ct (" << os << prec << ", " << compiler << " " << compiler_version << ", "
	          << mipp::InstructionFullType << ") " << affect_version << std::endl;
	std::cout << "Copyright (c) 2016-2018 - MIT license."                                      << std::endl;
	std::cout << "This is free software; see the source for copying conditions.  There is NO"  << std::endl;
	std::cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
	exit(EXIT_SUCCESS);
}

int read_arguments(const int argc, const char** argv, factory::Launcher::parameters &params)
{
	tools::Argument_handler ah(argc, argv);

	tools::Argument_map_info args;
	tools::Argument_map_group arg_group;

	std::vector<std::string> cmd_warn, cmd_error;

	params.get_description(args);

	auto arg_vals = ah.parse_arguments(args, cmd_warn, cmd_error);

	bool display_help = false;
	try
	{
		params.store(arg_vals);
	}
	catch(std::exception&)
	{
		display_help = true;
	}

	if (params.display_version)
		print_version();

	if (cmd_error.size() || display_help)
	{
		arg_group["sim"] = "Simulation parameter(s)";
		ah.print_help(args, arg_group, params.display_adv_help);

		if (cmd_error.size()) std::cerr << std::endl;
		for (auto w = 0; w < (int)cmd_error.size(); w++)
			std::cerr << rang::tag::error << cmd_error[w] << std::endl;

		if (cmd_warn.size()) std::cerr << std::endl;
		for (auto w = 0; w < (int)cmd_warn.size(); w++)
			std::cerr << rang::tag::warning << cmd_warn[w] << std::endl;
	}
	return (cmd_error.size() || display_help) ? EXIT_FAILURE : EXIT_SUCCESS;
}

#ifndef SYSTEMC
int main(int argc, char **argv)
#else
int sc_main(int argc, char **argv)
#endif
{
	int exit_code = EXIT_SUCCESS;
#ifdef ENABLE_MPI
	MPI_Init(nullptr, nullptr);
#endif

	factory::Launcher::parameters params("sim");
	if (read_arguments(argc, (const char**)argv, params) == EXIT_FAILURE)
		return EXIT_FAILURE;

	try
	{
		launcher::Launcher *launcher;
#ifdef MULTI_PREC
		switch (params.sim_prec)
		{
			case 8 : launcher = factory::Launcher::build<B_8, R_8, Q_8 >(params, argc, (const char**)argv); break;
			case 16: launcher = factory::Launcher::build<B_16,R_16,Q_16>(params, argc, (const char**)argv); break;
			case 32: launcher = factory::Launcher::build<B_32,R_32,Q_32>(params, argc, (const char**)argv); break;
#if defined(__x86_64) || defined(__x86_64__) || defined(_WIN64) || defined(__aarch64__)
			case 64: launcher = factory::Launcher::build<B_64,R_64,Q_64>(params, argc, (const char**)argv); break;
#endif
			default: launcher = nullptr; break;
		}
#else
		launcher = factory::Launcher::build<B,R,Q>(params, argc, (const char**)argv);
#endif
		if (launcher != nullptr)
		{
			exit_code = launcher->launch();
			delete launcher;
		}
	}
	catch(std::exception const& e)
	{
		rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);
	}

#ifdef ENABLE_MPI
	MPI_Finalize();
#endif

	return exit_code;
}

// #include <aff3ct.hpp>
// #include "Tools/Algo/histogram.h"

// int main(int argc, char **argv)
// {
// 	using namespace aff3ct;

// 	using T = float;

// 	const R ROP = -35.2;
// 	std::vector<T> px = {-0.0191885880000000, -0.0146338349447236, -0.0100790818894472, -0.00552432883417085, -0.000969575778894474, 0.00358517727638191, 0.00813993033165830, 0.0126946833869347, 0.0172494364422111, 0.0218041894974874, 0.0263589425527638, 0.0309136956080402, 0.0354684486633166, 0.0400232017185930, 0.0445779547738694, 0.0491327078291457, 0.0536874608844221, 0.0582422139396985, 0.0627969669949749, 0.0673517200502513, 0.0719064731055276, 0.0764612261608040, 0.0810159792160804, 0.0855707322713568, 0.0901254853266332, 0.0946802383819096, 0.0992349914371860, 0.103789744492462, 0.108344497547739, 0.112899250603015, 0.117454003658291, 0.122008756713568, 0.126563509768844, 0.131118262824121, 0.135673015879397, 0.140227768934673, 0.144782521989950, 0.149337275045226, 0.153892028100503, 0.158446781155779, 0.163001534211055, 0.167556287266332, 0.172111040321608, 0.176665793376884, 0.181220546432161, 0.185775299487437, 0.190330052542714, 0.194884805597990, 0.199439558653266, 0.203994311708543, 0.208549064763819, 0.213103817819095, 0.217658570874372, 0.222213323929648, 0.226768076984925, 0.231322830040201, 0.235877583095477, 0.240432336150754, 0.244987089206030, 0.249541842261307, 0.254096595316583, 0.258651348371859, 0.263206101427136, 0.267760854482412, 0.272315607537688, 0.276870360592965, 0.281425113648241, 0.285979866703518, 0.290534619758794, 0.295089372814070, 0.299644125869347, 0.304198878924623, 0.308753631979899, 0.313308385035176, 0.317863138090452, 0.322417891145729, 0.326972644201005, 0.331527397256281, 0.336082150311558, 0.340636903366834, 0.345191656422111, 0.349746409477387, 0.354301162532663, 0.358855915587940, 0.363410668643216, 0.367965421698493, 0.372520174753769, 0.377074927809045, 0.381629680864322, 0.386184433919598, 0.390739186974874, 0.395293940030151, 0.399848693085427, 0.404403446140704, 0.408958199195980, 0.413512952251256, 0.418067705306533, 0.422622458361809, 0.427177211417085, 0.431731964472362, 0.436286717527638, 0.440841470582915, 0.445396223638191, 0.449950976693467, 0.454505729748744, 0.459060482804020, 0.463615235859297, 0.468169988914573, 0.472724741969849, 0.477279495025126, 0.481834248080402, 0.486389001135678, 0.490943754190955, 0.495498507246231, 0.500053260301508, 0.504608013356784, 0.509162766412060, 0.513717519467337, 0.518272272522613, 0.522827025577890, 0.527381778633166, 0.531936531688442, 0.536491284743719, 0.541046037798995, 0.545600790854271, 0.550155543909548, 0.554710296964824, 0.559265050020101, 0.563819803075377, 0.568374556130653, 0.572929309185930, 0.577484062241206, 0.582038815296482, 0.586593568351759, 0.591148321407035, 0.595703074462312, 0.600257827517588, 0.604812580572864, 0.609367333628141, 0.613922086683417, 0.618476839738694, 0.623031592793970, 0.627586345849246, 0.632141098904523, 0.636695851959799, 0.641250605015075, 0.645805358070352, 0.650360111125628, 0.654914864180905, 0.659469617236181, 0.664024370291457, 0.668579123346734, 0.673133876402010, 0.677688629457286, 0.682243382512563, 0.686798135567839, 0.691352888623116, 0.695907641678392, 0.700462394733668, 0.705017147788945, 0.709571900844221, 0.714126653899498, 0.718681406954774, 0.723236160010050, 0.727790913065327, 0.732345666120603, 0.736900419175879, 0.741455172231156, 0.746009925286432, 0.750564678341709, 0.755119431396985, 0.759674184452261, 0.764228937507538, 0.768783690562814, 0.773338443618091, 0.777893196673367, 0.782447949728643, 0.787002702783920, 0.791557455839196, 0.796112208894472, 0.800666961949749, 0.805221715005025, 0.809776468060302, 0.814331221115578, 0.818885974170854, 0.823440727226131, 0.827995480281407, 0.832550233336683, 0.837104986391960, 0.841659739447236, 0.846214492502513, 0.850769245557789, 0.855323998613065, 0.859878751668342, 0.864433504723618, 0.868988257778894, 0.873543010834171, 0.878097763889447, 0.882652516944724, 0.887207270000000};
// 	std::vector<T> p1 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.000505050505050505, 0.000505050505050505, 0, 0.00101010101010101, 0.000505050505050505, 0.00101010101010101, 0, 0.000505050505050505, 0.00101010101010101, 0, 0.000505050505050505, 0.00101010101010101, 0.00101010101010101, 0.00353535353535354, 0.00101010101010101, 0.00101010101010101, 0.00353535353535354, 0.00353535353535354, 0.00454545454545455, 0.00353535353535354, 0.00505050505050505, 0.00555555555555556, 0.00606060606060606, 0.00656565656565657, 0.00505050505050505, 0.00757575757575758, 0.00555555555555556, 0.0101010101010101, 0.00656565656565657, 0.0101010101010101, 0.00707070707070707, 0.0106060606060606, 0.0121212121212121, 0.0131313131313131, 0.0146464646464646, 0.0146464646464646, 0.0156565656565657, 0.0186868686868687, 0.0232323232323232, 0.0176767676767677, 0.0161616161616162, 0.0237373737373737, 0.0111111111111111, 0.0176767676767677, 0.0131313131313131, 0.0222222222222222, 0.0176767676767677, 0.0207070707070707, 0.0207070707070707, 0.0242424242424242, 0.0161616161616162, 0.0186868686868687, 0.0191919191919192, 0.0202020202020202, 0.0212121212121212, 0.0212121212121212, 0.0166666666666667, 0.0196969696969697, 0.0217171717171717, 0.0202020202020202, 0.0222222222222222, 0.0156565656565657, 0.0131313131313131, 0.0186868686868687, 0.0151515151515152, 0.0181818181818182, 0.0131313131313131, 0.0151515151515152, 0.0171717171717172, 0.0136363636363636, 0.0181818181818182, 0.0106060606060606, 0.0131313131313131, 0.0111111111111111, 0.0131313131313131, 0.00808080808080808, 0.00959595959595960, 0.00808080808080808, 0.0106060606060606, 0.0101010101010101, 0.00606060606060606, 0.00606060606060606, 0.00808080808080808, 0.00656565656565657, 0.00505050505050505, 0.00808080808080808, 0.00555555555555556, 0.00505050505050505, 0.00505050505050505, 0.00101010101010101, 0.00404040404040404, 0.00353535353535354, 0.00353535353535354, 0.00202020202020202, 0.00404040404040404, 0.00151515151515152, 0.00202020202020202, 0.00252525252525253, 0.00303030303030303, 0.00101010101010101, 0.000505050505050505, 0.000505050505050505, 0.00252525252525253, 0.00101010101010101, 0, 0.000505050505050505, 0.00151515151515152, 0.00151515151515152, 0.000505050505050505, 0.00151515151515152, 0.000505050505050505, 0.00202020202020202, 0.000505050505050505, 0, 0, 0, 0.000505050505050505, 0.000505050505050505, 0.000505050505050505, 0, 0, 0, 0, 0.000505050505050505, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// 	std::vector<T> p0 = {0, 0.00101214574898785, 0.00101214574898785, 0.00455465587044534, 0.0177125506072875, 0.0339068825910931, 0.0455465587044534, 0.0632591093117409, 0.0713562753036437, 0.0804655870445344, 0.0885627530364373, 0.0779352226720648, 0.0683198380566802, 0.0819838056680162, 0.0668016194331984, 0.0460526315789474, 0.0546558704453441, 0.0430161943319838, 0.0278340080971660, 0.0298582995951417, 0.0161943319838057, 0.0161943319838057, 0.0121457489878543, 0.0101214574898785, 0.0121457489878543, 0.00607287449392713, 0.00556680161943320, 0.00556680161943320, 0.00455465587044534, 0.00253036437246964, 0.00354251012145749, 0.000506072874493927, 0.00101214574898785, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 	unsigned noise_length = 1e6;
// 	std::vector<T> noise(noise_length);

// 	tools::User_pdf_gen_std<T> noise_gene0;
// 	noise_gene0.add_distribution(ROP, new tools::Distribution<T>(px, p0));
// 	noise_gene0.generate(noise.data(), noise_length, ROP);

// 	std::ofstream file0("aff3ct_noise0.csv");
// 	histogram(file0, noise, noise_gene0.get_distribution(ROP)->get_min_x(), noise_gene0.get_distribution(ROP)->get_max_x());

// 	tools::User_pdf_gen_std<T> noise_gene1;
// 	noise_gene1.add_distribution(ROP, new tools::Distribution<T>(px, p1));
// 	noise_gene1.generate(noise.data(), noise_length, ROP);

// 	std::ofstream file1("aff3ct_noise1.csv");
// 	histogram(file1, noise, noise_gene1.get_distribution(ROP)->get_min_x(), noise_gene1.get_distribution(ROP)->get_max_x());

// }