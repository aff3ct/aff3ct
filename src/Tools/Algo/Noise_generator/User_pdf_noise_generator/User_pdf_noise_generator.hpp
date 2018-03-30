#ifndef USER_PDF_NOISE_GENERATOR_HPP
#define USER_PDF_NOISE_GENERATOR_HPP

#include <sstream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

#include "Tools/Algo/Distribution/Distribution.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/Arguments/Splitter/Splitter.hpp"

#include "../Noise_generator.hpp"

namespace aff3ct
{
namespace tools
{
/*
 * Warning all contained distributions are deleted at destruction time.
 */
template <typename R = float>
class User_pdf_noise_generator : public Noise_generator<R>
{
protected:
	std::map<int, Distribution<R>*> distributions; // distributions in function of the noise power

public:
	User_pdf_noise_generator() {}

	User_pdf_noise_generator(std::ifstream& f_distributions)
	{
		std::string line;
		std::getline(f_distributions, line);
		auto v_noise_power = tools::Splitter::split(line, "", "", ",;");

		// get the x values
		std::vector<std::vector<std::string>> v_x(v_noise_power.size());
		for (unsigned i = 0; i < v_x.size(); i++)
		{
			std::getline(f_distributions, line);

			v_x[i] = tools::Splitter::split(line, "", "", ",;");

			if (i != 0 && v_x[i].size() != v_x[i-1].size())
			{
				std::stringstream message;
				message << "'v_x[" << i << "]' does not have the same size than 'v_x[" << i -1 << "]' "
				        << "('v_x[" << i << "].size()' = " << v_x[i].size() << " and 'v_x[" << i -1 << "].size()' = " << v_x[i-1].size() << ").";
				throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}

		// get the y values
		std::vector<std::vector<std::string>> v_y(v_noise_power.size());
		for (unsigned i = 0; i < v_y.size(); i++)
		{
			std::getline(f_distributions, line);

			v_y[i] = tools::Splitter::split(line, "", "", ",;");

			if (i != 0 && v_y[i].size() != v_y[i-1].size())
			{
				std::stringstream message;
				message << "'v_y[" << i << "]' does not have the same size than 'v_y[" << i -1 << "]' "
				        << "('v_y[" << i << "].size()' = " << v_y[i].size() << " and 'v_y[" << i -1 << "].size()' = " << v_y[i-1].size() << ").";
				throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}

		// construct the distributions
		for (unsigned i = 0; i < v_y.size(); i++)
		{
			std::vector<R> v_x_R(v_x[i].size());
			std::vector<R> v_y_R(v_y[i].size());

			// check x and y are of the same length
			if (v_x_R.size() != v_y_R.size())
			{
				std::stringstream message;
				message << "'v_x_R' does not have the same size than 'v_x_R' "
				        << "('v_x_R.size()' = " << v_x_R.size() << " and 'v_x_R.size()' = " << v_x_R.size() << ").";
				throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}

			// convert string vector to 'R' vector
			for(unsigned j = 0; j < v_x_R.size(); j++)
			{
				v_x_R[j] = (R)stof(v_x[i][j]);
				v_y_R[j] = (R)stof(v_y[i][j]);
			}

			auto np = (R)stof(v_noise_power[i]);

			add_distribution(np, new Distribution<R>(v_x_R, v_y_R));
		}
	}

	virtual ~User_pdf_noise_generator()
	{
		for (auto& d : distributions)
			if (d.second)
				delete d.second;
	}

	const Distribution<R>* get_distribution(const R noise_power)
	{
		int np = (int)(noise_power*1000);

		auto it_dis = this->distributions.find(np);

		if (it_dis == this->distributions.end())
			return nullptr;

		return it_dis->second;
	}

	/*
	 * Add a distribution 'new_distribution' associated with the noise power 'noise_power'.
	 */
	void add_distribution(R noise_power, Distribution<R>* new_distribution)
	{
		if (get_distribution(noise_power))
		{
			std::stringstream message;
			message << "A distribution already exist for the given noise power 'noise_power' ('noise_power' = " << noise_power << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		int np = (int)(noise_power*1000);

		this->distributions[np] = new_distribution;
	}
};

template <typename R = float>
using User_pdf_gen = User_pdf_noise_generator<R>;

}
}

#endif /* USER_PDF_NOISE_GENERATOR_HPP */
