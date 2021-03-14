#include "Module/Adaptor/Adaptor.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Adaptor
::~Adaptor()
{
	for (auto b : this->buffer_to_free)
		delete[] b;

	(*this->buffer)[this->id].clear();
	(*this->first)[this->id] = 0;
	(*this->last )[this->id] = 0;
}

Adaptor* Adaptor
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
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
		this->buffer->push_back(std::vector<std::vector<int8_t*>>(this->n_sockets,
			std::vector<int8_t*>(this->buffer_size)));
	}
	else
	{
		this->id = (size_t)id;
		(*this->buffer)[this->id].resize(this->n_sockets, std::vector<int8_t*>(this->buffer_size));
	}

	this->buffer_to_free.clear();
	for (size_t s = 0; s < this->n_sockets; s++)
		for (size_t b = 0; b < this->buffer_size; b++)
		{
			(*this->buffer)[this->id][s][b] = new int8_t[this->n_frames * this->n_bytes[s]];
			this->buffer_to_free.push_back((*this->buffer)[this->id][s][b]);
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

void Adaptor
::send_cancel_signal()
{
	*this->waiting_canceled = true;
}

void Adaptor
::reset()
{
	*this->waiting_canceled = false;
	for (auto &a : *this->first.get()) a = 0;
	for (auto &a : *this->last .get()) a = 0;
	this->cur_id = 0;
	this->reset_buffer();
}

void Adaptor
::set_no_copy_pull(const bool no_copy_pull)
{
	this->no_copy_pull = no_copy_pull;
}

void Adaptor
::set_no_copy_push(const bool no_copy_push)
{
	this->no_copy_push = no_copy_push;
}

bool Adaptor
::is_no_copy_pull()
{
	return this->no_copy_pull;
}

bool Adaptor
::is_no_copy_push()
{
	return this->no_copy_push;
}

void Adaptor
::reset_buffer()
{
	size_t id_buff = 0;
	for (size_t s = 0; s < this->n_sockets; s++)
		for (size_t b = 0; b < this->buffer_size; b++)
			(*this->buffer)[this->id][s][b] = buffer_to_free[id_buff++];
}

void Adaptor
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Module::set_n_frames(n_frames);
		for (size_t s = 0; s < (*this->buffer)[this->id].size(); s++)
			for (size_t b = 0; b < (*this->buffer)[this->id][s].size(); b++)
			{
				auto old_ptr = (*this->buffer)[this->id][s][b];
				(*this->buffer)[this->id][s][b] = new int8_t[this->n_bytes[s] * n_frames];

				bool found = false;
				for (size_t bf = 0; bf < this->buffer_to_free.size(); bf++)
					if (this->buffer_to_free[bf] == old_ptr)
					{
						delete[] this->buffer_to_free[bf];
						this->buffer_to_free[bf] = (*this->buffer)[this->id][s][b];
						found = true;
						break;
					}

				if (found == false)
				{
					std::stringstream message;
					message << "This should never happen.";
					throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
				}
			}
	}
}