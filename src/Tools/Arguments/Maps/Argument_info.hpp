#ifndef ARGUMENT_INFO_HPP_
#define ARGUMENT_INFO_HPP_

#include <string>

#include "../Types/Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

struct Argument_info
{
	enum class Rank {OPTIONAL, REQUIRED, ADVANCED};

    Argument_info();
    Argument_info(Argument_type* type, const std::string& doc, const Rank rank = Rank::OPTIONAL);

	virtual ~Argument_info();

	virtual void clear();

	virtual Argument_info* clone() const;

	void change_type(Argument_type* new_type);
	void change_doc (const std::string& new_doc);
	void add_doc    (const std::string& new_doc);

	Argument_type* type = nullptr;
	std::string    doc  = "";
	Rank           rank = Rank::OPTIONAL;
};

}
}

#endif /* ARGUMENT_INFO_HPP_ */
