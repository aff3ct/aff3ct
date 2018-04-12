#ifndef ARGUMENT_TYPE_FILE_SYSTEM_HPP_
#define ARGUMENT_TYPE_FILE_SYSTEM_HPP_

#include <string>
#include <stdexcept>

#include "../Argument_type_limited.hpp"

namespace aff3ct
{
namespace tools
{

enum class openmode : uint8_t {read, write, read_write};

std::string openmode_to_string(const openmode& mode);

struct isFile
{
	static bool check(const std::string& filename);
};

struct isFolder
{
	static bool check(const std::string& foldername);
};

struct isPath
{
	static bool check(const std::string& path);
};

struct noCheck
{
	static bool check(const std::string&);
};


template <typename T = std::string, typename Read_F = noCheck, typename Write_F = noCheck, typename RW_F = noCheck, typename... Ranges>
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