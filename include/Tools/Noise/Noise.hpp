/*!
 * \file
 * \brief Class tools::Noise.
 */
#ifndef NOISE_HPP__
#define NOISE_HPP__

#include <string>
#include <vector>
#include <cstdint>
#include <functional>

#include "Tools/Algo/Callback/Callback.hpp"

namespace aff3ct
{
namespace tools
{
enum class Noise_type {SIGMA, ROP, EP};

template <typename R = float>
class Noise
{
protected:
	R value;
	Callback<> callback_update;

public:
	Noise();
	explicit Noise(R value);
	virtual ~Noise() = default;

	R get_value() const; // return the stocked noise, throw if not set

	bool is_set() const;

	void set_value(const R value); // set the noise val and call 'check' to check that it respects the rules

	virtual void copy(const Noise<R>& other); // set this noise as the 'other' one

	virtual std::string get_unity() const = 0; // return a string with the unity of the noise type
	virtual Noise_type  get_type () const = 0;

	bool is_of_type      (Noise_type t) const noexcept;
	void is_of_type_throw(Noise_type t) const;

	uint32_t record_callback_update(std::function<void()> callback);
	bool unrecord_callback_update(const uint32_t id);
	void clear_callbacks_update();

	static inline Noise_type str_to_type(const std::string& str);
	static inline std::string type_to_str(Noise_type t);

protected:
	virtual void check() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Noise/Noise.hxx"
#endif

#endif // NOISE_HPP__