#include <set>

#include "Module/Subchain/Subchain.hpp"
#include "Tools/Chain/Chain.hpp"

namespace aff3ct
{
namespace tools
{
size_t Chain
::get_n_threads() const
{
	return this->n_threads;
}

const std::vector<std::vector<module::Task*>>& Chain
::get_firsts_tasks() const
{
	return this->firsts_tasks;
}

const std::vector<std::vector<module::Task*>>& Chain
::get_lasts_tasks() const
{
	return this->lasts_tasks;
}

template <class C>
std::vector<C*> Chain
::get_modules(const bool subchain_modules) const
{
	std::vector<C*> ret;
	for (auto &mm : this->all_modules)
		for (auto &m : mm)
		{
			if (subchain_modules)
			{
				auto c = dynamic_cast<module::Subchain*>(m);
				if (c != nullptr)
				{
					auto subret = c->get_chain().get_modules<C>(subchain_modules);
					ret.insert(ret.end(), subret.begin(), subret.end());
				}
			}

			auto c = dynamic_cast<C*>(m);
			if (c != nullptr)
				ret.push_back(c);
		}

	return ret;
}

template <class SS>
inline void Chain
::_init(Generic_node<SS> *root)
{
	std::stringstream message;
	message << "This should never happen.";
	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
}

template <>
inline void Chain
::_init(Generic_node<tools::Sub_sequence_const> *root)
{
	this->duplicate<tools::Sub_sequence_const, const module::Module>(root);
	this->delete_tree(root);
}

template <>
inline void Chain
::_init(Generic_node<tools::Sub_sequence> *root)
{
	this->sequences[0] = root;

	std::set<module::Module*> modules_set;
	std::function<void(const Generic_node<tools::Sub_sequence>*)> collect_modules_list;
	collect_modules_list = [&](const Generic_node<tools::Sub_sequence> *node)
	{
		if (node != nullptr)
		{
			if (node->get_c())
				for (auto ta : node->get_c()->tasks)
					modules_set.insert(&ta->get_module());
			for (auto c : node->get_children())
				collect_modules_list(c);
		}
	};
	collect_modules_list(root);

	for (auto m : modules_set)
		this->all_modules[0].push_back(m);

	this->duplicate<tools::Sub_sequence, module::Module>(root);
}

}
}
