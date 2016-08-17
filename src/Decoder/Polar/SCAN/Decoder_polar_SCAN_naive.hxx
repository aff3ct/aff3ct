#include <iostream>
#include <iomanip>
#include <math.h>
#include <cassert>

#include "Decoder_polar_SCAN_naive.hpp"

/********************************************************************/
/** CONSTRUCTOR **/
/********************************************************************/
template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
Decoder_polar_SCAN_naive<B,R,I,F,V,H>
::Decoder_polar_SCAN_naive(const int &K, const int &m, const int &max_iter, const mipp::vector<B> &frozen_bits, 
                           const std::string name)
: Decoder<B,R>  (name        ),
  K             (K           ),
  m             (m           ),
  N             (1 << m      ),
  max_iter      (max_iter    ),
  layers_count  (m +1        ),
  frozen_bits   (frozen_bits ),
  feedback_graph(layers_count),
  soft_graph    (layers_count)
{
	assert(max_iter           >  0           );
	assert(frozen_bits.size() == (unsigned) N);

	for (auto t = 0; t < layers_count; t++)
	{
		feedback_graph[t].resize(N);
		soft_graph[t].resize(N);
	}
}

/********************************************************************/
/** load **/
/********************************************************************/
template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive<B,R,I,F,V,H>
::load_init()
{
	// init feedback graph (special case for the left most stage)
	for (auto i = 0; i < N; i++)
		if (frozen_bits[i])// if i is a frozen bit		
			feedback_graph[0][i] = sat_vals<R>().second;
		else		
			feedback_graph[0][i] = I();

	// init the rest of the feedback graph 
	for (auto t = 1; t < layers_count; t++)
		for (auto i = 0; i < N; i++)
			feedback_graph[t][i] = I();
	
	// init the softGraph 
	// (except for the layer "layers_count -1" because it will made by the "load" routine from the LLRs)
	for (auto t = 0; t < layers_count -1; t++)
		for (auto i = 0; i < N; i++)
			soft_graph[t][i] = I();
}

template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive<B,R,I,F,V,H>
::load(const mipp::vector<R>& Y_N)
{
	assert(Y_N.size() == (unsigned) N);

	load_init();
		
	// init the softGraph (special case for the right most stage)
	for (auto i = 0; i < N; i++)
		soft_graph[layers_count - 1][i] = Y_N[i];
}

/********************************************************************/
/** frame_decode **/
/********************************************************************/
template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive<B,R,I,F,V,H>
::decode()
{
	for (auto iter = 0; iter < max_iter; iter++)
	{
		for (auto i = 0; i < N; i++)
		{
			/**********************/
			/* forward (right to left) propagation of soft information in the graph */
			fb_compute_soft_output(i);
			/**********************/

			if (frozen_bits[i])// if i is a frozen bit
			{
				feedback_graph[0][i] = sat_vals<R>().second;
				soft_graph    [0][i] = sat_vals<R>().second;
			}
			else
				feedback_graph[0][i] = I();

			/**********************/
			/* propagate back (left to right) the soft information in the graph */
			set_soft_val_and_propagate(0, i, feedback_graph[0][i]);
			/**********************/
		}
	}
}

/********************************************************************/
/** frame store **/
/********************************************************************/
template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive<B,R,I,F,V,H>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() == (unsigned) K);

	auto k = 0;
	for (auto i = 0; i < N; i++)
		if (!frozen_bits[i]) // if i is not a frozen bit
			V_K[k++] = (H(soft_graph[0][i]) == 0) ? (B)0 : (B)1;
}

/********************************************************************/
/** set_and_propagate **/
/** set the soft information (l,j) to v and propagate the value in feedbackGraph**/
/********************************************************************/
template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive<B,R,I,F,V,H>
::set_soft_val_and_propagate(const int l, const int j, const R v)
{
	const auto up_ix = j - (1 << l);
	const auto down_ix = j;

	feedback_graph[l][j] = v; // set the bit in the feedback graph

	if ((j >> l) & 1) // propagate recursively if the l'th bit of j == 1
	{
		auto v1 = F(feedback_graph[l][up_ix  ], V(feedback_graph[l][down_ix], soft_graph[l + 1][down_ix]));
		auto v2 = V(feedback_graph[l][down_ix], F(feedback_graph[l][up_ix  ], soft_graph[l + 1][up_ix  ]));

		set_soft_val_and_propagate(l +1,   up_ix, v1); // recursive call
		set_soft_val_and_propagate(l +1, down_ix, v2); // recursive call
	}
}

/********************************************************************/
/** compute_soft_output **/
/********************************************************************/
template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive<B,R,I,F,V,H>
::fb_compute_soft_output(const int &i)
{
	auto l_start = compute_depth(i, this->m);

	for (auto l = l_start; l >= 0; l--) // for each layer
	{
		auto j_stop = 1 << (l);
		for (auto j = i; j < i + j_stop; j++)
		{
			auto f_up_ix   = j;
			auto f_down_ix = j + (1 << l);
			auto g_up_ix   = j - (1 << l);
			auto g_down_ix = j;

			if ((i & (1 << l)) == 0) // function F
				soft_graph[l][j] = F(soft_graph[l + 1][  f_up_ix], 
				                     V(soft_graph[l + 1][f_down_ix], feedback_graph[l][f_down_ix]));
			else // function G
				soft_graph[l][j] = V(soft_graph[l + 1][g_down_ix], 
				                     F(feedback_graph[l][  g_up_ix], soft_graph[l + 1][  g_up_ix]));
		}
	}
}

/********************************************************************/
/** Display_decoder_feedbackgraph **/
/********************************************************************/
template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive<B,R,I,F,V,H>
::display_decoder_graph()
{
	std::cout << "soft_graph:" << std::endl;
	for (auto i = 0; i < N; i++)
	{
		for (auto j = 0; j <= m; j++)
			std::cout << "\t\t" << std::setprecision(4) << std::setw(4) << soft_graph[j][i];
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;

	std::cout << "feedback_graph:" << std::endl;
	for (auto i = 0; i < N; i++)
	{
		for (auto j = 0; j <= m; j++)
			std::cout << "\t\t" << std::setprecision(4) << std::setw(4) << feedback_graph[j][i];
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
}
