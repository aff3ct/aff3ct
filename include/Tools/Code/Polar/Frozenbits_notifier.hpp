#ifndef FROZENBITS_NOTIFIER_HPP
#define FROZENBITS_NOTIFIER_HPP

namespace aff3ct
{
namespace tools
{
class Frozenbits_notifier
{
public:
	Frozenbits_notifier();
	virtual ~Frozenbits_notifier() = default;

	virtual void notify_frozenbits_update();
};
}
}

#endif /* FROZENBITS_NOTIFIER_HPP */
