#ifndef ERASED_PROBABILITY_HPP_
#define ERASED_PROBABILITY_HPP_

#include "Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Erasure_probability : public Noise<R>
{
public:
	Erasure_probability() = default;
	explicit Erasure_probability(R noise);
	template<typename T>
	Erasure_probability(const Erasure_probability<T>& other);
	virtual ~Erasure_probability() = default;

	virtual std::string get_unity() const;
	virtual Noise_type get_type() const;

	virtual Erasure_probability<R>* clone() const;

protected:
	virtual void check();
};

template <typename R = float>
using EP = Erasure_probability<R>;

}
}

#endif //ERASED_PROBABILITY_HPP_
