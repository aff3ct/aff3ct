//find ./src/ -type f -follow -print | grep "[.]h$"
#include <Tools/general_utils.h>
#include <Tools/types.h>
#include <Tools/Math/matrix.h>
#include <Tools/Math/utils.h>
#include <Tools/Math/max.h>
#include <Tools/Code/Polar/nodes_parser.h>
#include <Tools/Code/Polar/API/functions_polar_inter_intra.h>
#include <Tools/Code/Polar/API/functions_polar_intra.h>
#include <Tools/Code/Polar/API/functions_polar_intra_8bit.h>
#include <Tools/Code/Polar/API/functions_polar_seq.h>
#include <Tools/Code/Polar/API/functions_polar_inter_8bit_bitpacking.h>
#include <Tools/Code/Polar/API/functions_polar_intra_16bit.h>
#include <Tools/Code/Polar/API/functions_polar_inter.h>
#include <Tools/Code/Polar/API/functions_polar_intra_32bit.h>
#include <Tools/Code/Polar/decoder_polar_functions.h>
#include <Tools/Code/Polar/fb_extract.h>
#include <Tools/Perf/MIPP/math/sse_mathfun.h>
#include <Tools/Perf/MIPP/math/avx_mathfun.h>
#include <Tools/Perf/MIPP/math/neon_mathfun.h>
#include <Tools/Perf/MIPP/mipp_scalar_op.h>
#include <Tools/Perf/MIPP/mipp.h>
#include <Tools/Perf/Transpose/transpose_AVX.h>
#include <Tools/Perf/Transpose/transpose_SSE.h>
#include <Tools/Perf/Transpose/transpose_selector.h>
#include <Tools/Perf/Transpose/transpose_NEON.h>
#include <Tools/Display/bash_tools.h>
// #include <Tools/MSVC/dirent.h>
#include <Tools/params.h>


