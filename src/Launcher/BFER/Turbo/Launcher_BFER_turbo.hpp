#ifndef LAUNCHER_BFER_TURBO_HPP_
#define LAUNCHER_BFER_TURBO_HPP_

#include "../Launcher_BFER.hpp"

template <typename B, typename R, typename Q, typename QD>
class Launcher_BFER_turbo : public Launcher_BFER<B,R,Q>
{
public:
	Launcher_BFER_turbo(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_turbo() {};

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void build_simu();

	virtual std::vector<std::vector<std::string>> header_crc        ();
	virtual std::vector<std::vector<std::string>> header_encoder    ();
	virtual std::vector<std::vector<std::string>> header_interleaver();
	virtual std::vector<std::vector<std::string>> header_decoder    ();
};

#endif /* LAUNCHER_BFER_TURBO_HPP_ */
