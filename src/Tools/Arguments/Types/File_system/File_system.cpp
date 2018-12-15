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

bool aff3ct::tools::Is_file::check(const std::string& filename)
{
	std::ifstream f(filename);

	DIR *dp = opendir(filename.c_str());

	// if filename is a directory return false
	if (dp != nullptr)
	{
		closedir(dp);
		return false;
	}

	return f.good();
}

bool aff3ct::tools::Is_folder::check(const std::string& foldername)
{
	DIR *dp = opendir(foldername.c_str());

	if (dp != nullptr)
	{
		closedir(dp);
		return true;
	}

	return false;
}

bool aff3ct::tools::Is_path::check(const std::string& path)
{
	return Is_file::check(path) || Is_folder::check(path);
}

bool aff3ct::tools::No_check::check(const std::string&)
{
	return true;
}