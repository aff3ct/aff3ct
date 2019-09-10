#ifndef ARGUMENT_TYPE_FILE_SYSTEM_HPP_
#define ARGUMENT_TYPE_FILE_SYSTEM_HPP_

#include <string>
#include <iostream>
#include <stdexcept>

#include "Tools/system_functions.h"
#include "Tools/version.h"

#include "Tools/Arguments/Types/Argument_type_limited.hpp"

namespace aff3ct
{
namespace tools
{

enum class openmode : uint8_t {read, write, read_write};

template <typename Read_F>
std::string modify_path(const std::string& val)
{
	std::string binary_path = get_binary_path();
	if (!binary_path.empty())
	{
		std::string basedir, filename;
		split_path(binary_path, basedir, filename);

		std::string aff3ct_version = aff3ct::version();
		if (!aff3ct_version.empty() && aff3ct_version[0] == 'v')
			aff3ct_version.erase(0, 1); // rm the 'v'

		std::vector<std::string> paths = {
			"../../",
			"../../../",
			"../share/aff3ct-" + aff3ct_version + "/",
			"../../share/aff3ct-" + aff3ct_version + "/",
			"/usr/share/aff3ct-" + aff3ct_version + "/",
			"/usr/share/aff3ct-" + aff3ct_version + "/",
			"/usr/local/share/aff3ct-" + aff3ct_version + "/",
			"../share/aff3ct/",
			"../../share/aff3ct/",
			"/usr/share/aff3ct/",
			"/usr/local/share/aff3ct/",
		};

		for (auto &path : paths)
		{
			std::string full_path = (path[0] != '/') ? basedir + "/" : "";
			full_path += path + val;
#if defined(_WIN32) || defined(_WIN64)
			if (path[0] == '/')
				continue;
			std::replace(full_path.begin(), full_path.end(), '/', '\\');
#endif
			if (Read_F::check(full_path))
				return full_path;
		}
	}

	return "";
}

std::string openmode_to_string(const openmode& mode);

struct Is_file
{
	static bool check(const std::string& filename);
};

struct Is_folder
{
	static bool check(const std::string& foldername);
};

struct Is_path
{
	static bool check(const std::string& path);
};

struct No_check
{
	static bool check(const std::string&);
};

template <typename T = std::string, typename Read_F = No_check, typename Write_F = No_check, typename RW_F = No_check, typename... Ranges>
class File_system_type : public Argument_type_limited<T,Ranges...>
{
protected:
	const std::string name;
	const openmode    mode;

public:
	template <typename r, typename... R>
	explicit File_system_type(const std::string& name, const openmode& mode, const r* range, const R*... ranges)
	: Argument_type_limited<T,Ranges...>(generate_title(name, mode), range, ranges...), name(name), mode(mode)
	{ }

	explicit File_system_type(const std::string& name, const openmode& mode)
	: Argument_type_limited<T,Ranges...>(generate_title(name, mode)), name(name), mode(mode)
	{ }

	virtual ~File_system_type() {};

	virtual File_system_type<T, Read_F, Write_F, RW_F, Ranges...>* clone() const
	{
		auto* clone = new File_system_type<T, Read_F, Write_F, RW_F, Ranges...>(name, mode);

		return dynamic_cast<File_system_type<T, Read_F, Write_F, RW_F, Ranges...>*>(this->clone_ranges(clone));
	}

	template <typename... NewRanges>
	File_system_type<T, Read_F, Write_F, RW_F, Ranges..., NewRanges...>* clone(NewRanges*... new_ranges)
	{
		auto* clone = new File_system_type<T, Read_F, Write_F, RW_F, Ranges..., NewRanges...>(name, mode);

		this->clone_ranges(clone);

		clone->add_ranges(new_ranges...);

		return clone;
	}

	static std::string generate_title(const std::string& name, const openmode& mode)
	{
		return name + " [" + openmode_to_string(mode) + "]";
	}

	virtual T convert(const std::string& val) const
	{
		return val;
	}

	virtual void check(const std::string& val) const
	{
		auto str_val = this->convert(val);

		if (str_val.empty())
			throw std::runtime_error("shall be a " + name + " name");

		switch(mode)
		{
			case openmode::read :
				if(!Read_F::check(str_val))
					if (modify_path<Read_F>(str_val).empty())
						throw std::runtime_error("does not name an existing " + name);
				break;

			case openmode::write :
				if(!Write_F::check(str_val))
					throw std::runtime_error("does not name a " + name);
				break;

			case openmode::read_write : // nothing to check
				if(!RW_F::check(str_val))
					throw std::runtime_error("does not name a " + name);
				break;
		}

		this->check_ranges(str_val);
	}
};

}
}

#include "File.hpp"
#include "Folder.hpp"
#include "Path.hpp"

#endif /* ARGUMENT_TYPE_FILE_SYSTEM_HPP_ */