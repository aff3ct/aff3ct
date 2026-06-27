/*!
 * \file
 * \brief Class module::Decoder_polar_PAC_Fano_naive.
 */
#ifndef DECODER_POLAR_PAC_FANO_NAIVE
#define DECODER_POLAR_PAC_FANO_NAIVE

#include <set>
#include <string>
#include <vector>

#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hpp"
#include "Tools/Algo/Tree/Binary/Binary_node.hpp"
#include "Tools/Algo/Tree/Binary/Binary_tree_metric.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"

#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template<typename B = int32_t, typename R = float>
class Contents_PAC_Fano : public Contents_SC<B, R>
{
  public:
    std::vector<B> v;

    explicit Contents_PAC_Fano(int size)
      : Contents_SC<B, R>(size)
      , v(size)
    {
        /*std::cout << "Inside the Contents_PAC_Fano constructor function: " << s.size() << ", " << u.size() <<
         * std::endl;*/
    }
    virtual ~Contents_PAC_Fano() {}
};

template<typename B = int,
         typename R = float,
         tools::proto_f<R> F = tools::f_LLR,
         tools::proto_g<B, R> G = tools::g_LLR>
class Decoder_polar_PAC_Fano_naive
  : public Decoder_SIHO<B, R>
  , public tools::Interface_get_set_frozen_bits
{
  protected:
    const int m;         // graph depth
    const R metric_init; // init value of the metrics in the trees

    // Fano decoder parameters
    std::vector<double> P_err;
    std::vector<double> P_err_cum;
    std::vector<double> betas;
    std::vector<double> mus;
    std::vector<uint8_t> kappa;

    float DELTA;

    std::vector<bool> frozen_bits;

    std::vector<uint8_t> conv_reg;

    std::vector<std::vector<B>> currState;

    std::vector<B> cState;

    std::vector<int> Bvec;

    tools::Binary_tree_metric<Contents_PAC_Fano<B, R>, R> polar_trees;
    std::vector<tools::Binary_node<Contents_PAC_Fano<B, R>>*> leaves_array;

  public:
    Decoder_polar_PAC_Fano_naive(const int& K,
                                 const int& N,
                                 const std::vector<bool>& frozen_bits,
                                 const std::string& conv,
                                 const float DELTA,
                                 const std::vector<double>& Perr_);
    virtual ~Decoder_polar_PAC_Fano_naive();

    virtual Decoder_polar_PAC_Fano_naive<B, R, F, G>* clone() const;

    virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
    virtual const std::vector<bool>& get_frozen_bits() const;

  protected:
    void deep_copy(const Decoder_polar_PAC_Fano_naive<B, R, F, G>& m);
    void recursive_deep_copy(const tools::Binary_node<Contents_PAC_Fano<B, R>>* nref,
                             tools::Binary_node<Contents_PAC_Fano<B, R>>* nclone);

    void _load(const R* Y_N);
    void _decode(const size_t frame_id);
    int _decode_siho(const R* Y_N, B* V_K, const size_t frame_id);
    int _decode_siho_cw(const R* Y_N, B* V_N, const size_t frame_id);
    virtual void _store(B* V, bool coded = false) const;

  private:
    void recursive_compute_llr(tools::Binary_node<Contents_PAC_Fano<B, R>>* node_cur, int depth);
    void propagate_sums(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_cur);

    void BACK(std::vector<double>& r_values, std::vector<double>& betas, int j, double T, std::vector<uint8_t>& kappa);

    void update_threshold(double& threshold, double tau);

    void calc_Perr(double& ac, int i);

    double cal_PM(int i);

    double actual_metric_cal(double lru, uint8_t ucap);

    std::pair<B, std::vector<B>> conv1bitEnc(B cbit, std::vector<B>& state);

  protected:
    void recursive_allocate_nodes_contents(tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr,
                                           const int vector_size);
    void recursive_initialize_frozen_bits(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr,
                                          const std::vector<bool>& frozen_bits);
    void recursive_store(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr, B* V_K, int& k) const;
    void recursive_deallocate_nodes_contents(tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr);

    void apply_f(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr);
    void apply_g(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr);
    void compute_sums(const tools::Binary_node<Contents_PAC_Fano<B, R>>* node_curr);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar_PAC/Fano/Decoder_polar_PAC_Fano_naive.hxx"
#endif

#endif /* DECODER_POLAR_PAC_FANO_NAIVE_ */
