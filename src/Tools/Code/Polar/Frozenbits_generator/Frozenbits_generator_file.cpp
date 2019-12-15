#include <fstream>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"

using namespace aff3ct::tools;

Frozenbits_generator_file
::Frozenbits_generator_file(const int K, const int N, const std::string& filename)
: Frozenbits_generator(K, N), filename(filename)
{
}

Frozenbits_generator_file
::Frozenbits_generator_file(const int K, const int N)
: Frozenbits_generator(K, N), filename("")
{
}

Frozenbits_generator_file* Frozenbits_generator_file
::clone() const
{
	auto t = new Frozenbits_generator_file(*this);
	return t;
}

void Frozenbits_generator_file
::evaluate()
{
	if(!load_channels_file(this->filename, this->best_channels))
		throw invalid_argument(__FILE__, __LINE__, __func__, "'" + filename + "' file does not exist.");
}

bool Frozenbits_generator_file
::load_channels_file(const std::string& filename, std::vector<uint32_t>& best_channels)
{
	std::ifstream in_code(filename.c_str());

	if (in_code.is_open())
	{
		std::string trash;
		in_code >> trash; // N

		try
		{
			std::stoi(trash);
		}
		catch(std::exception&)
		{
			std::stringstream message;
			message << "'std::stoi' did not work, something went wrong when reading the file.";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if ((size_t)std::stoi(trash) != best_channels.size())
		{
			std::stringstream message;
			message << "'trash' has to be equal to 'N' ('trash' = " << trash << ", 'N' = " << this->N << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		in_code >> trash; // type
		in_code >> trash; // sigma

		for (unsigned i = 0; i < best_channels.size(); i++)
			in_code >> best_channels[i];

		in_code.close();
		return true;
	}
	else
		return false;
}

void Frozenbits_generator_file
::check_noise()
{
	Frozenbits_generator::check_noise();
}