#include <sstream>
#include <memory>

#include "Tools/Exception/exception.hpp"

/* // GENERATED DECODERS // */

#define ENABLE_SHORT_GENERATED_DECODERS

// before to uncomment these next lines, make sure to run the script to generate the decoders
// (see "scripts/generate_polar_decoders.sh")

// RATE 1/2
//#define ENABLE_DECODER_SC_FAST_N4_K2_SNR25
//#define ENABLE_DECODER_SC_FAST_N8_K4_SNR25
//#define ENABLE_DECODER_SC_FAST_N16_K8_SNR25
//#define ENABLE_DECODER_SC_FAST_N32_K16_SNR25
//#define ENABLE_DECODER_SC_FAST_N64_K32_SNR25
//#define ENABLE_DECODER_SC_FAST_N128_K64_SNR25
//#define ENABLE_DECODER_SC_FAST_N256_K128_SNR25
//#define ENABLE_DECODER_SC_FAST_N512_K256_SNR25
//#define ENABLE_DECODER_SC_FAST_N1024_K512_SNR25
//#define ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25
//#define ENABLE_DECODER_SC_FAST_N4096_K2048_SNR25
//#define ENABLE_DECODER_SC_FAST_N4096_K2048_SNR33
//#define ENABLE_DECODER_SC_FAST_N8192_K4096_SNR25
//#define ENABLE_DECODER_SC_FAST_N16384_K8192_SNR25
//#define ENABLE_DECODER_SC_FAST_N32768_K16384_SNR18
//#define ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25
//#define ENABLE_DECODER_SC_FAST_N65536_K32768_SNR25
//#define ENABLE_DECODER_SC_FAST_N131072_K65536_SNR25
//#define ENABLE_DECODER_SC_FAST_N262144_K131072_SNR25
//#define ENABLE_DECODER_SC_FAST_N524288_K262144_SNR25
//#define ENABLE_DECODER_SC_FAST_N1048576_K524288_SNR25

// RATE 5/6
//#define ENABLE_DECODER_SC_FAST_N4_K3_SNR40
//#define ENABLE_DECODER_SC_FAST_N8_K7_SNR40
//#define ENABLE_DECODER_SC_FAST_N16_K13_SNR40
//#define ENABLE_DECODER_SC_FAST_N32_K27_SNR40
//#define ENABLE_DECODER_SC_FAST_N64_K53_SNR40
//#define ENABLE_DECODER_SC_FAST_N128_K107_SNR40
//#define ENABLE_DECODER_SC_FAST_N256_K213_SNR40
//#define ENABLE_DECODER_SC_FAST_N512_K427_SNR40
//#define ENABLE_DECODER_SC_FAST_N1024_K853_SNR40
//#define ENABLE_DECODER_SC_FAST_N2048_K1707_SNR40
//#define ENABLE_DECODER_SC_FAST_N4096_K3413_SNR40
//#define ENABLE_DECODER_SC_FAST_N8192_K6827_SNR40
//#define ENABLE_DECODER_SC_FAST_N16384_K13653_SNR40
//#define ENABLE_DECODER_SC_FAST_N32768_K27307_SNR40
//#define ENABLE_DECODER_SC_FAST_N65536_K54613_SNR40
//#define ENABLE_DECODER_SC_FAST_N131072_K109227_SNR40
//#define ENABLE_DECODER_SC_FAST_N262144_K218453_SNR40
//#define ENABLE_DECODER_SC_FAST_N524288_K436907_SNR40
//#define ENABLE_DECODER_SC_FAST_N1048576_K873813_SNR40

// RATE 0.84
//#define ENABLE_DECODER_SC_FAST_N32768_K27568_SNR40

// RATE 9/10
//#define ENABLE_DECODER_SC_FAST_N2048_K1843_SNR40
//#define ENABLE_DECODER_SC_FAST_N16384_K14746_SNR40
//#define ENABLE_DECODER_SC_FAST_N32768_K29492_SNR40

// RATE 1/10, 2/10, ..., 9/10 => N = 2048
//#define ENABLE_DECODER_SC_FAST_N2048_K205_SNR25  // Q = 1/10
//#define ENABLE_DECODER_SC_FAST_N2048_K410_SNR25  // Q = 2/10
//#define ENABLE_DECODER_SC_FAST_N2048_K614_SNR25  // Q = 3/10
//#define ENABLE_DECODER_SC_FAST_N2048_K819_SNR25  // Q = 4/10
//#define ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25 // Q = 5/10
//#define ENABLE_DECODER_SC_FAST_N2048_K1229_SNR25 // Q = 6/10
//#define ENABLE_DECODER_SC_FAST_N2048_K1434_SNR25 // Q = 7/10
//#define ENABLE_DECODER_SC_FAST_N2048_K1638_SNR25 // Q = 8/10
//#define ENABLE_DECODER_SC_FAST_N2048_K1843_SNR25 // Q = 9/10

