#include <algorithm>

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
::put_n(const int8_t *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	// active waiting
	while (this->is_full(this->id));

	int8_t* out = (*this->buffer)[this->id][(*this->last)[this->id]].data();

	std::copy(in  + f_start * this->n_bytes,
	          in  + f_stop  * this->n_bytes,
	          out + f_start * this->n_bytes);

	(*this->last)[this->id] = ((*this->last)[this->id] +1) % this->buffer_size;
}

void Adaptor_n_to_1
::pull_1(int8_t *out, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	// active waiting
	while (this->is_empty(this->cur_id));

	const int8_t* in = (*this->buffer)[this->cur_id][(*this->last)[this->cur_id]].data();

	std::copy(in  + f_start * this->n_bytes,
	          in  + f_stop  * this->n_bytes,
	          out + f_start * this->n_bytes);

	(*this->first)[this->cur_id] = ((*this->first)[this->cur_id] +1) % this->buffer_size;

	do
	{
		this->cur_id = (this->cur_id +1) % this->buffer->size();
	}
	while((*this->buffer)[this->cur_id].size() == 0);
}
