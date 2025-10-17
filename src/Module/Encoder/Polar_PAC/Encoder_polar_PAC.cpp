#include <algorithm>
#include <cmath>
#include <sstream>
#include <streampu.hpp>
#include <string>

#include "Module/Encoder/Polar_PAC/Encoder_polar_PAC.hpp"
#include "Tools/Code/Polar/fb_assert.h"
#include "Tools/Exception/invalid_argument/invalid_argument.hpp"

using namespace aff3ct::module;

template<typename B>
Encoder_polar_PAC<B>::Encoder_polar_PAC(const int& K,
                                        const int& N,
                                        const std::vector<bool>& frozen_bits,
                                        const std::string& conv)
  : Encoder<B>(K, N)
  , m((int)std::log2(N))
  , frozen_bits(frozen_bits)
  , X_N_tmp(this->N)
{
    const std::string name = "Encoder_polar_PAC";
    this->set_name(name);
    for (auto& t : this->tasks)
        t->set_replicability(true);
    this->set_sys(false);

    if (this->N != (int)frozen_bits.size())
    {
        std::stringstream message;
        message << "'frozen_bits.size()' has to be equal to 'N' "
                   "('frozen_bits.size()' = "
                << frozen_bits.size() << ", 'N' = " << N << ").";
        throw spu::tools::length_error(__FILE__, __LINE__, __func__, message.str());
    }

    this->set_frozen_bits(frozen_bits);

    // conv_reg = { (B)1, (B)0, (B)1, (B)1, (B)0, (B)1, (B)1 };

    // std::copy(conv.begin(), conv.end(), conv_reg.begin());
    // conv_reg = { (B)1, (B)0, (B)0, (B)0, (B)0, (B)0, (B)0 };
    // std::cout << "Encoder conv: " << conv << std::endl;
    if (conv == "NO")
    {
        std::stringstream message;
        message << "'conv' has not been set";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    if (conv.substr(0, 2) != "0o")
    {
        std::stringstream message;
        message << "'conv' is not provided in the octal format";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    conv_reg.push_back(1);

    for (int i = 0; i < conv.length() - 2; i++)
    {
        uint32_t temp = conv[i + 2] - 48;
        uint8_t a = (temp >> 0) & 1;
        uint8_t b = (temp >> 1) & 1;
        uint8_t c = (temp >> 2) & 1;
        conv_reg.push_back(c);
        conv_reg.push_back(b);
        conv_reg.push_back(a);
    }

    // std::cout << "Inside the encoder conv_reg: " << conv_reg.size() << " ";
    // for (int i = 0; i < conv_reg.size(); i++)
    // {
    //     std::cout << (int)conv_reg[i] << ",";
    // }
    // std::cout << std::endl;

    curState.resize(conv_reg.size() - 1);

    // std::cout << "Inside the Encoder_polar_PAC constructor: " << conv_reg.size() << std::endl;
}

template<typename B>
Encoder_polar_PAC<B>*
Encoder_polar_PAC<B>::clone() const
{
    auto m = new Encoder_polar_PAC(*this);
    m->deep_copy(*this);
    return m;
}

template<typename B>
void
Encoder_polar_PAC<B>::_encode(const B* U_K, B* X_N, const size_t /*frame_id*/)
{
    /*std::cout << "Info bits positions: \n";*/
    /*std::cout << "Frozen bits: " << frozen_bits.size() << std::endl;*/
    /*for (int i = 0; i < this->N; i++)*/
    /*{*/
    /*    std::cout << ((frozen_bits[i]) ? (B)0 : i) << ",";*/
    /*}*/
    /*std::cout << std::endl;*/

    this->convert(U_K, X_N);
    this->convEnc(X_N);
    /*std::cout << "intermediate codeword: ";*/
    /*for (int i = 0; i < this->N; i++)*/
    /*    std::cout << X_N[i] << " ";*/
    /*std::cout << std::endl;*/
    this->light_encode(X_N);
}

template<typename B>
B
Encoder_polar_PAC<B>::conv1bitEnc(B cbit)
{

    B u = cbit && (B)conv_reg[0];
    for (int i = 1; i < conv_reg.size(); i++)
    {
        if (conv_reg[i] == 1)
        {
            u = u ^ curState[i - 1];
        }
    }
    for (int i = curState.size() - 1; i >= 1; i--)
        curState[i] = curState[i - 1];
    curState[0] = cbit;

    return u;
}

template<typename B>
void
Encoder_polar_PAC<B>::convEnc(B* X_N)
{
    /*std::cout << "Inside the conv encoder function \n";*/

    std::vector<uint8_t> cState(conv_reg.size() - 1, 0);
    std::vector<uint8_t> u(this->N, 0);
    std::fill(curState.begin(), curState.end(), 0);

    /*std::cout << "Original: [";*/
    for (int i = 0; i < this->N; ++i)
    {
        /*std::cout << X_N[i] << ",";*/
        X_N[i] = conv1bitEnc(X_N[i]);
    }
    /*std::cout << "]" << std::endl << "Encoded: [";*/
    /*for (int i = 0; i < this->N; i++)*/
    /*{*/
    /*    std::cout << X_N[i] << ",";*/
    /*}*/
    /*std::cout << "]" << std::endl;*/
}

template<typename B>
void
Encoder_polar_PAC<B>::light_encode(B* bits)
{
    for (auto k = (this->N >> 1); k > 0; k >>= 1)
        for (auto j = 0; j < this->N; j += 2 * k)
            for (auto i = 0; i < k; i++)
                bits[j + i] = bits[j + i] ^ bits[k + j + i];
}

template<typename B>
void
Encoder_polar_PAC<B>::convert(const B* U_K, B* U_N)
{
    if (U_K == U_N)
    {
        std::vector<B> U_K_tmp(this->K);
        std::copy(U_K, U_K + this->K, U_K_tmp.begin());

        auto j = 0;
        for (unsigned i = 0; i < frozen_bits.size(); i++)
            U_N[i] = (frozen_bits[i]) ? (B)0 : U_K_tmp[j++];
    }
    else
    {
        auto j = 0;
        for (unsigned i = 0; i < frozen_bits.size(); i++)
            U_N[i] = (frozen_bits[i]) ? (B)0 : U_K[j++];
    }
    /*for (int i = 0; i < this->N; i++)*/
    /*{*/
    /*    std::cout << ((frozen_bits[i]) ? (B)0 : i) << ",";*/
    /*}*/
}

template<typename B>
bool
Encoder_polar_PAC<B>::is_codeword(const B* X_N)
{
    std::copy(X_N, X_N + this->N, this->X_N_tmp.data());

    for (auto k = (this->N >> 1); k > 0; k >>= 1)
        for (auto j = 0; j < this->N; j += 2 * k)
        {
            for (auto i = 0; i < k; i++)
                this->X_N_tmp[j + i] = this->X_N_tmp[j + i] ^ this->X_N_tmp[k + j + i];

            if (this->frozen_bits[j + k - 1] && this->X_N_tmp[j + k - 1]) return false;
        }

    return true;
}

template<typename B>
void
Encoder_polar_PAC<B>::set_frozen_bits(const std::vector<bool>& frozen_bits)
{
    aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
    std::copy(frozen_bits.begin(), frozen_bits.end(), this->frozen_bits.begin());
    auto k = 0;
    for (auto n = 0; n < this->N; n++)
        if (!this->frozen_bits[n]) this->info_bits_pos[k++] = n;
}

template<typename B>
const std::vector<bool>&
Encoder_polar_PAC<B>::get_frozen_bits() const
{
    return this->frozen_bits;
}
// ====================================================================================
// explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_polar_PAC<B_8>;
template class aff3ct::module::Encoder_polar_PAC<B_16>;
template class aff3ct::module::Encoder_polar_PAC<B_32>;
template class aff3ct::module::Encoder_polar_PAC<B_64>;
#else
template class aff3ct::module::Encoder_polar_PAC<B>;
#endif
// ====================================================================================
// explicit template instantiation
