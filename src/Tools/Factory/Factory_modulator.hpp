#ifndef FACTORY_MODULATOR_HPP
#define FACTORY_MODULATOR_HPP

#include "../../Modulator/Modulator.hpp"

#include "../params.h"

template <typename B>
struct Factory_modulator
{
	static Modulator<B>* build();
};

#endif /* FACTORY_MODULATOR_HPP */