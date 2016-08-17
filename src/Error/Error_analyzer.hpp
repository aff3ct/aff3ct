#ifndef ERROR_ANALYZER_HPP_
#define ERROR_ANALYZER_HPP_

#include <chrono>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class Error_analyzer
{
protected:
	static bool interrupt;
	static bool first_interrupt;
	static bool over;
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_interrupt;
	static std::chrono::nanoseconds d_delta_interrupt;

	const int K;
	const int N;
	const int max_fe;
	      int n_frames;

	int n_bit_errors;
	int n_frame_errors;
	unsigned long long n_analyzed_frames;

    bool last_frame_error;

public:
	Error_analyzer(const int& K, const int& N, const int& max_fe, const int& n_frames = 1, 
	               const std::string name = "Error_analyzer");
	virtual ~Error_analyzer(){};

	void update_n_analyzed_frames();
	virtual void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V);

	bool fe_limit_achieved() const;
	int get_fe_limit() const;

	unsigned long long get_n_analyzed_frames() const;
	int get_n_fe() const;
	int get_n_be() const;

	R get_fer_value() const;
	R get_ber_value() const;

	int get_N()        const;
	int get_K()        const;
	int get_n_frames() const;
	void set_n_frames(const int n_frames);

	bool is_last_frame_error() { return last_frame_error; };

	static bool is_interrupt();
	static bool is_over();

private:
	static void signal_interrupt_handler(int signal);
};

#endif /* ERROR_ANALYZER_HPP_ */
