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
protected:
	std::unordered_map<std::type_index,std::string> type_names;

	std::map<std::string, std::string> req_args;
	std::map<std::string, std::string> opt_args;
	std::map<std::string, std::string> doc_args;

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
	virtual void print_header  ();
	virtual void build_simu    () = 0;
};

#endif /* LAUNCHER_HPP_ */
