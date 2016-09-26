#ifndef REORDERER_HPP_
#define REORDERER_HPP_

#include <vector>
#include "Tools/MIPP/mipp.h"

template <typename T>
struct Reorderer
{
public:
	static void apply    (const std::vector<const T*> in_data, T*              out_data, const int data_length);
	static void apply_rev(const T*                    in_data, std::vector<T*> out_data, const int data_length);
};

template <typename T, int N_FRAMES>
struct Reorderer_static
{
public:
	static void apply    (const std::vector<const T*> in_data, T*              out_data, const int data_length);
	static void apply_rev(const T*                    in_data, std::vector<T*> out_data, const int data_length);
};

#include "Reorderer.hxx"

#endif /* REORDERER_HPP_ */
