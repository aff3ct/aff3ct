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
::deep_copy(const Adaptor_n_to_1 &m)
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
	// 		(*this->cnd_put.get()).resize((*this->cnd_put.get()).size() +1);
	// 		(*this->mtx_put.get()).resize((*this->mtx_put.get()).size() +1);
	// 	}
	// }
}

void Adaptor_n_to_1
::put_n(const int8_t *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	if (this->active_waiting)
	{
		while (this->is_full(this->id));
	}
	else // passive waiting
	{
		if (this->is_full(this->id))
		{
			std::unique_lock<std::mutex> lock((*this->mtx_put.get())[this->id]);
			// std::cout << "put_n - Sleep (id = " << this->id << ")" << std::endl;
			(*this->cnd_put.get())[this->id].wait(lock, [this](){ return !this->is_full(this->id); });
			// std::cout << "put_n - Wake up (id = " << this->id << ")" << std::endl;
		}
	}

	// std::cout << "put_n - Out " << ((*this->last)[this->id] % this->buffer_size) << " (id = " << this->id << ")" << std::endl;
	int8_t* out = (*this->buffer)[this->id][(*this->last)[this->id] % this->buffer_size].data();

	std::copy(in  + f_start * this->n_bytes,
	          in  + f_stop  * this->n_bytes,
	          out + f_start * this->n_bytes);

	(*this->last)[this->id]++;

	if (!this->active_waiting) // passive waiting
	{
		if (this->is_full(this->id))
		{
			std::lock_guard<std::mutex> lock(*this->mtx_pull.get());
			// std::cout << "put_n - Notify (id = " << this->id << ")" << std::endl;
			(*this->cnd_pull.get()).notify_one();
		}
	}
}

void Adaptor_n_to_1
::pull_1(int8_t *out, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	if (this->active_waiting)
	{
		while (this->is_empty(this->cur_id));
	}
	else // passive waiting
	{
		if (this->is_empty(this->cur_id))
		{
			std::unique_lock<std::mutex> lock(*this->mtx_pull.get());
			// std::cout << "pull_1 - Sleep (cur_id = " << this->cur_id << ")" << std::endl;
			(*this->cnd_pull.get()).wait(lock, [this](){ return !this->is_empty(this->cur_id); });
			// std::cout << "pull_1 - Wake up (cur_id = " << this->cur_id << ")" << std::endl;
		}
	}

	// std::cout << "pull_1 - In " << ((*this->first)[this->cur_id] % this->buffer_size) << " (cur_id = "
	          // << this->cur_id << ")" << std::endl;
	const int8_t* in = (*this->buffer)[this->cur_id][(*this->first)[this->cur_id] % this->buffer_size].data();

	std::copy(in  + f_start * this->n_bytes,
	          in  + f_stop  * this->n_bytes,
	          out + f_start * this->n_bytes);

	(*this->first)[this->cur_id]++;

	if (!this->active_waiting) // passive waiting
	{
		if (this->is_empty(this->cur_id))
		{
			std::lock_guard<std::mutex> lock((*this->mtx_put.get())[this->cur_id]);
			// std::cout << "pull_1 - Notify -> " << this->cur_id << std::endl;
			(*this->cnd_put.get())[this->cur_id].notify_one();
		}
	}

	do
	{
		this->cur_id = (this->cur_id +1) % this->buffer->size();
	}
	while((*this->buffer)[this->cur_id].size() == 0);
}
