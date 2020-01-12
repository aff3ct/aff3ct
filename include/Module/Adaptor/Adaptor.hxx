#include <cmath>
#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Adaptor/Adaptor.hpp"

namespace aff3ct
{
namespace module
{

Adaptor
::Adaptor(const size_t n_elmts,
          const std::type_index datatype,
          const size_t buffer_size,
          const int n_frames)
: Module(n_frames),
  n_elmts(n_elmts),
  n_bytes(compute_bytes(n_elmts, datatype)),
  datatype(datatype),
  buffer_size(buffer_size),
  id(0),
  cur_id(0),
  buffer(new std::vector<std::vector<std::vector<int8_t>>>
  	(1, std::vector<std::vector<int8_t>>(buffer_size, std::vector<int8_t>(n_frames * n_bytes)))),
  first(new std::vector<std::atomic<size_t>>(1000)),
  last(new std::vector<std::atomic<size_t>>(1000)),
  waiting_canceled(new std::atomic<bool>(false))
{
	const std::string name = "Adaptor";
	this->set_name(name);
	this->set_short_name(name);

	if (buffer_size == 0)
	{
		std::stringstream message;
		message << "'buffer_size' has to be greater than 0 ('buffer_size' = " << buffer_size << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0 ('n_elmts' = " << n_elmts << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto &a : *this->first.get()) a = 0;
	for (auto &a : *this->last .get()) a = 0;

	this->tasks_with_nullptr.resize((size_t)adp::tsk::SIZE);
}

size_t Adaptor
::get_n_elmts() const
{
	return this->n_elmts;
}

size_t Adaptor
::get_n_bytes() const
{
	return this->n_bytes;
}

std::type_index Adaptor
::get_datatype() const
{
	return this->datatype;
}

bool Adaptor
::is_empty(const size_t id)
{
	return (*this->first)[id] == (*this->last)[id];
}

bool Adaptor
::is_full(const size_t id)
{
	// return (size_t)std::abs((int)(*this->last)[id] - (int)(*this->first)[id]) == this->buffer_size;
	return !this->n_free_slots(id);
}

size_t Adaptor
::n_fill_slots(const size_t id)
{
	return (*this->last)[id] - (*this->first)[id];
}

size_t Adaptor
::n_free_slots(const size_t id)
{
	return this->buffer_size - this->n_fill_slots(id);
}

}
}