// RATE 1/10, 2/10, ..., 9/10 => N = 32768
//#define ENABLE_DECODER_SC_FAST_N32768_K3277_SNR25  // Q = 1/10
//#define ENABLE_DECODER_SC_FAST_N32768_K6554_SNR25  // Q = 2/10
//#define ENABLE_DECODER_SC_FAST_N32768_K9830_SNR25  // Q = 3/10
//#define ENABLE_DECODER_SC_FAST_N32768_K13107_SNR25 // Q = 4/10
//#define ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25 // Q = 5/10
//#define ENABLE_DECODER_SC_FAST_N32768_K19661_SNR25 // Q = 6/10
//#define ENABLE_DECODER_SC_FAST_N32768_K22938_SNR25 // Q = 7/10
//#define ENABLE_DECODER_SC_FAST_N32768_K26214_SNR25 // Q = 8/10
//#define ENABLE_DECODER_SC_FAST_N32768_K29491_SNR25 // Q = 9/10

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------

// RATE 1/2
//#define ENABLE_DECODER_SCL_FAST_CA_N4_K2_SNR25

// RATE ???
//#define ENABLE_DECODER_SCL_FAST_CA_N2048_K1755_SNR35
//#define ENABLE_DECODER_SCL_FAST_CA_N256_K64_SNR30

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------

// RATE 1/2
#ifdef ENABLE_DECODER_SC_FAST_N4_K2_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8_K4_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K4_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K4_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16_K8_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K8_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K8_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32_K16_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K16_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K16_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N64_K32_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K32_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K32_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N128_K64_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K64_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K64_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N256_K128_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K128_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K128_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N512_K256_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K256_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K256_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1024_K512_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K512_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K512_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K2048_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K2048_SNR33
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR33.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR33.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8192_K4096_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K4096_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K4096_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K8192_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K8192_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K8192_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR18
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR18.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR18.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N65536_K32768_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N131072_K65536_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K65536_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K65536_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N262144_K131072_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K131072_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K131072_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N524288_K262144_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K262144_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K262144_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1048576_K524288_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K524288_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K524288_SNR25.hpp"
#endif
#endif
// RATE 5/6
#ifdef ENABLE_DECODER_SC_FAST_N4_K3_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K3_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K3_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8_K7_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K7_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K7_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16_K13_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K13_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K13_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32_K27_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K27_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K27_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N64_K53_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K53_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K53_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N128_K107_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K107_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K107_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N256_K213_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K213_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K213_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N512_K427_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K427_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K427_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1024_K853_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K853_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K853_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1707_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1707_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1707_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K3413_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K3413_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K3413_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8192_K6827_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K6827_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K6827_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K13653_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K13653_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K13653_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K27307_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27307_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27307_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N65536_K54613_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K54613_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K54613_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N131072_K109227_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K109227_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K109227_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N262144_K218453_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K218453_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K218453_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N524288_K436907_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K436907_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K436907_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1048576_K873813_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K873813_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K873813_SNR40.hpp"
#endif
#endif
// RATE .84
#ifdef ENABLE_DECODER_SC_FAST_N32768_K27568_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27568_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27568_SNR40.hpp"
#endif
#endif
// RATE 9/10
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1843_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR40.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K14746_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K14746_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K14746_SNR40.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K29492_SNR40
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29492_SNR40.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29492_SNR40.hpp"
#endif
#endif
// RATE 1/10, 2/10, ..., 9/10 => N = 2048
#ifdef ENABLE_DECODER_SC_FAST_N2048_K205_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K205_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K205_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K410_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K410_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K410_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K614_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K614_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K614_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K819_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K819_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K819_SNR25.hpp"
#endif
#endif
//#ifdef ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25
//#ifdef ENABLE_SHORT_GENERATED_DECODERS
//#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.hpp"
//#else
//#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.hpp"
//#endif
//#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1229_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1229_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1229_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1434_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1434_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1434_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1638_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1638_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1638_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1843_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR25.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR25.hpp"
#endif
#endif
// RATE 1/10, 2/10, ..., 9/10 => N = 32768
#ifdef ENABLE_DECODER_SC_FAST_N32768_K3277_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K3277_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K3277_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K6554_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K6554_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K6554_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K9830_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K9830_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K9830_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K13107_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K13107_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K13107_SNR25.hpp"
#endif
#endif
//#ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25
//#ifdef ENABLE_SHORT_GENERATED_DECODERS
//#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.short.hpp"
//#else
//#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.hpp"
//#endif
//#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K19661_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K19661_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K19661_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K22938_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K22938_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K22938_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K26214_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K26214_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K26214_SNR25.hpp"
#endif
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K29491_SNR25
#ifdef ENABLE_SHORT_GENERATED_DECODERS
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29491_SNR25.short.hpp"
#else
#include "Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29491_SNR25.hpp"
#endif
#endif

