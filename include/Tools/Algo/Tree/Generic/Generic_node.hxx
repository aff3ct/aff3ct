#include "Tools/Algo/Tree/Generic/Generic_node.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Generic_node<T>
::Generic_node(Generic_node<T>* father,
               std::vector<Generic_node<T>*> children,
               T* content,
               int depth,
               int lane_id,
               int child_id)
: father(father), children(children), contents(content), depth(depth), lane_id(lane_id), child_id(child_id)
{
}

template <typename T>
Generic_node<T>
::~Generic_node()
{
}

template <typename T>
bool Generic_node<T>
::is_root() const
{
	return (this->father == nullptr);
}

template <typename T>
bool Generic_node<T>
::is_leaf() const
{
	return !this->children.size();
}

template <typename T>
bool Generic_node<T>
::is_empty() const
{
	return (this->contents == nullptr);
}

template <typename T>
int Generic_node<T>
::get_pos() const
{
	if (this->is_root())
		return 0;
	else
		for (auto c = 0; c < (int)this->father->get_children().size(); c++)
			if (this->father->get_children()[c] == this)
				return c;

	return -1;
}

template <typename T>
Generic_node<T>* Generic_node<T>
::get_father() const
{
	return this->father;
}

template <typename T>
const std::vector<Generic_node<T>*>& Generic_node<T>
::get_children() const
{
	return this->children;
}

template <typename T>
T* Generic_node<T>
::get_contents() const
{
	return this->contents;
}

template <typename T>
T* Generic_node<T>
::get_c() const
{
	return this->get_contents();
}

template <typename T>
void Generic_node<T>
::set_contents(T* contents)
{
	this->contents = contents;
}

template <typename T>
int Generic_node<T>
::get_depth() const
{
	return this->depth;
}

template <typename T>
int Generic_node<T>
::get_lane_id() const
{
	return this->lane_id;
}

template <typename T>
int Generic_node<T>
::get_child_id() const
{
	return this->child_id;
}

template <typename T>
bool Generic_node<T>
::cut_child(const size_t pos)
{
	if (pos < this->children.size())
	{
		delete this->children[pos];
		this->children.erase(this->children.begin() + pos);

		for (auto c = 0; c < this->children.size(); c++)
			this->children[c].child_id = c;

		return true;
	}
	else
		return false;
}

template <typename T>
bool Generic_node<T>
::cut_children()
{
	if (this->children.size())
	{
		for (size_t c = 0; c < this->children.size(); c++)
			delete this->children[c];
		this->children.clear();
		return true;
	}
	else
		return false;
}

template <typename T>
void Generic_node<T>
::add_child(Generic_node<T>* child)
{
	this->children.push_back(child);
}

}
}
