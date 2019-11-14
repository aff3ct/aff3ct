/*!
 * \file
 * \brief Class module::Extractor.
 */
#ifndef EXTRACTOR_HPP_
#define EXTRACTOR_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace ext
	{
		enum class tsk : size_t { get_sys_llr, get_sys_bit, get_sys_and_par_llr, add_sys_and_ext_llr, SIZE };

		namespace sck
		{
			enum class get_sys_llr         : size_t { Y_N, Y_K     , SIZE };
			enum class get_sys_bit         : size_t { Y_N, V_K     , SIZE };
			enum class get_sys_and_par_llr : size_t { Y_N, sys, par, SIZE };
			enum class add_sys_and_ext_llr : size_t { ext, Y_N     , SIZE };
		}
	}

template <typename B = int, typename Q = float>
class Extractor : public Module
{
public:
	inline Task&   operator[](const ext::tsk                      t);
	inline Socket& operator[](const ext::sck::get_sys_llr         s);
	inline Socket& operator[](const ext::sck::get_sys_bit         s);
	inline Socket& operator[](const ext::sck::get_sys_and_par_llr s);
	inline Socket& operator[](const ext::sck::add_sys_and_ext_llr s);

protected:
	const int K;
	const int N;
	const int tail_length;

public:
	Extractor(const int K, const int N, const int tail_length = 0, const int n_frames = 1);
	virtual ~Extractor() = default;

	template <class A = std::allocator<Q>>
	void get_sys_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &Y_K, const int frame_id = -1);

	virtual void get_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id = -1);

	template <class AQ = std::allocator<Q>, class AB = std::allocator<B>>
	void get_sys_bit(const std::vector<Q,AQ> &Y_N, std::vector<B,AB> &V_K, const int frame_id = -1);

	virtual void get_sys_bit(const Q *Y_N, B *V_K, const int frame_id = -1);

	template <class A = std::allocator<Q>>
	void get_sys_and_par_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &sys, std::vector<Q,A> &par,
	                         const int frame_id = -1);

	virtual void get_sys_and_par_llr(const Q *Y_N, Q *sys, Q *par, const int frame_id = -1);

	template <class A = std::allocator<Q>>
	void add_sys_and_ext_llr(const std::vector<Q,A> &ext, std::vector<Q,A> &Y_N, const int frame_id = -1);

	virtual void add_sys_and_ext_llr(const Q *ext, Q *Y_N, const int frame_id = -1);

protected:
	virtual const std::vector<uint32_t>& get_info_bits_pos() = 0;

	virtual void _get_sys_llr        (const Q *Y_N, Q *Y_K,         const int frame_id);
	virtual void _get_sys_bit        (const Q *Y_N, B *V_K,         const int frame_id);
	virtual void _get_sys_and_par_llr(const Q *Y_N, Q *sys, Q *par, const int frame_id);
	virtual void _add_sys_and_ext_llr(const Q *ext, Q *Y_N,         const int frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Extractor/Extractor.hxx"
#endif

#endif /* ENCODER_HPP_ */
