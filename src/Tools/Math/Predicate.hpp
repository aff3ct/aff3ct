#ifndef PREDICATE_HPP
#define PREDICATE_HPP

class Predicate
{
public:
	Predicate() {}
	virtual ~Predicate() {}

	virtual bool operator()() = 0;
	virtual void reset     () = 0;
};

#endif /* PREDICATE_HPP */
