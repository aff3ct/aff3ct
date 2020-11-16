#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
Task& Monitor_BFER<B>
::operator[](const mnt::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B>
Socket& Monitor_BFER<B>
::operator[](const mnt::sck::check_errors s)
{
	return Module::operator[]((size_t)mnt::tsk::check_errors)[(size_t)s];
}

template <typename B>
Socket& Monitor_BFER<B>
::operator[](const mnt::sck::check_errors2 s)
{
	return Module::operator[]((size_t)mnt::tsk::check_errors2)[(size_t)s];
}

template <typename B>
template <class A>
int Monitor_BFER<B>
::check_errors(const std::vector<B,A>& U, const std::vector<B,A>& V, const int frame_id, const bool managed_memory)
{
	(*this)[mnt::sck::check_errors::U].bind(U);
	(*this)[mnt::sck::check_errors::V].bind(V);
	const auto &status = (*this)[mnt::tsk::check_errors].exec(frame_id, managed_memory);

	return status[0];
}

template <typename B>
template <class A>
int Monitor_BFER<B>
::check_errors2(const std::vector<B,A    >& U,
                const std::vector<B,A    >& V,
                      std::vector<int64_t>& FRA,
                      std::vector<int32_t>& BE,
                      std::vector<int32_t>& FE,
                      std::vector<float  >& BER,
                      std::vector<float  >& FER,
                const int frame_id, const bool managed_memory)
{
	(*this)[mnt::sck::check_errors2::U  ].bind(U);
	(*this)[mnt::sck::check_errors2::V  ].bind(V);
	(*this)[mnt::sck::check_errors2::FRA].bind(FRA);
	(*this)[mnt::sck::check_errors2::BE ].bind(BE);
	(*this)[mnt::sck::check_errors2::FE ].bind(FE);
	(*this)[mnt::sck::check_errors2::BER].bind(BER);
	(*this)[mnt::sck::check_errors2::FER].bind(FER);
	const auto &status = (*this)[mnt::tsk::check_errors2].exec(frame_id, managed_memory);

	return status[0];
}
}
}
