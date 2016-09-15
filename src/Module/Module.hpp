#ifndef MODULE_HPP_
#define MODULE_HPP_

#include <cassert>

class Module
{
protected:
	std::string name;     // module name
	int         n_frames; // number of frames to process in this module

public:
	Module(const int n_frames = 1, const std::string name = "Module") : name(name), n_frames(n_frames)
	{ assert(n_frames > 0); }

	virtual ~Module() {}

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}

	virtual int get_n_frames() const
	{
		return n_frames;
	}

	void rename(const std::string name)
	{
		this->name = name;
	}
};

#endif /* MODULE_HPP_ */
