#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Adaptor/Adaptor_n_to_1.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Adaptor_n_to_1* Adaptor_n_to_1
::clone() const
{
	auto m = new Adaptor_n_to_1(*this);
	m->deep_copy(*this);
	return m;
}

void Adaptor_n_to_1
::push_n(const std::vector<const int8_t*> &in, const int frame_id)
{
	if (this->active_waiting)
	{
		while (this->is_full(this->id) && !*this->waiting_canceled);
	}
	else // passive waiting
	{
		if (this->is_full(this->id) && !*this->waiting_canceled)
		{
			std::unique_lock<std::mutex> lock((*this->mtx_put.get())[this->id]);
			(*this->cnd_put.get())[this->id].wait(lock, [this]()
			{
				return !(this->is_full(this->id) && !*this->waiting_canceled);
			});
		}
	}

	if (*this->waiting_canceled)
		throw tools::waiting_canceled(__FILE__, __LINE__, __func__);

	if (!this->is_no_copy_push())
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		for (size_t s = 0; s < this->n_sockets; s++)
		{
			int8_t* out = (int8_t*)this->get_empty_buffer(s);

			std::copy(in[s] + f_start * this->n_bytes[s],
			          in[s] + f_stop  * this->n_bytes[s],
			          out   + f_start * this->n_bytes[s]);
		}

		this->wake_up_puller();
	}
}

void Adaptor_n_to_1
::pull_1(const std::vector<int8_t*> &out, const int frame_id)
{
	if (this->active_waiting)
	{
		while (this->is_empty(this->cur_id) && !*this->waiting_canceled);
	}
	else // passive waiting
	{
		if (this->is_empty(this->cur_id) && !*this->waiting_canceled)
		{
			std::unique_lock<std::mutex> lock(*this->mtx_pull.get());
			(*this->cnd_pull.get()).wait(lock, [this]()
			{
				return !(this->is_empty(this->cur_id) && !*this->waiting_canceled);
			});
		}
	}

	if (*this->waiting_canceled)
		throw tools::waiting_canceled(__FILE__, __LINE__, __func__);

	if (!this->is_no_copy_pull())
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		for (size_t s = 0; s < this->n_sockets; s++)
		{
			const int8_t* in = (const int8_t*)this->get_filled_buffer(s);

			std::copy(in     + f_start * this->n_bytes[s],
			          in     + f_stop  * this->n_bytes[s],
			          out[s] + f_start * this->n_bytes[s]);
		}

		this->wake_up_pusher();
	}
}

void Adaptor_n_to_1
::wake_up()
{
	if (!this->active_waiting) // passive waiting
	{
		for (size_t i = 0; i < this->buffer->size(); i++)
			if ((*this->buffer)[i].size() != 0)
			{
				std::unique_lock<std::mutex> lock((*this->mtx_put.get())[i]);
				(*this->cnd_put.get())[i].notify_all();
			}

		std::lock_guard<std::mutex> lock(*this->mtx_pull.get());
		(*this->cnd_pull.get()).notify_all();
	}
}

void Adaptor_n_to_1
::cancel_waiting()
{
	this->send_cancel_signal();
	this->wake_up();
}

void* Adaptor_n_to_1
::get_empty_buffer(const size_t sid)
{
	return (*this->buffer)[this->id][sid][(*this->last)[this->id] % this->buffer_size].data();
}

void* Adaptor_n_to_1
::get_filled_buffer(const size_t sid)
{
	return (*this->buffer)[this->cur_id][sid][(*this->first)[this->cur_id] % this->buffer_size].data();
}

void Adaptor_n_to_1
::wake_up_puller()
{
	(*this->last)[this->id]++;

	if (!this->active_waiting) // passive waiting
	{
		if (!this->is_empty(this->id))
		{
			std::lock_guard<std::mutex> lock(*this->mtx_pull.get());
			(*this->cnd_pull.get()).notify_one();
		}
	}
}

void Adaptor_n_to_1
::wake_up_pusher()
{
	(*this->first)[this->cur_id]++;

	if (!this->active_waiting) // passive waiting
	{
		if (!this->is_full(this->cur_id))
		{
			std::lock_guard<std::mutex> lock((*this->mtx_put.get())[this->cur_id]);
			(*this->cnd_put.get())[this->cur_id].notify_one();
		}
	}

	do
	{
		this->cur_id = (this->cur_id +1) % this->buffer->size();
	}
	while((*this->buffer)[this->cur_id].size() == 0);
}