//find ./src/ -type f -follow -print | grep "[.]hpp$"
#include <Tools/Threads/Barrier.hpp>
#include <Tools/Math/Galois.hpp>
#include <Tools/Factory/Factory_monitor.hpp>
#include <Tools/Factory/Polar/Factory_decoder_polar_gen.hpp>
#include <Tools/Factory/Polar/Factory_decoder_polar.hpp>
#include <Tools/Factory/Polar/Factory_encoder_polar.hpp>
#include <Tools/Factory/Polar/Factory_frozenbits_generator.hpp>
#include <Tools/Factory/Polar/Factory_puncturer_polar.hpp>
#include <Tools/Factory/Factory_quantizer.hpp>
#include <Tools/Factory/Factory_channel.hpp>
#include <Tools/Factory/Factory.hpp>
#include <Tools/Factory/RSC/Factory_decoder_RSC.hpp>
#include <Tools/Factory/RSC/Factory_encoder_RSC.hpp>
#include <Tools/Factory/Factory_interleaver.hpp>
#include <Tools/Factory/Factory_encoder_common.hpp>
#include <Tools/Factory/Repetition/Factory_decoder_repetition.hpp>
#include <Tools/Factory/Repetition/Factory_encoder_repetition.hpp>
#include <Tools/Factory/LDPC/Factory_encoder_LDPC.hpp>
#include <Tools/Factory/LDPC/Factory_decoder_LDPC.hpp>
#include <Tools/Factory/Coset/Factory_coset_real.hpp>
#include <Tools/Factory/Coset/Factory_coset_bit.hpp>
#include <Tools/Factory/BCH/Factory_encoder_BCH.hpp>
#include <Tools/Factory/BCH/Factory_decoder_BCH.hpp>
#include <Tools/Factory/Factory_CRC.hpp>
#include <Tools/Factory/Factory_source.hpp>
#include <Tools/Factory/Factory_modem.hpp>
#include <Tools/Factory/RA/Factory_encoder_RA.hpp>
#include <Tools/Factory/RA/Factory_decoder_RA.hpp>
#include <Tools/Factory/Turbo/Factory_encoder_turbo.hpp>
#include <Tools/Factory/Turbo/Factory_scaling_factor.hpp>
#include <Tools/Factory/Turbo/Factory_puncturer_turbo.hpp>
#include <Tools/Factory/Turbo/Factory_decoder_turbo.hpp>
#include <Tools/Algo/Sort/LC_sorter.hpp>
#include <Tools/Algo/Sort/LC_sorter_simd.hpp>
#include <Tools/Algo/PRNG/PRNG_MT19937_simd.hpp>
#include <Tools/Algo/PRNG/PRNG_MT19937.hpp>
#include <Tools/Algo/Predicate.hpp>
#include <Tools/Algo/Tree/Binary_node.hpp>
#include <Tools/Algo/Tree/Binary_tree.hpp>
#include <Tools/Algo/Tree/Binary_tree_metric.hpp>
#include <Tools/Algo/Noise/Fast/Noise_fast.hpp>
#include <Tools/Algo/Noise/Standard/Noise_std.hpp>
#include <Tools/Algo/Noise/MKL/Noise_MKL.hpp>
#include <Tools/Algo/Noise/Noise.hpp>
#include <Tools/Algo/Noise/GSL/Noise_GSL.hpp>
#include <Tools/Algo/Predicate_ite.hpp>
#include <Tools/Algo/Bit_packer.hpp>
#include <Tools/SystemC/SC_Router.hpp>
#include <Tools/SystemC/SC_Dummy.hpp>
#include <Tools/SystemC/SC_Predicate.hpp>
#include <Tools/SystemC/SC_Debug.hpp>
#include <Tools/SystemC/SC_Duplicator.hpp>
#include <Tools/Code/Polar/API/API_polar_static_intra_32bit.hpp>
#include <Tools/Code/Polar/API/API_polar_dynamic_seq.hpp>
#include <Tools/Code/Polar/API/API_polar.hpp>
#include <Tools/Code/Polar/API/API_polar_static_inter.hpp>
#include <Tools/Code/Polar/API/API_polar_static_inter_8bit_bitpacking.hpp>
#include <Tools/Code/Polar/API/API_polar_static_seq.hpp>
#include <Tools/Code/Polar/API/API_polar_dynamic_inter.hpp>
#include <Tools/Code/Polar/API/API_polar_static_intra_8bit.hpp>
#include <Tools/Code/Polar/API/API_polar_static_intra_16bit.hpp>
#include <Tools/Code/Polar/API/API_polar_dynamic_inter_8bit_bitpacking.hpp>
#include <Tools/Code/Polar/API/API_polar_dynamic_intra.hpp>
#include <Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp>
#include <Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp>
#include <Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp>
#include <Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp>
#include <Tools/Code/Polar/Patterns/Pattern_polar_r0_left.hpp>
#include <Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp>
#include <Tools/Code/Polar/Patterns/Pattern_polar_std.hpp>
#include <Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp>
#include <Tools/Code/Polar/Patterns/Pattern_polar_i.hpp>
#include <Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp>
#include <Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_r0_left.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_r1.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_rep_left.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_spc.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_rep.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_r0.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_std.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_spc.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_r1.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_rep.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_r0.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_rep_left.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_r0_left.hpp>
#include <Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_std.hpp>
#include <Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp>
#include <Tools/Code/Polar/Pattern_polar_parser.hpp>
#include <Tools/Code/LDPC/G/LDPC_G.hpp>
#include <Tools/Code/LDPC/AList_reader/AList_reader.hpp>
#include <Tools/Code/SCMA/modem_SCMA_functions.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/Self_corrected/Self_corrected.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp>
#include <Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker.hpp>
#include <Tools/Arguments_reader.hpp>
#include <Tools/Perf/Reorderer/Reorderer.hpp>
#include <Tools/Display/Frame_trace/Frame_trace.hpp>
#include <Tools/Display/Dumper/Dumper.hpp>
#include <Tools/Display/Dumper/Dumper_reduction.hpp>
#include <Tools/Display/Terminal/Terminal.hpp>
#include <Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp>
#include <Tools/Display/Terminal/BFER/Terminal_BFER.hpp>
#include <Tools/Codec/Polar/Codec_polar.hpp>
#include <Tools/Codec/Codec_SISO.hpp>
#include <Tools/Codec/RSC/Codec_RSC.hpp>
#include <Tools/Codec/Codec.hpp>
#include <Tools/Codec/Repetition/Codec_repetition.hpp>
#include <Tools/Codec/LDPC/Codec_LDPC.hpp>
#include <Tools/Codec/Uncoded/Codec_uncoded.hpp>
#include <Tools/Codec/BCH/Codec_BCH.hpp>
#include <Tools/Codec/RA/Codec_RA.hpp>
#include <Tools/Codec/Turbo/Codec_turbo.hpp>
#include <Module/Interleaver/Random/Interleaver_random.hpp>
#include <Module/Interleaver/Golden/Interleaver_golden.hpp>
// #include <Module/Interleaver/SPU_Interleaver.hpp>
#include <Module/Interleaver/Row_column/Interleaver_row_column.hpp>
// #include <Module/Interleaver/SC_Interleaver.hpp>
#include <Module/Interleaver/User/Interleaver_user.hpp>
#include <Module/Interleaver/Random_column/Interleaver_random_column.hpp>
#include <Module/Interleaver/Interleaver.hpp>
#include <Module/Interleaver/NO/Interleaver_NO.hpp>
#include <Module/Interleaver/CCSDS/Interleaver_CCSDS.hpp>
#include <Module/Interleaver/LTE/Interleaver_LTE.hpp>
// #include <Module/Modem/SC_Modem.hpp>
#include <Module/Modem/BPSK/Modem_BPSK_fast.hpp>
#include <Module/Modem/BPSK/Modem_BPSK.hpp>
#include <Module/Modem/PSK/Modem_PSK.hpp>
#include <Module/Modem/CPM/CPM_parameters.hpp>
#include <Module/Modem/CPM/CPE/Encoder_CPE_Rimoldi.hpp>
#include <Module/Modem/CPM/CPE/Encoder_CPE.hpp>
#include <Module/Modem/CPM/BCJR/CPM_BCJR.hpp>
#include <Module/Modem/CPM/Modem_CPM.hpp>
#include <Module/Modem/User/Modem_user.hpp>
#include <Module/Modem/QAM/Modem_QAM.hpp>
#include <Module/Modem/PAM/Modem_PAM.hpp>
// #include <Module/Modem/SPU_Modem.hpp>
#include <Module/Modem/SCMA/Modem_SCMA.hpp>
#include <Module/Modem/Modem.hpp>
#include <Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp>
// #include <Module/Puncturer/SPU_Puncturer.hpp>
#include <Module/Puncturer/Puncturer.hpp>
#include <Module/Puncturer/NO/Puncturer_NO.hpp>
// #include <Module/Puncturer/SC_Puncturer.hpp>
#include <Module/Puncturer/Turbo/Puncturer_turbo.hpp>
#include <Module/Encoder/Encoder_sys.hpp>
#include <Module/Encoder/Polar/Encoder_polar.hpp>
#include <Module/Encoder/Polar/Encoder_polar_sys.hpp>
#include <Module/Encoder/AZCW/Encoder_AZCW.hpp>
#include <Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp>
#include <Module/Encoder/RSC/Encoder_RSC_sys.hpp>
#include <Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp>
#include <Module/Encoder/RSC/Encoder_RSC3_CPE_sys.hpp>
// #include <Module/Encoder/SC_Encoder.hpp>
// #include <Module/Encoder/SPU_Encoder.hpp>
#include <Module/Encoder/Repetition/Encoder_repetition_sys.hpp>
#include <Module/Encoder/LDPC/Encoder_LDPC.hpp>
#include <Module/Encoder/LDPC/From_H/Encoder_LDPC_from_H.hpp>
#include <Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2.hpp>
#include <Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2_constants.hpp>
#include <Module/Encoder/Coset/Encoder_coset.hpp>
#include <Module/Encoder/User/Encoder_user.hpp>
#include <Module/Encoder/BCH/Encoder_BCH.hpp>
#include <Module/Encoder/Encoder.hpp>
#include <Module/Encoder/NO/Encoder_NO.hpp>
#include <Module/Encoder/RA/Encoder_RA.hpp>
#include <Module/Encoder/Turbo/Encoder_turbo.hpp>
#include <Module/Encoder/Turbo/Encoder_turbo_legacy.hpp>
#include <Module/Channel/Channel.hpp>
#include <Module/Channel/AWGN/Channel_AWGN_LLR.hpp>
// #include <Module/Channel/SPU_Channel.hpp>
#include <Module/Channel/User/Channel_user.hpp>
#include <Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp>
// #include <Module/Channel/SC_Channel.hpp>
#include <Module/Channel/NO/Channel_NO.hpp>
#include <Module/Decoder/Decoder.hpp>
#include <Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive.hpp>
#include <Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive_sys.hpp>
#include <Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hpp>
#include <Module/Decoder/Polar/SC/Decoder_polar_SC_naive_sys.hpp>
#include <Module/Decoder/Polar/SC/Decoder_polar_SC_fast_sys.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR15.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K873813_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K54613_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K524288_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K9830_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29491_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K19661_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K64_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K32_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K27_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K4_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29492_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K614_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K3413_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K13107_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K19661_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K7_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K109227_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1229_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K614_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K107_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K4096_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1638_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K3413_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR18.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K512_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29491_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K13_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K218453_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K13_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K53_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K436907_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K109227_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K873813_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K427_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K4_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K13653_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K14746_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1638_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K3_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K3277_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K3_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K14746_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K427_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K32_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K6827_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K13107_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K27_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K16_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K205_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K853_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K524288_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K819_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K131072_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K256_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K22938_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27307_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K22938_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K65536_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27568_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K128_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1434_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K6554_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27568_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K512_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K262144_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K26214_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K54613_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR33.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K6827_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K8192_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K65536_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K26214_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K8_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K128_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K213_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K436907_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR15.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K213_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K262144_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K13653_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K7_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K6554_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K205_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K53_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K218453_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR18.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K256_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K64_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1707_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K4096_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29492_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1229_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K16_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27307_SNR40.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K410_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1434_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1707_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K819_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K410_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K107_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K3277_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K8_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR33.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K9830_SNR25.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K853_SNR40.short.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K131072_SNR25.hpp>
// #include <Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K8192_SNR25.short.hpp>
#include <Module/Decoder/Polar/ASCL/Decoder_polar_ASCL_MEM_fast_CA_sys.hpp>
#include <Module/Decoder/Polar/ASCL/Decoder_polar_ASCL_fast_CA_sys.hpp>
#include <Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive_sys.hpp>
#include <Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive.hpp>
#include <Module/Decoder/Polar/SCL/Decoder_polar_SCL_fast_sys.hpp>
#include <Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA_sys.hpp>
#include <Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_MEM_fast_CA_sys.hpp>
#include <Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_fast_CA_sys.hpp>
#include <Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA.hpp>
// #include <Module/Decoder/Polar/SCL/CRC/Generated/Decoder_polar_SCL_fast_CA_sys_N4_K2_SNR25.hpp>
// #include <Module/Decoder/Polar/SCL/CRC/Generated/Decoder_polar_SCL_fast_CA_sys_N256_K64_SNR30.hpp>
// #include <Module/Decoder/Polar/SCL/CRC/Generated/Decoder_polar_SCL_fast_CA_sys_N2048_K1755_SNR35.hpp>
#include <Module/Decoder/Polar/SCL/Decoder_polar_SCL_MEM_fast_sys.hpp>
// #include <Module/Decoder/SPU_Decoder.hpp>
#include <Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std_json.hpp>
#include <Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std.hpp>
#include <Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic.hpp>
#include <Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra.hpp>
#include <Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_std.hpp>
#include <Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_fast.hpp>
#include <Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_very_fast.hpp>
#include <Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_std.hpp>
#include <Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter.hpp>
#include <Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_fast.hpp>
#include <Module/Decoder/RSC/BCJR/Decoder_RSC_BCJR.hpp>
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_very_fast.hpp>
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hpp>
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_scan.hpp>
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq.hpp>
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_std.hpp>
#include <Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x4_AVX.hpp>
#include <Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_AVX.hpp>
#include <Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_SSE.hpp>
#include <Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra.hpp>
// #include <Module/Decoder/SPU_SISO.hpp>
#include <Module/Decoder/SISO.hpp>
#include <Module/Decoder/Repetition/Decoder_repetition_fast.hpp>
#include <Module/Decoder/Repetition/Decoder_repetition_std.hpp>
#include <Module/Decoder/Repetition/Decoder_repetition.hpp>
// #include <Module/Decoder/SC_SISO.hpp>
#include <Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_offset_normalize_min_sum.hpp>
#include <Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_ONMS_inter.hpp>
#include <Module/Decoder/LDPC/BP/Layered/LSPA/Decoder_LDPC_BP_layered_log_sum_product.hpp>
#include <Module/Decoder/LDPC/BP/Layered/SPA/Decoder_LDPC_BP_layered_sum_product.hpp>
#include <Module/Decoder/LDPC/BP/Layered/Decoder_LDPC_BP_layered.hpp>
#include <Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp>
#include <Module/Decoder/LDPC/BP/Flooding/ONMS/Decoder_LDPC_BP_flooding_offset_normalize_min_sum.hpp>
#include <Module/Decoder/LDPC/BP/Flooding/LSPA/Decoder_LDPC_BP_flooding_log_sum_product.hpp>
#include <Module/Decoder/LDPC/BP/Flooding/SPA/Decoder_LDPC_BP_flooding_sum_product.hpp>
#include <Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding.hpp>
#include <Module/Decoder/BCH/Decoder_BCH.hpp>
#include <Module/Decoder/Decoder_SISO.hpp>
#include <Module/Decoder/NO/Decoder_NO.hpp>
#include <Module/Decoder/RA/Decoder_RA.hpp>
// #include <Module/Decoder/SC_Decoder.hpp>
#include <Module/Decoder/Turbo/Decoder_turbo_naive.hpp>
#include <Module/Decoder/Turbo/Decoder_turbo_fast.hpp>
#include <Module/Decoder/Turbo/Decoder_turbo.hpp>
// #include <Module/Coset/SPU_Coset.hpp>
#include <Module/Coset/Real/Coset_real.hpp>
#include <Module/Coset/Coset.hpp>
// #include <Module/Coset/SC_Coset.hpp>
#include <Module/Coset/Bit/Coset_bit.hpp>
// #include <Module/Source/SC_Source.hpp>
#include <Module/Source/Random/Source_random_fast.hpp>
#include <Module/Source/Random/Source_random.hpp>
#include <Module/Source/AZCW/Source_AZCW.hpp>
// #include <Module/Source/SPU_Source.hpp>
#include <Module/Source/Source.hpp>
#include <Module/Source/User/Source_user.hpp>
#include <Module/Monitor/Standard/Monitor_reduction_mpi.hpp>
#include <Module/Monitor/Standard/Monitor_std.hpp>
#include <Module/Monitor/Standard/Monitor_reduction.hpp>
#include <Module/Monitor/Monitor.hpp>
// #include <Module/Monitor/SC_Monitor.hpp>
// #include <Module/Monitor/SPU_Monitor.hpp>
#include <Module/Quantizer/Fast/Quantizer_fast.hpp>
#include <Module/Quantizer/Standard/Quantizer_standard.hpp>
#include <Module/Quantizer/Quantizer.hpp>
// #include <Module/Quantizer/SC_Quantizer.hpp>
#include <Module/Quantizer/Tricky/Quantizer_tricky.hpp>
// #include <Module/Quantizer/SPU_Quantizer.hpp>
#include <Module/Quantizer/NO/Quantizer_NO.hpp>
// #include <Module/CRC/SPU_CRC.hpp>
// #include <Module/CRC/SC_CRC.hpp>
#include <Module/CRC/Polynomial/CRC_polynomial.hpp>
#include <Module/CRC/Polynomial/CRC_polynomial_fast.hpp>
#include <Module/CRC/Polynomial/CRC_polynomial_inter.hpp>
#include <Module/CRC/NO/CRC_NO.hpp>
#include <Module/CRC/CRC.hpp>
#include <Module/Module.hpp>
#include <Launcher/EXIT/Polar/Launcher_EXIT_polar.hpp>
#include <Launcher/EXIT/RSC/Launcher_EXIT_RSC.hpp>
#include <Launcher/EXIT/Launcher_EXIT.hpp>
#include <Launcher/GEN/Polar/Launcher_GEN_polar.hpp>
#include <Launcher/GEN/Launcher_GEN.hpp>
#include <Launcher/BFER/Launcher_BFER.hpp>
#include <Launcher/BFER/Polar/Launcher_BFER_polar.hpp>
#include <Launcher/BFER/RSC/Launcher_BFER_RSC.hpp>
#include <Launcher/BFER/Repetition/Launcher_BFER_repetition.hpp>
#include <Launcher/BFER/LDPC/Launcher_BFER_LDPC.hpp>
#include <Launcher/BFER/Uncoded/Launcher_BFER_uncoded.hpp>
#include <Launcher/BFER/BCH/Launcher_BFER_BCH.hpp>
#include <Launcher/BFER/RA/Launcher_BFER_RA.hpp>
#include <Launcher/BFER/Turbo/Launcher_BFER_turbo.hpp>
#include <Launcher/BFERI/Launcher_BFERI.hpp>
#include <Launcher/BFERI/Polar/Launcher_BFERI_polar.hpp>
#include <Launcher/BFERI/RSC/Launcher_BFERI_RSC.hpp>
#include <Launcher/BFERI/LDPC/Launcher_BFERI_LDPC.hpp>
#include <Launcher/BFERI/Uncoded/Launcher_BFERI_uncoded.hpp>
#include <Launcher/Launcher.hpp>
#include <Simulation/Simulation.hpp>
#include <Simulation/EXIT/Simulation_EXIT.hpp>
#include <Simulation/GEN/Code/Polar/Generation_polar.hpp>
#include <Simulation/BFER/Standard/Threads/Simulation_BFER_std_threads.hpp>
#include <Simulation/BFER/Standard/StarPU/SPU_Simulation_BFER_std.hpp>
#include <Simulation/BFER/Standard/SystemC/SC_Simulation_BFER_std.hpp>
#include <Simulation/BFER/Standard/Simulation_BFER_std.hpp>
#include <Simulation/BFER/Iterative/Simulation_BFER_ite.hpp>
#include <Simulation/BFER/Iterative/Threads/Simulation_BFER_ite_threads.hpp>
#include <Simulation/BFER/Iterative/SystemC/SC_Simulation_BFER_ite.hpp>
#include <Simulation/BFER/Simulation_BFER.hpp>
// #include <aff3ct.hpp>
#include <Generator/Polar/SC/Generator_polar_SC_sys.hpp>
#include <Generator/Polar/Generator_polar.hpp>
#include <Generator/Polar/SCL/Generator_polar_SCL_sys.hpp>
#include <Generator/Generator.hpp>
