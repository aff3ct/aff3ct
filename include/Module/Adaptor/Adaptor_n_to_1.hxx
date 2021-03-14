#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Adaptor/Adaptor_n_to_1.hpp"

namespace aff3ct
{
namespace module
{

Task& Adaptor_n_to_1
::operator[](const adp::tsk t)
{
	return Module::operator[]((size_t)t);
}

Socket& Adaptor_n_to_1
::operator[](const adp::sck::push_n s)
{
	return Module::operator[]((size_t)adp::tsk::push_n)[(size_t)s];
}

Socket& Adaptor_n_to_1
::operator[](const adp::sck::pull_1 s)
{
	return Module::operator[]((size_t)adp::tsk::pull_1)[(size_t)s];
}

Adaptor_n_to_1
::Adaptor_n_to_1(const size_t n_elmts,
                 const std::type_index datatype,
                 const size_t buffer_size,
                 const bool active_waiting)
: Adaptor(n_elmts, datatype, buffer_size),
  active_waiting(active_waiting),
  cnd_put (new std::vector<std::condition_variable>(1000)),
  mtx_put (new std::vector<std::mutex             >(1000)),
  cnd_pull(new             std::condition_variable (    )),
  mtx_pull(new             std::mutex              (    ))
{
	this->init();
}

Adaptor_n_to_1
::Adaptor_n_to_1(const std::vector<size_t> &n_elmts,
                 const std::vector<std::type_index> &datatype,
                 const size_t buffer_size,
                 const bool active_waiting)
: Adaptor(n_elmts, datatype, buffer_size),
  active_waiting(active_waiting),
  cnd_put (new std::vector<std::condition_variable>(1000)),
  mtx_put (new std::vector<std::mutex             >(1000)),
  cnd_pull(new             std::condition_variable (    )),
  mtx_pull(new             std::mutex              (    ))
{
	this->init();
}

void Adaptor_n_to_1
::init()
{
	const std::string name = "Adaptor_n_to_1";
	this->set_name(name);
	this->set_short_name(name);
	this->set_single_wave(true);

	auto &p1 = this->create_task("push_n", (int)adp::tsk::push_n);
	std::vector<size_t> p1s_in;
	for (size_t s = 0; s < this->n_sockets; s++)
		p1s_in.push_back(this->create_socket_in(p1, "in" + std::to_string(s), this->n_elmts[s], this->datatype[s]));

	this->create_codelet(p1, [p1s_in](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &adp = static_cast<Adaptor_n_to_1&>(m);
		if (adp.is_no_copy_push())
		{
			adp.wait_push();
			// for debug mode coherence
			for (size_t s = 0; s < t.sockets.size() -1; s++)
				t.sockets[s]->dataptr = adp.get_empty_buffer(s);
		}
		else
		{
			std::vector<const int8_t*> sockets_dataptr(p1s_in.size());
			for (size_t s = 0; s < p1s_in.size(); s++)
				sockets_dataptr[s] = static_cast<const int8_t*>(t[p1s_in[s]].get_dataptr());
			adp.push_n(sockets_dataptr, frame_id);
		}
		return status_t::SUCCESS;
	});

	auto &p2 = this->create_task("pull_1", (int)adp::tsk::pull_1);
	std::vector<size_t> p2s_out;
	for (size_t s = 0; s < this->n_sockets; s++)
		p2s_out.push_back(this->create_socket_out(p2, "out" + std::to_string(s), this->n_elmts[s], this->datatype[s]));

	this->create_codelet(p2, [p2s_out](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &adp = static_cast<Adaptor_n_to_1&>(m);
		if (adp.is_no_copy_pull())
		{
			adp.wait_pull();
			// for debug mode coherence
			for (size_t s = 0; s < t.sockets.size() -1; s++)
				t.sockets[s]->bind(adp.get_filled_buffer(s));
		}
		else
		{
			std::vector<int8_t*> sockets_dataptr(p2s_out.size());
			for (size_t s = 0; s < p2s_out.size(); s++)
				sockets_dataptr[s] = static_cast<int8_t*>(t[p2s_out[s]].get_dataptr());
			adp.pull_1(sockets_dataptr, frame_id);
		}
		return status_t::SUCCESS;
	});
}
}
}
