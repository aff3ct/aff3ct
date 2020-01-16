#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Adaptor/Adaptor_1_to_n.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Adaptor_1_to_n* Adaptor_1_to_n
::clone() const
{
	auto m = new Adaptor_1_to_n(*this);
	m->deep_copy(*this);
	return m;
}

void Adaptor_1_to_n
::push_1(const int8_t *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	if (this->active_waiting)
	{
		while (this->is_full(this->cur_id) && !*this->waiting_canceled);
	}
	else // passive waiting
	{
		if (this->is_full(this->cur_id) && !*this->waiting_canceled)
		{
			std::unique_lock<std::mutex> lock(*this->mtx_put.get());
			(*this->cnd_put.get()).wait(lock, [this]()
			{
				return !(this->is_full(this->cur_id) && !*this->waiting_canceled);
			});
		}
	}

	if (*this->waiting_canceled)
		throw tools::waiting_canceled(__FILE__, __LINE__, __func__);

	int8_t* out = (*this->buffer)[this->cur_id][(*this->last)[this->cur_id] % this->buffer_size].data();

	std::copy(in  + f_start * this->n_bytes,
	          in  + f_stop  * this->n_bytes,
	          out + f_start * this->n_bytes);

	(*this->last)[this->cur_id]++;

	if (!this->active_waiting) // passive waiting
	{
		if (!this->is_empty(this->cur_id))
		{
			std::lock_guard<std::mutex> lock((*this->mtx_pull.get())[this->cur_id]);
			(*this->cnd_pull.get())[this->cur_id].notify_one();
		}
	}

	do
	{
		this->cur_id = (this->cur_id +1) % this->buffer->size();
	}
	while((*this->buffer)[this->cur_id].size() == 0);
}

void Adaptor_1_to_n
::pull_n(int8_t *out, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	if (this->active_waiting)
	{
		while (this->is_empty(this->id) && !*this->waiting_canceled);
	}
	else // passive waiting
	{
		if (this->is_empty(this->id) && !*this->waiting_canceled)
		{
			std::unique_lock<std::mutex> lock((*this->mtx_pull.get())[this->id]);
			(*this->cnd_pull.get())[this->id].wait(lock, [this]()
			{
				return !(this->is_empty(this->id) && !*this->waiting_canceled);
			});
		}
	}

	if (*this->waiting_canceled)
		throw tools::waiting_canceled(__FILE__, __LINE__, __func__);

	const int8_t* in = (*this->buffer)[this->id][(*this->first)[this->id] % this->buffer_size].data();

	std::copy(in  + f_start * this->n_bytes,
	          in  + f_stop  * this->n_bytes,
	          out + f_start * this->n_bytes);

	(*this->first)[this->id]++;

	if (!this->active_waiting) // passive waiting
	{
		if (!this->is_full(this->id))
		{
			std::lock_guard<std::mutex> lock(*this->mtx_put.get());
			(*this->cnd_put.get()).notify_one();
		}
	}
}

void Adaptor_1_to_n
::wake_up()
{
	if (!this->active_waiting) // passive waiting
	{
		for (size_t i = 0; i < this->buffer->size(); i++)
			if ((*this->buffer)[i].size() != 0)
			{
				std::unique_lock<std::mutex> lock((*this->mtx_pull.get())[i]);
				(*this->cnd_pull.get())[i].notify_all();
			}

		std::lock_guard<std::mutex> lock(*this->mtx_put.get());
		(*this->cnd_put.get()).notify_all();
	}
}

void Adaptor_1_to_n
::cancel_waiting()
{
	this->send_cancel_signal();
	this->wake_up();
}
