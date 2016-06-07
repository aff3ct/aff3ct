#ifndef FROZENBITS_GENERATOR_FILE_HPP_
#define FROZENBITS_GENERATOR_FILE_HPP_

#include <string>
#include <vector>

#include "Frozenbits_generator.hpp"

template <typename B>
class Frozenbits_generator_file : public Frozenbits_generator<B>
{
private:
	const std::string filename;

protected:
	Frozenbits_generator_file(const int K, const int N, const float sigma = 0.f);

public:
	Frozenbits_generator_file(const int K, const int N, const std::string filename);

	virtual ~Frozenbits_generator_file();

protected:
	void evaluate();
	bool load_channels_file(const std::string& filename);

};

#endif /* FROZENBITS_GENERATOR_FILE_HPP_ */
