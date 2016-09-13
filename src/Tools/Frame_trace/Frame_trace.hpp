#ifndef FRAME_TRACE_HPP
#define FRAME_TRACE_HPP

#include "../MIPP/mipp.h"

template <typename B>
class Frame_trace 
{
private:
	enum debug_version { BIT, REAL };
	std::ostream &stream;

	const int n_bits; // number of bits to display per frame (if = 0 display all the bits of the frame)

public:
	Frame_trace(const int n_bits = 0, std::ostream &stream = std::cout) : stream(stream), n_bits(n_bits) {};
	~Frame_trace(){}

	template <typename D>
	void display_bit_vector (mipp::vector<D> vec, mipp::vector<B> ref = {});

	template <typename D>
	void display_real_vector(mipp::vector<D> vec, mipp::vector<B> ref = {});

	template <typename D>
	void display_bit_vector (mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref = {});

	template <typename D>
	void display_real_vector(mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref = {});

private:
	template <typename D>
	void display_vector(mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref, debug_version version);

	template <typename D>
	void display_value (D value, debug_version version                                                         );

	template <typename D>
	void display_value (D value, debug_version version, B ref                                                  );
};

#include "Frame_trace.hxx"

#endif /* FRAME_TRACE_HPP */
