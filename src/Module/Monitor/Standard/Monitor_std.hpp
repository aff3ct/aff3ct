#ifndef MONITOR_STD_HPP_
#define MONITOR_STD_HPP_

#include <chrono>
#include <vector>
#include <functional>

#include "../Monitor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Monitor_std : public Monitor<B>
{
protected:
	const unsigned max_fe;

	unsigned long long n_bit_errors;
	unsigned long long n_frame_errors;
	unsigned long long n_analyzed_frames;

	std::vector<std::function<void(int )>> callbacks_fe;
	std::vector<std::function<void(void)>> callbacks_check;
	std::vector<std::function<void(void)>> callbacks_fe_limit_achieved;

public:
	Monitor_std(const int size, const unsigned max_fe, const int n_frames = 1, const std::string name = "Monitor_std");
	virtual ~Monitor_std(){};

	virtual bool fe_limit_achieved();
	unsigned get_fe_limit() const;

	virtual unsigned long long get_n_analyzed_fra() const;
	virtual unsigned long long get_n_fe          () const;
	virtual unsigned long long get_n_be          () const;

	float get_fer() const;
	float get_ber() const;

	virtual void add_handler_fe               (std::function<void(int )> callback);
	virtual void add_handler_check            (std::function<void(void)> callback);
	virtual void add_handler_fe_limit_achieved(std::function<void(void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

protected:
	virtual int _check_errors(const B *U, const B *V, const int frame_id);
};
}
}

#endif /* MONITOR_STD_HPP_ */
