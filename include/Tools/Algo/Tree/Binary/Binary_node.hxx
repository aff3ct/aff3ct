#include "Tools/Algo/Tree/Binary/Binary_node.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Binary_node<T>
::Binary_node(Binary_node<T>* father,
              Binary_node<T>* left,
              Binary_node<T>* right,
              T* content,
              int depth,
              int lane_id)
: father(father), left(left), right(right), contents(content), depth(depth), lane_id(lane_id)
{
}

template <typename T>
Binary_node<T>
::~Binary_node()
{
}

template <typename T>
bool Binary_node<T>
::is_root() const
{
	return (this->father == nullptr);
}

template <typename T>
bool Binary_node<T>
::is_leaf() const
{
	return (this->left == nullptr && this->right == nullptr);
}

template <typename T>
bool Binary_node<T>
::is_empty() const
{
	return (this->contents == nullptr);
}

template <typename T>
bool Binary_node<T>
::is_right() const
{

	return ((!this->is_root()) && (this->father->get_right() == this));
}

template <typename T>
bool Binary_node<T>
::is_left() const
{
	return ((!this->is_root()) && (this->father->get_left() == this));
}

template <typename T>
Binary_node<T>* Binary_node<T>
::get_father() const
{
	return this->father;
}

template <typename T>
Binary_node<T>* Binary_node<T>
::get_left() const
{
	return this->left;
}

template <typename T>
Binary_node<T>* Binary_node<T>
::get_right() const
{
	return this->right;
}

template <typename T>
T* Binary_node<T>
::get_contents() const
{
	return this->contents;
}

template <typename T>
void Binary_node<T>
::set_contents(T* contents)
{
	this->contents = contents;
}

template <typename T>
int Binary_node<T>
::get_depth() const
{
	return this->depth;
}

template <typename T>
int Binary_node<T>
::get_lane_id() const
{
	return this->lane_id;
}

template <typename T>
void Binary_node<T>
::cut_left()
{
	delete this->left;
	this->left = nullptr;
}

template <typename T>
void Binary_node<T>
::cut_right()
{
	delete this->right;
	this->right = nullptr;
}
}
}
