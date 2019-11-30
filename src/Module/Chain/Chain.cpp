#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Chain/Chain.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Chain
::Chain(const tools::Chain &chain)
: Module(chain.tasks_sequences[0][0]->get_module().get_n_frames()),
  chain(chain.clone())
{
	const std::string name = "Chain";
	this->set_name(name);
	this->set_short_name(name);

	if (chain.get_n_threads() != 1)
	{
		std::stringstream message;
		message << "'chain.get_n_threads()' has to be equal to 1 ('chain.get_n_threads()' = " << chain.get_n_threads()
		        << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("exec");

	auto &first = *this->chain->tasks_sequences[0][0];
	for (auto &s : first.sockets)
	{
		if (first.get_socket_type(*s) == socket_t::SIN)
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
		if (first.get_socket_type(*s) == socket_t::SIN_SOUT)
		{
			if (s->get_datatype() == typeid(int8_t ))
				this->template create_socket_in_out<int8_t >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int16_t))
				this->template create_socket_in_out<int16_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int32_t))
				this->template create_socket_in_out<int32_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int64_t))
				this->template create_socket_in_out<int64_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(float  ))
				this->template create_socket_in_out<float  >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(double ))
				this->template create_socket_in_out<double >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
		}
	}
	auto &last  = *this->chain->tasks_sequences[0][this->chain->tasks_sequences.size()-1];
	for (auto &s : last.sockets)
	{
		if (last.get_socket_type(*s) == socket_t::SOUT)
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
		if (last.get_socket_type(*s) == socket_t::SIN_SOUT)
		{
			if (s->get_datatype() == typeid(int8_t ))
				this->template create_socket_in_out<int8_t >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int16_t))
				this->template create_socket_in_out<int16_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int32_t))
				this->template create_socket_in_out<int32_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(int64_t))
				this->template create_socket_in_out<int64_t>(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(float  ))
				this->template create_socket_in_out<float  >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
			else if (s->get_datatype() == typeid(double ))
				this->template create_socket_in_out<double >(p, s->get_name(), s->get_n_elmts() / this->get_n_frames());
		}
	}

	this->create_codelet(p, [](Module &m, Task &t) -> int
	{
		auto &c = static_cast<Chain&>(m);

		auto &first = *c.chain->tasks_sequences[0][0];
		size_t sid_in = 0;
		for (auto &s : first.sockets)
		{
			if (first.get_socket_type(*s) == socket_t::SIN || first.get_socket_type(*s) == socket_t::SIN_SOUT)
				s->bind(t.sockets[sid_in]->get_dataptr());
			sid_in++;
		}

		auto ret = c.chain->exec();

		auto &last = *c.chain->tasks_sequences[0][c.chain->tasks_sequences.size()-1];
		size_t sid_out = 0;
		for (auto &s : last.sockets)
		{
			if (last.get_socket_type(*s) == socket_t::SOUT || last.get_socket_type(*s) == socket_t::SIN_SOUT)
				t.sockets[sid_out]->bind(s->get_dataptr());
			sid_out++;
		}

		return ret;
	});
}

Chain* Chain
::clone() const
{
	auto m = new Chain(*this);
	m->deep_copy(*this);
	return m;
}

void Chain
::deep_copy(const Chain& m)
{
	Module::deep_copy(m);
	if (m.chain != nullptr) this->chain.reset(m.chain->clone());
}
