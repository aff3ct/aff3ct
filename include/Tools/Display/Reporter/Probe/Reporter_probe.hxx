#include "Tools/Display/Reporter/Probe/Reporter_probe.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
size_t Reporter_probe
::col_size(const int col)
{
	return this->head[col] - this->tail[col] + (this->head[col] >= this->tail[col] ? 0 : this->buffer[col].size());
}

template <typename T>
bool Reporter_probe
::push(const int col, const T *elts)
{
	std::unique_lock<std::mutex> lck(this->mtx[col]);
	if (this->col_size<T>(col) >= this->buffer[col].size())
		return false;
	auto buff = reinterpret_cast<T*>(this->buffer[col][this->head[col] == this->buffer[col].size() ?
	                                 0 : this->head[col]].data());
	std::copy(elts, elts + this->datasizes[col], buff);
	this->head[col] = this->head[col] == this->buffer[col].size() ? 1 : this->head[col] +1;
	return true;
}

template <typename T>
bool Reporter_probe
::pull(const int col, T *elts)
{
	std::unique_lock<std::mutex> lck(this->mtx[col]);
	if (this->col_size<T>(col) == 0)
		return false;
	auto buff = reinterpret_cast<const T*>(this->buffer[col][this->tail[col] == this->buffer[col].size() ?
	                                       0 : this->tail[col]].data());
	std::copy(buff, buff + this->datasizes[col], elts);
	this->tail[col] = this->tail[col] == this->buffer[col].size() ? 1 : this->tail[col] +1;
	return true;
}

}
}
