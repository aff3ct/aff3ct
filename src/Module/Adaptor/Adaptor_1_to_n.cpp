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
::deep_copy(const Adaptor_1_to_n &m)
{
	Adaptor::deep_copy(m);

	// if (!this->active_waiting)
	// {
	// 	int id = -1;
	// 	for (size_t i = 0; i < this->buffer->size(); i++)
	// 		if ((*this->buffer)[i].size() == 0)
	// 			id = (int)i;

	// 	if (id == -1)
	// 	{
	// 		(*this->cnd_pull.get()).resize((*this->cnd_pull.get()).size() +1);
	// 		(*this->mtx_pull.get()).resize((*this->mtx_pull.get()).size() +1);
	// 	}
	// }
}

void Adaptor_1_to_n
::put_1(const int8_t *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	if (this->active_waiting)
	{
		while (this->is_full(this->cur_id));
	}
	else // passive waiting
	{
		if (this->is_full(this->cur_id))
		{
			std::unique_lock<std::mutex> lock(*this->mtx_put.get());
			// std::cout << "put_1 - Sleep (cur_id = " << this->cur_id << ")" << std::endl;
			(*this->cnd_put.get()).wait(lock, [this]() { return !this->is_full(this->cur_id); });
			// std::cout << "put_1 - Wake up (cur_id = " << this->cur_id << ")" << std::endl;
		}
	}

	// std::cout << "put_1 - Out " << ((*this->last)[this->cur_id] % this->buffer_size) << " (cur_id = "
	          // << this->cur_id << ")" << std::endl;
	int8_t* out = (*this->buffer)[this->cur_id][(*this->last)[this->cur_id] % this->buffer_size].data();

	std::copy(in  + f_start * this->n_bytes,
	          in  + f_stop  * this->n_bytes,
	          out + f_start * this->n_bytes);

	(*this->last)[this->cur_id]++;

	if (!this->active_waiting) // passive waiting
	{
		if (this->is_full(this->cur_id))
		{
			std::lock_guard<std::mutex> lock((*this->mtx_pull.get())[this->cur_id]);
			// std::cout << "put_1 - Notify -> " << this->cur_id << std::endl;
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
		while (this->is_empty(this->id));
	}
	else // passive waiting
	{
		if (this->is_empty(this->id))
		{
			std::unique_lock<std::mutex> lock((*this->mtx_pull.get())[this->id]);
			// std::cout << "pull_n - Sleep (id = " << this->id << ")" << std::endl;
			(*this->cnd_pull.get())[this->id].wait(lock, [this](){ return !this->is_empty(this->id); });
			// std::cout << "pull_n - Wake up (id = " << this->id << ")" << std::endl;
		}
	}

	// std::cout << "pull_n - In " << ((*this->first)[this->id] % this->buffer_size) << " (id = " << this->id << ")" << std::endl;
	const int8_t* in = (*this->buffer)[this->id][(*this->first)[this->id] % this->buffer_size].data();

	std::copy(in  + f_start * this->n_bytes,
	          in  + f_stop  * this->n_bytes,
	          out + f_start * this->n_bytes);

	(*this->first)[this->id]++;

	if (!this->active_waiting) // passive waiting
	{
		if (this->is_empty(this->id))
		{
			std::lock_guard<std::mutex> lock(*this->mtx_put.get());
			// std::cout << "pull_n - Notify (id = " << this->id << ")" << std::endl;
			(*this->cnd_put.get()).notify_one();
		}
	}
}
