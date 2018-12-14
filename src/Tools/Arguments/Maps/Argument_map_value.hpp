#ifndef ARGUMENT_MAP_VALUE_HPP_
#define ARGUMENT_MAP_VALUE_HPP_

#include <string>
#include <vector>
#include <stdexcept>
#include <map>

#include "Argument_info.hpp"
#include "Argument_tag.hpp"

namespace aff3ct
{
namespace tools
{

class Argument_map_value : public std::map<Argument_tag, std::pair<std::string, Argument_info*>>
{
public:
	using mother_t = std::map<Argument_tag, std::pair<std::string, Argument_info*>>;

public:

	bool exist(const Argument_tag &tags) const;

	/*!
	 * \brief Returns the value for an argument.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}
	 *
	 * \return the integer value of an argument with its tags (to use after the parse_arguments method).
	 */
	int to_int(const Argument_tag &tags) const;

	/*!
	 * \brief Returns the value for an argument.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return the floating-point value of an argument with its tags (to use after the parse_arguments method).
	 */
	float to_float(const Argument_tag &tags) const;

	/*!
	 * \brief Returns the file path (can be modified).
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return the file path (can be modified).
	 */
	std::string to_file(const Argument_tag &tags) const;

	/*!
	 * \brief Returns the path (can be modified).
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return the path (can be modified).
	 */
	std::string to_path(const Argument_tag &tags) const;

	/*!
	 * \brief Returns the folder path (can be modified).
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return the folder path (can be modified).
	 */
	std::string to_folder(const Argument_tag &tags) const;

	/*!
	 * \brief Returns the list of values for an argument.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return the  value of an argument with its tags (to use after the parse_arguments method).
	 */
	template <typename T>
	std::vector<T> to_list(const Argument_tag &tags) const
	{
		std::vector<T> list;

		try
		{
			auto& val_info_pair = mother_t::at(tags);

			auto* p_list = (std::vector<T>*)val_info_pair.second->type->get_val(val_info_pair.first);

			if (p_list != nullptr)
			{
				list.swap(*p_list);
				delete p_list;
			}
		}
		catch (std::exception&)
		{
		}

		return list;
	}

	std::string at(const Argument_tag &tags) const
	{
		return mother_t::at(tags).first;
	}
};

}
}

#endif /* ARGUMENT_MAP_VALUE_HPP_ */
