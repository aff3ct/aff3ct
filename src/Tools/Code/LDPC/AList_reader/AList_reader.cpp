#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "AList_reader.hpp"

std::vector<std::string> split(const std::string &s)
{
	std::string buf;                 // have a buffer string
	std::stringstream ss(s);         // insert the string into a stream
	std::vector<std::string> tokens; // create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;
}

bool getline(std::ifstream &file, std::string &line)
{
	while (std::getline(file, line))
		if (line[0] != '#' && !std::all_of(line.begin(),line.end(),isspace))
			break;
	if (file.eof() || file.fail() || file.bad())  return false;

	return true;
}

AList_reader
::AList_reader(std::string filename)
: n_VN(0), n_CN(0), VN_max_degree(0), CN_max_degree(0), n_branches(0), VN_to_CN(), CN_to_VN()
{
	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		auto success = this->read_format1(file);
		file.close();

		if (!success)
		{
			file.open(filename.c_str(), std::ios::in);
			success = this->read_format2(file);
			file.close();

			if (!success)
			{
				file.open(filename.c_str(), std::ios::in);
				success = this->read_format3(file);
				file.close();

				if (!success)
				{
					std::cerr << bold_red("(EE) The \"")
					          << bold_red(filename)
					          << bold_red("\" file is not supported by ")
					          << bold_red("the AList reader, exiting.")
					          << std::endl;
					std::exit(-1);
				}
			}
		}
	}
	else
	{
		std::cerr << bold_red("(EE) Can't open \"") << bold_red(filename) << bold_red("\" file, exiting.") << std::endl;
		std::exit(-1);
	}
}

// perfect AList format
bool AList_reader
::read_format1(std::ifstream &file)
{
	file >> this->n_VN;
	file >> this->n_CN;
	file >> this->VN_max_degree;
	file >> this->CN_max_degree;

	if (this->n_VN          > 0 &&
	    this->n_CN          > 0 &&
	    this->VN_max_degree > 0 &&
	    this->CN_max_degree > 0)
	{
		this->VN_to_CN.resize(this->n_VN);
		for (auto i = 0; i < (int)this->n_VN; i++)
		{
			int n_connections = 0;
			file >> n_connections;

			if (n_connections > 0)
			{
				this->VN_to_CN[i].resize(n_connections);
				this->n_branches += n_connections;
			}
			else
				return false;
		}

		this->CN_to_VN.resize(this->n_CN);
		for (auto i = 0; i < (int)this->n_CN; i++)
		{
			int n_connections = 0;
			file >> n_connections;

			if (n_connections > 0)
				this->CN_to_VN[i].resize(n_connections);
			else
				return false;
		}

		for (auto i = 0; i < (int)this->n_VN; i++)
		{
			for (auto j = 0; j < (int)this->VN_max_degree; j++)
			{
				int C_node_id = 0;
				file >> C_node_id;

				if ((C_node_id >  0 && j <  (int)this->VN_to_CN[i].size()) ||
					(C_node_id <= 0 && j >= (int)this->VN_to_CN[i].size()))
				{
					if (C_node_id)
						this->VN_to_CN[i][j] = C_node_id -1;
				}
				else
					return false;
			}
		}

		for (auto i = 0; i < (int)this->n_CN; i++)
		{
			for (auto j = 0; j < (int)this->CN_max_degree; j++)
			{
				int V_node_id = 0;
				file >> V_node_id;

				if ((V_node_id >  0 && j <  (int)this->CN_to_VN[i].size()) ||
					(V_node_id <= 0 && j >= (int)this->CN_to_VN[i].size()))
				{
					if (V_node_id)
						this->CN_to_VN[i][j] = V_node_id -1;
				}
				else
					return false;
			}
		}
	}
	else
		return false;

	return true;
}

