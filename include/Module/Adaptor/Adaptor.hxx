#include <cmath>
#include <string>
#include <sstream>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Module/Adaptor/Adaptor.hpp"

namespace aff3ct
{
namespace module
{

Adaptor
::Adaptor(const size_t n_elmts,
          const std::type_index datatype,
          const size_t buffer_size)
: Module(),
  n_elmts(1, n_elmts),
  n_bytes(1, tools::compute_bytes(n_elmts, datatype)),
  datatype(1, datatype),
  buffer_size(buffer_size),
  n_sockets(1),
  id(0),
  cur_id(0),
  buffer(new std::vector<std::vector<std::vector<int8_t*>>>
      (1, std::vector<std::vector<int8_t*>>(1,
          std::vector<int8_t*>(buffer_size)))),
  first(new std::vector<std::atomic<uint64_t>>(1000)),
  last(new std::vector<std::atomic<uint64_t>>(1000)),
  waiting_canceled(new std::atomic<bool>(false)),
  no_copy_pull(false),
  no_copy_push(false)
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

	for (size_t s = 0; s < this->n_sockets; s++)
		for (size_t b = 0; b < this->buffer_size; b++)
		{
			(*this->buffer)[this->id][s][b] = new int8_t[this->n_frames * this->n_bytes[s]];
			this->buffer_to_free.push_back((*this->buffer)[this->id][s][b]);
		}

	for (auto &a : *this->first.get()) a = 0;
	for (auto &a : *this->last .get()) a = 0;

	this->tasks_with_nullptr.resize((size_t)adp::tsk::SIZE);
}

Adaptor
::Adaptor(const std::vector<size_t> &n_elmts,
          const std::vector<std::type_index> &datatype,
          const size_t buffer_size)
: Module(),
  n_elmts(n_elmts),
  n_bytes(tools::compute_bytes(n_elmts, datatype)),
  datatype(datatype),
  buffer_size(buffer_size),
  n_sockets(n_elmts.size()),
  id(0),
  cur_id(0),
  buffer(new std::vector<std::vector<std::vector<int8_t*>>>
  	(1, std::vector<std::vector<int8_t*>>(n_sockets,
  		std::vector<int8_t*>(buffer_size)))),
  first(new std::vector<std::atomic<uint64_t>>(1000)),
  last(new std::vector<std::atomic<uint64_t>>(1000)),
  waiting_canceled(new std::atomic<bool>(false)),
  no_copy_pull(false),
  no_copy_push(false)
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

	if (n_elmts.size() == 0)
	{
		std::stringstream message;
		message << "'n_elmts.size()' has to be greater than 0 ('n_elmts.size()' = " << n_elmts.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t e = 0; e < n_elmts.size(); e++)
	{
		if (n_elmts[e] == 0)
		{
			std::stringstream message;
			message << "'n_elmts[e]' has to be greater than 0 ('e' = " << e << ", 'n_elmts[e]' = "
			        << n_elmts[e] << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	for (size_t s = 0; s < this->n_sockets; s++)
		for (size_t b = 0; b < this->buffer_size; b++)
		{
			(*this->buffer)[this->id][s][b] = new int8_t[this->n_frames * this->n_bytes[s]];
			this->buffer_to_free.push_back((*this->buffer)[this->id][s][b]);
		}

	for (auto &a : *this->first.get()) a = 0;
	for (auto &a : *this->last .get()) a = 0;

	this->tasks_with_nullptr.resize((size_t)adp::tsk::SIZE);
}

size_t Adaptor
::get_n_elmts(const size_t sid) const
{
	return this->n_elmts[sid];
}

size_t Adaptor
::get_n_bytes(const size_t sid) const
{
	return this->n_bytes[sid];
}

std::type_index Adaptor
::get_datatype(const size_t sid) const
{
	return this->datatype[sid];
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
