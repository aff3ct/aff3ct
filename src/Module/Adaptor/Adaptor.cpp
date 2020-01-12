#include "Module/Adaptor/Adaptor.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Adaptor
::~Adaptor()
{
	(*this->buffer)[this->id].clear();
	(*this->first)[this->id] = 0;
	(*this->last )[this->id] = 0;
}

void Adaptor
::deep_copy(const Adaptor &m)
{
	Module::deep_copy(m);

	int id = -1;
	for (size_t i = 0; i < this->buffer->size(); i++)
		if ((*this->buffer)[i].size() == 0)
			id = (int)i;

	if (id == -1)
	{
		this->id = this->buffer->size();
		this->buffer->push_back(
			std::vector<std::vector<int8_t>>(this->buffer_size, std::vector<int8_t>(this->n_frames * this->n_bytes)));
	}
	else
	{
		this->id = (size_t)id;
		(*this->buffer)[this->id].resize(buffer_size, std::vector<int8_t>(this->n_frames * this->n_bytes));
	}

	if (this->id >= this->first->size())
	{
		std::stringstream message;
		message << "'id' can't be higher than 'first->size()' ('id' = " << this->id
		        << ", 'first->size()' = " << this->first->size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->waiting_canceled.reset(new std::atomic<bool>(m.waiting_canceled->load()));
}

size_t Adaptor
::compute_bytes(const size_t n_elmts, const std::type_index type)
{
	     if (type == typeid(int8_t )) return n_elmts * sizeof(int8_t );
	else if (type == typeid(int16_t)) return n_elmts * sizeof(int16_t);
	else if (type == typeid(int32_t)) return n_elmts * sizeof(int32_t);
	else if (type == typeid(int64_t)) return n_elmts * sizeof(int64_t);
	else if (type == typeid(float  )) return n_elmts * sizeof(float  );
	else if (type == typeid(double )) return n_elmts * sizeof(double );
	else
	{
		std::stringstream message;
		message << "This should never happen.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

void Adaptor::send_cancel_signal()
{
	*this->waiting_canceled = true;
}

void Adaptor::reset()
{
	*this->waiting_canceled = false;
}
