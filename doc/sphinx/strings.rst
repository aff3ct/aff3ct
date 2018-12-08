.. ------------------------------------------------ factory Launcher parameters

.. |factory::Launcher::parameters::p+cde-type,C| replace::
   select the code type you want to use.

.. |factory::Launcher::parameters::p+type| replace::
   select the type of simulation to launch (default is BFER).

.. |factory::Launcher::parameters::p+prec,p| replace::
   the simulation precision in bits.

.. |factory::Launcher::parameters::help,h| replace::
   print this help.

.. |factory::Launcher::parameters::Help,H| replace::
   print this help with the advanced arguments.

.. |factory::Launcher::parameters::version,v| replace::
   print informations about the version of the code.

.. |factory::Launcher::parameters::except-no-bt| replace::
   do not print the backtrace when displaying exception.

.. |factory::Launcher::parameters::except-a2l| replace::
   enhance the backtrace when displaying exception by changing program
   addresses into file names and lines (may take some seconds).

.. |factory::Launcher::parameters::p+no-legend| replace::
   do not display any legend when launching the simulation.

.. |factory::Launcher::parameters::p+no-colors| replace::
   disable the colors in the shell.

.. ---------------------------------------------- factory Simulation parameters

.. |factory::Simulation::parameters::p+meta| replace::
   print the output with metadata, takes the simulation title.

.. |factory::Simulation::parameters::p+stop-time| replace::
   time in sec after what the current simulated noise stops (0 is infinite).

.. |factory::Simulation::parameters::p+max-fra,n| replace::
   maximum number of frames to play after what the current simulated noise stops
   (0 is infinite).

.. |factory::Simulation::parameters::p+crit-nostop| replace::
   The stop criteria arguments -stop-time or -max-fra kill the current simulated
   noise point but not the simulation.

.. |factory::Simulation::parameters::p+dbg| replace::
   enable debug mode: print array values after each step.

.. |factory::Simulation::parameters::p+dbg-hex| replace::
   debug mode prints values in the hexadecimal format.

.. |factory::Simulation::parameters::p+dbg-prec| replace::
   set the precision of real elements when displayed in debug mode.

.. |factory::Simulation::parameters::p+dbg-limit,d| replace::
   set the max number of elements to display in the debug mode.

.. |factory::Simulation::parameters::p+dbg-fra| replace::
   set the max number of frames to display in the debug mode.

.. |factory::Simulation::parameters::p+stats| replace::
   display statistics module by module.

