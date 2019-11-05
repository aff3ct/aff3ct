/*!
 * \file
 * \brief Class tools::Noise.
 */
#ifndef NOISE_HPP__
#define NOISE_HPP__

#include <string>
#include <vector>
#include <functional>

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
	std::vector<std::pair<std::function<void()>, const void*>> callbacks_changed;

public:
	Noise();
	explicit Noise(R value);
	virtual ~Noise() = default;

	R get_value() const; // return the stocked noise, throw if not set

	void set_value(const R value); // set the noise val and call 'check' to check that it respects the rules

	virtual void copy(const Noise<R>& other); // set this noise as the 'other' one

	virtual std::string get_unity() const = 0; // return a string with the unity of the noise type
	virtual Noise_type  get_type () const = 0;

	bool is_of_type      (Noise_type t) const noexcept;
	void is_of_type_throw(Noise_type t) const;

	void register_callback_changed(std::function<void()> callback, const void *obj_ptr = nullptr);
	void unregister_callbacks_changed(const void *obj_ptr);
	void clear_callbacks_changed();

	static inline Noise_type str_to_type(const std::string& str);
	static inline std::string type_to_str(Noise_type t);

protected:
	void notify_noise_changed();
	virtual void check() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Noise/Noise.hxx"
#endif

#endif // NOISE_HPP__