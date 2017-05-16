#ifndef FRAME_DUMPER_REDUCTION_HPP_
#define FRAME_DUMPER_REDUCTION_HPP_

#include "Frame_dumper.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Frame_dumper_reduction : Frame_dumper<B,R>
{
protected:
	std::vector<Frame_dumper<B,R>*> dumpers;

public:
	Frame_dumper_reduction(std::vector<Frame_dumper<B,R>*> dumpers, const int K_info, const int K, const int N,
	                                                                const int N_mod, const int n_frames);
	Frame_dumper_reduction(std::vector<Frame_dumper<B,R>*> dumpers, const int K_info, const int K, const int N,
	                                                                const int n_frames);
	Frame_dumper_reduction(std::vector<Frame_dumper<B,R>*> dumpers, const int K, const int N, const int n_frames);
	Frame_dumper_reduction(std::vector<Frame_dumper<B,R>*> dumpers, const int K_info, const int n_frames);

	virtual ~Frame_dumper_reduction();

	virtual void dump (const std::string& base_path);
	virtual void clear(                            );

private:
	void checks();
};
}
}

#endif /* FRAME_DUMPER_REDUCTION_HPP_ */
