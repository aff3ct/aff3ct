#ifndef ALIST_READER_HPP_
#define ALIST_READER_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

class AList_reader
{
private:
	unsigned int n_VN;
	unsigned int n_CN;
	unsigned int VN_max_degree;
	unsigned int CN_max_degree;
	unsigned int n_branches;

	mipp::vector<mipp::vector<unsigned int>> VN_to_CN;
	mipp::vector<mipp::vector<unsigned int>> CN_to_VN;

public:
	AList_reader(std::string filename);
	virtual ~AList_reader();

	unsigned int get_n_VN();
	unsigned int get_n_CN();
	unsigned int get_VN_max_degree();
	unsigned int get_CN_max_degree();
	unsigned int get_n_branches();

	mipp::vector<mipp::vector<unsigned int>> get_VN_to_CN();
	mipp::vector<mipp::vector<unsigned int>> get_CN_to_VN();

	mipp::vector<unsigned char> get_n_VN_per_CN       ();
	mipp::vector<unsigned char> get_n_CN_per_VN       ();
	mipp::vector<unsigned int > get_linear_VN_to_CN   ();
	mipp::vector<unsigned int > get_linear_CN_to_VN   ();
	mipp::vector<unsigned int > get_branches_transpose();
};

#endif /* ALIST_READER_HPP_ */
