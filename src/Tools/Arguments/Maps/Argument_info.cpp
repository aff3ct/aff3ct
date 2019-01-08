#include "Argument_info.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Argument_info
::Argument_info()
{ }

Argument_info
::Argument_info(Argument_type* type, const std::string& doc, const arg_rank rank, const std::string key)
: type(type), doc(doc), rank(rank), key(key)
{}


Argument_info
::~Argument_info()
{
	clear();
}

void Argument_info
::clear()
{
	if (type != nullptr)
		delete type;

	type = nullptr;
}

void Argument_info
::change_type(Argument_type* new_type)
{
	if (type != nullptr)
		delete type;

	type = new_type;
}

void Argument_info
::change_doc(const std::string& new_doc)
{
	doc = new_doc;
}

void Argument_info
::add_doc(const std::string& new_doc)
{
	doc += new_doc;
}

Argument_info* Argument_info
::clone() const
{
		Argument_type* arg_t = nullptr;

		if (type != nullptr)
			arg_t = type->clone();

		return new Argument_info(arg_t, doc, rank);
}

