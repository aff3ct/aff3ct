#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include "Module/Decoder/Decoder.hpp"
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <string>
#endif
#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>
#include <streampu.hpp>
#include <tuple>

#include "Module/Decoder/Polar_PAC/Fano/Decoder_polar_PAC_Fano_naive.hpp"
#include "Tools/Code/Polar/fb_assert.h"

namespace aff3ct
{
namespace module
{
template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
Decoder_polar_PAC_Fano_naive<B, R, F, G>::Decoder_polar_PAC_Fano_naive(const int& K,
                                                                       const int& N,
                                                                       const std::vector<bool>& frozen_bits,
                                                                       const std::string& conv,
                                                                       const float DELTA,
                                                                       const std::vector<double>& Perr_)
  : Decoder_SIHO<B, R>(K, N)
  , m((int)std::log2(N))
  , metric_init(std::numeric_limits<R>::min())
  , frozen_bits(frozen_bits)
  , polar_trees(tools::Binary_tree_metric<Contents_PAC_Fano<B, R>, R>(this->m + 1, metric_init))
  , DELTA(DELTA)
  , P_err(Perr_)
  , Bvec(K, 0)
  , P_err_cum(N, 0.0)
  , betas(K, 0.0)
  , kappa(K, 0)
  , mus(N, 0.0)
{
    const std::string name = "Decoder_polar_PAC_Fano_naive";
    this->set_name(name);

    if (!spu::tools::is_power_of_2(this->N))
    {
        std::stringstream message;
        message << "'N' has to be a power of 2 ('N' = " << N << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    if (this->N != (int)frozen_bits.size())
    {
        std::stringstream message;
        message << "'frozen_bits.size()' has to be equal to 'N' "
                   "('frozen_bits.size()' = "
                << frozen_bits.size() << ", 'N' = " << N << ").";
        throw spu::tools::length_error(__FILE__, __LINE__, __func__, message.str());
    }

    auto k = 0;
    for (auto i = 0; i < this->N; i++)
        if (frozen_bits[i] == 0) k++;
    if (this->K != k)
    {
        std::stringstream message;
        message << "The number of information bits in the frozen_bits is "
                   "invalid ('K' = "
                << K << ", 'k' = " << k << ").";
        throw spu::tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
    }

    this->recursive_allocate_nodes_contents(this->polar_trees.get_root(), this->N);
    this->recursive_initialize_frozen_bits(this->polar_trees.get_root(), frozen_bits);

    leaves_array = this->polar_trees.get_leaves();

    for (auto& t : this->tasks)
        t->set_replicability(true);

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

    for (size_t i = 0; i < conv.length() - 2; i++)
    {
        uint32_t temp = conv[i + 2] - 48;
        // std::cout << "v(" << temp << ")";
        uint8_t a = (temp >> 0) & 1;
        uint8_t b = (temp >> 1) & 1;
        uint8_t c = (temp >> 2) & 1;
        conv_reg.push_back(c);
        conv_reg.push_back(b);
        conv_reg.push_back(a);
    }

    cState.resize(this->conv_reg.size() - 1, 0);

    int idx = 0;
    for (int ii = 0; ii < this->N; ii++)
    {
        if (!this->frozen_bits[ii])
        {
            Bvec[idx++] = ii;
        }
    }

    for (int i = 0; i < N; i++)
    {
        double a_;
        this->calc_Perr(a_, i);
        this->P_err_cum[i] = a_;
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
Decoder_polar_PAC_Fano_naive<B, R, F, G>::~Decoder_polar_PAC_Fano_naive()
{
    this->recursive_deallocate_nodes_contents(this->polar_trees.get_root());
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
Decoder_polar_PAC_Fano_naive<B, R, F, G>*
Decoder_polar_PAC_Fano_naive<B, R, F, G>::clone() const
{
    auto m = new Decoder_polar_PAC_Fano_naive(*this);
    m->deep_copy(*this);
    return m;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::deep_copy(const Decoder_polar_PAC_Fano_naive<B, R, F, G>& m)
{
    spu::module::Stateful::deep_copy(m);
    this->leaves_array.clear();
    this->recursive_deep_copy(m.polar_trees.get_root(), this->polar_trees.get_root());
    this->leaves_array = this->polar_trees.get_leaves();
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::recursive_deep_copy(const tools::Binary_node<Contents_PAC_Fano<B, R>>* nref,
                                                              tools::Binary_node<Contents_PAC_Fano<B, R>>* nclone)
{
    auto cref = nref->get_contents();
    auto cclone = new Contents_PAC_Fano<B, R>(*cref);
    nclone->set_contents(cclone);

    if (!nref->is_leaf() && !nclone->is_leaf())
    {
        if (nref->get_left() != nullptr) this->recursive_deep_copy(nref->get_left(), nclone->get_left());

        if (nref->get_right() != nullptr) this->recursive_deep_copy(nref->get_right(), nclone->get_right());
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::set_frozen_bits(const std::vector<bool>& fb)
{
    aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
    std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());

    int idx = 0;
    for (int i = 0; i < this->N; i++)
    {
        if (!this->frozen_bits[i])
        {
            Bvec[idx++] = i;
        }
    }

    this->recursive_initialize_frozen_bits(this->polar_trees.get_root(), frozen_bits);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
const std::vector<bool>&
Decoder_polar_PAC_Fano_naive<B, R, F, G>::get_frozen_bits() const
{
    return this->frozen_bits;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::_load(const R* Y_N)
{
    auto* contents = this->polar_trees.get_root()->get_contents();
    for (auto i = 0; i < this->N; i++)
        contents->lambda[i] = Y_N[i];
    this->polar_trees.set_path_metric(metric_init);

    currState.resize(this->K + 1, std::vector<B>(this->conv_reg.size() - 1, 0));

    std::fill(cState.begin(), cState.end(), 0);
    std::fill(betas.begin(), betas.end(), 0.0);
    std::fill(kappa.begin(), kappa.end(), 0);
    std::fill(mus.begin(), mus.end(), 0.0);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
double
Decoder_polar_PAC_Fano_naive<B, R, F, G>::cal_PM(int i)
{
    double ac_0 = 0;
    for (int j = 0; j <= i; ++j)
    {
        auto node_cur = this->leaves_array[j];
        ac_0 += actual_metric_cal(node_cur->get_c()->lambda[0], node_cur->get_c()->s[0] ? 1 : 0);
    }
    return ac_0;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
double
Decoder_polar_PAC_Fano_naive<B, R, F, G>::actual_metric_cal(double lru, uint8_t ucap)
{
    double a0 = -1 * std::log(1 + std::exp(-(1 - 2 * (double)ucap) * lru));
    return a0;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::_decode(const size_t frame_id)
{

    int i = 0;
    int j = -1;
    double threshold = 0.0f;
    double B_ = -1.0f;
    double mu = 0.0f;
    double expected_metric = 0.0f;

    double temp_metric = 0.0f;

    expected_metric = this->P_err_cum[this->N - 1];
    std::pair<B, std::vector<B>> curpr0;
    std::pair<B, std::vector<B>> curpr1;

    std::vector<double> r_values(3, 0.0);
    std::vector<std::vector<B>> cms_01;
    cms_01.resize(2, std::vector<B>(this->conv_reg.size() - 1, 0));

    // run through each leaf
    while (i < this->N)
    {
        // compute LLR for current leaf
        this->recursive_compute_llr(leaves_array[i], tools::compute_depth(i, this->m));
        temp_metric = this->P_err_cum[i];

        auto cur_leaf = leaves_array[i];
        // if current leaf is a frozen bit
        if (leaves_array[i]->get_c()->is_frozen_bit)
        {
            auto min_phi = std::numeric_limits<R>::max();
            cur_leaf->get_c()->v[0] = 0;
            /*std::cout << cur_leaf->get_c()->v.size() << ", " <<
             * cur_leaf->get_c()->s.size() << std::endl;*/
            std::pair<B, std::vector<B>> res = conv1bitEnc((B)0, cState);

            cur_leaf->get_c()->s[0] = res.first ? spu::tools::bit_init<B>() : 0;
            cState = res.second;

            this->propagate_sums(cur_leaf);

            // auto phi_cur =
            //   tools::phi<R>(polar_trees.get_path_metric(), cur_leaf->get_c()->lambda[0], cur_leaf->get_c()->s[0]);
            // this->polar_trees.set_path_metric(phi_cur);

            double cur_llr = cur_leaf->get_c()->lambda[0];
            if (i == 0)
            {
                mus[i] = this->actual_metric_cal(cur_llr, res.first ? 1 : 0) + expected_metric;
                // mus[i] = phi_cur + expected_metric;
            }
            else
            {
                mus[i] = mus[i - 1] + this->actual_metric_cal(cur_llr, res.first ? 1 : 0);
                // mus[i] = phi_cur;
            }

            // this->polar_trees.set_path_metric(mus[i]);
            mus[i] = mus[i] - temp_metric;
            i++;
        }
        else
        {
            double cur_llr = cur_leaf->get_c()->lambda[0];
            double exp_metric = temp_metric;
            curpr0 = conv1bitEnc((B)0, cState);
            curpr1 = conv1bitEnc((B)1, cState);
            double mu_0 = cal_PM(i - 1) + actual_metric_cal(cur_llr, curpr0.first ? 1 : 0) - exp_metric;
            double mu_1 = cal_PM(i - 1) + actual_metric_cal(cur_llr, curpr1.first ? 1 : 0) - exp_metric;
            std::vector<B> ucaps(2, 0);
            cms_01[0] = curpr0.second;
            cms_01[1] = curpr1.second;

            ucaps[0] = curpr0.first;
            ucaps[1] = curpr1.first;

            double mu_max = std::max(mu_0, mu_1);
            double mu_min = std::min(mu_0, mu_1);

            int v_max = 0;
            int v_min = 0;
            if (mu_max == mu_0)
            {
                v_max = 0;
            }
            else
            {
                v_max = 1;
            }

            if (mu_min == mu_0)
            {
                v_min = 0;
            }
            else
            {
                v_min = 1;
            }

            if (mu_max > threshold)
            {
                if (B_ == -1)
                {
                    cur_leaf->get_c()->s[0] = ucaps[v_max] ? spu::tools::bit_init<B>() : 0;
                    propagate_sums(cur_leaf);

                    kappa[j + 1] = 0;
                    betas[j + 1] = mu_max;
                    mus[i] = mu_max;
                    // vis[i] = (uint8_t)v_max;
                    cur_leaf->get_c()->v[0] = (B)v_max;
                    currState[j + 1] = cState;
                    cState = cms_01[v_max];
                    if (j == -1)
                    {
                        mu = 0;
                    }
                    else if (j != -1)
                    {
                        mu = betas[j];
                    }
                    if (mu < (threshold + DELTA))
                    {
                        this->update_threshold(threshold, betas[j + 1]);
                    }

                    j++;
                    i++;
                }
                else if (B_ == 1)
                {
                    if (mu_min > threshold)
                    {
                        // u_matrix[n][i] = ucaps[v_min];
                        cur_leaf->get_c()->s[0] = ucaps[v_min] ? spu::tools::bit_init<B>() : 0;
                        // vis[i] = (uint8_t)v_min;
                        cur_leaf->get_c()->v[0] = (B)v_min;
                        // updatepartialsum(u_matrix, n, i);
                        this->propagate_sums(cur_leaf);
                        // updatepartialsum(u_matrix, n, i);
                        betas[j + 1] = mu_min;
                        kappa[j + 1] = 1;
                        mus[i] = mu_min;
                        currState[j + 1] = cState;
                        cState = cms_01[v_min];
                        j++;
                        i++;
                        B_ = -1;
                    }
                    else if (mu_min <= threshold)
                    {
                        if (j == -1)
                        {
                            threshold -= DELTA;
                            B_ = -1;
                        }
                        else if (j != -1)
                        {
                            int jp = j;
                            std::vector<B> csp = cState;
                            // BACK is to be called here ... with return arugment T,j,B
                            // std::cout << "back0_before:" << j << "\n";

                            this->BACK(r_values, betas, j, threshold, kappa);
                            threshold = r_values[0];
                            j = r_values[1];
                            B_ = r_values[2];

                            if (jp == j)
                            {
                                cState = csp;
                            }
                            else
                            {
                                cState = currState[j + 1];
                            }
                            i = Bvec[j + 1];
                        }
                    }
                }
            }
            else if (mu_max <= threshold)
            {
                if (j == -1)
                {
                    // here
                    threshold -= DELTA;
                }
                else if (j != -1)
                {
                    // here
                    int jp = j;
                    std::vector<B> csp = cState;
                    // BACK is to be called here with return argurment in order T,j,B

                    this->BACK(r_values, betas, j, threshold, kappa);

                    threshold = r_values[0];
                    j = r_values[1];
                    B_ = r_values[2];

                    i = Bvec[j + 1];
                    if (j == jp)
                    {
                        cState = csp;
                    }
                    else
                    {
                        cState = currState[j + 1];
                    }
                }
            }
        }

        // propagate sums
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
int
Decoder_polar_PAC_Fano_naive<B, R, F, G>::_decode_siho(const R* Y_N, B* V_K, const size_t frame_id)
{
    //	auto t_load = std::chrono::steady_clock::now(); //
    //-----------------------------------------------------------
    // LOAD
    this->_load(Y_N);
    //	auto d_load = std::chrono::steady_clock::now() - t_load;

    // auto t_decod = std::chrono::steady_clock::now(); //
    //--------------------------------------------------------
    // DECODE
    this->_decode(frame_id);
    // auto d_decod = std::chrono::steady_clock::now() - t_decod;
    // auto rvalue = std::chrono::duration_cast<std::chrono::milliseconds>(d_decod).count();
    // std::cout << "The time taken: " << rvalue << std::endl;

    //	auto t_store = std::chrono::steady_clock::now(); //
    //---------------------------------------------------------
    // STORE
    this->_store(V_K);
    //	auto d_store = std::chrono::steady_clock::now() - t_store;

    //	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,
    // d_load);
    // std::cout << "dec::tm:decode_siho::decode = " << (size_t)dec::tm::decode_siho::decode << std::endl;
    // (*this)[dec::tsk::decode_siho].update_timer((size_t)dec::tm::decode_siho::decode, d_decod);
    //	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,
    // d_store);
    //
    // for (int i = 0; i < this->K; i++)
    // {
    //     std::cout << V_K[i] << ",";
    // }
    // std::cout << std::endl;

    return 0;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
int
Decoder_polar_PAC_Fano_naive<B, R, F, G>::_decode_siho_cw(const R* Y_N, B* V_N, const size_t frame_id)
{
    //	auto t_load = std::chrono::steady_clock::now(); //
    //-----------------------------------------------------------
    // LOAD
    this->_load(Y_N);
    //	auto d_load = std::chrono::steady_clock::now() - t_load;

    //	auto t_decod = std::chrono::steady_clock::now(); //
    //--------------------------------------------------------
    // DECODE
    this->_decode(frame_id);
    //	auto d_decod = std::chrono::steady_clock::now() - t_decod;

    //	auto t_store = std::chrono::steady_clock::now(); //
    //---------------------------------------------------------
    // STORE
    this->_store(V_N, true);
    //	auto d_store = std::chrono::steady_clock::now() - t_store;

    //	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,
    // d_load);
    //	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode,
    // d_decod);
    //	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,
    // d_store);

    return 0;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::_store(B* V, bool coded) const
{
    auto* root = (tools::Binary_node<Contents_PAC_Fano<B, R>>*)this->polar_trees.get_root();
    if (!coded)
    {
        /*std::cout << "coded Inside the store\n";*/
        auto k = 0;
        this->recursive_store(root, V, k);
    }
    else
    {
        auto* contents_root = root->get_c();
        std::copy(contents_root->v.begin(), contents_root->v.begin() + this->N, V);
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::recursive_compute_llr(tools::Binary_node<Contents_PAC_Fano<B, R>>* node_cur,
                                                                int depth)
{
    if (depth != 0) recursive_compute_llr(node_cur->get_father(), --depth);

    if (node_cur->is_left()) this->apply_f((tools::Binary_node<Contents_PAC_Fano<B, R>>*)node_cur->get_father());

    if (node_cur->is_right()) this->apply_g((tools::Binary_node<Contents_PAC_Fano<B, R>>*)node_cur->get_father());
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::propagate_sums(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_cur)
{
    if (!node_cur->is_leaf()) this->compute_sums((tools::Binary_node<Contents_PAC_Fano<B, R>>*)node_cur);

    if (node_cur->is_right() && !node_cur->is_root()) this->propagate_sums(node_cur->get_father());
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::recursive_allocate_nodes_contents(
  tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr,
  const int vector_size)
{
    if (node_curr != nullptr)
    {
        node_curr->set_contents(new Contents_PAC_Fano<B, R>(vector_size));

        this->recursive_allocate_nodes_contents(node_curr->get_left(), vector_size / 2);
        this->recursive_allocate_nodes_contents(node_curr->get_right(), vector_size / 2);
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::recursive_initialize_frozen_bits(
  const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr,
  const std::vector<bool>& frozen_bits)
{
    auto* contents = node_curr->get_contents();

    if (!node_curr->is_leaf()) // stop condition
    {
        this->recursive_initialize_frozen_bits(node_curr->get_left(),
                                               frozen_bits); // recursive call
        this->recursive_initialize_frozen_bits(node_curr->get_right(),
                                               frozen_bits); // recursive call
    }
    else
        contents->is_frozen_bit = frozen_bits[node_curr->get_lane_id()];
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::recursive_store(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr,
                                                          B* V_K,
                                                          int& k) const
{
    /*std::cout << "Inside the: " << __func__ << std::endl;*/
    auto* contents = node_curr->get_contents();

    if (!node_curr->is_leaf()) // stop condition
    {
        this->recursive_store(node_curr->get_left(), V_K, k);  // recursive call
        this->recursive_store(node_curr->get_right(), V_K, k); // recursive call
    }
    else if (!frozen_bits[node_curr->get_lane_id()])
        V_K[k++] = contents->v[0] ? 1 : 0;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::recursive_deallocate_nodes_contents(
  tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr)
{
    if (node_curr != nullptr)
    {
        this->recursive_deallocate_nodes_contents(node_curr->get_left());  // recursive call
        this->recursive_deallocate_nodes_contents(node_curr->get_right()); // recursive call

        auto* contents = node_curr->get_contents();
        delete contents;
        node_curr->set_contents(nullptr);
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::apply_f(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr)
{
    const auto size = (int)node_curr->get_c()->lambda.size();
    const auto size_2 = size / 2;

    const auto* node_left = node_curr->get_left(); // get left node

    for (auto i = 0; i < size_2; i++)
        node_left->get_c()->lambda[i] = F(node_curr->get_c()->lambda[i], // apply f()
                                          node_curr->get_c()->lambda[size_2 + i]);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::apply_g(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr)
{
    const auto size = (int)node_curr->get_c()->lambda.size();
    const auto size_2 = size / 2;

    const auto* node_left = node_curr->get_left();   // get left node
    const auto* node_right = node_curr->get_right(); // get right node

    for (auto i = 0; i < size_2; i++)
        node_right->get_c()->lambda[i] = G(node_curr->get_c()->lambda[i], // apply g()
                                           node_curr->get_c()->lambda[size_2 + i],
                                           node_left->get_c()->s[i]);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::compute_sums(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr)
{
    const auto size = (int)node_curr->get_c()->lambda.size();
    const auto size_2 = size / 2;

    const auto* node_left = node_curr->get_left();   // get left node
    const auto* node_right = node_curr->get_right(); // get right node

    for (auto i = 0; i < size_2; i++)
        node_curr->get_c()->s[i] = node_left->get_c()->s[i] ^ node_right->get_c()->s[i]; // bit xor

    for (auto i = 0; i < size_2; i++)
        node_curr->get_c()->s[size_2 + i] = node_right->get_c()->s[i]; // bit eq
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::BACK(std::vector<double>& r_values,
                                               std::vector<double>& betas,
                                               int j,
                                               double T,
                                               std::vector<uint8_t>& kappa)
{
    double mu;
    while (true)
    {
        if (j == -1)
        {
            mu = -std::numeric_limits<double>::max();
        }
        if (j == 0)
        {
            mu = 0;
        }
        else if (j >= 1)
        {
            mu = betas[j - 1];
        }
        if (mu >= T)
        {
            j = j - 1;
            if (kappa[j + 1] == 0)
            {
                int B_ = 1;
                r_values[0] = T;
                r_values[1] = j;
                r_values[2] = B_;
                return;
                // needs to return T,j,B here
            }
        }
        else if (mu < T)
        {
            T = T - DELTA;
            int B_ = -1;
            r_values[0] = T;
            r_values[1] = j;
            r_values[2] = B_;
            return;
            // needs to return T,j,B here also...
        }
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::update_threshold(double& threshold, double tau)
{
    while (threshold + DELTA <= tau)
    {
        threshold += DELTA;
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_Fano_naive<B, R, F, G>::calc_Perr(double& ac, int i)
{
    double ac_ = 0;
    for (int j = 0; j < i; ++j)
    {
        ac_ += std::log(1 - this->P_err[j]);
    }
    ac = ac_;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
std::pair<B, std::vector<B>>
Decoder_polar_PAC_Fano_naive<B, R, F, G>::conv1bitEnc(B cbit, std::vector<B>& state)
{

    B u = (B)cbit && (B)conv_reg[0];
    for (size_t i = 1; i < conv_reg.size(); i++)
    {
        if (conv_reg[i] == (B)1)
        {
            u = u ^ state[i - 1];
        }
    }

    std::vector<B> nextState(conv_reg.size() - 1, 0);
    nextState[0] = cbit;
    int n_i = 1;
    for (size_t i = 0; i < state.size() - 1; i++)
    {
        nextState[n_i] = state[i];
        n_i++;
    }

    return std::make_pair(u, nextState);
}

}
}
