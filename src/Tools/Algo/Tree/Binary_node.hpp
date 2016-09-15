#ifndef BINARY_NODE_HPP_
#define BINARY_NODE_HPP_

template <typename R>
class Binary_tree;

template <typename T = float>
class Binary_node
{
	friend Binary_tree<T>;

private:
	Binary_node<T>* father;
	Binary_node<T>* left;
	Binary_node<T>* right;

	T* contents;

	const int depth;
	const int lane_id;

public:
	Binary_node(Binary_node<T>* father_node, 
	            Binary_node<T>* left_node, 
	            Binary_node<T>* right_node, 
	            T* content, 
	            int depth, 
	            int lane_id);

	virtual ~Binary_node();

	inline bool is_root() const;
	inline bool is_leaf() const;
	inline bool is_empty() const;
	inline bool is_right() const;
	inline bool is_left() const;
	inline Binary_node<T>* get_father() const;
	inline Binary_node<T>* get_left() const;
	inline Binary_node<T>* get_right() const;
	inline T* get_contents() const;
	inline T* get_c() const { return this->get_contents(); }
	inline void set_contents(T* content);
	inline int get_depth() const;
	inline int get_lane_id() const;
	inline void cut_left();
	inline void cut_right();
};

#include "Binary_node.hxx"

#endif /* BINARY_NODE_HPP_ */
