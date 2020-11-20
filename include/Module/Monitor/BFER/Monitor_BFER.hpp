/*!
 * \file
 * \brief Class module::Monitor_BFER.
 */
#ifndef MONITOR_BFER_HPP_
#define MONITOR_BFER_HPP_

#include <vector>
#include <memory>
#include <cstdint>
#include <functional>

#include "Tools/Algo/Histogram.hpp"
#include "Tools/Algo/Callback/Callback.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Monitor/Monitor.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Monitor_BFER : public Monitor
{
public:
	inline Task&   operator[](const mnt::tsk                t);
	inline Socket& operator[](const mnt::sck::check_errors  s);
	inline Socket& operator[](const mnt::sck::check_errors2 s);

protected:
	struct Attributes
	{
		unsigned long long n_fra; // the number of checked frames
		unsigned long long n_be;  // the number of wrong bits
		unsigned long long n_fe;  // the number of wrong frames

		Attributes();
		void reset();
		Attributes& operator+=(const Attributes& a);
	};

private:
	const int      K;                    // Number of source bits
	const unsigned max_fe;               // max number of wrong frames to get then fe_limit_achieved() returns true else if 0
	const unsigned max_n_frames;         // max number of frames to check then frame_limit_achieved() returns true else if 0
	const bool     count_unknown_values; // take into account or not the unknown values as wrong values in the checked frames

	Attributes vals;
	tools::Histogram<int> err_hist; // the error histogram record
	bool err_hist_activated;

	tools::Callback<unsigned, int> callback_fe;
	tools::Callback<             > callback_check;
	tools::Callback<             > callback_fe_limit_achieved;

public:
	Monitor_BFER(const int K, const unsigned max_fe, const unsigned max_n_frames = 0, const bool count_unknown_values = false);

	virtual ~Monitor_BFER() = default;

	virtual Monitor_BFER<B>* clone() const;

	bool equivalent(const Monitor_BFER<B>& m, bool do_throw = false) const; // check if this monitor and "m" have equivalent construction arguments
	                                                                        // and then can be merged by "collect" or "copy" methods
	/*!
	 * \brief Compares two messages and counts the number of frame errors and bit errors.
	 *
	 * Typically this method is called at the very end of a communication chain.
	 *
	 * \param U: the original message (from the Source or the CRC).
	 * \param Y: the decoded message (from the Decoder).
	 */
	template <class A = std::allocator<B>>
	int check_errors(const std::vector<B,A>& U,
	                 const std::vector<B,A>& V,
	                 const int frame_id = -1,
	                 const bool managed_memory = true);

	template <class A = std::allocator<B>>
	int check_errors2(const std::vector<B,A    >& U,
	                  const std::vector<B,A    >& V,
	                        std::vector<int64_t>& FRA,
	                        std::vector<int32_t>& BE,
	                        std::vector<int32_t>& FE,
	                        std::vector<float  >& BER,
	                        std::vector<float  >& FER,
	                  const int frame_id = -1,
	                  const bool managed_memory = true);

	int check_errors(const B *U,
	                 const B *V,
	                 const int frame_id = -1,
	                 const bool managed_memory = true);

	int check_errors2(const B       *U,
	                  const B       *V,
	                        int64_t *FRA,
	                        int32_t *BE,
	                        int32_t *FE,
	                        float   *BER,
	                        float   *FER,
	                  const int frame_id = -1,
	                  const bool managed_memory = true);

	bool    fe_limit_achieved() const;
	bool frame_limit_achieved() const;
	virtual bool is_done() const;

	int                   get_K                   () const;
	bool                  get_count_unknown_values() const;
	unsigned              get_max_fe              () const;
	unsigned              get_max_n_frames        () const;
	unsigned long long    get_n_analyzed_fra      () const;
	unsigned long long    get_n_fe                () const;
	unsigned long long    get_n_be                () const;
	float                 get_fer                 () const;
	float                 get_ber                 () const;

	const tools::Histogram<int>& get_err_hist     () const;
	void activate_err_histogram(bool val);

	virtual uint32_t record_callback_fe               (std::function<void(unsigned, int)> callback);
	virtual uint32_t record_callback_check            (std::function<void(void         )> callback);
	virtual uint32_t record_callback_fe_limit_achieved(std::function<void(void         )> callback);

	virtual bool unrecord_callback_fe               (const uint32_t id);
	virtual bool unrecord_callback_check            (const uint32_t id);
	virtual bool unrecord_callback_fe_limit_achieved(const uint32_t id);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor& m,         bool fully = false);
	virtual void collect(const Monitor_BFER<B>& m, bool fully = false);
	virtual void collect(const Attributes& v);

	Monitor_BFER<B>& operator+=(const Monitor_BFER<B>& m); // not full "collect" call

	virtual void copy(const Monitor& m,         bool fully = false);
	virtual void copy(const Monitor_BFER<B>& m, bool fully = false);
	virtual void copy(const Attributes& v);

	Monitor_BFER<B>& operator=(const Monitor_BFER<B>& m); // not full "copy" call

protected:
	const Attributes& get_attributes() const;

	virtual int _check_errors(const B *U, const B *V, const size_t frame_id);

	virtual int _check_errors2(const B       *U,
	                           const B       *V,
	                                 int64_t *FRA,
	                                 int32_t *BE,
	                                 int32_t *FE,
	                                 float   *BER,
	                                 float   *FER,
	                           const size_t frame_id);

	virtual int __check_errors(const B *U, const B *V, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Monitor/BFER/Monitor_BFER.hxx"
#endif

#endif /* MONITOR_BFER_HPP_ */