// format with comments or/and no padding with 0
bool AList_reader
::read_format2(std::ifstream &file)
{
	std::string line;

	if (!getline(file, line)) return false;
	auto values = split(line);
	if (values.size() < 2) return false;

	this->n_VN = std::stoi(values[0]);
	this->n_CN = std::stoi(values[1]);

	if (this->n_VN <= 0 || this->n_CN <= 0) return false;

	if (!getline(file, line)) return false;
	values = split(line);
	if (values.size() < 2) return false;

	this->VN_max_degree = std::stoi(values[0]);
	this->CN_max_degree = std::stoi(values[1]);

	if (this->VN_max_degree <= 0 || this->CN_max_degree <= 0) return false;

	if (!getline(file, line)) return false;
	values = split(line);
	if (values.size() < this->n_VN) return false;

	this->VN_to_CN.resize(this->n_VN);
	for (auto i = 0; i < (int)this->n_VN; i++)
	{
		auto n_connections = std::stoi(values[i]);
		if (n_connections > 0)
		{
			this->VN_to_CN[i].resize(n_connections);
			this->n_branches += n_connections;
		}
		else
			return false;
	}

	if (!getline(file, line)) return false;
	values = split(line);
	if (values.size() < this->n_CN) return false;

	this->CN_to_VN.resize(this->n_CN);
	for (auto i = 0; i < (int)this->n_CN; i++)
	{
		auto n_connections = std::stoi(values[i]);
		if (n_connections > 0)
			this->CN_to_VN[i].resize(n_connections);
		else
			return false;
	}

	for (auto i = 0; i < (int)this->n_VN; i++)
	{
		if (!getline(file, line)) return false;
		values = split(line);

		if (values.size() < this->VN_to_CN[i].size()) return false;

		for (auto j = 0; j < (int)this->VN_max_degree; j++)
		{
			auto C_node_id = (j < (int)values.size()) ? std::stoi(values[j]) : 0;
			if ((C_node_id >  0 && j <  (int)this->VN_to_CN[i].size()) ||
				(C_node_id <= 0 && j >= (int)this->VN_to_CN[i].size()))
			{
				if (C_node_id)
					this->VN_to_CN[i][j] = C_node_id -1;
			}
			else
				return false;
		}
	}

	for (auto i = 0; i < (int)this->n_CN; i++)
	{
		if (!getline(file, line)) return false;
		values = split(line);

		if (values.size() < this->CN_to_VN[i].size()) return false;

		for (auto j = 0; j < (int)this->CN_max_degree; j++)
		{
			auto V_node_id = (j < (int)values.size()) ? std::stoi(values[j]) : 0;
			if ((V_node_id >  0 && j <  (int)this->CN_to_VN[i].size()) ||
				(V_node_id <= 0 && j >= (int)this->CN_to_VN[i].size()))
			{
				if (V_node_id)
					this->CN_to_VN[i][j] = V_node_id -1;
			}
			else
				return false;
		}
	}

	return true;
}

// format ANR NAND
bool AList_reader
::read_format3(std::ifstream &file)
{
	std::string line;

	if (!getline(file, line)) return false;
	auto values = split(line);
	if (values.size() < 2) return false;

	this->n_CN = std::stoi(values[0]);
	this->n_VN = std::stoi(values[1]);

	if (this->n_VN <= 0 || this->n_CN <= 0) return false;

	if (!getline(file, line)) return false;
	values = split(line);
	if (values.size() < this->n_CN) return false;

	this->CN_max_degree = 0;
	this->CN_to_VN.resize(this->n_CN);
	for (auto i = 0; i < (int)this->n_CN; i++)
	{
		auto n_connections = std::stoi(values[i]);
		this->CN_max_degree = std::max(this->CN_max_degree, (unsigned)n_connections);
		if (n_connections > 0)
		{
			this->CN_to_VN[i].resize(n_connections);
			this->n_branches += n_connections;
		}
		else
			return false;
	}

	this->VN_to_CN.resize(this->n_VN);
	for (auto i = 0; i < (int)this->n_CN; i++)
	{
		if (!getline(file, line)) return false;
		values = split(line);

		if (values.size() < this->CN_to_VN[i].size()) return false;

		for (auto j = 0; j < (int)this->CN_to_VN[i].size(); j++)
		{
			auto V_node_id = std::stoi(values[j]);
			this->CN_to_VN[i][j] = V_node_id;

			this->VN_to_CN[V_node_id].push_back(i);
		}
	}

	// find the VN max degree
	this->VN_max_degree = 0;
	for (auto i = 0; i < (int)this->n_VN; i++)
		this->VN_max_degree = std::max(this->VN_max_degree, (unsigned)this->VN_to_CN[i].size());

	return true;
}

AList_reader
::~AList_reader()
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
