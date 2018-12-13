#ifndef ARGUMENT_INFO_HPP_
#define ARGUMENT_INFO_HPP_

#include <string>

#include "../Types/Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

enum class arg_rank : uint8_t {OPT, REQ, ADV};

struct Argument_info
{
    Argument_info();
    Argument_info(Argument_type* type, const std::string& doc, const arg_rank rank = arg_rank::OPT,
                  const std::string key = "");

	virtual ~Argument_info();

	virtual void clear();

	virtual Argument_info* clone() const;

	void change_type(Argument_type* new_type);
	void change_doc (const std::string& new_doc);
	void add_doc    (const std::string& new_doc);

	Argument_type* type = nullptr;
	std::string    doc  = "";
	arg_rank       rank = arg_rank::OPT;
	std::string    key  = "";
};

}
}

#endif /* ARGUMENT_INFO_HPP_ */
