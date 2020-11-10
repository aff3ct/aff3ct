#include <string>
#include <algorithm>
#include <typeindex>

#include "Module/Loop/CRC/Loop_CRC.hpp"

namespace aff3ct
{
namespace module
{

template <typename I, typename O>
Loop_CRC<I,O>
::Loop_CRC(const tools::Predicate &predicate,
           const CRC<I> &crc,
           const size_t n_elmts_in,
           const size_t n_elmts_out,
           const size_t crc_ite_start)
: Loop_predicate<O>(predicate, n_elmts_in, typeid(I), n_elmts_out),
  crc(crc.clone()),
  crc_ite_start(crc_ite_start)
{
	const std::string name = "Loop_CRC";
	this->set_name(name);
	this->set_n_frames(this->crc->get_n_frames());

	if (this->crc->get_size() + this->crc->get_K() != (int)n_elmts_in)
	{
		std::stringstream message;
		message << "'n_elmts_in' has to be equal to 'crc->get_size()' + 'crc->get_K()' ('n_elmts_in' = " << n_elmts_in
		        << ", 'crc->get_size()' = " << this->crc->get_size() << ", 'crc->get_K()' = "
		        << this->crc->get_K() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename I, typename O>
Loop_CRC<I,O>* Loop_CRC<I,O>
::clone() const
{
	auto m = new Loop_CRC(*this);
	m->deep_copy(*this);
	return m;
}

template <typename I, typename O>
void Loop_CRC<I,O>
::deep_copy(const Loop_CRC<I,O> &m)
{
	Loop_predicate<O>::deep_copy(m);
	if (m.crc != nullptr) this->crc.reset(dynamic_cast<CRC<I>*>(m.crc->clone()));
}

template <typename I, typename O>
bool Loop_CRC<I,O>
::_stop(const int8_t *in, const size_t frame_id)
{
	// this->n_calls++; // is performed into 'Loop_predicate<O>::_stop' function
	if (Loop_predicate<O>::_stop(in, frame_id))
		return true;

	if (Loop::get_n_calls() > crc_ite_start)
	{
		auto ret_val = this->__stop(in + 0 * this->n_bytes_in, 0);
		for (size_t f = 1; f < this->get_n_frames(); f++)
			ret_val &= this->__stop(in + f * this->n_bytes_in, f);

		return ret_val;
	}
	else
		return false;
}

template <typename I, typename O>
bool Loop_CRC<I,O>
::__stop(const int8_t *in, const size_t frame_id)
{
	return this->crc->check((const I*)in, frame_id, false) ? true : false;
}

template <typename I, typename O>
void Loop_CRC<I,O>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Loop_predicate<O>::set_n_frames(n_frames);
		this->crc->set_n_frames(n_frames);
	}
}

}
}
