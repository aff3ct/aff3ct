#include <fstream>
#include <dirent.h>

#include "File_system.hpp"

std::string aff3ct::tools::openmode_to_string(const openmode& mode)
{
	std::string str;

	switch(mode)
	{
		case openmode::read :
			str += "read only";
			break;

		case openmode::write :
			str += "write only";
			break;

		case openmode::read_write : // nothing to check
			str += "read/write";
			break;
	}

	return str;
}

bool aff3ct::tools::isFile::check(const std::string& filename)
{
	std::ifstream f(filename);

	auto is_good = f.good();
	std::cout << "# (DBG) filename = '" << filename << "', exist = " << is_good << std::endl;

	return f.good();
}

bool aff3ct::tools::isFolder::check(const std::string& foldername)
{
	DIR *dp = opendir(foldername.c_str());

	std::cout << "# (DBG) foldername = '" << foldername << "', exist = " << (dp != nullptr) << std::endl;

	if (dp != nullptr)
	{
		closedir(dp);
		return true;
	}

	return false;
}

bool aff3ct::tools::isPath::check(const std::string& path)
{
	return isFile::check(path) || isFolder::check(path);
}

bool aff3ct::tools::noCheck::check(const std::string&)
{
	return true;
}