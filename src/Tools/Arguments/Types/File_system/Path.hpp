#ifndef ARGUMENT_TYPE_PATH_HPP_
#define ARGUMENT_TYPE_PATH_HPP_

#include "File_system.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = std::string, typename... Ranges>
class Path_type : public File_system_type<T, Is_path, No_check, No_check, Ranges...>
{
public:
	using parent_type = File_system_type<T, Is_path, No_check, No_check, Ranges...>;

	template <typename r, typename... R>
	explicit Path_type(const openmode& mode, const r* range, const R*... ranges)
	: parent_type("path", mode, range, ranges...)
	{ }

	explicit Path_type(const openmode& mode)
	: parent_type("path", mode)
	{ }

	virtual ~Path_type() {};
};

template <typename T = std::string, typename... Ranges>
typename Path_type<T,Ranges...>::parent_type* Path(openmode mode, Ranges*... ranges)
{
	return new Path_type<T,Ranges...>(mode, ranges...);
}

}
}
#endif /* ARGUMENT_TYPE_PATH_HPP_ */