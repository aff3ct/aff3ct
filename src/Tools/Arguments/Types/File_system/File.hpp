#ifndef ARGUMENT_TYPE_FILE_HPP_
#define ARGUMENT_TYPE_FILE_HPP_

#include "File_system.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = std::string, typename... Ranges>
class File_type : public File_system_type<T, Is_file, No_check, No_check, Ranges...>
{
public:
	using parent_type = File_system_type<T, Is_file, No_check, No_check, Ranges...>;

	template <typename r, typename... R>
	explicit File_type(const openmode& mode, const r* range, const R*... ranges)
	: parent_type("file", mode, range, ranges...)
	{ }

	explicit File_type(const openmode& mode)
	: parent_type("file", mode)
	{ }

	virtual ~File_type() {};
};

template <typename T = std::string, typename... Ranges>
typename File_type<T,Ranges...>::parent_type* File(openmode mode, Ranges*... ranges)
{
	return new File_type<T,Ranges...>(mode, ranges...);
}

}
}
#endif /* ARGUMENT_TYPE_FILE_HPP_ */