#ifndef ALIST_READER_HPP_
#define ALIST_READER_HPP_

#include <fstream>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace tools
{
class AList_reader
{
private:
	unsigned int n_VN;
	unsigned int n_CN;
	unsigned int VN_max_degree;
	unsigned int CN_max_degree;
	unsigned int n_branches;

	std::vector<std::vector<unsigned int>> VN_to_CN;
	std::vector<std::vector<unsigned int>> CN_to_VN;
	mipp::vector<unsigned int> branches_transpose;

public:
	AList_reader(std::string filename);
	virtual ~AList_reader();

	unsigned int get_n_VN() const;
	unsigned int get_n_CN() const;
	unsigned int get_VN_max_degree() const;
	unsigned int get_CN_max_degree() const;
	unsigned int get_n_branches() const;

	const std::vector<std::vector<unsigned int>>& get_VN_to_CN() const;
	const std::vector<std::vector<unsigned int>>& get_CN_to_VN() const;

	      mipp::vector<unsigned char>  get_n_VN_per_CN       () const;
	      mipp::vector<unsigned char>  get_n_CN_per_VN       () const;
	      mipp::vector<unsigned int >  get_linear_VN_to_CN   () const;
	      mipp::vector<unsigned int >  get_linear_CN_to_VN   () const;
	const mipp::vector<unsigned int >& get_branches_transpose() const;

private:
	bool read_format1(std::ifstream &file);
	bool read_format2(std::ifstream &file);
	bool read_format3(std::ifstream &file);
	void compute_branches_transpose();
};
}
}

#endif /* ALIST_READER_HPP_ */
