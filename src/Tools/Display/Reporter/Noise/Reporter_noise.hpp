#ifndef REPORTER_NOISE_HPP_
#define REPORTER_NOISE_HPP_

#include <memory>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Display/Reporter/Reporter.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Reporter_noise : public Reporter
{
public:
	explicit Reporter_noise(const Noise<R>* const* noise, const bool show_sigma = false);
	explicit Reporter_noise(const Noise<R>*        noise, const bool show_sigma = false);
	explicit Reporter_noise(const Noise<R>&        noise, const bool show_sigma = false);

	explicit Reporter_noise(const std::shared_ptr<Noise<R>>* noise, const bool show_sigma = false);
	explicit Reporter_noise(const std::shared_ptr<Noise<R>>& noise, const bool show_sigma = false);
	explicit Reporter_noise(const std::unique_ptr<Noise<R>>* noise, const bool show_sigma = false);
	explicit Reporter_noise(const std::unique_ptr<Noise<R>>& noise, const bool show_sigma = false);

	virtual ~Reporter_noise();

	report_t report(bool final = false);

private:
	struct Noise_ptr;

	explicit Reporter_noise(Noise_ptr* noise_ptr, const bool show_sigma = false);
	Noise_ptr* noise_ptr;

protected:
	const Noise_type saved_noise_type;
	group_t noise_group;
	const bool show_sigma;

protected:
	const Noise<R>* get_noise_ptr() const;
};
}
}

#endif /* REPORTER_NOISE_HPP_ */
