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
::stop(const int8_t *in, const int frame_id)
{
	// this->n_calls++; // is performed into 'Loop_predicate<O>::stop' function
	if (Loop_predicate<O>::stop(in, frame_id))
		return true;

	if (Loop::get_n_calls() > crc_ite_start)
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		auto ret_val = this->_stop(in + f_start * this->n_bytes_in, f_start);
		for (auto f = f_start+1; f < f_stop; f++)
			ret_val &= this->_stop(in + f * this->n_bytes_in, f);

		return ret_val;
	}
	else
		return false;
}

template <typename I, typename O>
bool Loop_CRC<I,O>
::_stop(const int8_t *in, const int frame_id)
{
	return this->crc->check((const I*)in, 1, 0) ? true : false;
}

}
}
