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

#include "Module/Decoder/Polar_PAC/SCL/Decoder_polar_PAC_SCL_naive.hpp"
#include "Tools/Code/Polar/fb_assert.h"

namespace aff3ct
{
namespace module
{
template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
Decoder_polar_PAC_SCL_naive<B, R, F, G>::Decoder_polar_PAC_SCL_naive(const int& K,
                                                                     const int& N,
                                                                     const int& L,
                                                                     const std::vector<bool>& frozen_bits,
                                                                     const std::string& conv)
  : Decoder_SIHO<B, R>(K, N)
  , m((int)std::log2(N))
  , metric_init(std::numeric_limits<R>::min())
  , frozen_bits(frozen_bits)
  , L(L)
  , polar_trees(L, tools::Binary_tree_metric<Contents_PAC_SCL<B, R>, R>(this->m + 1, metric_init))
{
    /*std::cout << "Inside the SCL naive decoder: " << __FILE__ << std::endl;*/
    const std::string name = "Decoder_polar_PAC_SCL_naive";
    this->set_name(name);

    /*std::cout << "Inside the SCL naive decoder: " << __LINE__ << std::endl;*/
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

    if (this->L <= 0 || !spu::tools::is_power_of_2(this->L))
    {
        std::stringstream message;
        message << "'L' has to be a positive power of 2 ('L' = " << L << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
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

    this->active_paths.insert(0);
    for (auto i = 0; i < L; i++)
    {
        this->recursive_allocate_nodes_contents(this->polar_trees[i].get_root(), this->N);
        this->recursive_initialize_frozen_bits(this->polar_trees[i].get_root(), frozen_bits);
    }

    for (auto i = 0; i < L; i++)
        leaves_array.push_back(this->polar_trees[i].get_leaves());

    for (auto& t : this->tasks)
        t->set_replicability(true);

    /*conv_reg = { spu::tools::bit_init<B>(), (B)0, spu::tools::bit_init<B>(),*/
    /*             spu::tools::bit_init<B>(), (B)0, spu::tools::bit_init<B>(),*/
    /*             spu::tools::bit_init<B>() };*/
    /**/
    // conv_reg = { (B)1, (B)0, (B)1, (B)1, (B)0, (B)1, (B)1 };
    // conv_reg = { (B)1, (B)0, (B)0, (B)0, (B)0, (B)0, (B)0 };
    /*std::cout << "conv_reg[0]: " << conv_reg[0] << ","*/
    /*          << spu::tools::bit_init<B>() << std::endl;*/

    // std::cout << "Conv string: " << conv << std::endl;
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

    // std::cout << "Inside the decoder conv_reg: " << conv_reg.size() << " ";
    // for (int i = 0; i < conv_reg.size(); i++)
    // {
    //     std::cout << (int)conv_reg[i] << ",";
    // }
    // std::cout << std::endl;

    for (auto i = 0; i < L; i++)
        curStates.push_back(std::vector<B>(conv_reg.size() - 1, (B)0));
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
Decoder_polar_PAC_SCL_naive<B, R, F, G>::~Decoder_polar_PAC_SCL_naive()
{
    for (auto i = 0; i < L; i++)
        this->recursive_deallocate_nodes_contents(this->polar_trees[i].get_root());
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
Decoder_polar_PAC_SCL_naive<B, R, F, G>*
Decoder_polar_PAC_SCL_naive<B, R, F, G>::clone() const
{
    auto m = new Decoder_polar_PAC_SCL_naive(*this);
    m->deep_copy(*this);
    return m;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::deep_copy(const Decoder_polar_PAC_SCL_naive<B, R, F, G>& m)
{
    spu::module::Stateful::deep_copy(m);
    this->leaves_array.clear();
    for (auto i = 0; i < L; i++)
    {
        this->recursive_deep_copy(m.polar_trees[i].get_root(), this->polar_trees[i].get_root());
        this->leaves_array.push_back(this->polar_trees[i].get_leaves());
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::recursive_deep_copy(const tools::Binary_node<Contents_PAC_SCL<B, R>>* nref,
                                                             tools::Binary_node<Contents_PAC_SCL<B, R>>* nclone)
{
    auto cref = nref->get_contents();
    auto cclone = new Contents_PAC_SCL<B, R>(*cref);
    nclone->set_contents(cclone);

    if (!nref->is_leaf() && !nclone->is_leaf())
    {
        if (nref->get_left() != nullptr) this->recursive_deep_copy(nref->get_left(), nclone->get_left());

        if (nref->get_right() != nullptr) this->recursive_deep_copy(nref->get_right(), nclone->get_right());
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::set_frozen_bits(const std::vector<bool>& fb)
{
    aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
    std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());

    /*std::cout << "info bits: ";*/
    /*for (int i = 0; i < this->N; i++)*/
    /*{*/
    /*    if (!fb[i])*/
    /*    {*/
    /*        std::cout << i << ",";*/
    /*    }*/
    /*}*/
    /*std::cout << std::endl;*/

    for (auto i = 0; i < L; i++)
        this->recursive_initialize_frozen_bits(this->polar_trees[i].get_root(), frozen_bits);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
const std::vector<bool>&
Decoder_polar_PAC_SCL_naive<B, R, F, G>::get_frozen_bits() const
{
    return this->frozen_bits;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::_load(const R* Y_N)
{

    for (auto path = 0; path < this->L; path++)
    {
        auto* contents = this->polar_trees[path].get_root()->get_contents();
        for (auto i = 0; i < this->N; i++)
            contents->lambda[i] = Y_N[i];
        polar_trees[path].set_path_metric(metric_init);
    }

    // initialization
    active_paths.clear();
    active_paths.insert(0);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::_decode(const size_t frame_id)
{
    std::set<int> last_active_paths;
    int cur_path;

    using ConvType = std::tuple<std::pair<B, std::vector<B>>, std::pair<B, std::vector<B>>>;

    // tuples to be sorted. <Path,estimated bit,metric>
    // tuples to be sorted. <Path, deconv bit, metric, estimate bit, index, state>
    std::vector<std::tuple<int, B, R, B, int, ConvType>> metrics_vec;

    std::vector<std::pair<B, std::vector<B>>> mko;
    std::vector<std::pair<B, std::vector<B>>> mkz;
    // std::vector<int16_t> path_list_index(this->L, 0);
    // std::cout << "The value of m: " << this->m << std::endl;

    // run through each leaf
    for (auto leaf_index = 0; leaf_index < this->N; leaf_index++)
    {
        // std::cout << "The compute depth: " << tools::compute_depth(leaf_index, this->m) << "," << leaf_index << ","
        //           << this->m << std::endl;
        // compute LLR for current leaf
        for (auto path : active_paths)
            this->recursive_compute_llr(leaves_array[path][leaf_index], tools::compute_depth(leaf_index, this->m));

        // for (auto aupp : curStates[0])
        //     std::cout << aupp << ",";
        // std::cout << std::endl;
        //
        // if current leaf is a frozen bit
        if (leaves_array[0][leaf_index]->get_c()->is_frozen_bit)
        {
            auto min_phi = std::numeric_limits<R>::max();
            for (auto path : active_paths)
            {
                auto cur_leaf = leaves_array[path][leaf_index];
                cur_leaf->get_c()->v[0] = 0;
                /*std::cout << cur_leaf->get_c()->v.size() << ", " <<
                 * cur_leaf->get_c()->s.size() << std::endl;*/
                std::pair<B, std::vector<B>> res = conv1bitEnc((B)0, curStates[path]);

                cur_leaf->get_c()->s[0] = res.first ? spu::tools::bit_init<B>() : 0;
                curStates[path] = res.second;

                auto phi_cur = tools::phi<R>(
                  polar_trees[path].get_path_metric(), cur_leaf->get_c()->lambda[0], cur_leaf->get_c()->s[0]);
                this->polar_trees[path].set_path_metric(phi_cur);

                min_phi = std::min<R>(min_phi, phi_cur);
            }

            // normalization
            for (auto path : active_paths)
                this->polar_trees[path].set_path_metric(this->polar_trees[path].get_path_metric() - min_phi);
        }
        else
        {
            // metrics vec used to store values of hypothetic path metrics
            metrics_vec.clear();
            // mkz.clear();
            // mko.clear();
            // std::vector<int16_t> path_list_index(this->L, 0);
            auto min_phi = std::numeric_limits<R>::max();
            int16_t ci = 0;
            for (auto path : active_paths)
            {
                auto cur_leaf = leaves_array[path][leaf_index];
                std::pair<B, std::vector<B>> u0Pair = conv1bitEnc((B)0, curStates[path]);
                std::pair<B, std::vector<B>> u1Pair = conv1bitEnc((B)1, curStates[path]);
                // mkz.push_back(conv1bitEnc((B)0, curStates[path]));
                // mko.push_back(conv1bitEnc((B)1, curStates[path]));

                // mkz.push_back(u0Pair);
                // mko.push_back(u1Pair);
                // B u0 = mkz[ci].first;
                // B u1 = mko[ci].first;
                B u0 = u0Pair.first ? spu::tools::bit_init<B>() : 0;
                B u1 = u1Pair.first ? spu::tools::bit_init<B>() : 0;
                ConvType curConv = std::make_tuple(u0Pair, u1Pair);

                R phi0 = tools::phi<B, R>(polar_trees[path].get_path_metric(), cur_leaf->get_c()->lambda[0], u0);
                R phi1 = tools::phi<B, R>(polar_trees[path].get_path_metric(), cur_leaf->get_c()->lambda[0], u1);
                metrics_vec.push_back(std::make_tuple(path, u0, phi0, (B)0, ci, curConv));
                metrics_vec.push_back(std::make_tuple(path, u1, phi1, (B)1, ci, curConv));
                /*metrics_vec.push_back(std::make_tuple(path, u0, phi0));*/
                /*metrics_vec.push_back(std::make_tuple(path, u1, phi1));*/
                // path_list_index[path] = ci;

                min_phi = std::min<R>(min_phi, phi0);
                min_phi = std::min<R>(min_phi, phi1);

                ci++;
            }

            // normalization
            for (auto vec : metrics_vec)
                std::get<2>(vec) -= min_phi;

            if (active_paths.size() <= (unsigned)(L / 2))
            {
                last_active_paths = active_paths;
                for (auto path : last_active_paths)
                {
                    auto cur_it = std::find_if(metrics_vec.begin(),
                                               metrics_vec.end(),
                                               [path](std::tuple<int, B, R, B, int, ConvType> x)
                                               { return std::get<0>(x) == path; });
                    if (cur_it != metrics_vec.end())
                        this->duplicate_path(
                          path, leaf_index, std::get<0>(std::get<5>(*cur_it)), std::get<1>(std::get<5>(*cur_it)));
                    // this->duplicate_path(path, leaf_index, mkz[path_list_index[path]], mko[path_list_index[path]]);
                }
            }
            else
            {
                // sort hypothetic metrics
                std::sort(metrics_vec.begin(),
                          metrics_vec.end(),
                          [](std::tuple<int, B, R, B, int, ConvType> x, std::tuple<int, B, R, B, int, ConvType> y)
                          { return std::get<2>(x) < std::get<2>(y); });

                // search in worst metrics. If a path is found twice, erase it
                for (auto it = metrics_vec.begin() + metrics_vec.size() / 2; it != metrics_vec.end(); ++it)
                {
                    cur_path = std::get<0>(*it);

                    auto it_double = std::find_if(it + 1,
                                                  metrics_vec.end(),
                                                  [cur_path](std::tuple<int, B, R, B, int, ConvType> x)
                                                  { return std::get<0>(x) == cur_path; });

                    if (it_double != metrics_vec.end()) active_paths.erase(std::get<0>(*it));
                }

                // remove worst metrics from list
                metrics_vec.resize(metrics_vec.size() / 2);

                for (auto it = metrics_vec.begin(); it != metrics_vec.end(); ++it)
                {
                    cur_path = std::get<0>(*it);

                    auto it_double = std::find_if(it + 1,
                                                  metrics_vec.end(),
                                                  [cur_path](std::tuple<int, B, R, B, int, ConvType> x)
                                                  { return std::get<0>(x) == cur_path; });

                    if (it_double != metrics_vec.end())
                    {
                        // duplicate
                        metrics_vec.erase(it_double);
                        // duplicate_path(std::get<0>(*it), leaf_index, mkz[std::get<0>(*it)], mko[std::get<0>(*it)]);
                        duplicate_path(
                          std::get<0>(*it), leaf_index, std::get<0>(std::get<5>(*it)), std::get<1>(std::get<5>(*it)));
                    }
                    else
                    {
                        if (std::get<3>(*it) == (B)0)
                            curStates[std::get<0>(*it)] = std::get<0>(std::get<5>(*it)).second;

                        if (std::get<3>(*it) == (B)1)
                            curStates[std::get<0>(*it)] = std::get<1>(std::get<5>(*it)).second;
                        // if (std::get<3>(*it) == (B)0) curStates[std::get<0>(*it)] = mkz[std::get<0>(*it)].second;
                        // if (std::get<3>(*it) == (B)1) curStates[std::get<0>(*it)] = mko[std::get<0>(*it)].second;

                        // choose
                        leaves_array[std::get<0>(*it)][leaf_index]->get_c()->s[0] = std::get<1>(*it);
                        leaves_array[std::get<0>(*it)][leaf_index]->get_c()->v[0] = std::get<3>(*it);
                        polar_trees[std::get<0>(*it)].set_path_metric(std::get<2>(*it));
                    }
                }
            }
        }

        // propagate sums
        for (auto path : active_paths)
            this->propagate_sums(leaves_array[path][leaf_index]);
    }

    this->select_best_path(frame_id);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
int
Decoder_polar_PAC_SCL_naive<B, R, F, G>::_decode_siho(const R* Y_N, B* V_K, const size_t frame_id)
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
Decoder_polar_PAC_SCL_naive<B, R, F, G>::_decode_siho_cw(const R* Y_N, B* V_N, const size_t frame_id)
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
Decoder_polar_PAC_SCL_naive<B, R, F, G>::_store(B* V, bool coded) const
{
    auto* root = (tools::Binary_node<Contents_PAC_SCL<B, R>>*)this->polar_trees[*active_paths.begin()].get_root();
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
        /*std::cout << "States: " << std::endl;*/
        /*for (int i = 0; i < this->conv_reg.size() - 1; i++)*/
        /*{*/
        /*    std::cout << this->curStates[0][i] << " , ";*/
        /*}*/
        /*std::cout << std::endl;*/
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::recursive_compute_llr(tools::Binary_node<Contents_PAC_SCL<B, R>>* node_cur,
                                                               int depth)
{
    if (depth != 0) recursive_compute_llr(node_cur->get_father(), --depth);

    if (node_cur->is_left()) this->apply_f((tools::Binary_node<Contents_PAC_SCL<B, R>>*)node_cur->get_father());

    if (node_cur->is_right()) this->apply_g((tools::Binary_node<Contents_PAC_SCL<B, R>>*)node_cur->get_father());

    // auto lambdas = node_cur->get_c()->lambda;
    // std::cout << "LLRs at depth: " << depth << std::endl;
    // for (int i = 0; i < lambdas.size(); i++)
    // {
    //     std::cout << lambdas[i] << ",";
    // }
    // std::cout << std::endl;
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::propagate_sums(const tools::Binary_node<Contents_PAC_SCL<B, R>>* node_cur)
{
    if (!node_cur->is_leaf()) this->compute_sums((tools::Binary_node<Contents_PAC_SCL<B, R>>*)node_cur);

    if (node_cur->is_right() && !node_cur->is_root()) this->propagate_sums(node_cur->get_father());
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::recursive_duplicate_tree_llr(
  tools::Binary_node<Contents_PAC_SCL<B, R>>* node_a,
  tools::Binary_node<Contents_PAC_SCL<B, R>>* node_b)
{
    node_b->get_c()->lambda = node_a->get_c()->lambda;

    if (!node_a->get_father()->is_root())
        this->recursive_duplicate_tree_llr(node_a->get_father(), node_b->get_father());
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::recursive_duplicate_tree_sums(
  tools::Binary_node<Contents_PAC_SCL<B, R>>* node_a,
  tools::Binary_node<Contents_PAC_SCL<B, R>>* node_b,
  tools::Binary_node<Contents_PAC_SCL<B, R>>* node_caller)
{
    if (node_a->get_left() != node_caller && !node_a->is_leaf())
    {
        node_b->get_left()->get_c()->s = node_a->get_left()->get_c()->s;
        // node_b->get_left()->get_c()->v = node_a->get_left()->get_c()->v;
    }

    if (!node_a->is_root()) this->recursive_duplicate_tree_sums(node_a->get_father(), node_b->get_father(), node_a);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::duplicate_path(int path,
                                                        int leaf_index,
                                                        std::pair<B, std::vector<B>> mkz,
                                                        std::pair<B, std::vector<B>> mko)
{
    std::vector<tools::Binary_node<Contents_PAC_SCL<B, R>>*> path_leaves, newpath_leaves;
    int newpath = 0;
    while (active_paths.find(newpath++) != active_paths.end())
    {
    };
    newpath--;

    active_paths.insert(newpath);

    path_leaves = leaves_array[path];

    newpath_leaves = leaves_array[newpath];
    // for (int i = 0; i < conv_reg.size() - 1; i++)
    //     curStates[newpath][i] = curStates[path][i];

    for (auto i = 0; i < leaf_index; i++)
    {
        newpath_leaves[i]->get_c()->s = path_leaves[i]->get_c()->s;
        newpath_leaves[i]->get_c()->v = path_leaves[i]->get_c()->v;
    }

    recursive_duplicate_tree_sums(leaves_array[path][leaf_index], leaves_array[newpath][leaf_index], nullptr);

    if (leaf_index < this->N - 1)
        recursive_duplicate_tree_llr(leaves_array[path][leaf_index + 1], leaves_array[newpath][leaf_index + 1]);

    curStates[newpath] = mko.second;
    leaves_array[newpath][leaf_index]->get_c()->s[0] = mko.first ? spu::tools::bit_init<B>() : 0;
    leaves_array[newpath][leaf_index]->get_c()->v[0] = 1;
    polar_trees[newpath].set_path_metric(tools::phi<B, R>(polar_trees[path].get_path_metric(),
                                                          leaves_array[path][leaf_index]->get_c()->lambda[0],
                                                          leaves_array[newpath][leaf_index]->get_c()->s[0]));

    curStates[path] = mkz.second;
    leaves_array[path][leaf_index]->get_c()->s[0] = mkz.first ? spu::tools::bit_init<B>() : 0;
    leaves_array[path][leaf_index]->get_c()->v[0] = 0;
    polar_trees[path].set_path_metric(tools::phi<B, R>(polar_trees[path].get_path_metric(),
                                                       leaves_array[path][leaf_index]->get_c()->lambda[0],
                                                       leaves_array[path][leaf_index]->get_c()->s[0]));
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::select_best_path(const size_t /*frame*/)
{
    int best_path = 0;
    if (active_paths.size() >= 1) best_path = *active_paths.begin();

    for (int path : active_paths)
        if (polar_trees[path].get_path_metric() < polar_trees[best_path].get_path_metric()) best_path = path;

    active_paths.clear();
    active_paths.insert(best_path);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::recursive_allocate_nodes_contents(
  tools::Binary_node<Contents_PAC_SCL<B, R>>* node_curr,
  const int vector_size)
{
    if (node_curr != nullptr)
    {
        node_curr->set_contents(new Contents_PAC_SCL<B, R>(vector_size));
        /*std::cout << "Inside the recursive_allocate_nodes_contents function: "
         * << node_curr->get_c()->v.size() <<
         * ","*/
        /*          << node_curr->get_c()->s.size() << std::endl;*/

        this->recursive_allocate_nodes_contents(node_curr->get_left(), vector_size / 2);
        this->recursive_allocate_nodes_contents(node_curr->get_right(), vector_size / 2);
    }
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::recursive_initialize_frozen_bits(
  const tools::Binary_node<Contents_PAC_SCL<B, R>>* node_curr,
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
Decoder_polar_PAC_SCL_naive<B, R, F, G>::recursive_store(const tools::Binary_node<Contents_PAC_SCL<B, R>>* node_curr,
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
Decoder_polar_PAC_SCL_naive<B, R, F, G>::recursive_deallocate_nodes_contents(
  tools::Binary_node<Contents_PAC_SCL<B, R>>* node_curr)
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
Decoder_polar_PAC_SCL_naive<B, R, F, G>::apply_f(const tools::Binary_node<Contents_PAC_SCL<B, R>>* node_curr)
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
Decoder_polar_PAC_SCL_naive<B, R, F, G>::apply_g(const tools::Binary_node<Contents_PAC_SCL<B, R>>* node_curr)
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
Decoder_polar_PAC_SCL_naive<B, R, F, G>::compute_sums(const tools::Binary_node<Contents_PAC_SCL<B, R>>* node_curr)
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
std::pair<B, std::vector<B>>
Decoder_polar_PAC_SCL_naive<B, R, F, G>::conv1bitEnc(B cbit, std::vector<B>& state)
{

    B u = (B)cbit && (B)conv_reg[0];
    /*B u = spu::tools::bop_and((B)conv_reg[0], (B)cbit);*/
    /*std::cout << "Bit u: " << u << ", conv_reg[0]: " << conv_reg[0] << ",
     * cbit: " << (B)cbit << std::endl;*/
    for (int i = 1; i < conv_reg.size(); i++)
    {
        if (conv_reg[i] == (B)1)
        {
            u = u ^ state[i - 1];
            /*u = spu::tools::bop_xor(u, curStates[l][i - 1]);*/
        }
        /*std::cout << "i: " << i << ", conv_reg[i]: " << conv_reg[i] << ",
         * curStates[l][i-1]: " << curStates[l][i - 1]*/
        /*          << std::endl;*/
    }

    std::vector<B> nextState(conv_reg.size() - 1, 0);
    nextState[0] = cbit;
    int n_i = 1;
    for (int i = 0; i < state.size() - 1; i++)
        nextState[n_i++] = state[i];

    /*for (int i = curStates[l].size() - 1; i >= 1; i--)*/
    /*    curStates[l][i] = (B)curStates[l][i - 1];*/
    /*curStates[l][0] = (B)cbit;*/
    /**/
    /*std::cout << "The current state: \n";*/
    /*for (int i = 0; i < curStates[l].size(); i++)*/
    /*{*/
    /*    std::cout << curStates[l][i] << " ";*/
    /*}*/
    /*std::cout << std::endl;*/

    return std::make_pair(u, nextState);
}

template<typename B, typename R, tools::proto_f<R> F, tools::proto_g<B, R> G>
void
Decoder_polar_PAC_SCL_naive<B, R, F, G>::convEnc(B* X_N)
{
    /*std::cout << "Inside the conv encoder function \n";*/
    /*std::vector<uint8_t> cState(conv_reg.size() - 1, 0);*/

    std::vector<uint8_t> u(this->N, 0);

    /*for (int i = 0; i < this->N; ++i)*/
    /*{*/
    /*    X_N[i] = conv1bitEnc(X_N[i]);*/
    /*}*/
}

}
}
