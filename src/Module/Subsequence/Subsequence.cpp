#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Sequence/Sequence.hpp"
#include "Module/Subsequence/Subsequence.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Subsequence
::Subsequence(tools::Sequence &sequence)
: Module(),
  sequence_extern(&sequence)
{
	this->init();
}

Subsequence
::Subsequence(const tools::Sequence &sequence)
: Module(),
  sequence_cloned(sequence.clone()), sequence_extern(nullptr)
{
	this->init();
}

void Subsequence
::init()
{
	const std::string name = "Subsequence";
	this->set_name(name);
	this->set_short_name(name);
	this->set_single_wave(true);

	auto &sequence = this->get_sequence();

	if (sequence.get_n_threads() != 1)
	{
		std::stringstream message;
		message << "'sequence.get_n_threads()' has to be equal to 1 ('sequence.get_n_threads()' = " << sequence.get_n_threads()
		        << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("exec");
	p.set_autoalloc(true);

	auto &firsts = sequence.get_firsts_tasks()[0];
	for (auto &first : firsts) for (auto &s : first->sockets)
	{
		if (first->get_socket_type(*s) == socket_t::SIN)
		{
			if (s->get_datatype() == typeid(int8_t ))
				this->template create_socket_in<int8_t >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int16_t))
				this->template create_socket_in<int16_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int32_t))
				this->template create_socket_in<int32_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int64_t))
				this->template create_socket_in<int64_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(float  ))
				this->template create_socket_in<float  >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(double ))
				this->template create_socket_in<double >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
		}
	}
	auto &lasts  = sequence.get_lasts_tasks()[0];
	for (auto &last : lasts) for (auto &s : last->sockets)
	{
		if (last->get_socket_type(*s) == socket_t::SOUT && s->get_name() != "status")
		{
			if (s->get_datatype() == typeid(int8_t ))
				this->template create_socket_out<int8_t >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int16_t))
				this->template create_socket_out<int16_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int32_t))
				this->template create_socket_out<int32_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int64_t))
				this->template create_socket_out<int64_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(float  ))
				this->template create_socket_out<float  >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(double ))
				this->template create_socket_out<double >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
		}
	}

	size_t sid = 0;
	for (auto &last : lasts) for (auto &s : last->sockets)
	{
		if (last->get_socket_type(*s) == socket_t::SOUT && s->get_name() != "status")
		{
			while (p.get_socket_type(*p.sockets[sid]) != socket_t::SOUT) sid++;
			p.sockets[sid++]->bind(*s);
		}
	}

	this->create_codelet(p, [](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &ss = static_cast<Subsequence&>(m);

		auto &firsts = ss.get_sequence().get_firsts_tasks()[0];
		size_t sid = 0;
		for (auto &first : firsts) for (auto &s : first->sockets)
		{
			if (first->get_socket_type(*s) == socket_t::SIN)
			{
				while (t.get_socket_type(*t.sockets[sid]) != socket_t::SIN) sid++;
				s->bind(t.sockets[sid++]->get_dataptr());
			}
		}

		// execute all frames sequentially
		ss.get_sequence().exec_seq();

		return status_t::SUCCESS;
	});
}

tools::Sequence& Subsequence
::get_sequence()
{
	if (this->sequence_extern)
		return *this->sequence_extern;
	else
		return *this->sequence_cloned;
}

Subsequence* Subsequence
::clone() const
{
	auto m = new Subsequence(*this);
	m->deep_copy(*this);
	return m;
}

void Subsequence
::deep_copy(const Subsequence& m)
{
	Module::deep_copy(m);
	if (m.sequence_cloned != nullptr)
		this->sequence_cloned.reset(m.sequence_cloned->clone());
	else
	{
		this->sequence_cloned.reset(m.sequence_extern->clone());
		this->sequence_extern = nullptr;
	}

	auto &lasts = this->get_sequence().get_lasts_tasks()[0];

	auto &p = (*this)[ssq::tsk::exec];

	size_t sid = 0;
	for (auto &last : lasts) for (auto &s : last->sockets)
	{
		if (last->get_socket_type(*s) == socket_t::SOUT && s->get_name() != "status")
		{
			while (p.get_socket_type(*p.sockets[sid]) != socket_t::SOUT) sid++;
			p.sockets[sid++]->bind(*s);
		}
	}
}

void Subsequence
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		auto &p = *this->tasks[0];
		auto &lasts = this->get_sequence().get_lasts_tasks()[0];
		size_t sid = 0;
		for (auto &last : lasts) for (auto &s : last->sockets)
		{
			if (last->get_socket_type(*s) == socket_t::SOUT && s->get_name() != "status")
			{
				while (p.get_socket_type(*p.sockets[sid]) != socket_t::SOUT) sid++;
				p.sockets[sid++]->unbind(*s);
			}
		}

		Module::set_n_frames(n_frames);

		if (this->sequence_extern)
			this->sequence_extern->set_n_frames(n_frames);
		else
			this->sequence_cloned->set_n_frames(n_frames);

		sid = 0;
		for (auto &last : lasts) for (auto &s : last->sockets)
		{
			if (last->get_socket_type(*s) == socket_t::SOUT && s->get_name() != "status")
			{
				while (p.get_socket_type(*p.sockets[sid]) != socket_t::SOUT) sid++;
				p.sockets[sid++]->bind(*s);
			}
		}
	}
}
