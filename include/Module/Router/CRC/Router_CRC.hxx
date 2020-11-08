#include <string>
#include <algorithm>
#include <typeindex>

#include "Module/Router/CRC/Router_CRC.hpp"

namespace aff3ct
{
namespace module
{

template <typename I, typename O>
Router_CRC<I,O>
::Router_CRC(const CRC<I> &crc, const size_t n_elmts_in, const size_t n_elmts_out)
: Router(n_elmts_in, typeid(I), n_elmts_out, typeid(O), 2),
  crc(crc.clone())
{
	const std::string name = "Router_CRC";
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
Router_CRC<I,O>* Router_CRC<I,O>
::clone() const
{
	auto m = new Router_CRC(*this);
	m->deep_copy(*this);
	return m;
}

template <typename I, typename O>
void Router_CRC<I,O>
::deep_copy(const Router_CRC<I,O> &m)
{
	Module::deep_copy(m);
	if (m.crc != nullptr) this->crc.reset(dynamic_cast<CRC<I>*>(m.crc->clone()));
}

template <typename I, typename O>
size_t Router_CRC<I,O>
::__route(const int8_t *in, const int frame_id)
{
	return this->crc->check((const I*)in) ? 1 : 0;
}

template <typename I, typename O>
size_t Router_CRC<I,O>
::select_route_inter(const size_t a, const size_t b)
{
	return std::min(a, b);
}

}
}