#ifdef ENABLE_DECODER_SCL_FAST_CA_N4_K2_SNR25
#include "Module/Decoder/Polar/SCL/CRC/Generated/Decoder_polar_SCL_fast_CA_sys_N4_K2_SNR25.hpp"
#endif

#ifdef ENABLE_DECODER_SCL_FAST_CA_N2048_K1755_SNR35
#include "Module/Decoder/Polar/SCL/CRC/Generated/Decoder_polar_SCL_fast_CA_sys_N2048_K1755_SNR35.hpp"
#endif

#ifdef ENABLE_DECODER_SCL_FAST_CA_N256_K64_SNR30
#include "Module/Decoder/Polar/SCL/CRC/Generated/Decoder_polar_SCL_fast_CA_sys_N256_K64_SNR30.hpp"
#endif

//#define API_POLAR_DYNAMIC 1

#ifdef API_POLAR_DYNAMIC
#include "Tools/Code/Polar/API/API_polar_dynamic_seq.hpp"
#include "Tools/Code/Polar/API/API_polar_dynamic_inter.hpp"
#include "Tools/Code/Polar/API/API_polar_dynamic_inter_8bit_bitpacking.hpp"
#include "Tools/Code/Polar/API/API_polar_dynamic_intra.hpp"
#else
#include "Tools/Code/Polar/API/API_polar_static_seq.hpp"
#include "Tools/Code/Polar/API/API_polar_static_inter.hpp"
#include "Tools/Code/Polar/API/API_polar_static_inter_8bit_bitpacking.hpp"
#include "Tools/Code/Polar/API/API_polar_static_intra_8bit.hpp"
#include "Tools/Code/Polar/API/API_polar_static_intra_16bit.hpp"
#include "Tools/Code/Polar/API/API_polar_static_intra_32bit.hpp"
#endif

