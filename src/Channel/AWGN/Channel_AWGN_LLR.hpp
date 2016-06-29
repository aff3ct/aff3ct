#ifndef CHANNEL_AWGN_LLR_HPP_
#define CHANNEL_AWGN_LLR_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "Channel_AWGN.hpp"

template <typename R>
class Channel_AWGN_LLR : public Channel_AWGN<R>
{
public:
    Channel_AWGN_LLR(const R& sigma, const int seed = 0, const R scaling_factor = 1);
    virtual ~Channel_AWGN_LLR();

    void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};

#endif // CHANNEL_AWGN_LLR_HPP_