.. |factory::Simulation::parameters::p+threads,t| replace::
   enable multi-threaded mode and specify the number of threads (0 means the
   maximum supported by the core.

.. |factory::Simulation::parameters::p+seed,S| replace::
   seed used in the simulation to initialize the pseudo random generators in
   general.

.. |factory::Simulation::parameters::p+mpi-comm| replace::
   MPI communication frequency between the nodes (in millisec).

.. ---------------------------------------------------- factory BFER parameters

.. |factory::BFER::parameters::p+coset,c| replace::
   enable the coset approach.

.. |factory::BFER::parameters::p+err-trk| replace::
   enable the tracking of the bad frames (by default the frames are stored in
   the current folder).

.. |factory::BFER::parameters::p+err-trk-rev| replace::
   automatically replay the saved frames.

.. |factory::BFER::parameters::p+err-trk-path| replace::
   base path for the files where the bad frames will be stored or read.

.. |factory::BFER::parameters::p+err-trk-thold| replace::
   dump only frames with a bit error count above or equal to this threshold.

.. |factory::BFER::parameters::p+coded| replace::
   enable the coded monitoring (extends the monitored bits to the entire
   codeword).

.. |factory::BFER::parameters::p+mutinfo| replace::
   allow the computation of the mutual information.

.. ------------------------------------------------ factory BFER_ite parameters

.. |factory::BFER_ite::parameters::p+ite,I| replace::
   number of global iterations between the demodulator and the decoder.

.. |factory::BFER_ite::parameters::p+crc-start| replace::
   iteration number to start the CRC checking in the turbo demodulation process.

.. ------------------------------------------------ factory BFER_std parameters

.. ---------------------------------------------------- factory EXIT parameters

.. |factory::EXIT::parameters::p+siga-range| replace::
   sigma range used in EXIT charts (Matlab style: "0.5:2.5,2.55,2.6:0.05:3"
   with a default step of 0.1).

.. |factory::EXIT::parameters::p+siga-min,a| replace::
   sigma min value used in EXIT charts.

.. |factory::EXIT::parameters::p+siga-max,A| replace::
   sigma max value used in EXIT charts.

.. |factory::EXIT::parameters::p+siga-step| replace::
   sigma step value used in EXIT charts.

.. ------------------------------------------------- factory Channel parameters

.. |factory::Channel::parameters::p+fra-size,N| replace::
   number of symbols by frame.

.. |factory::Channel::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Channel::parameters::p+type| replace::
   type of the channel to use in the simulation ('USER' has an output got from a
   file when 'USER_ADD' has an additive noise got from a file, 'USER_BEC' and
   'USER_BSC' have their event draws from the file).

.. |factory::Channel::parameters::p+implem| replace::
   select the implementation of the algorithm to generate noise.

.. |factory::Channel::parameters::p+path| replace::
   path to a noisy file, to use with "--chn-type USER or USER_ADD" or to a
   gain file (used with "--chn-type RAYLEIGH_USER").

.. |factory::Channel::parameters::p+blk-fad| replace::
   block fading policy for the RAYLEIGH channel.

.. |factory::Channel::parameters::p+noise| replace::
   noise value (for SIGMA, ROP or EP noise type).

.. |factory::Channel::parameters::p+seed,S| replace::
   seed used to initialize the pseudo random generators.

.. |factory::Channel::parameters::p+add-users| replace::
   add all the users (= frames) before generating the noise.

.. |factory::Channel::parameters::p+complex| replace::
   enable complex noise generation.

.. |factory::Channel::parameters::p+gain-occur| replace::
   the number of times a gain is used on consecutive symbols (used with
   "--chn-type RAYLEIGH_USER").

.. --------------------------------------------------- factory Codec parameters

.. ----------------------------------------------- factory Codec_BCH parameters

.. ---------------------------------------------- factory Codec_LDPC parameters

.. --------------------------------------------- factory Codec_polar parameters

.. ------------------------------------------------ factory Codec_RA parameters

.. ---------------------------------------- factory Codec_repetition parameters

.. ------------------------------------------------ factory Codec_RS parameters

.. ----------------------------------------------- factory Codec_RSC parameters

.. -------------------------------------------- factory Codec_RSC_DB parameters

.. --------------------------------------------- factory Codec_turbo parameters

.. ------------------------------------------ factory Codec_turbo_DB parameters

.. ------------------------------------- factory Codec_turbo_product parameters

.. ------------------------------------------- factory Codec_uncoded parameters

.. --------------------------------------------------- factory Coset parameters

.. |factory::Coset::parameters::p+size,N| replace::
   coset size.

.. |factory::Coset::parameters::p+type| replace::
   coset type.

.. |factory::Coset::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. ----------------------------------------------------- factory CRC parameters

.. |factory::CRC::parameters::p+info-bits,K| replace::
   number of generated bits (information bits, the CRC is not included).

.. |factory::CRC::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::CRC::parameters::p+type,p+poly| replace::
   select the CRC type/polynomial you want to use (ex: "8-DVB-S2": 0xD5,
   "16-IBM": 0x8005, "24-LTEA": 0x864CFB, "32-GZIP": 0x04C11DB7).

.. |factory::CRC::parameters::p+implem| replace::
   select the CRC implementation you want to use.

.. |factory::CRC::parameters::p+size| replace::
   size of the CRC (divisor size in bit -1), required if you selected an unknown
   CRC.

.. ------------------------------------------------- factory Decoder parameters

.. |factory::Decoder::parameters::p+cw-size,N| replace::
   the codeword size.

.. |factory::Decoder::parameters::p+info-bits,K| replace::
   useful number of bit transmitted (information bits).

.. |factory::Decoder::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Decoder::parameters::p+type,D| replace::
   select the algorithm you want to decode the codeword.

.. |factory::Decoder::parameters::p+implem| replace::
   select the implementation of the algorithm to decode.

.. |factory::Decoder::parameters::p+hamming| replace::
   enable the computation of the Hamming distance instead of the Euclidean
   distance in the ML/CHASE decoders.

.. |factory::Decoder::parameters::p+flips| replace::
   set the maximum number of flips in the CHASE decoder.

.. --------------------------------------------- factory Decoder_BCH parameters

.. |factory::Decoder_BCH::parameters::p+corr-pow,T| replace::
   correction power of the BCH code.

.. -------------------------------------------- factory Decoder_LDPC parameters

.. |factory::Decoder_LDPC::parameters::p+h-path| replace::
   path to the H matrix (AList or QC formated file).

.. |factory::Decoder_LDPC::parameters::p+ite,i| replace::
   maximal number of iterations in the LDPC decoder.

.. |factory::Decoder_LDPC::parameters::p+off| replace::
   offset used in the offset min-sum BP algorithm (works only with
   "--dec-implem NMS").

.. |factory::Decoder_LDPC::parameters::p+mwbf| replace::
   factor used in the modified WBF algorithm (works only with
   "--dec-implem WBF"). Set 0 for basic WBF.

.. |factory::Decoder_LDPC::parameters::p+norm| replace::
   normalization factor used in the normalized min-sum BP algorithm (works only
   with "--dec-implem NMS").

.. |factory::Decoder_LDPC::parameters::p+no-synd| replace::
   disable the syndrome detection (disable the stop criterion in the LDPC
   decoders).

.. |factory::Decoder_LDPC::parameters::p+synd-depth| replace::
   successive number of iterations to validate the syndrome detection.

.. |factory::Decoder_LDPC::parameters::p+simd| replace::
   the SIMD strategy you want to use.

.. |factory::Decoder_LDPC::parameters::p+min| replace::
   the MIN implementation for the nodes (AMS decoder).

.. |factory::Decoder_LDPC::parameters::p+h-reorder| replace::
   specify if the check nodes (CNs) from H have to be reordered, 'NONE': do
   nothing (default), 'ASC': from the smallest to the biggest CNs, 'DSC': from
   the biggest to the smallest CNs.

.. ---------------------------------------------- factory Decoder_NO parameters

.. ------------------------------------------- factory Decoder_polar parameters

.. |factory::Decoder_polar::parameters::p+ite,i| replace::
   maximal number of iterations in the SCAN decoder.

.. |factory::Decoder_polar::parameters::p+lists,L| replace::
   maximal number of paths in the SCL decoder.

.. |factory::Decoder_polar::parameters::p+simd| replace::
   the SIMD strategy you want to use.

.. |factory::Decoder_polar::parameters::p+polar-nodes| replace::
   the type of nodes you want to detect in the Polar tree (ex:
   "{R0,R1,R0L,REP_2-8,REPL,SPC_4+}").

.. |factory::Decoder_polar::parameters::p+partial-adaptive| replace::
   enable the partial adaptive mode for the ASCL decoder (by default full
   adaptive is selected).

.. |factory::Decoder_polar::parameters::p+no-sys| replace::
   does not suppose a systematic encoding.

.. ---------------------------------------------- factory Decoder_RA parameters

.. |factory::Decoder_RA::parameters::p+ite,i| replace::
   maximal number of iterations in the decoder.

.. -------------------------------------- factory Decoder_repetition parameters

.. |factory::Decoder_repetition::parameters::p+no-buff| replace::
   does not suppose a buffered encoding.

.. ---------------------------------------------- factory Decoder_RS parameters

.. |factory::Decoder_RS::parameters::p+corr-pow,T| replace::
   correction power of the RS code.

.. --------------------------------------------- factory Decoder_RSC parameters

.. |factory::Decoder_RSC::parameters::p+simd| replace::
   the SIMD strategy you want to use.

.. |factory::Decoder_RSC::parameters::p+max| replace::
   the MAX implementation for the nodes.

.. |factory::Decoder_RSC::parameters::p+no-buff| replace::
   does not suppose a buffered encoding.

.. |factory::Decoder_RSC::parameters::p+poly| replace::
   the polynomials describing RSC code, should be of the form "{A,B}".

.. |factory::Decoder_RSC::parameters::p+std| replace::
   select a standard and set automatically some parameters (overwritten with
   user given arguments).

.. ------------------------------------------ factory Decoder_RSC_DB parameters

.. |factory::Decoder_RSC_DB::parameters::p+max| replace::
   the MAX implementation for the nodes.

.. |factory::Decoder_RSC_DB::parameters::p+no-buff| replace::
   does not suppose a buffered encoding.

.. ------------------------------------------- factory Decoder_turbo parameters

.. |factory::Decoder_turbo::parameters::p+ite,i| replace::
   maximal number of iterations in the turbo.

.. |factory::Decoder_turbo::parameters::p+sc| replace::
   enables the self corrected decoder (requires "--crc-type").

.. |factory::Decoder_turbo::parameters::p+json| replace::
   enable the json output trace.

.. |factory::Decoder_turbo::parameters::p+crc-start| replace::
   set the iteration to start the CRC checking.

.. ---------------------------------------- factory Decoder_turbo_DB parameters

.. |factory::Decoder_turbo_DB::parameters::p+ite,i| replace::
   maximal number of iterations in the turbo.

.. |factory::Decoder_turbo_DB::parameters::p+crc-start| replace::
   set the iteration to start the CRC checking.

.. ----------------------------------- factory Decoder_turbo_product parameters

.. |factory::Decoder_turbo_product::parameters::p+ite,i| replace::
   maximal number of iterations in the turbo.

.. |factory::Decoder_turbo_product::parameters::p+alpha| replace::
   weighting factor, one by half iteration (so twice more than number of
   iterations). If not enough given values, then automatically extends the last
   to all iterations.

.. |factory::Decoder_turbo_product::parameters::p+beta| replace::
   reliability factor, one by half iteration (so twice more than number of
   iterations). If not enough given values, then automatically extends the last
   to all iterations. If not given, then computes beta dynamically from the
   least reliable position metrics.

.. |factory::Decoder_turbo_product::parameters::p+p| replace::
   number of least reliable positions.

.. |factory::Decoder_turbo_product::parameters::p+t| replace::
   number of test vectors (0 means equal to 2^p).

.. |factory::Decoder_turbo_product::parameters::p+c| replace::
   number of competitors (0 means equal to number of test vectors, 1 means only
   the decided word).

.. |factory::Decoder_turbo_product::parameters::p+ext| replace::
   extends code with a parity bits.

.. |factory::Decoder_turbo_product::parameters::p+cp-coef| replace::
   the 5 Chase Pyndiah constant coefficients "a,b,c,d,e".

.. ------------------------------------------------- factory Encoder parameters

.. |factory::Encoder::parameters::p+info-bits,K| replace::
   useful number of bit transmitted (information bits).

.. |factory::Encoder::parameters::p+cw-size,N| replace::
   the codeword size.

.. |factory::Encoder::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Encoder::parameters::p+type| replace::
   type of the encoder to use in the simulation.

.. |factory::Encoder::parameters::p+path| replace::
   path to a file containing one or a set of pre-computed codewords, to use with
   "--enc-type USER".

.. |factory::Encoder::parameters::p+start-idx| replace::
   start idx to use in the USER type encoder.

.. |factory::Encoder::parameters::p+seed,S| replace::
   seed used to initialize the pseudo random generators.

.. --------------------------------------------- factory Encoder_BCH parameters

.. -------------------------------------------- factory Encoder_LDPC parameters

.. |factory::Encoder_LDPC::parameters::p+h-path| replace::
   path to the H matrix (AList formated file, required by the "LDPC_H" encoder).

.. |factory::Encoder_LDPC::parameters::p+g-path| replace::
   path to the G matrix (AList formated file, required by the "LDPC" encoder).

.. |factory::Encoder_LDPC::parameters::p+h-reorder| replace::
   specify if the check nodes (CNs) from H have to be reordered, 'NONE': do
   nothing (default), 'ASC': from the smallest to the biggest CNs, 'DSC': from
   the biggest to the smallest CNs.

.. |factory::Encoder_LDPC::parameters::p+g-method| replace::
   The method used to generate G from H when using 'LDPC_H' encoder type.
   'LU_DEC' method generates a hollow G thanks to the LU decomposition with a
   guarantee to have the systematic identity (do not work with irregular
   matrices) when the 'IDENTITY' method generate an identity on H to get the
   parity part.

.. |factory::Encoder_LDPC::parameters::p+g-save-path| replace::
   path where the generated G matrix with the 'LDPC_H' encoder type will be
   saved.

.. ---------------------------------------------- factory Encoder_NO parameters

.. |factory::Encoder_NO::parameters::p+info-bits,K| replace::
   useful number of bit transmitted (information bits).

.. |factory::Encoder_NO::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Encoder_NO::parameters::p+type| replace::
   type of the encoder to use in the simulation.

.. ------------------------------------------- factory Encoder_polar parameters

.. |factory::Encoder_polar::parameters::p+no-sys| replace::
   disable the systematic encoding.

.. ---------------------------------------------- factory Encoder_RA parameters

.. -------------------------------------- factory Encoder_repetition parameters

.. |factory::Encoder_repetition::parameters::p+no-buff| replace::
   disable the buffered encoding.

.. ---------------------------------------------- factory Encoder_RS parameters

.. --------------------------------------------- factory Encoder_RSC parameters

.. |factory::Encoder_RSC::parameters::p+no-buff| replace::
   disable the buffered encoding.

.. |factory::Encoder_RSC::parameters::p+poly| replace::
   the polynomials describing RSC code, should be of the form "{A,B}".

.. |factory::Encoder_RSC::parameters::p+std| replace::
   select a standard and set automatically some parameters (overwritten with
   user given arguments).

.. ------------------------------------------ factory Encoder_RSC_DB parameters

.. |factory::Encoder_RSC_DB::parameters::p+std| replace::
   select a standard and set automatically some parameters (overwritten with
   user given arguments).

.. |factory::Encoder_RSC_DB::parameters::p+no-buff| replace::
   disable the buffered encoding.

.. ------------------------------------------- factory Encoder_turbo parameters

.. |factory::Encoder_turbo::parameters::p+json-path| replace::
   path to store the encoder and decoder traces formated in JSON.

.. ---------------------------------------- factory Encoder_turbo_DB parameters

.. ----------------------------------- factory Encoder_turbo_product parameters

.. |factory::Encoder_turbo_product::parameters::p+ext| replace::
   extends code with a parity bits.

.. --------------------------------------------- factory Interleaver parameters

.. --------------------------------------------------- factory Modem parameters

.. |factory::Modem::parameters::p+fra-size,N| replace::
   number of symbols by frame.

.. |factory::Modem::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Modem::parameters::p+type| replace::
   type of the modulation to use in the simulation.

.. |factory::Modem::parameters::p+implem| replace::
   select the implementation of the modem.

.. |factory::Modem::parameters::p+bps| replace::
   select the number of bits per symbol (default is 1).

.. |factory::Modem::parameters::p+const-path| replace::
   path to the ordered modulation symbols (constellation), to use with
   "--mod-type USER".

.. |factory::Modem::parameters::p+cpm-std| replace::
   the selection of a default CPM standard hardly implemented (any of those
   parameters is overwritten if the argument is given by the user).

.. |factory::Modem::parameters::p+cpm-L| replace::
   CPM pulse width or CPM memory (default is 2).

.. |factory::Modem::parameters::p+cpm-k| replace::
   modulation index numerator (default is 1).

.. |factory::Modem::parameters::p+cpm-p| replace::
   modulation index denominator (default is 2).

.. |factory::Modem::parameters::p+cpm-upf| replace::
   select the symbol sampling factor (default is 1).

.. |factory::Modem::parameters::p+cpm-map| replace::
   symbols mapping layout (default is NATURAL).

.. |factory::Modem::parameters::p+cpm-ws| replace::
   wave shape (default is GMSK).

.. |factory::Modem::parameters::p+max| replace::
   select the type of the max operation to use in the demodulator.

.. |factory::Modem::parameters::p+noise| replace::
   noise variance value for the demodulator.

.. |factory::Modem::parameters::p+no-sig2| replace::
   turn off the division by sigma square in the demodulator.

.. |factory::Modem::parameters::p+psi| replace::
   select the type of the psi function to use in the SCMA demodulator.

.. |factory::Modem::parameters::p+ite| replace::
   number of iteration in the demodulator.

.. |factory::Modem::parameters::p+rop-est| replace::
   set the number of known bits for the ROP estimation in the OOK demodulator on
   an optical channel (when 0, the ROP is known).

.. ------------------------------------------------- factory Monitor parameters

.. -------------------------------------------- factory Monitor_BFER parameters

.. |factory::Monitor_BFER::parameters::p+info-bits,K| replace::
   number of bits to check.

.. |factory::Monitor_BFER::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Monitor_BFER::parameters::p+max-fe,e| replace::
   max number of frame errors for each noise point simulation.

.. |factory::Monitor_BFER::parameters::p+max-fra,n| replace::
   maximum number of frames for each noise point simulation.

.. |factory::Monitor_BFER::parameters::p+err-hist| replace::
   activate the histogram of the number of errors per frame. Set the max number
   of bit error per frame included in the histogram (0 is no limit).

.. |factory::Monitor_BFER::parameters::p+err-hist-path| replace::
   path to the output histogram (default is './hist', add automatically the
   current noise value and the extension '.txt')

.. -------------------------------------------- factory Monitor_EXIT parameters

.. |factory::Monitor_EXIT::parameters::p+size,K| replace::
   number of bits to check.

.. |factory::Monitor_EXIT::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Monitor_EXIT::parameters::p+trials,n| replace::
   number of frames to simulate per sigma A value.

.. ---------------------------------------------- factory Monitor_MI parameters

.. |factory::Monitor_MI::parameters::p+fra-size,N| replace::
   the frame size for the mutual information computation.

.. |factory::Monitor_MI::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Monitor_MI::parameters::p+trials,n| replace::
   number of frames to simulate.

.. ----------------------------------------------- factory Puncturer parameters

.. |factory::Puncturer::parameters::p+info-bits,K| replace::
   useful number of bit transmitted (information bits).

.. |factory::Puncturer::parameters::p+fra-size,N| replace::
   useful number of bit transmitted (information bits).

.. |factory::Puncturer::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Puncturer::parameters::p+type| replace::
   code puncturer type.

.. ------------------------------------------ factory Puncturer_LDPC parameters

.. |factory::Puncturer_LDPC::parameters::p+cw-size,N_cw| replace::
   the codeword size.

.. |factory::Puncturer_LDPC::parameters::p+pattern| replace::
   puncturing pattern for the LDPC encoder/decoder (size = N_Code/Z)
   (ex: "1,1,1,0").

.. ----------------------------------------- factory Puncturer_polar parameters

.. ----------------------------------------- factory Puncturer_turbo parameters

.. |factory::Puncturer_turbo::parameters::p+pattern| replace::
   puncturing pattern for the turbo encoder (ex: "11,10,01").

.. |factory::Puncturer_turbo::parameters::p+tail-length| replace::
   total number of tail bits at the end of the frame.

.. |factory::Puncturer_turbo::parameters::p+no-buff| replace::
   does not suppose a buffered encoding.

.. -------------------------------------- factory Puncturer_turbo_DB parameters

.. ----------------------------------------------- factory Quantizer parameters

.. |factory::Quantizer::parameters::p+size,N| replace::
   number of real to quantize.

.. |factory::Quantizer::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Quantizer::parameters::p+type| replace::
   type of the quantizer to use in the simulation.

.. |factory::Quantizer::parameters::p+implem| replace::
   select the implementation of quantizer.

.. |factory::Quantizer::parameters::p+dec| replace::
   the position of the fixed point in the quantified representation.

.. |factory::Quantizer::parameters::p+bits| replace::
   the number of bits used for the quantizer.

.. |factory::Quantizer::parameters::p+range| replace::
   the min/max bound for the tricky quantizer.

.. -------------------------------------------------- factory Source parameters

.. |factory::Source::parameters::p+info-bits,K| replace::
   number of generated bits (information bits).

.. |factory::Source::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Source::parameters::p+type| replace::
   method used to generate the codewords.

.. |factory::Source::parameters::p+implem| replace::
   select the implementation of the algorithm to generate the information bits.

.. |factory::Source::parameters::p+path| replace::
   path to a file containing one or a set of pre-computed source bits, to use
   with "--src-type USER".

.. |factory::Source::parameters::p+start-idx| replace::
   start idx to use in the USER type source.

.. |factory::Source::parameters::p+seed,S| replace::
   seed used to initialize the pseudo random generators.

.. ------------------------------------ factory Frozenbits_generator parameters

.. |factory::Frozenbits_generator::parameters::p+info-bits,K| replace::
   useful number of bit transmitted (information bits).

.. |factory::Frozenbits_generator::parameters::p+cw-size,N| replace::
   the codeword size.

.. |factory::Frozenbits_generator::parameters::p+sigma| replace::
   sigma value for the polar codes generation (adaptive frozen bits if sigma is
   not set).

.. |factory::Frozenbits_generator::parameters::p+gen-method| replace::
   select the frozen bits generation method.

.. |factory::Frozenbits_generator::parameters::p+awgn-path| replace::
   path to a file or a directory containing the best channels to use for
   information bits.

.. |factory::Frozenbits_generator::parameters::p+pb-path| replace::
   path of the polar bounds code generator (generates best channels to use).

.. ------------------------------------------ factory Flip_and_check parameters

.. |factory::Flip_and_check::parameters::p+size| replace::
   size (in bit) of the extrinsic for the fnc processing.

.. |factory::Flip_and_check::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Flip_and_check::parameters::p+| replace::
   enables the flip and check decoder (requires "--crc-type").

.. |factory::Flip_and_check::parameters::p+q| replace::
   set the search's space for the fnc algorithm.

.. |factory::Flip_and_check::parameters::p+ite-m| replace::
   set first iteration at which the fnc is used.

.. |factory::Flip_and_check::parameters::p+ite-M| replace::
   set last iteration at which the fnc is used.

.. |factory::Flip_and_check::parameters::p+ite-s| replace::
   set iteration step for the fnc algorithm.

.. |factory::Flip_and_check::parameters::p+ite,i| replace::
   maximal number of iterations in the turbo.

.. |factory::Flip_and_check::parameters::p+crc-start| replace::
   set the iteration to start the CRC checking.

.. --------------------------------------- factory Flip_and_check_DB parameters

.. ------------------------------------------ factory Scaling_factor parameters

.. |factory::Scaling_factor::parameters::p+type| replace::
   scaling factor type.

.. |factory::Scaling_factor::parameters::p+ite| replace::
   number of iterations.

.. ------------------------------------------------ factory Terminal parameters

.. |factory::Terminal::parameters::p+type| replace::
   type of the terminal to use to display results.

.. |factory::Terminal::parameters::p+no| replace::
   disable reporting for each iteration.

.. |factory::Terminal::parameters::p+freq| replace::
   display frequency in ms (refresh time step for each iteration, 0 = disable
   display refresh).

.. ---------------------------------------- factory Interleaver_core parameters

.. |factory::Interleaver_core::parameters::p+size| replace::
   number of symbols to interleave.

.. |factory::Interleaver_core::parameters::p+fra,F| replace::
   set the number of inter frame level to process.

.. |factory::Interleaver_core::parameters::p+type| replace::
   type of the interleaver to use in the simulation.

.. |factory::Interleaver_core::parameters::p+path| replace::
   specify the path to the interleaver file (to use with "--itl-type USER").

.. |factory::Interleaver_core::parameters::p+cols| replace::
   specify the number of columns used for the RAND_COL, ROW_COL or COL_ROW
   interleaver.

.. |factory::Interleaver_core::parameters::p+uni| replace::
   enable the regeneration of the interleaver at each new frame.

.. |factory::Interleaver_core::parameters::p+seed,S| replace::
   seed used to initialize the pseudo random generators.

.. |factory::Interleaver_core::parameters::p+read-order| replace::
   read order of the COL_ROW and ROW_COL interleavers.

.. --------------------------------------------------- factory Noise parameters

.. |factory::Noise::parameters::p+noise-range,R| replace::
   noise energy range to run (Matlab style: "0.5:2.5,2.55,2.6:0.05:3" with a
   default step of 0.1).

.. |factory::Noise::parameters::p+noise-min,m| replace::
   minimal noise energy to simulate.

.. |factory::Noise::parameters::p+noise-max,M| replace::
   maximal noise energy to simulate.

.. |factory::Noise::parameters::p+noise-step,s| replace::
   noise energy step between each simulation iteration.

.. |factory::Noise::parameters::p+pdf-path| replace::
   A file that contains PDF for different SNR. Set the SNR range from the given
   ones. Overwritten by -R or limited by -m and -M with a minimum step of -s.

.. |factory::Noise::parameters::p+noise-type,E| replace::
   select the type of NOISE: SNR per Symbol / SNR per information Bit /
   Received Optical Power / Erasure Probability.