#include "Factory/Module/Decoder/Polar/Decoder_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B, typename Q, class API_polar>
module::Decoder_SIHO<B,Q>* Decoder_polar
::_build_gen(const module::CRC<B> *crc, module::Encoder<B> *encoder) const
{
	if (this->type == "SC")
	{
		// RATE 1/2
#ifdef ENABLE_DECODER_SC_FAST_N4_K2_SNR25
		if (this->implem == "N4_K2_SNR25"           ) return new module::Decoder_polar_SC_fast_sys_N4_K2_SNR25           <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8_K4_SNR25
		if (this->implem == "N8_K4_SNR25"           ) return new module::Decoder_polar_SC_fast_sys_N8_K4_SNR25           <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16_K8_SNR25
		if (this->implem == "N16_K8_SNR25"          ) return new module::Decoder_polar_SC_fast_sys_N16_K8_SNR25          <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32_K16_SNR25
		if (this->implem == "N32_K16_SNR25"         ) return new module::Decoder_polar_SC_fast_sys_N32_K16_SNR25         <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N64_K32_SNR25
		if (this->implem == "N64_K32_SNR25"         ) return new module::Decoder_polar_SC_fast_sys_N64_K32_SNR25         <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N128_K64_SNR25
		if (this->implem == "N128_K64_SNR25"        ) return new module::Decoder_polar_SC_fast_sys_N128_K64_SNR25        <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N256_K128_SNR25
		if (this->implem == "N256_K128_SNR25"       ) return new module::Decoder_polar_SC_fast_sys_N256_K128_SNR25       <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N512_K256_SNR25
		if (this->implem == "N512_K256_SNR25"       ) return new module::Decoder_polar_SC_fast_sys_N512_K256_SNR25       <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1024_K512_SNR25
		if (this->implem == "N1024_K512_SNR25"      ) return new module::Decoder_polar_SC_fast_sys_N1024_K512_SNR25      <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25
		if (this->implem == "N2048_K1024_SNR25"     ) return new module::Decoder_polar_SC_fast_sys_N2048_K1024_SNR25     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K2048_SNR25
		if (this->implem == "N4096_K2048_SNR25"     ) return new module::Decoder_polar_SC_fast_sys_N4096_K2048_SNR25     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K2048_SNR33
		if (this->implem == "N4096_K2048_SNR33"     ) return new module::Decoder_polar_SC_fast_sys_N4096_K2048_SNR33     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8192_K4096_SNR25
		if (this->implem == "N8192_K4096_SNR25"     ) return new module::Decoder_polar_SC_fast_sys_N8192_K4096_SNR25     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K8192_SNR25
		if (this->implem == "N16384_K8192_SNR25"    ) return new module::Decoder_polar_SC_fast_sys_N16384_K8192_SNR25    <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR18
		if (this->implem == "N32768_K16384_SNR18"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K16384_SNR18   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25
		if (this->implem == "N32768_K16384_SNR25"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K16384_SNR25   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N65536_K32768_SNR25
		if (this->implem == "N65536_K32768_SNR25"   ) return new module::Decoder_polar_SC_fast_sys_N65536_K32768_SNR25   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N131072_K65536_SNR25
		if (this->implem == "N131072_K65536_SNR25"  ) return new module::Decoder_polar_SC_fast_sys_N131072_K65536_SNR25  <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N262144_K131072_SNR25
		if (this->implem == "N262144_K131072_SNR25" ) return new module::Decoder_polar_SC_fast_sys_N262144_K131072_SNR25 <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N524288_K262144_SNR25
		if (this->implem == "N524288_K262144_SNR25" ) return new module::Decoder_polar_SC_fast_sys_N524288_K262144_SNR25 <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1048576_K524288_SNR25
		if (this->implem == "N1048576_K524288_SNR25") return new module::Decoder_polar_SC_fast_sys_N1048576_K524288_SNR25<B, Q, API_polar>(this->K, this->N_cw);
#endif
		// RATE 5/6
#ifdef ENABLE_DECODER_SC_FAST_N4_K3_SNR40
		if (this->implem == "N4_K3_SNR40"           ) return new module::Decoder_polar_SC_fast_sys_N4_K3_SNR40           <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8_K7_SNR40
		if (this->implem == "N8_K7_SNR40"           ) return new module::Decoder_polar_SC_fast_sys_N8_K7_SNR40           <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16_K13_SNR40
		if (this->implem == "N16_K13_SNR40"         ) return new module::Decoder_polar_SC_fast_sys_N16_K13_SNR40         <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32_K27_SNR40
		if (this->implem == "N32_K27_SNR40"         ) return new module::Decoder_polar_SC_fast_sys_N32_K27_SNR40         <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N64_K53_SNR40
		if (this->implem == "N64_K53_SNR40"         ) return new module::Decoder_polar_SC_fast_sys_N64_K53_SNR40         <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N128_K107_SNR40
		if (this->implem == "N128_K107_SNR40"       ) return new module::Decoder_polar_SC_fast_sys_N128_K107_SNR40       <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N256_K213_SNR40
		if (this->implem == "N256_K213_SNR40"       ) return new module::Decoder_polar_SC_fast_sys_N256_K213_SNR40       <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N512_K427_SNR40
		if (this->implem == "N512_K427_SNR40"       ) return new module::Decoder_polar_SC_fast_sys_N512_K427_SNR40       <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1024_K853_SNR40
		if (this->implem == "N1024_K853_SNR40"      ) return new module::Decoder_polar_SC_fast_sys_N1024_K853_SNR40      <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1707_SNR40
		if (this->implem == "N2048_K1707_SNR40"     ) return new module::Decoder_polar_SC_fast_sys_N2048_K1707_SNR40     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K3413_SNR40
		if (this->implem == "N4096_K3413_SNR40"     ) return new module::Decoder_polar_SC_fast_sys_N4096_K3413_SNR40     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8192_K6827_SNR40
		if (this->implem == "N8192_K6827_SNR40"     ) return new module::Decoder_polar_SC_fast_sys_N8192_K6827_SNR40     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K13653_SNR40
		if (this->implem == "N16384_K13653_SNR40"   ) return new module::Decoder_polar_SC_fast_sys_N16384_K13653_SNR40   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K27307_SNR40
		if (this->implem == "N32768_K27307_SNR40"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K27307_SNR40   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N65536_K54613_SNR40
		if (this->implem == "N65536_K54613_SNR40"   ) return new module::Decoder_polar_SC_fast_sys_N65536_K54613_SNR40   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N131072_K109227_SNR40
		if (this->implem == "N131072_K109227_SNR40" ) return new module::Decoder_polar_SC_fast_sys_N131072_K109227_SNR40 <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N262144_K218453_SNR40
		if (this->implem == "N262144_K218453_SNR40" ) return new module::Decoder_polar_SC_fast_sys_N262144_K218453_SNR40 <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N524288_K436907_SNR40
		if (this->implem == "N524288_K436907_SNR40" ) return new module::Decoder_polar_SC_fast_sys_N524288_K436907_SNR40 <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1048576_K873813_SNR40
		if (this->implem == "N1048576_K873813_SNR40") return new module::Decoder_polar_SC_fast_sys_N1048576_K873813_SNR40<B, Q, API_polar>(this->K, this->N_cw);
#endif
		// RATE 0.84
#ifdef ENABLE_DECODER_SC_FAST_N32768_K27568_SNR40
		if (this->implem == "N32768_K27568_SNR40"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K27568_SNR40   <B, Q, API_polar>(this->K, this->N_cw);
#endif
		// RATE 9/10
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1843_SNR40
		if (this->implem == "N2048_K1843_SNR40"     ) return new module::Decoder_polar_SC_fast_sys_N2048_K1843_SNR40     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K14746_SNR40
		if (this->implem == "N16384_K14746_SNR40"   ) return new module::Decoder_polar_SC_fast_sys_N16384_K14746_SNR40   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K29492_SNR40
		if (this->implem == "N32768_K29492_SNR40"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K29492_SNR40   <B, Q, API_polar>(this->K, this->N_cw);
#endif
		// RATE 1/10, 2/10, ..., 9/10 => this->N_cw = 2048
#ifdef ENABLE_DECODER_SC_FAST_N2048_K205_SNR25
		if (this->implem == "N2048_K205_SNR25"      ) return new module::Decoder_polar_SC_fast_sys_N2048_K205_SNR25      <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K410_SNR25
		if (this->implem == "N2048_K410_SNR25"      ) return new module::Decoder_polar_SC_fast_sys_N2048_K410_SNR25      <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K614_SNR25
		if (this->implem == "N2048_K614_SNR25"      ) return new module::Decoder_polar_SC_fast_sys_N2048_K614_SNR25      <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K819_SNR25
		if (this->implem == "N2048_K819_SNR25"      ) return new module::Decoder_polar_SC_fast_sys_N2048_K819_SNR25      <B, Q, API_polar>(this->K, this->N_cw);
#endif
//#ifdef ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25
//		if (this->implem == "N2048_K1024_SNR25"     ) return new module::Decoder_polar_SC_fast_sys_N2048_K1024_SNR25     <B, Q, API_polar>(this->K, this->N_cw);
//#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1229_SNR25
		if (this->implem == "N2048_K1229_SNR25"     ) return new module::Decoder_polar_SC_fast_sys_N2048_K1229_SNR25     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1434_SNR25
		if (this->implem == "N2048_K1434_SNR25"     ) return new module::Decoder_polar_SC_fast_sys_N2048_K1434_SNR25     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1638_SNR25
		if (this->implem == "N2048_K1638_SNR25"     ) return new module::Decoder_polar_SC_fast_sys_N2048_K1638_SNR25     <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1843_SNR25
		if (this->implem == "N2048_K1843_SNR25"     ) return new module::Decoder_polar_SC_fast_sys_N2048_K1843_SNR25     <B, Q, API_polar>(this->K, this->N_cw);
#endif
		// RATE 1/10, 2/10, ..., 9/10 => this->N_cw = 32768
#ifdef ENABLE_DECODER_SC_FAST_N32768_K3277_SNR25
		if (this->implem == "N32768_K3277_SNR25"    ) return new module::Decoder_polar_SC_fast_sys_N32768_K3277_SNR25    <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K6554_SNR25
		if (this->implem == "N32768_K6554_SNR25"    ) return new module::Decoder_polar_SC_fast_sys_N32768_K6554_SNR25    <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K9830_SNR25
		if (this->implem == "N32768_K9830_SNR25"    ) return new module::Decoder_polar_SC_fast_sys_N32768_K9830_SNR25    <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K13107_SNR25
		if (this->implem == "N32768_K13107_SNR25"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K13107_SNR25   <B, Q, API_polar>(this->K, this->N_cw);
#endif
//#ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25
//		if (this->implem == "N32768_K16384_SNR25"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K16384_SNR25   <B, Q, API_polar>(this->K, this->N_cw);
//#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K19661_SNR25
		if (this->implem == "N32768_K19661_SNR25"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K19661_SNR25   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K22938_SNR25
		if (this->implem == "N32768_K22938_SNR25"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K22938_SNR25   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K26214_SNR25
		if (this->implem == "N32768_K26214_SNR25"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K26214_SNR25   <B, Q, API_polar>(this->K, this->N_cw);
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K29491_SNR25
		if (this->implem == "N32768_K29491_SNR25"   ) return new module::Decoder_polar_SC_fast_sys_N32768_K29491_SNR25   <B, Q, API_polar>(this->K, this->N_cw);
#endif
	}
	else if (this->type == "SCL" && crc != nullptr && std::unique_ptr<module::CRC<B>>(crc->clone())->get_size() > 0)
	{
#ifdef ENABLE_DECODER_SCL_FAST_CA_N4_K2_SNR25
		if (this->implem == "CA_N4_K2_SNR25"        ) return new module::Decoder_polar_SCL_fast_CA_sys_N4_K2_SNR25       <B, Q, API_polar>(this->K, this->N_cw, this->L, *crc);
#endif
#ifdef ENABLE_DECODER_SCL_FAST_CA_N2048_K1755_SNR35
		if (this->implem == "CA_N2048_K1755_SNR35"  ) return new module::Decoder_polar_SCL_fast_CA_sys_N2048_K1755_SNR35 <B, Q, API_polar>(this->K, this->N_cw, this->L, *crc);
#endif
#ifdef ENABLE_DECODER_SCL_FAST_CA_N256_K64_SNR30
		if (this->implem == "CA_N256_K64_SNR30"     ) return new module::Decoder_polar_SCL_fast_CA_sys_N256_K64_SNR30    <B, Q, API_polar>(this->K, this->N_cw, this->L, *crc);
#endif
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_polar
::build_gen(const module::CRC<B> *crc, module::Encoder<B> *encoder) const
{
	if (this->systematic)
	{
		if (this->simd_strategy == "INTER" && this->type == "SC")
		{
			if (typeid(B) == typeid(signed char))
			{
#ifdef AFF3CT_POLAR_BIT_PACKING
#ifdef API_POLAR_DYNAMIC
				using API_polar = tools::API_polar_dynamic_inter_8bit_bitpacking<B,Q>;
#else
				using API_polar = tools::API_polar_static_inter_8bit_bitpacking<B,Q>;
#endif
#else
#ifdef API_POLAR_DYNAMIC
				using API_polar = tools::API_polar_dynamic_inter<B,Q>;
#else
				using API_polar = tools::API_polar_static_inter<B,Q>;
#endif
#endif
				return _build_gen<B,Q,API_polar>(crc, encoder);
			}
			else
			{
#ifdef API_POLAR_DYNAMIC
				using API_polar = tools::API_polar_dynamic_inter<B,Q>;
#else
				using API_polar = tools::API_polar_static_inter<B,Q>;
#endif
				return _build_gen<B,Q,API_polar>(crc, encoder);
			}
		}
		else if (this->simd_strategy == "INTRA")
		{
			if (typeid(B) == typeid(signed char))
			{
#ifdef API_POLAR_DYNAMIC
				using API_polar = tools::API_polar_dynamic_intra<B,Q>;
#else
				using API_polar = tools::API_polar_static_intra_8bit<B,Q>;
#endif
				return _build_gen<B,Q,API_polar>(crc, encoder);
			}
			else if (typeid(B) == typeid(short))
			{
#ifdef API_POLAR_DYNAMIC
				using API_polar = tools::API_polar_dynamic_intra<B,Q>;
#else
				using API_polar = tools::API_polar_static_intra_16bit<B,Q>;
#endif
				return _build_gen<B,Q,API_polar>(crc, encoder);
			}
			else if (typeid(B) == typeid(int))
			{
#ifdef API_POLAR_DYNAMIC
				using API_polar = tools::API_polar_dynamic_intra<B,Q>;
#else
				using API_polar = tools::API_polar_static_intra_32bit<B,Q>;
#endif
				return _build_gen<B,Q,API_polar>(crc, encoder);
			}
		}
		else if (this->simd_strategy.empty())
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_seq<B,Q>;
#else
			using API_polar = tools::API_polar_static_seq<B,Q>;
#endif
			return _build_gen<B,Q,API_polar>(crc, encoder);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

const std::vector<bool>& Decoder_polar
::get_frozen_bits(const std::string &implem)
{
	// RATE 1/2
#ifdef ENABLE_DECODER_SC_FAST_N4_K2_SNR25
	if (implem == "N4_K2_SNR25"           ) return module::Decoder_polar_SC_fast_sys_fb_4_2_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8_K4_SNR25
	if (implem == "N8_K4_SNR25"           ) return module::Decoder_polar_SC_fast_sys_fb_8_4_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16_K8_SNR25
	if (implem == "N16_K8_SNR25"          ) return module::Decoder_polar_SC_fast_sys_fb_16_8_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32_K16_SNR25
	if (implem == "N32_K16_SNR25"         ) return module::Decoder_polar_SC_fast_sys_fb_32_16_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N64_K32_SNR25
	if (implem == "N64_K32_SNR25"         ) return module::Decoder_polar_SC_fast_sys_fb_64_32_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N128_K64_SNR25
	if (implem == "N128_K64_SNR25"        ) return module::Decoder_polar_SC_fast_sys_fb_128_64_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N256_K128_SNR25
	if (implem == "N256_K128_SNR25"       ) return module::Decoder_polar_SC_fast_sys_fb_256_128_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N512_K256_SNR25
	if (implem == "N512_K256_SNR25"       ) return module::Decoder_polar_SC_fast_sys_fb_512_256_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1024_K512_SNR25
	if (implem == "N1024_K512_SNR25"      ) return module::Decoder_polar_SC_fast_sys_fb_1024_512_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25
	if (implem == "N2048_K1024_SNR25"     ) return module::Decoder_polar_SC_fast_sys_fb_2048_1024_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K2048_SNR25
	if (implem == "N4096_K2048_SNR25"     ) return module::Decoder_polar_SC_fast_sys_fb_4096_2048_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K2048_SNR33
	if (implem == "N4096_K2048_SNR33"     ) return module::Decoder_polar_SC_fast_sys_fb_4096_2048_33;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8192_K4096_SNR25
	if (implem == "N8192_K4096_SNR25"     ) return module::Decoder_polar_SC_fast_sys_fb_8192_4096_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K8192_SNR25
	if (implem == "N16384_K8192_SNR25"    ) return module::Decoder_polar_SC_fast_sys_fb_16384_8192_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR18
	if (implem == "N32768_K16384_SNR18"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_16384_18;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25
	if (implem == "N32768_K16384_SNR25"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_16384_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N65536_K32768_SNR25
	if (implem == "N65536_K32768_SNR25"   ) return module::Decoder_polar_SC_fast_sys_fb_65536_32768_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N131072_K65536_SNR25
	if (implem == "N131072_K65536_SNR25"  ) return module::Decoder_polar_SC_fast_sys_fb_131072_65536_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N262144_K131072_SNR25
	if (implem == "N262144_K131072_SNR25" ) return module::Decoder_polar_SC_fast_sys_fb_262144_131072_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N524288_K262144_SNR25
	if (implem == "N524288_K262144_SNR25" ) return module::Decoder_polar_SC_fast_sys_fb_524288_262144_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1048576_K524288_SNR25
	if (implem == "N1048576_K524288_SNR25") return module::Decoder_polar_SC_fast_sys_fb_1048576_524288_25;
#endif

	// RATE 5/6
#ifdef ENABLE_DECODER_SC_FAST_N4_K3_SNR40
	if (implem == "N4_K3_SNR40"           ) return module::Decoder_polar_SC_fast_sys_fb_4_3_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8_K7_SNR40
	if (implem == "N8_K7_SNR40"           ) return module::Decoder_polar_SC_fast_sys_fb_8_7_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16_K13_SNR40
	if (implem == "N16_K13_SNR40"         ) return module::Decoder_polar_SC_fast_sys_fb_16_13_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32_K27_SNR40
	if (implem == "N32_K27_SNR40"         ) return module::Decoder_polar_SC_fast_sys_fb_32_27_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N64_K53_SNR40
	if (implem == "N64_K53_SNR40"         ) return module::Decoder_polar_SC_fast_sys_fb_64_53_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N128_K107_SNR40
	if (implem == "N128_K107_SNR40"       ) return module::Decoder_polar_SC_fast_sys_fb_128_107_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N256_K213_SNR40
	if (implem == "N256_K213_SNR40"       ) return module::Decoder_polar_SC_fast_sys_fb_256_213_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N512_K427_SNR40
	if (implem == "N512_K427_SNR40"       ) return module::Decoder_polar_SC_fast_sys_fb_512_427_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1024_K853_SNR40
	if (implem == "N1024_K853_SNR40"      ) return module::Decoder_polar_SC_fast_sys_fb_1024_853_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1707_SNR40
	if (implem == "N2048_K1707_SNR40"     ) return module::Decoder_polar_SC_fast_sys_fb_2048_1707_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N4096_K3413_SNR40
	if (implem == "N4096_K3413_SNR40"     ) return module::Decoder_polar_SC_fast_sys_fb_4096_3413_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N8192_K6827_SNR40
	if (implem == "N8192_K6827_SNR40"     ) return module::Decoder_polar_SC_fast_sys_fb_8192_6827_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K13653_SNR40
	if (implem == "N16384_K13653_SNR40"   ) return module::Decoder_polar_SC_fast_sys_fb_16384_13653_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K27307_SNR40
	if (implem == "N32768_K27307_SNR40"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_27307_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N65536_K54613_SNR40
	if (implem == "N65536_K54613_SNR40"   ) return module::Decoder_polar_SC_fast_sys_fb_65536_54613_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N131072_K109227_SNR40
	if (implem == "N131072_K109227_SNR40" ) return module::Decoder_polar_SC_fast_sys_fb_131072_109227_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N262144_K218453_SNR40
	if (implem == "N262144_K218453_SNR40" ) return module::Decoder_polar_SC_fast_sys_fb_262144_218453_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N524288_K436907_SNR40
	if (implem == "N524288_K436907_SNR40" ) return module::Decoder_polar_SC_fast_sys_fb_524288_436907_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N1048576_K873813_SNR40
	if (implem == "N1048576_K873813_SNR40") return module::Decoder_polar_SC_fast_sys_fb_1048576_873813_40;
#endif

	// RATE 0.84
#ifdef ENABLE_DECODER_SC_FAST_N32768_K27568_SNR40
	if (implem == "N32768_K27568_SNR40"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_27568_40;
#endif

	// RATE 9/10
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1843_SNR40
	if (implem == "N2048_K1843_SNR40"     ) return module::Decoder_polar_SC_fast_sys_fb_2048_1843_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N16384_K14746_SNR40
	if (implem == "N16384_K14746_SNR40"   ) return module::Decoder_polar_SC_fast_sys_fb_16384_14746_40;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K29492_SNR40
	if (implem == "N32768_K29492_SNR40"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_29492_40;
#endif

	// RATE 1/10, 2/10, ..., 9/10 => N = 2048
#ifdef ENABLE_DECODER_SC_FAST_N2048_K205_SNR25
	if (implem == "N2048_K205_SNR25"      ) return module::Decoder_polar_SC_fast_sys_fb_2048_205_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K410_SNR25
	if (implem == "N2048_K410_SNR25"      ) return module::Decoder_polar_SC_fast_sys_fb_2048_410_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K614_SNR25
	if (implem == "N2048_K614_SNR25"      ) return module::Decoder_polar_SC_fast_sys_fb_2048_614_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K819_SNR25
	if (implem == "N2048_K819_SNR25"      ) return module::Decoder_polar_SC_fast_sys_fb_2048_819_25;
#endif
// #ifdef ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25
// 	if (implem == "N2048_K1024_SNR25"     ) return module::Decoder_polar_SC_fast_sys_fb_2048_1024_25;
// #endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1229_SNR25
	if (implem == "N2048_K1229_SNR25"     ) return module::Decoder_polar_SC_fast_sys_fb_2048_1229_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1434_SNR25
	if (implem == "N2048_K1434_SNR25"     ) return module::Decoder_polar_SC_fast_sys_fb_2048_1434_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1638_SNR25
	if (implem == "N2048_K1638_SNR25"     ) return module::Decoder_polar_SC_fast_sys_fb_2048_1638_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N2048_K1843_SNR25
	if (implem == "N2048_K1843_SNR25"     ) return module::Decoder_polar_SC_fast_sys_fb_2048_1843_25;
#endif

	// RATE 1/10, 2/10, ..., 9/10 => N = 32768
#ifdef ENABLE_DECODER_SC_FAST_N32768_K3277_SNR25
	if (implem == "N32768_K3277_SNR25"    ) return module::Decoder_polar_SC_fast_sys_fb_32768_3277_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K6554_SNR25
	if (implem == "N32768_K6554_SNR25"    ) return module::Decoder_polar_SC_fast_sys_fb_32768_6554_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K9830_SNR25
	if (implem == "N32768_K9830_SNR25"    ) return module::Decoder_polar_SC_fast_sys_fb_32768_9830_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K13107_SNR25
	if (implem == "N32768_K13107_SNR25"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_13107_25;
#endif
// #ifdef ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25
// 	if (implem == "N32768_K16384_SNR25"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_16384_25;
// #endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K19661_SNR25
	if (implem == "N32768_K19661_SNR25"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_19661_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K22938_SNR25
	if (implem == "N32768_K22938_SNR25"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_22938_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K26214_SNR25
	if (implem == "N32768_K26214_SNR25"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_26214_25;
#endif
#ifdef ENABLE_DECODER_SC_FAST_N32768_K29491_SNR25
	if (implem == "N32768_K29491_SNR25"   ) return module::Decoder_polar_SC_fast_sys_fb_32768_29491_25;
#endif

	// RATE 1/2
#ifdef ENABLE_DECODER_SCL_FAST_CA_N4_K2_SNR25
	if (implem == "CA_N4_K2_SNR25"        ) return module::Decoder_polar_SCL_fast_CA_sys_fb_4_2_25;
#endif

	// RATE ??
#ifdef ENABLE_DECODER_SCL_FAST_CA_N2048_K1755_SNR35
	if (implem == "CA_N2048_K1755_SNR35"  ) return module::Decoder_polar_SCL_fast_CA_sys_fb_2048_1755_35;
#endif
#ifdef ENABLE_DECODER_SCL_FAST_CA_N256_K64_SNR30
	if (implem == "CA_N256_K64_SNR30"     ) return module::Decoder_polar_SCL_fast_CA_sys_fb_256_64_30;
#endif

	std::stringstream message;
	message << "'implem' frozen bits does not exist ('implem' = " << implem << ").";
	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar::build_gen<B_8 ,Q_8 >(const module::CRC<B_8 >*, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_polar::build_gen<B_16,Q_16>(const module::CRC<B_16>*, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_polar::build_gen<B_32,Q_32>(const module::CRC<B_32>*, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_polar::build_gen<B_64,Q_64>(const module::CRC<B_64>*, module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_polar::build_gen<B,Q>(const module::CRC<B>*, module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
