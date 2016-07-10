#ifndef FACTORY_MODULATOR_HPP
#define FACTORY_MODULATOR_HPP

#include "../../Modulator/Modulator.hpp"

#include "../params.h"

template <typename B, typename R>
struct Factory_modulator
{
	static Modulator<B,R>* build(const t_channel_param &chan_params);
};

#endif /* FACTORY_MODULATOR_HPP */