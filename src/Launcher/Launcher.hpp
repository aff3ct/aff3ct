#ifndef LAUNCHER_HPP_
#define LAUNCHER_HPP_

#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

#include "Tools/types.h"
#include "Tools/params.h"
#include "Tools/Arguments_reader.hpp"
#include "Simulation/Simulation.hpp"

template <typename B, typename R, typename Q>
class Launcher
{
private:
	int max_n_characters;

protected:
	std::unordered_map<std::type_index,std::string> type_names;

	std::map<std::vector<std::string>, std::vector<std::string>> req_args;
	std::map<std::vector<std::string>, std::vector<std::string>> opt_args;

	Arguments_reader ar;

	parameters params;

	Simulation *simu;
	std::ostream &stream;
	
public:
	Launcher(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher();

	void launch();

protected:
	virtual void build_args    ();
	virtual void store_args    ();
	        void read_arguments();

	virtual std::vector<std::vector<std::string>> header_simulation ();
	virtual std::vector<std::vector<std::string>> header_code       ();
	virtual std::vector<std::vector<std::string>> header_source     ();
	virtual std::vector<std::vector<std::string>> header_crc        ();
	virtual std::vector<std::vector<std::string>> header_encoder    ();
	virtual std::vector<std::vector<std::string>> header_puncturer  ();
	virtual std::vector<std::vector<std::string>> header_interleaver();
	virtual std::vector<std::vector<std::string>> header_modulator  ();
	virtual std::vector<std::vector<std::string>> header_channel    ();
	virtual std::vector<std::vector<std::string>> header_demodulator();
	virtual std::vector<std::vector<std::string>> header_depuncturer();
	virtual std::vector<std::vector<std::string>> header_quantizer  ();
	virtual std::vector<std::vector<std::string>> header_decoder    ();
	virtual std::vector<std::vector<std::string>> header_monitor    ();
	virtual std::vector<std::vector<std::string>> header_terminal   ();

	void print_parameters(std::string grp_name, std::vector<std::vector<std::string>> params);

	void print_header();

	virtual void build_simu() = 0;

private:
	void compute_max_n_characters();
};

#endif /* LAUNCHER_HPP_ */
