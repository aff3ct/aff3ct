#include <fstream>

#include "AList_reader.hpp"
#include "Tools/Display/bash_tools.h"

AList_reader::
AList_reader(std::string filename)
: n_VN(0), n_CN(0), VN_max_degree(0), CN_max_degree(0), n_branches(0), VN_to_CN(), CN_to_VN()
{
	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		file >> this->n_VN;
		file >> this->n_CN;
		file >> this->VN_max_degree;
		file >> this->CN_max_degree;

		assert(this->n_VN          > 0 &&
		       this->n_CN          > 0 &&
		       this->VN_max_degree > 0 &&
		       this->CN_max_degree > 0);

		this->VN_to_CN.resize(this->n_VN);
		for (auto i = 0; i < (int)this->n_VN; i++)
		{
			int n_connections = 0;
			file >> n_connections;

			assert(n_connections > 0);

			this->VN_to_CN[i].resize(n_connections);
			this->n_branches += n_connections;
		}

		this->CN_to_VN.resize(this->n_CN);
		for (auto i = 0; i < (int)this->n_CN; i++)
		{
			int n_connections = 0;
			file >> n_connections;

			assert(n_connections > 0);

			this->CN_to_VN[i].resize(n_connections);
		}

		for (auto i = 0; i < (int)this->n_VN; i++)
		{
			for (auto j = 0; j < (int)this->VN_max_degree; j++)
			{
				int C_node_id = 0;
				file >> C_node_id;

				assert((C_node_id >  0 && j <  (int)this->VN_to_CN[i].size()) ||
				       (C_node_id <= 0 && j >= (int)this->VN_to_CN[i].size()));

				if (C_node_id)
					this->VN_to_CN[i][j] = C_node_id -1;
			}
		}

		for (auto i = 0; i < (int)this->n_CN; i++)
		{
			for (auto j = 0; j < (int)this->CN_max_degree; j++)
			{
				int V_node_id = 0;
				file >> V_node_id;

				assert((V_node_id >  0 && j <  (int)this->CN_to_VN[i].size()) ||
				       (V_node_id <= 0 && j >= (int)this->CN_to_VN[i].size()));

				if (V_node_id)
					this->CN_to_VN[i][j] = V_node_id -1;
			}
		}

		file.close();
	}
	else
	{
		std::cerr << bold_red("(EE) Can't open \"") << bold_red(filename) << bold_red("\" file, exiting.")
		          << std::endl;
		std::exit(-1);
	}
}

AList_reader::
~AList_reader()
{
}

unsigned int AList_reader
::get_n_VN() const
{
	return this->n_VN;
}

unsigned int AList_reader
::get_n_CN() const
{
	return this->n_CN;
}

unsigned int AList_reader
::get_VN_max_degree() const
{
	return this->VN_max_degree;
}

unsigned int AList_reader
::get_CN_max_degree() const
{
	return this->CN_max_degree;
}

unsigned int AList_reader
::get_n_branches() const
{
	return this->n_branches;
}

const std::vector<std::vector<unsigned int>>& AList_reader
::get_VN_to_CN() const
{
	return this->VN_to_CN;
}

const std::vector<std::vector<unsigned int>>& AList_reader
::get_CN_to_VN() const
{
	return this->CN_to_VN;
}

mipp::vector<unsigned char> AList_reader
::get_n_CN_per_VN() const
{
	mipp::vector<unsigned char> n_C_per_V(this->VN_to_CN.size());

	for (auto i = 0; i < (int)this->VN_to_CN.size(); i++)
		n_C_per_V[i] = (unsigned char)this->VN_to_CN[i].size();

	return n_C_per_V;
}

mipp::vector<unsigned char> AList_reader
::get_n_VN_per_CN() const
{
	mipp::vector<unsigned char> n_V_per_C((int)this->CN_to_VN.size());

	for (auto i = 0; i < (int)this->CN_to_VN.size(); i++)
		n_V_per_C[i] = (unsigned char)this->CN_to_VN[i].size();

	return n_V_per_C;
}

mipp::vector<unsigned int> AList_reader
::get_linear_VN_to_CN() const
{
	mipp::vector<unsigned int> linear_V_to_C(this->n_branches);

	auto k = 0;
	for (auto i = 0; i < (int)this->VN_to_CN.size(); i++)
		for (auto j = 0; j < (int)this->VN_to_CN[i].size(); j++)
			linear_V_to_C[k++] = this->VN_to_CN[i][j];

	return linear_V_to_C;
}

mipp::vector<unsigned int> AList_reader
::get_linear_CN_to_VN() const
{
	mipp::vector<unsigned int> linear_C_to_V(this->n_branches);

	auto k = 0;
	for (auto i = 0; i < (int)this->CN_to_VN.size(); i++)
		for (auto j = 0; j < (int)this->CN_to_VN[i].size(); j++)
			linear_C_to_V[k++] = this->CN_to_VN[i][j];

	return linear_C_to_V;
}

mipp::vector<unsigned int> AList_reader
::get_branches_transpose() const
{
	mipp::vector<unsigned int> transpose(this->n_branches);

	mipp::vector<unsigned char> connections(VN_to_CN.size(), 0);

	auto k = 0;
	for (auto i = 0; i < (int)CN_to_VN.size(); i++)
	{
		for (auto j = 0; j < (int)CN_to_VN[i].size(); j++)
		{
			auto id_V = CN_to_VN[i][j];

			auto branch_id = 0;
			for (auto ii = 0; ii < (int)id_V; ii++)
				branch_id += (int)VN_to_CN[ii].size();
			branch_id += connections[id_V];
			connections[id_V]++;

			assert(connections[id_V] <= (int)VN_to_CN[id_V].size());

			transpose[k] = branch_id;
			k++;
		}
	}

	return transpose;
}
