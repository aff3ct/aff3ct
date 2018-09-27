#ifndef AFF3CT_HPP
#define AFF3CT_HPP

#ifndef MATRIX_UTILS_H__
#include <Tools/Algo/Matrix/matrix_utils.h>
#endif
#ifndef FUNCTIONS_POLAR_INTER_H_
#include <Tools/Code/Polar/API/functions_polar_inter.h>
#endif
#ifndef FUNCTIONS_POLAR_INTER_8BIT_BITPACKING_H_
#include <Tools/Code/Polar/API/functions_polar_inter_8bit_bitpacking.h>
#endif
#ifndef FUNCTIONS_POLAR_INTER_INTRA_H_
#include <Tools/Code/Polar/API/functions_polar_inter_intra.h>
#endif
#ifndef FUNCTIONS_POLAR_INTRA_H_
#include <Tools/Code/Polar/API/functions_polar_intra.h>
#endif
#ifndef FUNCTIONS_POLAR_INTRA_16BIT_H_
#include <Tools/Code/Polar/API/functions_polar_intra_16bit.h>
#endif
#ifndef FUNCTIONS_POLAR_INTRA_32BIT_H_
#include <Tools/Code/Polar/API/functions_polar_intra_32bit.h>
#endif
#ifndef FUNCTIONS_POLAR_INTRA_8BIT_H_
#include <Tools/Code/Polar/API/functions_polar_intra_8bit.h>
#endif
#ifndef FUNCTIONS_POLAR_SEQ_H_
#include <Tools/Code/Polar/API/functions_polar_seq.h>
#endif
#ifndef DECODER_POLAR_FUNCTIONS_H
#include <Tools/Code/Polar/decoder_polar_functions.h>
#endif
#ifndef FB_EXTRACT_HPP
#include <Tools/Code/Polar/fb_extract.h>
#endif
#ifndef NODES_PARSER_HPP
#include <Tools/Code/Polar/nodes_parser.h>
#endif
#ifndef RANG_FORMAT_H_
#include <Tools/Display/rang_format/rang_format.h>
#endif
#ifndef GENERAL_UTILS_H_
#include <Tools/general_utils.h>
#endif
#ifndef INTERPOLATION_H_
#include <Tools/Math/interpolation.h>
#endif
#ifndef MATRIX_H
#include <Tools/Math/matrix.h>
#endif
#ifndef MAX_H
#include <Tools/Math/max.h>
#endif
#ifndef NUMERICAL_INTEGRATION_H_
#include <Tools/Math/numerical_integration.h>
#endif
#ifndef MATH_UTILS_H
#include <Tools/Math/utils.h>
#endif
#ifndef NOISE_UTILS_HPP__
#include <Tools/Noise/noise_utils.h>
#endif
#ifndef HARD_DECIDE_H_
#include <Tools/Perf/common/hard_decide.h>
#endif
#ifndef MUTUAL_INFO_H__
#include <Tools/Perf/common/mutual_info.h>
#endif
#ifndef COMPUTE_PARITY_H_
#include <Tools/Perf/compute_parity.h>
#endif
#ifndef BITWISE_DIFF_H__
#include <Tools/Perf/distance/Bitwise_diff.h>
#endif
#ifndef BOOLEAN_DIFF_H__
#include <Tools/Perf/distance/Boolean_diff.h>
#endif
#ifndef DISTANCE_H__
#include <Tools/Perf/distance/distance.h>
#endif
#ifndef HAMMING_DISTANCE_H_
#include <Tools/Perf/distance/hamming_distance.h>
#endif
#ifndef TRANSPOSE_AVX_H
#include <Tools/Perf/Transpose/transpose_AVX.h>
#endif
#ifndef TRANSPOSE_NEON_H
#include <Tools/Perf/Transpose/transpose_NEON.h>
#endif
#ifndef TRANSPOSE_SELECTOR_H
#include <Tools/Perf/Transpose/transpose_selector.h>
#endif
#ifndef TRANSPOSE_SSE_H
#include <Tools/Perf/Transpose/transpose_SSE.h>
#endif
#ifndef SYSTEM_FUNCTIONS_H_
#include <Tools/system_functions.h>
#endif
#ifndef TYPES_H_
#include <Tools/types.h>
#endif
#ifndef VERSION_H_
#include <Tools/version.h>
#endif
#ifndef COMMAND_PARSER_HPP
#include <Factory/Command_parser.hpp>
#endif
#ifndef FACTORY_HPP
#include <Factory/Factory.hpp>
#endif
#ifndef FACTORY_SIMULATION_MAIN_H_
#include <Factory/Launcher/Launcher.hpp>
#endif
#ifndef FACTORY_CHANNEL_HPP
#include <Factory/Module/Channel/Channel.hpp>
#endif
#ifndef FACTORY_CODEC_BCH_HPP
#include <Factory/Module/Codec/BCH/Codec_BCH.hpp>
#endif
#ifndef FACTORY_CODEC_HPP_
#include <Factory/Module/Codec/Codec.hpp>
#endif
#ifndef FACTORY_CODEC_HIHO_HPP_
#include <Factory/Module/Codec/Codec_HIHO.hpp>
#endif
#ifndef FACTORY_CODEC_SIHO_HPP_
#include <Factory/Module/Codec/Codec_SIHO.hpp>
#endif
#ifndef FACTORY_CODEC_SIHO_HIHO_HPP_
#include <Factory/Module/Codec/Codec_SIHO_HIHO.hpp>
#endif
#ifndef FACTORY_CODEC_SISO_HPP_
#include <Factory/Module/Codec/Codec_SISO.hpp>
#endif
#ifndef FACTORY_CODEC_SISO_SIHO_HPP_
#include <Factory/Module/Codec/Codec_SISO_SIHO.hpp>
#endif
#ifndef FACTORY_CODEC_LDPC_HPP
#include <Factory/Module/Codec/LDPC/Codec_LDPC.hpp>
#endif
#ifndef FACTORY_CODEC_POLAR_HPP
#include <Factory/Module/Codec/Polar/Codec_polar.hpp>
#endif
#ifndef FACTORY_CODEC_RA_HPP
#include <Factory/Module/Codec/RA/Codec_RA.hpp>
#endif
#ifndef FACTORY_CODEC_REPETITION_HPP
#include <Factory/Module/Codec/Repetition/Codec_repetition.hpp>
#endif
#ifndef FACTORY_CODEC_RS_HPP
#include <Factory/Module/Codec/RS/Codec_RS.hpp>
#endif
#ifndef FACTORY_CODEC_RSC_HPP
#include <Factory/Module/Codec/RSC/Codec_RSC.hpp>
#endif
#ifndef FACTORY_CODEC_RSC_DB_HPP
#include <Factory/Module/Codec/RSC_DB/Codec_RSC_DB.hpp>
#endif
#ifndef FACTORY_CODEC_TURBO_HPP
#include <Factory/Module/Codec/Turbo/Codec_turbo.hpp>
#endif
#ifndef FACTORY_CODEC_TURBO_DB_HPP
#include <Factory/Module/Codec/Turbo_DB/Codec_turbo_DB.hpp>
#endif
#ifndef FACTORY_CODEC_TURBO_PRODUCT_HPP
#include <Factory/Module/Codec/Turbo_product/Codec_turbo_product.hpp>
#endif
#ifndef FACTORY_CODEC_UNCODED_HPP
#include <Factory/Module/Codec/Uncoded/Codec_uncoded.hpp>
#endif
#ifndef FACTORY_COSET_HPP
#include <Factory/Module/Coset/Coset.hpp>
#endif
#ifndef FACTORY_CRC_HPP
#include <Factory/Module/CRC/CRC.hpp>
#endif
#ifndef FACTORY_DECODER_BCH_HPP
#include <Factory/Module/Decoder/BCH/Decoder_BCH.hpp>
#endif
#ifndef FACTORY_DECODER_HPP_
#include <Factory/Module/Decoder/Decoder.hpp>
#endif
#ifndef FACTORY_DECODER_LDPC_HPP
#include <Factory/Module/Decoder/LDPC/Decoder_LDPC.hpp>
#endif
#ifndef FACTORY_DECODER_NO_HPP
#include <Factory/Module/Decoder/NO/Decoder_NO.hpp>
#endif
#ifndef FACTORY_DECODER_POLAR_HPP
#include <Factory/Module/Decoder/Polar/Decoder_polar.hpp>
#endif
#ifndef FACTORY_DECODER_RA_HPP
#include <Factory/Module/Decoder/RA/Decoder_RA.hpp>
#endif
#ifndef FACTORY_DECODER_REPETITION_HPP
#include <Factory/Module/Decoder/Repetition/Decoder_repetition.hpp>
#endif
#ifndef FACTORY_DECODER_RS_HPP
#include <Factory/Module/Decoder/RS/Decoder_RS.hpp>
#endif
#ifndef FACTORY_DECODER_RSC_HPP
#include <Factory/Module/Decoder/RSC/Decoder_RSC.hpp>
#endif
#ifndef FACTORY_DECODER_RSC_DB_HPP
#include <Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp>
#endif
#ifndef FACTORY_DECODER_TURBO_HPP
#include <Factory/Module/Decoder/Turbo/Decoder_turbo.hpp>
#endif
#ifndef FACTORY_DECODER_TURBO_DB_HPP
#include <Factory/Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp>
#endif
#ifndef FACTORY_DECODER_TURBO_PRODUCT_HPP
#include <Factory/Module/Decoder/Turbo_product/Decoder_turbo_product.hpp>
#endif
#ifndef FACTORY_ENCODER_BCH_HPP
#include <Factory/Module/Encoder/BCH/Encoder_BCH.hpp>
#endif
#ifndef FACTORY_ENCODER_HPP
#include <Factory/Module/Encoder/Encoder.hpp>
#endif
#ifndef FACTORY_ENCODER_LDPC_HPP
#include <Factory/Module/Encoder/LDPC/Encoder_LDPC.hpp>
#endif
#ifndef FACTORY_ENCODER_POLAR_HPP
#include <Factory/Module/Encoder/Polar/Encoder_polar.hpp>
#endif
#ifndef FACTORY_ENCODER_RA_HPP
#include <Factory/Module/Encoder/RA/Encoder_RA.hpp>
#endif
#ifndef FACTORY_ENCODER_REPETITION_HPP
#include <Factory/Module/Encoder/Repetition/Encoder_repetition.hpp>
#endif
#ifndef FACTORY_ENCODER_RS_HPP
#include <Factory/Module/Encoder/RS/Encoder_RS.hpp>
#endif
#ifndef FACTORY_ENCODER_RSC_HPP
#include <Factory/Module/Encoder/RSC/Encoder_RSC.hpp>
#endif
#ifndef FACTORY_ENCODER_RSC_DB_HPP
#include <Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp>
#endif
#ifndef FACTORY_ENCODER_TURBO_HPP
#include <Factory/Module/Encoder/Turbo/Encoder_turbo.hpp>
#endif
#ifndef FACTORY_ENCODER_TURBO_DB_HPP
#include <Factory/Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp>
#endif
#ifndef FACTORY_ENCODER_TURBO_PRODUCT_HPP
#include <Factory/Module/Encoder/Turbo_product/Encoder_turbo_product.hpp>
#endif
#ifndef FACTORY_INTERLEAVER_HPP
#include <Factory/Module/Interleaver/Interleaver.hpp>
#endif
#ifndef FACTORY_MODEM_HPP
#include <Factory/Module/Modem/Modem.hpp>
#endif
#ifndef FACTORY_MONITOR_BFER_HPP
#include <Factory/Module/Monitor/BFER/Monitor_BFER.hpp>
#endif
#ifndef FACTORY_MONITOR_EXIT_HPP
#include <Factory/Module/Monitor/EXIT/Monitor_EXIT.hpp>
#endif
#ifndef FACTORY_MONITOR_MI_HPP
#include <Factory/Module/Monitor/MI/Monitor_MI.hpp>
#endif
#ifndef FACTORY_MONITOR_HPP
#include <Factory/Module/Monitor/Monitor.hpp>
#endif
#ifndef FACTORY_PUNCTURER_LDPC_HPP
#include <Factory/Module/Puncturer/LDPC/Puncturer_LDPC.hpp>
#endif
#ifndef FACTORY_PUNCTURER_POLAR_HPP
#include <Factory/Module/Puncturer/Polar/Puncturer_polar.hpp>
#endif
#ifndef FACTORY_PONCTURER_HPP_
#include <Factory/Module/Puncturer/Puncturer.hpp>
#endif
#ifndef FACTORY_PUNCTURER_TURBO_HPP
#include <Factory/Module/Puncturer/Turbo/Puncturer_turbo.hpp>
#endif
#ifndef FACTORY_PUNCTURER_TURBO_DB_HPP
#include <Factory/Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp>
#endif
#ifndef FACTORY_QUANTIZER_HPP
#include <Factory/Module/Quantizer/Quantizer.hpp>
#endif
#ifndef FACTORY_SOURCE_HPP
#include <Factory/Module/Source/Source.hpp>
#endif
#ifndef FACTORY_SIMULATION_BFER_HPP_
#include <Factory/Simulation/BFER/BFER.hpp>
#endif
#ifndef FACTORY_SIMULATION_BFER_ITE_HPP_
#include <Factory/Simulation/BFER/BFER_ite.hpp>
#endif
#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#include <Factory/Simulation/BFER/BFER_std.hpp>
#endif
#ifndef FACTORY_SIMULATION_EXIT_HPP_
#include <Factory/Simulation/EXIT/EXIT.hpp>
#endif
#ifndef FACTORY_SIMULATION_HPP
#include <Factory/Simulation/Simulation.hpp>
#endif
#ifndef FACTORY_FROZENBITS_GENERATOR_HPP
#include <Factory/Tools/Code/Polar/Frozenbits_generator.hpp>
#endif
#ifndef FACTORY_FLIP_AND_CHECK_HPP
#include <Factory/Tools/Code/Turbo/Flip_and_check.hpp>
#endif
#ifndef FACTORY_FLIP_AND_CHECK_DB_HPP
#include <Factory/Tools/Code/Turbo/Flip_and_check_DB.hpp>
#endif
#ifndef FACTORY_SCALING_FACTOR_HPP
#include <Factory/Tools/Code/Turbo/Scaling_factor.hpp>
#endif
#ifndef FACTORY_TERMINAL_HPP_
#include <Factory/Tools/Display/Terminal/Terminal.hpp>
#endif
#ifndef FACTORY_INTERLEAVER_CORE_HPP
#include <Factory/Tools/Interleaver/Interleaver_core.hpp>
#endif
#ifndef FACTORY_NOISE_HPP
#include <Factory/Tools/Noise/Noise.hpp>
#endif
#ifndef LAUNCHER_BCH_HPP_
#include <Launcher/Code/BCH/BCH.hpp>
#endif
#ifndef LAUNCHER_LDPC_HPP_
#include <Launcher/Code/LDPC/LDPC.hpp>
#endif
#ifndef LAUNCHER_POLAR_HPP_
#include <Launcher/Code/Polar/Polar.hpp>
#endif
#ifndef LAUNCHER_RA_HPP_
#include <Launcher/Code/RA/RA.hpp>
#endif
#ifndef LAUNCHER_REPETITION_HPP_
#include <Launcher/Code/Repetition/Repetition.hpp>
#endif
#ifndef LAUNCHER_RS_HPP_
#include <Launcher/Code/RS/RS.hpp>
#endif
#ifndef LAUNCHER_RSC_HPP_
#include <Launcher/Code/RSC/RSC.hpp>
#endif
#ifndef LAUNCHER_RSC_DB_HPP_
#include <Launcher/Code/RSC_DB/RSC_DB.hpp>
#endif
#ifndef LAUNCHER_TURBO_HPP_
#include <Launcher/Code/Turbo/Turbo.hpp>
#endif
#ifndef LAUNCHER_TURBO_DB_HPP_
#include <Launcher/Code/Turbo_DB/Turbo_DB.hpp>
#endif
#ifndef LAUNCHER_TURBO_PRODUCT_HPP_
#include <Launcher/Code/Turbo_product/Turbo_product.hpp>
#endif
#ifndef LAUNCHER_UNCODED_HPP_
#include <Launcher/Code/Uncoded/Uncoded.hpp>
#endif
#ifndef LAUNCHER_HPP_
#include <Launcher/Launcher.hpp>
#endif
#ifndef LAUNCHER_BFER_ITE_HPP_
#include <Launcher/Simulation/BFER_ite.hpp>
#endif
#ifndef LAUNCHER_BFER_STD_HPP_
#include <Launcher/Simulation/BFER_std.hpp>
#endif
#ifndef LAUNCHER_EXIT_HPP_
#include <Launcher/Simulation/EXIT.hpp>
#endif
#ifndef CHANNEL_AWGN_LLR_HPP_
#include <Module/Channel/AWGN/Channel_AWGN_LLR.hpp>
#endif
#ifndef CHANNEL_BEC_HPP_
#include <Module/Channel/Binary_erasure/Channel_binary_erasure.hpp>
#endif
#ifndef CHANNEL_BSC_HPP_
#include <Module/Channel/Binary_symmetric/Channel_binary_symmetric.hpp>
#endif
#ifndef CHANNEL_HPP_
#include <Module/Channel/Channel.hpp>
#endif
#ifndef CHANNELS_NO_HPP_
#include <Module/Channel/NO/Channel_NO.hpp>
#endif
#ifndef CHANNEL_OPTICAL_HPP_
#include <Module/Channel/Optical/Channel_optical.hpp>
#endif
#ifndef CHANNEL_RAYLEIGH_LLR_HPP_
#include <Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp>
#endif
#ifndef CHANNEL_RAYLEIGH_LLR_USER_HPP_
#include <Module/Channel/Rayleigh/Channel_Rayleigh_LLR_user.hpp>
#endif
#ifndef CHANNELS_USER_HPP_
#include <Module/Channel/User/Channel_user.hpp>
#endif
#ifndef CODEC_BCH_HPP_
#include <Module/Codec/BCH/Codec_BCH.hpp>
#endif
#ifndef CODEC_HPP_
#include <Module/Codec/Codec.hpp>
#endif
#ifndef CODEC_HIHO_HPP_
#include <Module/Codec/Codec_HIHO.hpp>
#endif
#ifndef CODEC_SIHO_HPP_
#include <Module/Codec/Codec_SIHO.hpp>
#endif
#ifndef CODEC_SIHO_HIHO_HPP_
#include <Module/Codec/Codec_SIHO_HIHO.hpp>
#endif
#ifndef CODEC_SISO_HPP_
#include <Module/Codec/Codec_SISO.hpp>
#endif
#ifndef CODEC_SISO_SIHO_HPP_
#include <Module/Codec/Codec_SISO_SIHO.hpp>
#endif
#ifndef CODEC_LDPC_HPP_
#include <Module/Codec/LDPC/Codec_LDPC.hpp>
#endif
#ifndef CODEC_POLAR_HPP_
#include <Module/Codec/Polar/Codec_polar.hpp>
#endif
#ifndef CODEC_RA_HPP_
#include <Module/Codec/RA/Codec_RA.hpp>
#endif
#ifndef CODEC_REPETITION_HPP_
#include <Module/Codec/Repetition/Codec_repetition.hpp>
#endif
#ifndef CODEC_RS_HPP_
#include <Module/Codec/RS/Codec_RS.hpp>
#endif
#ifndef CODEC_RSC_HPP_
#include <Module/Codec/RSC/Codec_RSC.hpp>
#endif
#ifndef CODEC_RSC_DB_HPP_
#include <Module/Codec/RSC_DB/Codec_RSC_DB.hpp>
#endif
#ifndef CODEC_TURBO_HPP_
#include <Module/Codec/Turbo/Codec_turbo.hpp>
#endif
#ifndef CODEC_TURBO_DB_HPP_
#include <Module/Codec/Turbo_DB/Codec_turbo_DB.hpp>
#endif
#ifndef CODEC_TURBO_PRODUCT_HPP_
#include <Module/Codec/Turbo_product/Codec_turbo_product.hpp>
#endif
#ifndef CODEC_UNCODED_HPP_
#include <Module/Codec/Uncoded/Codec_uncoded.hpp>
#endif
#ifndef COSET_BIT_HPP_
#include <Module/Coset/Bit/Coset_bit.hpp>
#endif
#ifndef COSET_HPP_
#include <Module/Coset/Coset.hpp>
#endif
#ifndef COSET_REAL_HPP_
#include <Module/Coset/Real/Coset_real.hpp>
#endif
#ifndef CRC_HPP_
#include <Module/CRC/CRC.hpp>
#endif
#ifndef CRC_NO_HPP_
#include <Module/CRC/NO/CRC_NO.hpp>
#endif
#ifndef CRC_POLYNOMIAL_HPP_
#include <Module/CRC/Polynomial/CRC_polynomial.hpp>
#endif
#ifndef CRC_POLYNOMIAL_FAST_HPP_
#include <Module/CRC/Polynomial/CRC_polynomial_fast.hpp>
#endif
#ifndef CRC_POLYNOMIAL_INTER_HPP_
#include <Module/CRC/Polynomial/CRC_polynomial_inter.hpp>
#endif
#ifndef DECODER_BCH
#include <Module/Decoder/BCH/Decoder_BCH.hpp>
#endif
#ifndef DECODER_BCH_FAST
#include <Module/Decoder/BCH/Fast/Decoder_BCH_fast.hpp>
#endif
#ifndef DECODER_BCH_GENIUS
#include <Module/Decoder/BCH/Genius/Decoder_BCH_genius.hpp>
#endif
#ifndef DECODER_BCH_STD
#include <Module/Decoder/BCH/Standard/Decoder_BCH_std.hpp>
#endif
#ifndef DECODER_HPP_
#include <Module/Decoder/Decoder.hpp>
#endif
#ifndef DECODER_HIHO_HPP_
#include <Module/Decoder/Decoder_HIHO.hpp>
#endif
#ifndef DECODER_SIHO_HPP_
#include <Module/Decoder/Decoder_SIHO.hpp>
#endif
#ifndef DECODER_SIHO_HIHO_HPP_
#include <Module/Decoder/Decoder_SIHO_HIHO.hpp>
#endif
#ifndef DECODER_SISO_HPP_
#include <Module/Decoder/Decoder_SISO.hpp>
#endif
#ifndef DECODER_SISO_SIHO_HPP_
#include <Module/Decoder/Decoder_SISO_SIHO.hpp>
#endif
#ifndef DECODER_CHASE_STD_HPP_
#include <Module/Decoder/Generic/Chase/Decoder_chase_std.hpp>
#endif
#ifndef DECODER_MAXIMUM_LIKELIHOO_HPP_
#include <Module/Decoder/Generic/ML/Decoder_maximum_likelihood.hpp>
#endif
#ifndef DECODER_MAXIMUM_LIKELIHOOD_NAIVE_HPP_
#include <Module/Decoder/Generic/ML/Decoder_maximum_likelihood_naive.hpp>
#endif
#ifndef DECODER_MAXIMUM_LIKELIHOOD_STD_HPP_
#include <Module/Decoder/Generic/ML/Decoder_maximum_likelihood_std.hpp>
#endif
#ifndef DECODER_LDPC_BIT_FLIPPING_HPP_
#include <Module/Decoder/LDPC/BF/Decoder_LDPC_bit_flipping.hpp>
#endif
#ifndef DECODER_LDPC_BIT_FLIPPING_OMWBF_HPP_
#include <Module/Decoder/LDPC/BF/OMWBF/Decoder_LDPC_bit_flipping_OMWBF.hpp>
#endif
#ifndef DECODER_LDPC_BP_HPP_
#include <Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp>
#endif
#ifndef DECODER_LDPC_BP_FLOODING_HPP_
#include <Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding.hpp>
#endif
#ifndef DECODER_LDPC_BP_FLOODING_INTER_HPP_
#include <Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding_inter.hpp>
#endif
#ifndef DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_
#include <Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp>
#endif
#ifndef DECODER_LDPC_BP_HORIZONTAL_LAYERED_HPP_
#include <Module/Decoder/LDPC/BP/Horizontal_layered/Decoder_LDPC_BP_horizontal_layered.hpp>
#endif
#ifndef DECODER_LDPC_BP_HORIZONTAL_LAYERED_INTER_HPP_
#include <Module/Decoder/LDPC/BP/Horizontal_layered/Decoder_LDPC_BP_horizontal_layered_inter.hpp>
#endif
#ifndef DECODER_LDPC_BP_HORIZONTAL_LAYERED_ONMS_INTER_HPP_
#include <Module/Decoder/LDPC/BP/Horizontal_layered/ONMS/Decoder_LDPC_BP_horizontal_layered_ONMS_inter.hpp>
#endif
#ifndef DECODER_LDPC_BP_PEELING_HPP
#include <Module/Decoder/LDPC/BP/Peeling/Decoder_LDPC_BP_peeling.hpp>
#endif
#ifndef DECODER_LDPC_BP_VERTICAL_LAYERED_HPP_
#include <Module/Decoder/LDPC/BP/Vertical_layered/Decoder_LDPC_BP_vertical_layered.hpp>
#endif
#ifndef DECODER_LDPC_BP_VERTICAL_LAYERED_INTER_HPP_
#include <Module/Decoder/LDPC/BP/Vertical_layered/Decoder_LDPC_BP_vertical_layered_inter.hpp>
#endif
#ifndef DECODER_NO_HPP_
#include <Module/Decoder/NO/Decoder_NO.hpp>
#endif
#ifndef DECODER_POLAR_ASCL_FAST_SYS_CA
#include <Module/Decoder/Polar/ASCL/Decoder_polar_ASCL_fast_CA_sys.hpp>
#endif
#ifndef DECODER_POLAR_ASCL_MEM_FAST_SYS_CA
#include <Module/Decoder/Polar/ASCL/Decoder_polar_ASCL_MEM_fast_CA_sys.hpp>
#endif
#ifndef DECODER_POLAR_SC_FAST_SYS_
#include <Module/Decoder/Polar/SC/Decoder_polar_SC_fast_sys.hpp>
#endif
#ifndef DECODER_POLAR_SC_NAIVE_
#include <Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hpp>
#endif
#ifndef DECODER_POLAR_SC_NAIVE_SYS_
#include <Module/Decoder/Polar/SC/Decoder_polar_SC_naive_sys.hpp>
#endif
#ifndef DECODER_POLAR_SCAN_NAIVE_H_
#include <Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive.hpp>
#endif
#ifndef DECODER_POLAR_SCAN_NAIVE_SYS_
#include <Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive_sys.hpp>
#endif
#ifndef DECODER_POLAR_SCL_FAST_SYS_CA
#include <Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_fast_CA_sys.hpp>
#endif
#ifndef DECODER_POLAR_SCL_MEM_FAST_SYS_CA
#include <Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_MEM_fast_CA_sys.hpp>
#endif
#ifndef DECODER_POLAR_SCL_NAIVE_CA_
#include <Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA.hpp>
#endif
#ifndef DECODER_POLAR_SCL_NAIVE_CA_SYS_
#include <Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA_sys.hpp>
#endif
#ifndef DECODER_POLAR_SCL_FAST_SYS
#include <Module/Decoder/Polar/SCL/Decoder_polar_SCL_fast_sys.hpp>
#endif
#ifndef DECODER_POLAR_SCL_MEM_FAST_SYS
#include <Module/Decoder/Polar/SCL/Decoder_polar_SCL_MEM_fast_sys.hpp>
#endif
#ifndef DECODER_POLAR_SCL_NAIVE
#include <Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive.hpp>
#endif
#ifndef DECODER_POLAR_SCL_NAIVE_SYS
#include <Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive_sys.hpp>
#endif
#ifndef DECODER_RA
#include <Module/Decoder/RA/Decoder_RA.hpp>
#endif
#ifndef DECODER_REPETITION
#include <Module/Decoder/Repetition/Decoder_repetition.hpp>
#endif
#ifndef DECODER_REPETITION_FAST
#include <Module/Decoder/Repetition/Decoder_repetition_fast.hpp>
#endif
#ifndef DECODER_REPETITION_STD
#include <Module/Decoder/Repetition/Decoder_repetition_std.hpp>
#endif
#ifndef DECODER_RS
#include <Module/Decoder/RS/Decoder_RS.hpp>
#endif
#ifndef DECODER_RS_GENIUS
#include <Module/Decoder/RS/Genius/Decoder_RS_genius.hpp>
#endif
#ifndef DECODER_RS_STD
#include <Module/Decoder/RS/Standard/Decoder_RS_std.hpp>
#endif
#ifndef DECODER_RSC_BCJR_HPP_
#include <Module/Decoder/RSC/BCJR/Decoder_RSC_BCJR.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTER_HPP_
#include <Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTER_FAST_HPP_
#include <Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_fast.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTER_STD_HPP_
#include <Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_std.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTER_VERY_FAST_HPP_
#include <Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_very_fast.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTER_INTRA_HPP_
#include <Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTER_INTRA_FAST_X2_AVX_HPP_
#include <Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_AVX.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTER_INTRA_FAST_X2_SSE_HPP_
#include <Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_SSE.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTER_INTRA_FAST_X4_AVX_HPP_
#include <Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x4_AVX.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTRA_HPP_
#include <Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTRA_FAST_HPP_
#include <Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_fast.hpp>
#endif
#ifndef DECODER_RSC_BCJR_INTRA_STD_HPP_
#include <Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_std.hpp>
#endif
#ifndef DECODER_RSC_BCJR_SEQ_HPP_
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq.hpp>
#endif
#ifndef DECODER_RSC_BCJR_SEQ_FAST_HPP_
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hpp>
#endif
#ifndef DECODER_RSC_BCJR_SEQ_SCAN_HPP_
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_scan.hpp>
#endif
#ifndef DECODER_RSC_BCJR_STD_SEQ_HPP_
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_std.hpp>
#endif
#ifndef DECODER_RSC_BCJR_SEQ_VERY_FAST_HPP_
#include <Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_very_fast.hpp>
#endif
#ifndef DECODER_RSC_BCJR_SEQ_GENERIC_HPP_
#include <Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic.hpp>
#endif
#ifndef DECODER_RSC_BCJR_STD_GENERIC_SEQ_HPP_
#include <Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std.hpp>
#endif
#ifndef DECODER_RSC_BCJR_STD_GENERIC_SEQ_JSON_HPP_
#include <Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std_json.hpp>
#endif
#ifndef DECODER_RSC_DB_BCJR_HPP_
#include <Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp>
#endif
#ifndef DECODER_RSC_DB_BCJR_DVB_RCS1_HPP_
#include <Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB_RCS1.hpp>
#endif
#ifndef DECODER_RSC_DB_BCJR_DVB_RCS2_HPP_
#include <Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB_RCS2.hpp>
#endif
#ifndef DECODER_RSC_DB_BCJR_GENERIC_HPP_
#include <Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_generic.hpp>
#endif
#ifndef DECODER_TURBO_HPP_
#include <Module/Decoder/Turbo/Decoder_turbo.hpp>
#endif
#ifndef DECODER_TURBO_FAST_SYS_
#include <Module/Decoder/Turbo/Decoder_turbo_fast.hpp>
#endif
#ifndef DECODER_TURBO_NAIVE_HPP
#include <Module/Decoder/Turbo/Decoder_turbo_std.hpp>
#endif
#ifndef DECODER_TURBO_DB_HPP
#include <Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp>
#endif
#ifndef DECODER_CHASE_PYNDIAH_HPP_
#include <Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp>
#endif
#ifndef Decoder_chase_pyndiah_fast_HPP_
#include <Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah_fast.hpp>
#endif
#ifndef DECODER_TURBO_PRODUCT_HPP_
#include <Module/Decoder/Turbo_product/Decoder_turbo_product.hpp>
#endif
#ifndef ENCODER_AZCW_HPP_
#include <Module/Encoder/AZCW/Encoder_AZCW.hpp>
#endif
#ifndef ENCODER_BCH_HPP_
#include <Module/Encoder/BCH/Encoder_BCH.hpp>
#endif
#ifndef ENCODER_COSET_HPP_
#include <Module/Encoder/Coset/Encoder_coset.hpp>
#endif
#ifndef ENCODER_HPP_
#include <Module/Encoder/Encoder.hpp>
#endif
#ifndef ENCODER_LDPC_DVBS2_HPP_
#include <Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2.hpp>
#endif
#ifndef ENCODER_LDPC_HPP_
#include <Module/Encoder/LDPC/Encoder_LDPC.hpp>
#endif
#ifndef ENCODER_LDPC_FROM_H_HPP_
#include <Module/Encoder/LDPC/From_H/Encoder_LDPC_from_H.hpp>
#endif
#ifndef ENCODER_LDPC_FROM_IRA_HPP_
#include <Module/Encoder/LDPC/From_IRA/Encoder_LDPC_from_IRA.hpp>
#endif
#ifndef ENCODER_LDPC_FROM_QC_HPP_
#include <Module/Encoder/LDPC/From_QC/Encoder_LDPC_from_QC.hpp>
#endif
#ifndef ENCODER_NO_HPP_
#include <Module/Encoder/NO/Encoder_NO.hpp>
#endif
#ifndef ENCODER_POLAR_HPP_
#include <Module/Encoder/Polar/Encoder_polar.hpp>
#endif
#ifndef ENCODER_POLAR_SYS_HPP_
#include <Module/Encoder/Polar/Encoder_polar_sys.hpp>
#endif
#ifndef ENCODER_RA_HPP_
#include <Module/Encoder/RA/Encoder_RA.hpp>
#endif
#ifndef ENCODER_REPETITION_SYS_HPP_
#include <Module/Encoder/Repetition/Encoder_repetition_sys.hpp>
#endif
#ifndef ENCODER_RS_HPP_
#include <Module/Encoder/RS/Encoder_RS.hpp>
#endif
#ifndef ENCODER_RSC3_CPE_SYS_HPP_
#include <Module/Encoder/RSC/Encoder_RSC3_CPE_sys.hpp>
#endif
#ifndef ENCODER_RSC_GENERIC_JSON_SYS_HPP_
#include <Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp>
#endif
#ifndef ENCODER_RSC_GENERIC_SYS_HPP_
#include <Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp>
#endif
#ifndef ENCODER_RSC_SYS_HPP_
#include <Module/Encoder/RSC/Encoder_RSC_sys.hpp>
#endif
#ifndef ENCODER_RSC_DB_HPP_
#include <Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp>
#endif
#ifndef ENCODER_TURBO_HPP_
#include <Module/Encoder/Turbo/Encoder_turbo.hpp>
#endif
#ifndef ENCODER_TURBO_LEGACY_HPP_
#include <Module/Encoder/Turbo/Encoder_turbo_legacy.hpp>
#endif
#ifndef ENCODER_TURBO_DB_HPP_
#include <Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp>
#endif
#ifndef ENCODER_TURBO_PRODUCT_HPP_
#include <Module/Encoder/Turbo_product/Encoder_turbo_product.hpp>
#endif
#ifndef ENCODER_USER_HPP_
#include <Module/Encoder/User/Encoder_user.hpp>
#endif
#ifndef INTERLEAVER_HPP_
#include <Module/Interleaver/Interleaver.hpp>
#endif
#ifndef MODEM_BPSK_HPP_
#include <Module/Modem/BPSK/Modem_BPSK.hpp>
#endif
#ifndef MODEM_BPSK_FAST_HPP_
#include <Module/Modem/BPSK/Modem_BPSK_fast.hpp>
#endif
#ifndef CPM_BCJR_HPP_
#include <Module/Modem/CPM/BCJR/CPM_BCJR.hpp>
#endif
#ifndef ENCODER_CPE_HPP_
#include <Module/Modem/CPM/CPE/Encoder_CPE.hpp>
#endif
#ifndef ENCODER_CPE_RIMOLDI_HPP_
#include <Module/Modem/CPM/CPE/Encoder_CPE_Rimoldi.hpp>
#endif
#ifndef CPM_PARAMETERS_HPP_
#include <Module/Modem/CPM/CPM_parameters.hpp>
#endif
#ifndef MODEM_CPM_HPP_
#include <Module/Modem/CPM/Modem_CPM.hpp>
#endif
#ifndef MODEM_HPP_
#include <Module/Modem/Modem.hpp>
#endif
#ifndef MODEM_OOK_HPP_
#include <Module/Modem/OOK/Modem_OOK.hpp>
#endif
#ifndef MODEM_OOK_AWGN_HPP_
#include <Module/Modem/OOK/Modem_OOK_AWGN.hpp>
#endif
#ifndef MODEM_OOK_BEC_HPP_
#include <Module/Modem/OOK/Modem_OOK_BEC.hpp>
#endif
#ifndef MODEM_OOK_BSC_HPP_
#include <Module/Modem/OOK/Modem_OOK_BSC.hpp>
#endif
#ifndef MODEM_OOK_OPTICAL_HPP_
#include <Module/Modem/OOK/Modem_OOK_optical.hpp>
#endif
#ifndef MODEM_PAM_HPP_
#include <Module/Modem/PAM/Modem_PAM.hpp>
#endif
#ifndef MODEM_PSK_HPP_
#include <Module/Modem/PSK/Modem_PSK.hpp>
#endif
#ifndef MODEM_QAM_HPP_
#include <Module/Modem/QAM/Modem_QAM.hpp>
#endif
#ifndef MODEM_SCMA_HPP_
#include <Module/Modem/SCMA/Modem_SCMA.hpp>
#endif
#ifndef MODEM_USER_HPP_
#include <Module/Modem/User/Modem_user.hpp>
#endif
#ifndef MODULE_HPP_
#include <Module/Module.hpp>
#endif
#ifndef MONITOR_BFER_HPP_
#include <Module/Monitor/BFER/Monitor_BFER.hpp>
#endif
#ifndef MONITOR_EXIT_HPP_
#include <Module/Monitor/EXIT/Monitor_EXIT.hpp>
#endif
#ifndef MONITOR_MI_HPP_
#include <Module/Monitor/MI/Monitor_MI.hpp>
#endif
#ifndef MONITOR_HPP_
#include <Module/Monitor/Monitor.hpp>
#endif
#ifndef MONITOR_REDUCTION_HPP_
#include <Module/Monitor/Monitor_reduction.hpp>
#endif
#ifndef MONITOR_REDUCTION_MPI_HPP_
#include <Module/Monitor/Monitor_reduction_MPI.hpp>
#endif
#ifndef PUNCTURER_LDPC_HPP_
#include <Module/Puncturer/LDPC/Puncturer_LDPC.hpp>
#endif
#ifndef PUNCTURER_NO_HPP_
#include <Module/Puncturer/NO/Puncturer_NO.hpp>
#endif
#ifndef PUNCTURER_POLAR_SHORTLAST_HPP_
#include <Module/Puncturer/Polar/Puncturer_polar_shortlast.hpp>
#endif
#ifndef PUNCTURER_HPP_
#include <Module/Puncturer/Puncturer.hpp>
#endif
#ifndef PUNCTURER_TURBO_HPP_
#include <Module/Puncturer/Turbo/Puncturer_turbo.hpp>
#endif
#ifndef PUNCTURER_TURBO_DB_HPP_
#include <Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp>
#endif
#ifndef QUANTIZER_CUSTOM_HPP_
#include <Module/Quantizer/Custom/Quantizer_custom.hpp>
#endif
#ifndef QUANTIZER_NO_HPP_
#include <Module/Quantizer/NO/Quantizer_NO.hpp>
#endif
#ifndef QUANTIZER_POW2_HPP_
#include <Module/Quantizer/Pow2/Quantizer_pow2.hpp>
#endif
#ifndef QUANTIZER_POW2_FAST_HPP_
#include <Module/Quantizer/Pow2/Quantizer_pow2_fast.hpp>
#endif
#ifndef QUANTIZER_HPP_
#include <Module/Quantizer/Quantizer.hpp>
#endif
#ifndef SC_MODULE_HPP_
#include <Module/SC_Module.hpp>
#endif
#ifndef SOCKET_HPP_
#include <Module/Socket.hpp>
#endif
#ifndef SOURCE_AZCW_HPP_
#include <Module/Source/AZCW/Source_AZCW.hpp>
#endif
#ifndef SOURCE_RANDOM_HPP_
#include <Module/Source/Random/Source_random.hpp>
#endif
#ifndef SOURCE_RANDOM_FAST_HPP_
#include <Module/Source/Random/Source_random_fast.hpp>
#endif
#ifndef SOURCE_HPP_
#include <Module/Source/Source.hpp>
#endif
#ifndef SOURCE_USER_HPP_
#include <Module/Source/User/Source_user.hpp>
#endif
#ifndef TASK_HPP_
#include <Module/Task.hpp>
#endif
#ifndef SIMULATION_BFER_HPP_
#include <Simulation/BFER/BFER.hpp>
#endif
#ifndef SIMULATION_BFER_ITE_HPP_
#include <Simulation/BFER/Iterative/BFER_ite.hpp>
#endif
#ifndef SC_SIMULATION_BFER_ITE_HPP_
#include <Simulation/BFER/Iterative/SystemC/SC_BFER_ite.hpp>
#endif
#ifndef SIMULATION_BFER_ITE_THREADS_HPP_
#include <Simulation/BFER/Iterative/Threads/BFER_ite_threads.hpp>
#endif
#ifndef SIMULATION_BFER_STD_HPP_
#include <Simulation/BFER/Standard/BFER_std.hpp>
#endif
#ifndef SC_SIMULATION_BFER_STD_HPP_
#include <Simulation/BFER/Standard/SystemC/SC_BFER_std.hpp>
#endif
#ifndef SIMULATION_BFER_STD_THREADS_HPP_
#include <Simulation/BFER/Standard/Threads/BFER_std_threads.hpp>
#endif
#ifndef SIMULATION_EXIT_HPP_
#include <Simulation/EXIT/EXIT.hpp>
#endif
#ifndef SIMULATION_HPP_
#include <Simulation/Simulation.hpp>
#endif
#ifndef BIT_PACKER_HPP_
#include <Tools/Algo/Bit_packer.hpp>
#endif
#ifndef DRAW_GENERATOR_HPP_
#include <Tools/Algo/Draw_generator/Draw_generator.hpp>
#endif
#ifndef EVENT_GENERATOR_HPP
#include <Tools/Algo/Draw_generator/Event_generator/Event_generator.hpp>
#endif
#ifndef EVENT_GENERATOR_FAST_HPP
#include <Tools/Algo/Draw_generator/Event_generator/Fast/Event_generator_fast.hpp>
#endif
#ifndef EVENT_GENERATOR_GSL_HPP
#include <Tools/Algo/Draw_generator/Event_generator/GSL/Event_generator_GSL.hpp>
#endif
#ifndef EVENT_GENERATOR_MKL_HPP
#include <Tools/Algo/Draw_generator/Event_generator/MKL/Event_generator_MKL.hpp>
#endif
#ifndef EVENT_GENERATOR_STD_HPP
#include <Tools/Algo/Draw_generator/Event_generator/Standard/Event_generator_std.hpp>
#endif
#ifndef GAUSSIAN_NOISE_GENERATOR_FAST_HPP_
#include <Tools/Algo/Draw_generator/Gaussian_noise_generator/Fast/Gaussian_noise_generator_fast.hpp>
#endif
#ifndef GAUSSIAN_NOISE_GENERATOR_HPP_
#include <Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp>
#endif
#ifndef GAUSSIAN_NOISE_GENERATOR_GSL_HPP_
#include <Tools/Algo/Draw_generator/Gaussian_noise_generator/GSL/Gaussian_noise_generator_GSL.hpp>
#endif
#ifndef GAUSSIAN_NOISE_GENERATOR_MKL_HPP_
#include <Tools/Algo/Draw_generator/Gaussian_noise_generator/MKL/Gaussian_noise_generator_MKL.hpp>
#endif
#ifndef GAUSSIAN_NOISE_GENERATOR_STD_HPP_
#include <Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp>
#endif
#ifndef User_pdf_noise_generator_fast_HPP_
#include <Tools/Algo/Draw_generator/User_pdf_noise_generator/Fast/User_pdf_noise_generator_fast.hpp>
#endif
#ifndef User_pdf_noise_generator_GSL_HPP_
#include <Tools/Algo/Draw_generator/User_pdf_noise_generator/GSL/User_pdf_noise_generator_GSL.hpp>
#endif
#ifndef User_pdf_noise_generator_MKL_HPP_
#include <Tools/Algo/Draw_generator/User_pdf_noise_generator/MKL/User_pdf_noise_generator_MKL.hpp>
#endif
#ifndef USER_PDF_NOISE_GENERATOR_STD_HPP_
#include <Tools/Algo/Draw_generator/User_pdf_noise_generator/Standard/User_pdf_noise_generator_std.hpp>
#endif
#ifndef USER_PDF_NOISE_GENERATOR_HPP
#include <Tools/Algo/Draw_generator/User_pdf_noise_generator/User_pdf_noise_generator.hpp>
#endif
#ifndef HISTOGRAM_H__
#include <Tools/Algo/Histogram.hpp>
#endif
#ifndef FULL_MATRIX_HPP_
#include <Tools/Algo/Matrix/Full_matrix/Full_matrix.hpp>
#endif
#ifndef MATRIX_HPP_
#include <Tools/Algo/Matrix/Matrix.hpp>
#endif
#ifndef SPARSE_MATRIX_HPP_
#include <Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp>
#endif
#ifndef PREDICATE_HPP
#include <Tools/Algo/Predicate.hpp>
#endif
#ifndef PREDICATE_ITE_HPP
#include <Tools/Algo/Predicate_ite.hpp>
#endif
#ifndef PRNG_MT19937_HPP
#include <Tools/Algo/PRNG/PRNG_MT19937.hpp>
#endif
#ifndef PRNG_MT19937_SIMD_HPP
#include <Tools/Algo/PRNG/PRNG_MT19937_simd.hpp>
#endif
#ifndef LC_SORTER_HPP
#include <Tools/Algo/Sort/LC_sorter.hpp>
#endif
#ifndef LC_SORTER_SIMD_HPP
#include <Tools/Algo/Sort/LC_sorter_simd.hpp>
#endif
#ifndef BINARY_NODE_HPP_
#include <Tools/Algo/Tree/Binary_node.hpp>
#endif
#ifndef BINARY_TREE_HPP_
#include <Tools/Algo/Tree/Binary_tree.hpp>
#endif
#ifndef BINARY_TREE_METRIC_HPP_
#include <Tools/Algo/Tree/Binary_tree_metric.hpp>
#endif
#ifndef ARGUMENT_HANDLER_HPP_
#include <Tools/Arguments/Argument_handler.hpp>
#endif
#ifndef ARGUMENT_TOOLS_HPP_
#include <Tools/Arguments/Argument_tools.hpp>
#endif
#ifndef ARGUMENT_INFO_HPP_
#include <Tools/Arguments/Maps/Argument_info.hpp>
#endif
#ifndef ARGUMENT_LINKS_HPP_
#include <Tools/Arguments/Maps/Argument_links.hpp>
#endif
#ifndef ARGUMENT_MAP_GROUP_HPP_
#include <Tools/Arguments/Maps/Argument_map_group.hpp>
#endif
#ifndef ARGUMENT_MAP_INFO_HPP_
#include <Tools/Arguments/Maps/Argument_map_info.hpp>
#endif
#ifndef ARGUMENT_MAP_VALUE_HPP_
#include <Tools/Arguments/Maps/Argument_map_value.hpp>
#endif
#ifndef ARGUMENT_TAG_HPP_
#include <Tools/Arguments/Maps/Argument_tag.hpp>
#endif
#ifndef ARGUMENT_RANGE_HPP_
#include <Tools/Arguments/Ranges/Argument_range.hpp>
#endif
#ifndef ARGUMENT_FUNCTION_RANGE_HPP_
#include <Tools/Arguments/Ranges/Function.hpp>
#endif
#ifndef ARGUMENT_RANGE_LENGTH_HPP_
#include <Tools/Arguments/Ranges/Length.hpp>
#endif
#ifndef ARGUMENT_MAX_RANGE_HPP_
#include <Tools/Arguments/Ranges/Max.hpp>
#endif
#ifndef ARGUMENT_MIN_RANGE_HPP_
#include <Tools/Arguments/Ranges/Min.hpp>
#endif
#ifndef ARGUMENT_RANGE_NEGATIVE_HPP_
#include <Tools/Arguments/Ranges/Negative.hpp>
#endif
#ifndef ARGUMENT_RANGE_NON_ZERO_HPP_
#include <Tools/Arguments/Ranges/Non_zero.hpp>
#endif
#ifndef ARGUMENT_RANGE_POSITIVE_HPP_
#include <Tools/Arguments/Ranges/Positive.hpp>
#endif
#ifndef ARGUMENT_EXAMPLE_SET_RANGE_HPP_
#include <Tools/Arguments/Ranges/Set/Example_set.hpp>
#endif
#ifndef ARGUMENT_EXCLUDING_SET_RANGE_HPP_
#include <Tools/Arguments/Ranges/Set/Excluding_set.hpp>
#endif
#ifndef ARGUMENT_INCLUDING_SET_RANGE_HPP_
#include <Tools/Arguments/Ranges/Set/Including_set.hpp>
#endif
#ifndef ARGUMENT_SET_RANGE_HPP_
#include <Tools/Arguments/Ranges/Set/Set.hpp>
#endif
#ifndef ARGUMENT_SPLITTER_HPP_
#include <Tools/Arguments/Splitter/Splitter.hpp>
#endif
#ifndef ARGUMENT_TYPE_HPP_
#include <Tools/Arguments/Types/Argument_type.hpp>
#endif
#ifndef ARGUMENT_TYPE_LIMITED_HPP_
#include <Tools/Arguments/Types/Argument_type_limited.hpp>
#endif
#ifndef ARGUMENT_TYPE_LIST_HPP_
#include <Tools/Arguments/Types/Container/List.hpp>
#endif
#ifndef ARGUMENT_TYPE_TEXT_HPP_
#include <Tools/Arguments/Types/Container/Text.hpp>
#endif
#ifndef ARGUMENT_TYPE_FILE_HPP_
#include <Tools/Arguments/Types/File_system/File.hpp>
#endif
#ifndef ARGUMENT_TYPE_FILE_SYSTEM_HPP_
#include <Tools/Arguments/Types/File_system/File_system.hpp>
#endif
#ifndef ARGUMENT_TYPE_FOLDER_HPP_
#include <Tools/Arguments/Types/File_system/Folder.hpp>
#endif
#ifndef ARGUMENT_TYPE_PATH_HPP_
#include <Tools/Arguments/Types/File_system/Path.hpp>
#endif
#ifndef ARGUMENT_TYPE_NONE_HPP_
#include <Tools/Arguments/Types/None.hpp>
#endif
#ifndef ARGUMENT_TYPE_BOOLEAN_HPP_
#include <Tools/Arguments/Types/Number/Boolean.hpp>
#endif
#ifndef ARGUMENT_TYPE_INTEGER_HPP_
#include <Tools/Arguments/Types/Number/Integer.hpp>
#endif
#ifndef ARGUMENT_TYPE_REAL_HPP_
#include <Tools/Arguments/Types/Number/Real.hpp>
#endif
#ifndef ARGUMENT_UTILITIES_HPP
#include <Tools/Arguments/utilities.hpp>
#endif
#ifndef AUTO_CLONED_UNIQUE_PTR_HPP__
#include <Tools/auto_cloned_unique_ptr.hpp>
#endif
#ifndef BCH_POLYNOMIAL_GENERATOR_HPP
#include <Tools/Code/BCH/BCH_polynomial_generator.hpp>
#endif
#ifndef ALIST_HPP_
#include <Tools/Code/LDPC/AList/AList.hpp>
#endif
#ifndef LDPC_MATRIX_HANDLER_HPP_
#include <Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp>
#endif
#ifndef QC_HPP_
#include <Tools/Code/LDPC/QC/QC.hpp>
#endif
#ifndef DVBS2_CONSTANTS_HPP_
#include <Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants.hpp>
#endif
#ifndef DVBS2_CONSTANTS_16200_HPP_
#include <Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants_16200.hpp>
#endif
#ifndef DVBS2_CONSTANTS_64800_HPP_
#include <Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants_64800.hpp>
#endif
#ifndef LDPC_SYNDROME_HPP_
#include <Tools/Code/LDPC/Syndrome/LDPC_syndrome.hpp>
#endif
#ifndef UPDATE_RULE_AMS_HPP
#include <Tools/Code/LDPC/Update_rule/AMS/Update_rule_AMS.hpp>
#endif
#ifndef UPDATE_RULE_AMS_SIMD_HPP
#include <Tools/Code/LDPC/Update_rule/AMS/Update_rule_AMS_simd.hpp>
#endif
#ifndef UPDATE_RULE_LSPA_HPP
#include <Tools/Code/LDPC/Update_rule/LSPA/Update_rule_LSPA.hpp>
#endif
#ifndef UPDATE_RULE_LSPA_SIMD_HPP
#include <Tools/Code/LDPC/Update_rule/LSPA/Update_rule_LSPA_simd.hpp>
#endif
#ifndef UPDATE_RULE_MS_HPP
#include <Tools/Code/LDPC/Update_rule/MS/Update_rule_MS.hpp>
#endif
#ifndef UPDATE_RULE_MS_SIMD_HPP
#include <Tools/Code/LDPC/Update_rule/MS/Update_rule_MS_simd.hpp>
#endif
#ifndef UPDATE_RULE_NMS_HPP
#include <Tools/Code/LDPC/Update_rule/NMS/Update_rule_NMS.hpp>
#endif
#ifndef UPDATE_RULE_NMS_SIMD_HPP
#include <Tools/Code/LDPC/Update_rule/NMS/Update_rule_NMS_simd.hpp>
#endif
#ifndef UPDATE_RULE_OMS_HPP
#include <Tools/Code/LDPC/Update_rule/OMS/Update_rule_OMS.hpp>
#endif
#ifndef UPDATE_RULE_OMS_SIMD_HPP
#include <Tools/Code/LDPC/Update_rule/OMS/Update_rule_OMS_simd.hpp>
#endif
#ifndef UPDATE_RULE_SPA_HPP
#include <Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA.hpp>
#endif
#ifndef UPDATE_RULE_SPA_SIMD_HPP
#include <Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA_simd.hpp>
#endif
#ifndef API_POLAR_HPP_
#include <Tools/Code/Polar/API/API_polar.hpp>
#endif
#ifndef API_POLAR_DYNAMIC_INTER_HPP_
#include <Tools/Code/Polar/API/API_polar_dynamic_inter.hpp>
#endif
#ifndef API_POLAR_DYNAMIC_INTER_8BIT_BITPACKING_HPP_
#include <Tools/Code/Polar/API/API_polar_dynamic_inter_8bit_bitpacking.hpp>
#endif
#ifndef API_POLAR_DYNAMIC_INTRA_HPP_
#include <Tools/Code/Polar/API/API_polar_dynamic_intra.hpp>
#endif
#ifndef API_POLAR_DYNAMIC_SEQ_HPP_
#include <Tools/Code/Polar/API/API_polar_dynamic_seq.hpp>
#endif
#ifndef API_POLAR_STATIC_INTER_HPP_
#include <Tools/Code/Polar/API/API_polar_static_inter.hpp>
#endif
#ifndef API_POLAR_STATIC_INTER_8BIT_BITPACKING_HPP_
#include <Tools/Code/Polar/API/API_polar_static_inter_8bit_bitpacking.hpp>
#endif
#ifndef API_POLAR_STATIC_INTRA_16BIT_HPP_
#include <Tools/Code/Polar/API/API_polar_static_intra_16bit.hpp>
#endif
#ifndef API_POLAR_STATIC_INTRA_32BIT_HPP_
#include <Tools/Code/Polar/API/API_polar_static_intra_32bit.hpp>
#endif
#ifndef API_POLAR_STATIC_INTRA_8BIT_HPP_
#include <Tools/Code/Polar/API/API_polar_static_intra_8bit.hpp>
#endif
#ifndef API_POLAR_STATIC_SEQ_HPP_
#include <Tools/Code/Polar/API/API_polar_static_seq.hpp>
#endif
#ifndef FROZENBITS_GENERATOR_HPP_
#include <Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp>
#endif
#ifndef FROZENBITS_GENERATOR_FILE_HPP_
#include <Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp>
#endif
#ifndef FROZENBITS_GENERATOR_GA_HPP_
#include <Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp>
#endif
#ifndef FROZENBITS_GENERATOR_TV_HPP_
#include <Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp>
#endif
#ifndef FROZENBITS_NOTIFIER_HPP
#include <Tools/Code/Polar/Frozenbits_notifier.hpp>
#endif
#ifndef PATTERN_POLAR_INTERFACE_HPP_
#include <Tools/Code/Polar/Patterns/Pattern_polar_i.hpp>
#endif
#ifndef PATTERN_POLAR_RATE_0_HPP_
#include <Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp>
#endif
#ifndef PATTERN_POLAR_RATE_0_LEFT_HPP_
#include <Tools/Code/Polar/Patterns/Pattern_polar_r0_left.hpp>
#endif
#ifndef PATTERN_POLAR_RATE_1_HPP_
#include <Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp>
#endif
#ifndef PATTERN_POLAR_REP_HPP_
#include <Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp>
#endif
#ifndef PATTERN_POLAR_REP_LEFT_HPP_
#include <Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp>
#endif
#ifndef PATTERN_POLAR_SPC_HPP_
#include <Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp>
#endif
#ifndef PATTERN_POLAR_STANDARD_HPP_
#include <Tools/Code/Polar/Patterns/Pattern_polar_std.hpp>
#endif
#ifndef PATTERN_POLAR_PARSER_HPP
#include <Tools/Code/Polar/Pattern_polar_parser.hpp>
#endif
#ifndef RS_POLYNOMIAL_GENERATOR_HPP
#include <Tools/Code/RS/RS_polynomial_generator.hpp>
#endif
#ifndef MODEM_SCMA_FUNCTIONS_HPP
#include <Tools/Code/SCMA/modem_SCMA_functions.hpp>
#endif
#ifndef CRC_CHECKER_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker.hpp>
#endif
#ifndef CRC_CHECKER_DB_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker_DB.hpp>
#endif
#ifndef FLIP_AND_CHECK_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp>
#endif
#ifndef FLIP_AND_CHECK_DB_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check_DB.hpp>
#endif
#ifndef POST_PROCESSING_SISO_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp>
#endif
#ifndef SCALING_FACTOR_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp>
#endif
#ifndef SCALING_FACTOR_ADAPTIVE_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_adaptive.hpp>
#endif
#ifndef SCALING_FACTOR_ARRAY_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp>
#endif
#ifndef SCALING_FACTOR_CONSTANT_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp>
#endif
#ifndef SCALING_FACTOR_SEQ_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp>
#endif
#ifndef SCALING_FACTOR_VEC_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp>
#endif
#ifndef SELF_CORRECTED_HPP
#include <Tools/Code/Turbo/Post_processing_SISO/Self_corrected/Self_corrected.hpp>
#endif
#ifndef DUMPER_HPP_
#include <Tools/Display/Dumper/Dumper.hpp>
#endif
#ifndef DUMPER_REDUCTION_HPP_
#include <Tools/Display/Dumper/Dumper_reduction.hpp>
#endif
#ifndef FRAME_TRACE_HPP
#include <Tools/Display/Frame_trace/Frame_trace.hpp>
#endif
#ifndef Reporter_BFER_HPP_
#include <Tools/Display/Reporter/BFER/Reporter_BFER.hpp>
#endif
#ifndef Reporter_EXIT_HPP_
#include <Tools/Display/Reporter/EXIT/Reporter_EXIT.hpp>
#endif
#ifndef REPORTER_MI_HPP_
#include <Tools/Display/Reporter/MI/Reporter_MI.hpp>
#endif
#ifndef REPORTER_NOISE_HPP_
#include <Tools/Display/Reporter/Noise/Reporter_noise.hpp>
#endif
#ifndef REPORTER_HPP__
#include <Tools/Display/Reporter/Reporter.hpp>
#endif
#ifndef REPORTER_MONITOR_HPP_
#include <Tools/Display/Reporter/Reporter_monitor.hpp>
#endif
#ifndef REPORTER_THROUGHPUT_HPP_
#include <Tools/Display/Reporter/Throughput/Reporter_throughput.hpp>
#endif
#ifndef STATISTICS_HPP_
#include <Tools/Display/Statistics/Statistics.hpp>
#endif
#ifndef TERMINAL_STD_HPP_
#include <Tools/Display/Terminal/Standard/Terminal_std.hpp>
#endif
#ifndef TERMINAL_HPP_
#include <Tools/Display/Terminal/Terminal.hpp>
#endif
#ifndef CANNOT_ALLOCATE_HPP_
#include <Tools/Exception/cannot_allocate/cannot_allocate.hpp>
#endif
#ifndef DOMAIN_ERROR_HPP_
#include <Tools/Exception/domain_error/domain_error.hpp>
#endif
#ifndef EXCEPTION_HPP_
#include <Tools/Exception/exception.hpp>
#endif
#ifndef INVALID_ARGUMENT_HPP_
#include <Tools/Exception/invalid_argument/invalid_argument.hpp>
#endif
#ifndef LENGTH_ERROR_HPP_
#include <Tools/Exception/length_error/length_error.hpp>
#endif
#ifndef LOGIC_ERROR_HPP_
#include <Tools/Exception/logic_error/logic_error.hpp>
#endif
#ifndef OUT_OF_RANGE_HPP_
#include <Tools/Exception/out_of_range/out_of_range.hpp>
#endif
#ifndef OVERFLOW_ERROR_HPP_
#include <Tools/Exception/overflow_error/overflow_error.hpp>
#endif
#ifndef RANGE_ERROR_HPP_
#include <Tools/Exception/range_error/range_error.hpp>
#endif
#ifndef RUNTIME_ERROR_HPP_
#include <Tools/Exception/runtime_error/runtime_error.hpp>
#endif
#ifndef UNDERFLOW_ERROR_HPP_
#include <Tools/Exception/underflow_error/underflow_error.hpp>
#endif
#ifndef UNIMPLEMENTED_ERROR_HPP_
#include <Tools/Exception/unimplemented_error/unimplemented_error.hpp>
#endif
#ifndef INTERLEAVER_CORE_ARP_DVB1_HPP
#include <Tools/Interleaver/ARP/Interleaver_core_ARP_DVB_RCS1.hpp>
#endif
#ifndef INTERLEAVER_CORE_ARP_DVB2_HPP
#include <Tools/Interleaver/ARP/Interleaver_core_ARP_DVB_RCS2.hpp>
#endif
#ifndef INTERLEAVER_CORE_CCSDS_HPP
#include <Tools/Interleaver/CCSDS/Interleaver_core_CCSDS.hpp>
#endif
#ifndef INTERLEAVER_CORE_COLUMN_ROW_HPP
#include <Tools/Interleaver/Column_row/Interleaver_core_column_row.hpp>
#endif
#ifndef INTERLEAVER_CORE_GOLDEN_HPP
#include <Tools/Interleaver/Golden/Interleaver_core_golden.hpp>
#endif
#ifndef INTERLEAVER_CORE_HPP_
#include <Tools/Interleaver/Interleaver_core.hpp>
#endif
#ifndef INTERLEAVER_CORE_LTE_HPP
#include <Tools/Interleaver/LTE/Interleaver_core_LTE.hpp>
#endif
#ifndef INTERLEAVER_CORE_NO_HPP
#include <Tools/Interleaver/NO/Interleaver_core_NO.hpp>
#endif
#ifndef INTERLEAVER_CORE_RANDOM_HPP
#include <Tools/Interleaver/Random/Interleaver_core_random.hpp>
#endif
#ifndef INTERLEAVER_CORE_RANDOM_COLUMN_HPP
#include <Tools/Interleaver/Random_column/Interleaver_core_random_column.hpp>
#endif
#ifndef INTERLEAVER_CORE_ROW_COLUMN_HPP
#include <Tools/Interleaver/Row_column/Interleaver_core_row_column.hpp>
#endif
#ifndef INTERLEAVER_CORE_USER_HPP
#include <Tools/Interleaver/User/Interleaver_core_user.hpp>
#endif
#ifndef DISTRIBUTION_HPP__
#include <Tools/Math/Distribution/Distribution.hpp>
#endif
#ifndef DISTRIBUTIONS_HPP
#include <Tools/Math/Distribution/Distributions.hpp>
#endif
#ifndef GALOIS_HPP
#include <Tools/Math/Galois.hpp>
#endif
#ifndef ERASED_PROBABILITY_HPP_
#include <Tools/Noise/Event_probability.hpp>
#endif
#ifndef NOISE_HPP__
#include <Tools/Noise/Noise.hpp>
#endif
#ifndef RECEIVED_OPTICAL_POWER_HPP_
#include <Tools/Noise/Received_optical_power.hpp>
#endif
#ifndef SIGMA_HPP_
#include <Tools/Noise/Sigma.hpp>
#endif
#ifndef REORDERER_HPP_
#include <Tools/Perf/Reorderer/Reorderer.hpp>
#endif
#ifndef SC_DEBUG_HPP_
#include <Tools/SystemC/SC_Debug.hpp>
#endif
#ifndef SC_DUMMY_HPP_
#include <Tools/SystemC/SC_Dummy.hpp>
#endif
#ifndef SC_DUPLICATOR_HPP_
#include <Tools/SystemC/SC_Duplicator.hpp>
#endif
#ifndef SC_FUNNEL_HPP_
#include <Tools/SystemC/SC_Funnel.hpp>
#endif
#ifndef SC_PREDICATE_HPP_
#include <Tools/SystemC/SC_Predicate.hpp>
#endif
#ifndef SC_ROUTER_HPP_
#include <Tools/SystemC/SC_Router.hpp>
#endif
#ifndef BARRIER_HPP
#include <Tools/Threads/Barrier.hpp>
#endif

#endif