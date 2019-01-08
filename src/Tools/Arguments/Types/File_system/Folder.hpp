#ifndef ARGUMENT_TYPE_FOLDER_HPP_
#define ARGUMENT_TYPE_FOLDER_HPP_

#include "File_system.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = std::string, typename... Ranges>
class Folder_type : public File_system_type<T, Is_folder, No_check, No_check, Ranges...>
{
public:
	using parent_type = File_system_type<T, Is_folder, No_check, No_check, Ranges...>;

	template <typename r, typename... R>
	explicit Folder_type(const openmode& mode, const r* range, const R*... ranges)
	: parent_type("folder", mode, range, ranges...)
	{ }

	explicit Folder_type(const openmode& mode)
	: parent_type("folder", mode)
	{ }

	virtual ~Folder_type() {};
};

template <typename T = std::string, typename... Ranges>
typename Folder_type<T,Ranges...>::parent_type* Folder(openmode mode, Ranges*... ranges)
{
	return new Folder_type<T,Ranges...>(mode, ranges...);
}

}
}
#endif /* ARGUMENT_TYPE_FOLDER_HPP_ */