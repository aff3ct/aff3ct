#include <type_traits>
#include <sstream>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"

#include "Modem_OOK_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

//std::vector<std::vector<float>> llrs;
//size_t llr_idx;

template <typename B, typename R, typename Q>
Modem_OOK_optical<B,R,Q>
::Modem_OOK_optical(const int N, const tools::Distributions<R>& dist, const tools::Noise<R>& noise, const int n_frames)
: Modem_OOK<B,R,Q>(N, noise, n_frames), dist(dist), current_dist(nullptr)
{
	const std::string name = "Modem_OOK_optical";
	this->set_name(name);

//	std::ifstream file("/media/ohartmann/DATA/Documents/Projets/CNES_AIRBUS/matrices/2018_05_03/vectorTestIMS/TestVec ROP -32/AFF3CT/LLR.txt");
//
//	if (!file.is_open())
//		throw tools::runtime_error();
//
//	unsigned length, n_llrs;
//	file >> n_llrs >> length;
//
//	llrs.resize(n_llrs, std::vector<float>(length));
//
//	for (unsigned i = 0; i < n_llrs; i++)
//	{
//		for (unsigned j = 0; j < length; j++)
//			file >> llrs[i][j];
//	}
//	llr_idx = 0;
}

template <typename B, typename R, typename Q>
void Modem_OOK_optical<B,R,Q>
::set_noise(const tools::Noise<R>& noise)
{
	Modem_OOK<B,R,Q>::set_noise(noise);

	this->current_dist = &dist.get_distribution(this->n->get_noise());

	if (this->current_dist == nullptr)
	{
		std::stringstream message;
		message << "Undefined noise power 'this->n->get_noise()' in the given distributions"
		           " ('this->n->get_noise()' = " << this->n->get_noise() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, typename Q>
void Modem_OOK_optical<B,R,Q>
::check_noise()
{
	Modem_OOK<B,R,Q>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::ROP);
}

template <typename B, typename R, typename Q>
void Modem_OOK_optical<B,R,Q>
::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (this->current_dist == nullptr)
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No valid noise has been set.");

	const auto& pdf_x  = this->current_dist->get_pdf_x();
	const auto& pdf_y0 = this->current_dist->get_pdf_y()[0];
	const auto& pdf_y1 = this->current_dist->get_pdf_y()[1];

	const Q min_value = (Q)1e-10; // when prob_1 ou prob_0 = 0;

	for (auto i = 0; i < this->N_fil; i++)
	{
		auto x_pos = tools::get_closest_index(pdf_x.begin(), pdf_x.end(), Y_N1[i]);

		auto prob_0 = pdf_y0[x_pos] == (Q)0 ? min_value : pdf_y0[x_pos];
		auto prob_1 = pdf_y1[x_pos] == (Q)0 ? min_value : pdf_y1[x_pos];

		Y_N2[i] = std::log(prob_0/prob_1);
	}
}

//template <typename B, typename R, typename Q>
//void Modem_optical<B,R,Q>
//::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
//{
//	std::copy(llrs[llr_idx].begin(), llrs[llr_idx].end(), Y_N2);
//	llr_idx = (llr_idx + 1) % llrs.size();
//};

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Modem_OOK_optical<B_8,R_8,R_8>;
template class aff3ct::module::Modem_OOK_optical<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_OOK_optical<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK_optical<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK_optical<B_32,R_32,R_32>;
template class aff3ct::module::Modem_OOK_optical<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_OOK_optical<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template class aff3ct::module::Modem_OOK_optical<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
