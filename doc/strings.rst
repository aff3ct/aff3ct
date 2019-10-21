.. ------------------------------------------------ factory Launcher parameters

.. |factory::Launcher::parameters::p+cde-type,C| replace::
   Select the channel code family to simulate.

.. |factory::Launcher::parameters::p+type| replace::
   Select the type of simulation (or communication chain skeleton).

.. |factory::Launcher::parameters::p+prec,p| replace::
   Specify the representation of the real numbers in the receiver part of the
   chain.

.. |factory::Launcher::parameters::help,h| replace::
   Print the help with all the required (denoted as ``{R}``) and optional
   arguments. The latter change depending on the selected simulation type and
   code.

.. |factory::Launcher::parameters::Help,H| replace::
   Print the help like with the ``--help, -h`` parameter plus advanced arguments
   (denoted as ``{A}``).

.. |factory::Launcher::parameters::version,v| replace::
   Print informations about the version of the source code and compilation
   options.

.. |factory::Launcher::parameters::except-no-bt| replace::
   Disable the backtrace display when running an exception.

.. |factory::Launcher::parameters::except-a2l| replace::
   Enhance the backtrace when displaying exception. This change the program
   addresses into filenames and lines. It may take some seconds to do this work.

.. |factory::Launcher::parameters::no-legend| replace::
   Disable the legend display (remove all the lines beginning by the ``#``
   character).

.. |factory::Launcher::parameters::full-legend| replace::
   Display the legend with all modules details when launching the simulation.

.. |factory::Launcher::parameters::no-colors| replace::
   Disable the colors in the shell.

.. |factory::Launcher::parameters::keys,k| replace::
   Display the parameter keys in the help.

.. ---------------------------------------------- factory Simulation parameters

.. |factory::Simulation::parameters::p+meta| replace::
   Add meta-data at the beginning of the |AFF3CT| standard output (INI format is
   used). The value of the parameter will be affected to the *title* meta-data
   and the *command line* will be added.

.. |factory::Simulation::parameters::p+stop-time| replace::
   Set the maximum time (in seconds) to simulate per noise point. When a noise
   point reaches the maximum time limit, the simulation is stopped. 0 value
   means no limit.

.. |factory::Simulation::parameters::p+max-fra,n| replace::
   Set the maximum number of frames to simulate per noise point. When a noise
   point reaches the maximum frame limit, the simulation is stopped. 0 value
   means no limit.

.. |factory::Simulation::parameters::p+crit-nostop| replace::
   Stop only the current noise point instead of the whole simulation.

.. |factory::Simulation::parameters::p+dbg| replace::
   Enable the debug mode. This print the input and the output frames after each
   task execution.

.. |factory::Simulation::parameters::p+dbg-hex| replace::
   Enable the debug mode and **print values in the hexadecimal format**. This
   mode is useful for having a fully accurate representation of floating
   numbers.

.. |factory::Simulation::parameters::p+dbg-prec| replace::
   Enable the debug mode and **set the decimal precision** (number of digits for
   the decimal part) of the floating-point elements.

.. |factory::Simulation::parameters::p+dbg-limit,d| replace::
   Enable the debug mode and **set the max number of elements** to display per
   frame. 0 value means there is no dump limit.

.. |factory::Simulation::parameters::p+dbg-fra| replace::
   Enable the debug mode and **set the max number of frames** to display. 0
   value means there is no frame limit. By default, a task works on one frame at
   a time.

.. |factory::Simulation::parameters::p+stats| replace::
   Display statistics for each task. Those statistics are shown after each
   simulated |SNR| point.

.. |factory::Simulation::parameters::p+threads,t| replace::
   Specify the number of threads used in the simulation. The 0 default value
   will automatically set the number of threads to the hardware number of
   threads available on the machine.

.. |factory::Simulation::parameters::p+seed,S| replace::
   Set the |PRNG| seed used in the Monte Carlo simulation.

.. ---------------------------------------------------- factory BFER parameters

.. |factory::BFER::parameters::p+coset,c| replace::
   Enable the *coset* approach.

.. |factory::BFER::parameters::p+err-trk| replace::
   Track the erroneous frames. When an error is found, the information bits from
   the source, the codeword from the encoder and the applied noise from the
   channel are dumped in several files.

.. |factory::BFER::parameters::p+err-trk-rev| replace::
   Replay dumped frames. By default this option reverts the ``--sim-err-trk``
   parameter by replaying the erroneous frames that have been dumped.

.. |factory::BFER::parameters::p+err-trk-path| replace::
   Specify the base path for the ``--sim-err-trk`` and ``--sim-err-trk-rev``
   parameters.

.. |factory::BFER::parameters::p+err-trk-thold| replace::
   Specify a threshold value in number of erroneous bits before which a frame is
   dumped.

.. |factory::BFER::parameters::p+coded| replace::
   Enable the coded monitoring.

.. |factory::BFER::parameters::p+sigma| replace::
   Show the standard deviation (:math:`\sigma`) of the Gaussian/Normal
   distribution in the terminal.

.. |factory::BFER::parameters::p+mutinfo| replace::
   Enable the computation of the mutual information (|MI|).

.. |factory::BFER::parameters::p+red-lazy| replace::
   Enable the lazy synchronization between the various monitor threads.

.. |factory::BFER::parameters::p+red-lazy-freq| replace::
   Set the time interval (in milliseconds) between the synchronizations of the
   monitor threads.

.. |factory::BFER::parameters::p+mpi-comm-freq| replace::
   Set the time interval (in milliseconds) between the |MPI| communications.
   Increase this interval will reduce the |MPI| communications overhead.

.. ------------------------------------------------ factory BFER_ite parameters

.. |factory::BFER_ite::parameters::p+ite,I| replace::
   Set the number of global iterations between the demodulator and the decoder.

.. |factory::BFER_ite::parameters::p+crc-start| replace::
   Set the number of simulation iterations to proceed before starting the |CRC|
   checking in the turbo demodulation process. It reduces the number of false
   positive |CRC| detections.

.. ------------------------------------------------ factory BFER_std parameters

.. ---------------------------------------------------- factory EXIT parameters

.. |factory::EXIT::parameters::p+siga-range| replace::
   Set the sigma range used in |EXIT| charts (|MATLAB| style:
   "0.5:2.5,2.55,2.6:0.05:3" with a default step of 0.1).

.. |factory::EXIT::parameters::p+siga-min,a| replace::
   Set the sigma minimum value used in |EXIT| charts.

.. |factory::EXIT::parameters::p+siga-max,A| replace::
   Set the sigma maximum value used in |EXIT| charts.

.. |factory::EXIT::parameters::p+siga-step| replace::
   Set the sigma step value used in |EXIT| charts.

.. ------------------------------------------------- factory Channel parameters

.. |factory::Channel::parameters::p+fra-size,N| replace::
   Set the number of symbols by frame.

.. |factory::Channel::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Channel::parameters::p+type| replace::
   Select the channel type.

.. |factory::Channel::parameters::p+implem| replace::
   Select the implementation of the algorithm to generate the noise.

.. |factory::Channel::parameters::p+path| replace::
   Give the path to a file containing the noise.

.. |factory::Channel::parameters::p+blk-fad| replace::
   Set the block fading policy for the Rayleigh channel.

.. |factory::Channel::parameters::p+noise| replace::
   Set the noise value (for ``SIGMA``, ``ROP`` or ``EP`` noise type).

.. |factory::Channel::parameters::p+seed,S| replace::
   Set the seed used to initialize the |PRNG|.

.. |factory::Channel::parameters::p+add-users| replace::
   Add all the users (= frames) before generating the noise.

.. |factory::Channel::parameters::p+complex| replace::
   Enable complex noise generation.

.. |factory::Channel::parameters::p+gain-occur| replace::
   Give the number of times a gain is used on consecutive symbols. It is used in
   the ``RAYLEIGH_USER`` channel while applying gains read from the given file.

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
   Set the frame size.

.. |factory::Coset::parameters::p+type| replace::
   Set the coset type.

.. |factory::Coset::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. ----------------------------------------------------- factory CRC parameters

.. |factory::CRC::parameters::p+info-bits,K| replace::
   Set the number of generated bits (information bits :math:`K`, the CRC is not
   included).

.. |factory::CRC::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::CRC::parameters::p+type,p+poly| replace::
   Select the |CRC| type you want to use among the predefined (or not)
   polynomials.

.. |factory::CRC::parameters::p+implem| replace::
   Select the |CRC| implementation you want to use.

.. |factory::CRC::parameters::p+size| replace::
   Size the |CRC| (divisor size in bits minus one), required if you selected an
   unknown |CRC|.

.. ------------------------------------------------- factory Decoder parameters

.. |factory::Decoder::parameters::p+cw-size,N| replace::
   Set the codeword size :math:`N`.

.. |factory::Decoder::parameters::p+info-bits,K| replace::
   Set the number of information bits :math:`K`.

.. |factory::Decoder::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Decoder::parameters::p+type,D| replace::
   Select the decoder algorithm.

.. |factory::Decoder::parameters::p+implem| replace::
   Select the implementation of the decoder algorithm.

.. |factory::Decoder::parameters::p+hamming| replace::
   Compute the `Hamming distance`_ instead of the `Euclidean distance`_ in the
   |ML| and Chase decoders.

.. |factory::Decoder::parameters::p+flips| replace::
   Set the maximum number of bit flips in the decoding algorithm.

.. |factory::Decoder::parameters::p+seed| replace::
   Specify the decoder |PRNG| seed (if the decoder uses one).

.. --------------------------------------------- factory Decoder_BCH parameters

.. |factory::Decoder_BCH::parameters::p+corr-pow,T| replace::
   Set the correction power of the |BCH| decoder. This value corresponds to the
   number of errors that the decoder is able to correct.

.. -------------------------------------------- factory Decoder_LDPC parameters

.. |factory::Decoder_LDPC::parameters::p+h-path| replace::
   Give the path to the :math:`H` parity matrix. Support the AList and the |QC|
   formats.

.. |factory::Decoder_LDPC::parameters::p+ite,i| replace::
   Set the maximal number of iterations in the |LDPC| decoder.

.. |factory::Decoder_LDPC::parameters::p+off| replace::
   Set the offset used in the |OMS| update rule.

.. |factory::Decoder_LDPC::parameters::p+mwbf-factor| replace::
   Give the weighting factor used in the |MWBF| algorithm.

.. |factory::Decoder_LDPC::parameters::p+norm| replace::
   Set the normalization factor used in the |NMS| update rule.

.. |factory::Decoder_LDPC::parameters::p+no-synd| replace::
   Disable the syndrome detection, all the |LDPC| decoding iterations will be
   performed.

.. |factory::Decoder_LDPC::parameters::p+synd-depth| replace::
   Set the number of iterations to process before enabling the syndrome
   detection. In some cases, it can help to avoid false positive detections.

.. |factory::Decoder_LDPC::parameters::p+simd| replace::
   Select the |SIMD| strategy.

.. |factory::Decoder_LDPC::parameters::p+min| replace::
   Define the :math:`\min^*` operator approximation used in the |AMS| update
   rule.

.. |factory::Decoder_LDPC::parameters::p+h-reorder| replace::
   Specify the order of execution of the |CNs| in the decoding process depending
   on their degree.

.. |factory::Decoder_LDPC::parameters::p+ppbf-proba| replace::
   Give the probabilities of the Bernouilli distribution of the |PPBF|.
   The number of given values must be equal to the biggest variable node degree
   plus two.

.. ---------------------------------------------- factory Decoder_NO parameters

.. ------------------------------------------- factory Decoder_polar parameters

.. |factory::Decoder_polar::parameters::p+ite,i| replace::
   Set the number of decoding iterations in the |SCAN| decoder.

.. |factory::Decoder_polar::parameters::p+lists,L| replace::
   Set the number of lists to maintain in the |SCL| and |A-SCL| decoders.

.. |factory::Decoder_polar::parameters::p+simd| replace::
   Select the |SIMD| strategy.

.. |factory::Decoder_polar::parameters::p+polar-nodes| replace::
   Set the rules to enable in the tree simplifications process. This parameter
   is compatible with the |SC| ``FAST``, the |SCL| ``FAST``, |SCL|-MEM ``FAST``,
   the |A-SCL| ``FAST`` and the the |A-SCL|-MEM ``FAST`` decoders.

.. |factory::Decoder_polar::parameters::p+partial-adaptive| replace::
   Select the partial adaptive (|PA-SCL|) variant of the |A-SCL| decoder (by
   default the |FA-SCL| is selected).

.. |factory::Decoder_polar::parameters::p+no-sys| replace::
   Enable non-systematic encoding.

.. ---------------------------------------------- factory Decoder_RA parameters

.. |factory::Decoder_RA::parameters::p+ite,i| replace::
   Set the number of iterations to perform in the decoder.

.. -------------------------------------- factory Decoder_repetition parameters

.. |factory::Decoder_repetition::parameters::p+no-buff| replace::
   Do not suppose a buffered encoding.

.. ---------------------------------------------- factory Decoder_RS parameters

.. |factory::Decoder_RS::parameters::p+corr-pow,T| replace::
   Set the correction power of the |RS| decoder. This value corresponds to the
   number of symbols errors that the decoder is able to correct.

.. --------------------------------------------- factory Decoder_RSC parameters

.. |factory::Decoder_RSC::parameters::p+simd| replace::
   Select the |SIMD| strategy.

.. |factory::Decoder_RSC::parameters::p+max| replace::
   Select the approximation of the :math:`\max^*` operator used in the trellis
   decoding.

.. |factory::Decoder_RSC::parameters::p+no-buff| replace::
   Do not suppose a buffered encoding.

.. |factory::Decoder_RSC::parameters::p+poly| replace::
   Set the polynomials describing |RSC| code, should be of the form "{A,B}".

.. |factory::Decoder_RSC::parameters::p+std| replace::
   Select a standard.

.. ------------------------------------------ factory Decoder_RSC_DB parameters

.. |factory::Decoder_RSC_DB::parameters::p+max| replace::
   Select the approximation of the :math:`\max^*` operator used in the trellis
   decoding.

.. |factory::Decoder_RSC_DB::parameters::p+no-buff| replace::
   Do not suppose a buffered encoding.

.. ------------------------------------------- factory Decoder_turbo parameters

.. |factory::Decoder_turbo::parameters::p+ite,i| replace::
   Set the maximal number of iterations in the Turbo decoder.

.. |factory::Decoder_turbo::parameters::p+sc| replace::
   Enable the Self-Corrected (|SCo|) decoder.

.. |factory::Decoder_turbo::parameters::p+json| replace::
   Enable the |JSON| output trace.

.. |factory::Decoder_turbo::parameters::p+crc-start| replace::
   Set the first iteration to start the |CRC| checking.

.. ---------------------------------------- factory Decoder_turbo_DB parameters

.. |factory::Decoder_turbo_DB::parameters::p+ite,i| replace::
   Set the maximal number of iterations in the Turbo decoder.

.. |factory::Decoder_turbo_DB::parameters::p+crc-start| replace::
   Set the first iteration to start the |CRC| checking.

.. ----------------------------------- factory Decoder_turbo_product parameters

.. |factory::Decoder_turbo_product::parameters::p+ite,i| replace::
   Set the number of iterations in the turbo decoding process.

.. |factory::Decoder_turbo_product::parameters::p+alpha| replace::
   Give the *weighting factor* alpha, one by half iteration (so twice more than
   the number of iterations).

.. |factory::Decoder_turbo_product::parameters::p+beta| replace::
   Give the *reliability factor* beta, one by half iteration (so twice more than
   the number of iterations).

.. |factory::Decoder_turbo_product::parameters::p+p| replace::
   Set the number of *least reliable positions*.

.. |factory::Decoder_turbo_product::parameters::p+t| replace::
   Set the *number of test vectors*. A value of 0 means equal to :math:`2^p`
   where :math:`p` is the number of least reliable positions.

.. |factory::Decoder_turbo_product::parameters::p+c| replace::
   Set the *number of competitors*. A value of 0 means that the latter is set
   to the number of test vectors, 1 means only the decided word.

.. |factory::Decoder_turbo_product::parameters::p+ext| replace::
   Extend the code with parity bits.

.. |factory::Decoder_turbo_product::parameters::p+cp-coef| replace::
   Give the 5 ``CP`` constant coefficients :math:`a, b, c, d, e`.

.. ------------------------------------------------- factory Encoder parameters

.. |factory::Encoder::parameters::p+info-bits,K| replace::
   Set the number of information bits :math:`K`.

.. |factory::Encoder::parameters::p+cw-size,N| replace::
   Set the codeword size :math:`N`.

.. |factory::Encoder::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Encoder::parameters::p+type| replace::
   Select the encoder type.

.. |factory::Encoder::parameters::p+path| replace::
   Set the path to a file containing one or more codewords, to use with the
   ``USER`` encoder.

.. |factory::Encoder::parameters::p+start-idx| replace::
   Give the start index to use in the ``USER`` encoder. It is the index of the
   first codeword to read from the given file.

.. |factory::Encoder::parameters::p+seed,S| replace::
   Set the seed used to initialize the |PRNG|.

.. --------------------------------------------- factory Encoder_BCH parameters

.. -------------------------------------------- factory Encoder_LDPC parameters

.. |factory::Encoder_LDPC::parameters::p+h-path| replace::
   Set the path to the :math:`H` matrix (AList formated file, required by the
   ``LDPC_H`` encoder).

.. |factory::Encoder_LDPC::parameters::p+g-path| replace::
   Give the path to the :math:`G` generator matrix in an AList or |QC| formated
   file.

.. |factory::Encoder_LDPC::parameters::p+h-reorder| replace::
   Specify if the |CNs| from :math:`H` have to be reordered, ``NONE``: do
   nothing (default), ``ASC``: from the smallest to the biggest |CNs|, ``DSC``:
   from the biggest to the smallest |CNs|.

.. |factory::Encoder_LDPC::parameters::p+g-method| replace::
   Specify the method used to build the :math:`G` generator matrix from the
   :math:`H` parity matrix when using the ``LDPC_H`` encoder.

.. |factory::Encoder_LDPC::parameters::p+g-save-path| replace::
   Set the file path where the :math:`G` generator matrix will be saved (AList
   file format). To use with the ``LDPC_H`` encoder.

.. ---------------------------------------------- factory Encoder_NO parameters

.. |factory::Encoder_NO::parameters::p+info-bits,K| replace::
   Set the number of information bits :math:`K`.

.. |factory::Encoder_NO::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Encoder_NO::parameters::p+type| replace::
   Select the encoder type.

.. ------------------------------------------- factory Encoder_polar parameters

.. |factory::Encoder_polar::parameters::p+no-sys| replace::
   Enable non-systematic encoding. By default the encoding process is
   systematic.

.. ---------------------------------------------- factory Encoder_RA parameters

.. -------------------------------------- factory Encoder_repetition parameters

.. |factory::Encoder_repetition::parameters::p+no-buff| replace::
   Disable the buffered encoding.

.. ---------------------------------------------- factory Encoder_RS parameters

.. --------------------------------------------- factory Encoder_RSC parameters

.. |factory::Encoder_RSC::parameters::p+no-buff| replace::
   Disable the buffered encoding.

.. |factory::Encoder_RSC::parameters::p+poly| replace::
   Set the polynomials that define the |RSC| code (or the trellis structure).
   The expected form is :math:`\{A,B\}` where :math:`A` and :math:`B` are given
   in octal.

.. |factory::Encoder_RSC::parameters::p+std| replace::
   Select a standard: set automatically some parameters (can be overwritten by
   user given arguments).

.. ------------------------------------------ factory Encoder_RSC_DB parameters

.. |factory::Encoder_RSC_DB::parameters::p+std| replace::
   Select a standard.

.. |factory::Encoder_RSC_DB::parameters::p+no-buff| replace::
   Disable the buffered encoding.

.. ------------------------------------------- factory Encoder_turbo parameters

.. |factory::Encoder_turbo::parameters::p+json-path| replace::
   Select the file path to dump the encoder and decoder internal values (in
   |JSON| format).

.. ---------------------------------------- factory Encoder_turbo_DB parameters

.. ----------------------------------- factory Encoder_turbo_product parameters

.. |factory::Encoder_turbo_product::parameters::p+ext| replace::
   Extend the *sub-encoder* codeword with a parity bit in order to increase the
   distance of the code.

.. --------------------------------------------- factory Interleaver parameters

.. --------------------------------------------------- factory Modem parameters

.. |factory::Modem::parameters::p+fra-size,N| replace::
   Set the number of symbols by frame.

.. |factory::Modem::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Modem::parameters::p+type| replace::
   Select the modulation type.

.. |factory::Modem::parameters::p+implem| replace::
   Select the |modem| implementation.

.. |factory::Modem::parameters::p+bps| replace::
   Set the number of bits used to generate a symbol (|BPS|).

.. |factory::Modem::parameters::p+const-path| replace::
   Give the path to the ordered modulation symbols (constellation), to use with
   the ``USER`` |modem|.

.. |factory::Modem::parameters::p+codebook| replace::
   Give the path to the codebook, to use with the ``SCMA`` |modem|.

.. |factory::Modem::parameters::p+cpm-std| replace::
   Set the |CPM| parameters according to a standard.

.. |factory::Modem::parameters::p+cpm-L| replace::
   Set the |CPM| *pulse width* (also called *memory depth*).

.. |factory::Modem::parameters::p+cpm-k| replace::
   Set the |CPM| *index numerator*.

.. |factory::Modem::parameters::p+cpm-p| replace::
   Set the |CPM| *index denominator*.

.. |factory::Modem::parameters::p+cpm-upf| replace::
   Select the symbol upsampling factor in the |CPM|.

.. |factory::Modem::parameters::p+cpm-map| replace::
   Select the |CPM| *symbols mapping layout*.

.. |factory::Modem::parameters::p+cpm-ws| replace::
   Select the |CPM| *wave shape*.

.. |factory::Modem::parameters::p+max| replace::
   Select the approximation of the :math:`\max^*` operator used in the |PAM|,
   |QAM|, |PSK|, |CPM| and user demodulators.

.. |factory::Modem::parameters::p+noise| replace::
   Set the noise variance value for the demodulator.

.. |factory::Modem::parameters::p+no-sig2| replace::
   Turn off the division by :math:`\sigma^2` in the demodulator where
   :math:`\sigma` is the Gaussian noise variance.

.. |factory::Modem::parameters::p+psi| replace::
   Select the :math:`\psi` function used in the |SCMA| demodulator.

.. |factory::Modem::parameters::p+ite| replace::
   Set the number of iterations in the |SCMA| demodulator.

.. |factory::Modem::parameters::p+rop-est| replace::
   Set the number of known bits for the |ROP| estimation in the |OOK|
   demodulator on an optical channel.

.. ------------------------------------------------- factory Monitor parameters

.. -------------------------------------------- factory Monitor_BFER parameters

.. |factory::Monitor_BFER::parameters::p+info-bits,K| replace::
   Set the number of bits to check.

.. |factory::Monitor_BFER::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Monitor_BFER::parameters::p+max-fe,e| replace::
   Set the maximum number of frame errors to simulated for each noise point.

.. |factory::Monitor_BFER::parameters::p+max-fra,n| replace::
   Set the maximum number of frames to simulate for each noise point.

.. |factory::Monitor_BFER::parameters::p+err-hist| replace::
   Enable the construction of the errors per frame histogram. Set also the
   maximum number of bit errors per frame included in the histogram (0 means no
   limit).

.. |factory::Monitor_BFER::parameters::p+err-hist-path| replace::
   Path to the output histogram. When the files are dumped, the current noise
   value is added to this name with the ``.txt`` extension.

.. -------------------------------------------- factory Monitor_EXIT parameters

.. |factory::Monitor_EXIT::parameters::p+size,K| replace::
   Set the number of bits to check.

.. |factory::Monitor_EXIT::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Monitor_EXIT::parameters::p+trials,n| replace::
   Set the number of frames to simulate per :math:`\sigma A` value.

.. ---------------------------------------------- factory Monitor_MI parameters

.. |factory::Monitor_MI::parameters::p+fra-size,N| replace::
   Select the frame size for the mutual information computation.

.. |factory::Monitor_MI::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Monitor_MI::parameters::p+trials,n| replace::
   Set the number of frames to simulate.

.. ----------------------------------------------- factory Puncturer parameters

.. |factory::Puncturer::parameters::p+info-bits,K| replace::
   Set the number of information bits :math:`K`.

.. |factory::Puncturer::parameters::p+fra-size,N| replace::
   Set the frame size :math:`N`. This is not necessarily the codeword size if a
   puncturing pattern is used.

.. |factory::Puncturer::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Puncturer::parameters::p+type| replace::
   Select the puncturer type.

.. ------------------------------------------ factory Puncturer_LDPC parameters

.. |factory::Puncturer_LDPC::parameters::p+cw-size,N_cw| replace::
   Select the codeword size :math:`N`.

.. |factory::Puncturer_LDPC::parameters::p+pattern| replace::
   Give the puncturing pattern following the |LDPC| code.

.. ----------------------------------------- factory Puncturer_polar parameters

.. ----------------------------------------- factory Puncturer_turbo parameters

.. |factory::Puncturer_turbo::parameters::p+pattern| replace::
   Define the puncturing pattern.

.. |factory::Puncturer_turbo::parameters::p+tail-length| replace::
   Set the total number of tail bits at the end of the frame.

.. |factory::Puncturer_turbo::parameters::p+no-buff| replace::
   Do not suppose a buffered encoding.

.. -------------------------------------- factory Puncturer_turbo_DB parameters

.. ----------------------------------------------- factory Quantizer parameters

.. |factory::Quantizer::parameters::p+size,N| replace::
   Set the number of real to quantize.

.. |factory::Quantizer::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Quantizer::parameters::p+type| replace::
   Select the quantizer type.

.. |factory::Quantizer::parameters::p+implem| replace::
   Select the implementation of the quantizer.

.. |factory::Quantizer::parameters::p+dec| replace::
   Set the position of the decimal point in the quantified representation.

.. |factory::Quantizer::parameters::p+bits| replace::
   Set the number of bits used in the fixed-point representation.

.. |factory::Quantizer::parameters::p+range| replace::
   Select the min/max bounds for the ``CUSTOM`` quantizer.

.. -------------------------------------------------- factory Source parameters

.. |factory::Source::parameters::p+info-bits,K| replace::
   Select the number of information bits :math:`K`.

.. |factory::Source::parameters::p+fra,F| replace::
   Set the number of frames to process for each task execution.

.. |factory::Source::parameters::p+type| replace::
   Method used to generate the :math:`K` information bits.

.. |factory::Source::parameters::p+implem| replace::
   Select the implementation of the algorithm to generate the information bits.

.. |factory::Source::parameters::p+path| replace::
   Set the path to a file containing one or more frames (informations bits), to
   use with the ``USER`` source type.

.. |factory::Source::parameters::p+start-idx| replace::
   Give the start index to use in the ``USER`` source type. It is the index of
   the first frame to read from the given file.

.. |factory::Source::parameters::p+seed,S| replace::
   Set the seed used to initialize the |PRNGs|.

.. ------------------------------------ factory Frozenbits_generator parameters

.. |factory::Frozenbits_generator::parameters::p+info-bits,K| replace::
   Select the number of information bits :math:`K`.

.. |factory::Frozenbits_generator::parameters::p+cw-size,N| replace::
   Select the codeword size :math:`N`.

.. |factory::Frozenbits_generator::parameters::p+noise| replace::
   Select the noise for which the frozen bits will be optimized.

.. |factory::Frozenbits_generator::parameters::p+gen-method| replace::
   Select the frozen bits generation method.

.. |factory::Frozenbits_generator::parameters::p+awgn-path| replace::
   Set the path to a file or a directory containing the best channels to select
   the frozen bits.

.. |factory::Frozenbits_generator::parameters::p+dump-path| replace::
   Set the path to store the best channels.

.. |factory::Frozenbits_generator::parameters::p+pb-path| replace::
   Set the path of the polar bounds code generator (generates best channels to
   use).

.. ------------------------------------------ factory Flip_and_check parameters

.. |factory::Flip_and_check::parameters::p+| replace::
   Enable the |FNC| post processing technique.

.. |factory::Flip_and_check::parameters::p+size| replace::
   Set the size (in bit) of the extrinsic for the |FNC| processing.

.. |factory::Flip_and_check::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Flip_and_check::parameters::p+q| replace::
   Set the search space for the |FNC| technique.

.. |factory::Flip_and_check::parameters::p+ite-m| replace::
   Set the first iteration at which the |FNC| is used.

.. |factory::Flip_and_check::parameters::p+ite-M| replace::
   Set the last iteration at which the |FNC| is used.

.. |factory::Flip_and_check::parameters::p+ite-s| replace::
   Set the iteration step for the |FNC| technique.

.. |factory::Flip_and_check::parameters::p+ite,i| replace::
   Set the maximal number of iterations in the Turbo decoding process.

.. |factory::Flip_and_check::parameters::p+crc-start| replace::
   Set the iteration to start the |CRC| checking.

.. --------------------------------------- factory Flip_and_check_DB parameters

.. ------------------------------------------ factory Scaling_factor parameters

.. |factory::Scaling_factor::parameters::p+type| replace::
   Select a scaling factor (|SF|) to be applied to the extrinsic values after
   each half iteration.

.. |factory::Scaling_factor::parameters::p+ite| replace::
   Set the number of iterations.

.. ------------------------------------------------ factory Terminal parameters

.. |factory::Terminal::parameters::p+type| replace::
   Select the terminal type (the format to display the results).

.. |factory::Terminal::parameters::p+no| replace::
   Disable completely the terminal report.

.. |factory::Terminal::parameters::p+freq| replace::
   Set the display frequency (refresh time) of the intermediate results in
   milliseconds. Setting 0 disables the display of the intermediate results.

.. ---------------------------------------- factory Interleaver_core parameters

.. |factory::Interleaver_core::parameters::p+size| replace::
   Select the number of symbols to interleave.

.. |factory::Interleaver_core::parameters::p+fra,F| replace::
   Set the number of inter frame level to process.

.. |factory::Interleaver_core::parameters::p+type| replace::
   Select the interleaver type.

.. |factory::Interleaver_core::parameters::p+path| replace::
   Set the file path to the interleaver |LUT| (to use with the ``USER``
   interleaver).

.. |factory::Interleaver_core::parameters::p+cols| replace::
   Specify the number of columns used for the ``RAND_COL``, ``ROW_COL`` or
   ``COL_ROW`` interleavers.

.. |factory::Interleaver_core::parameters::p+uni| replace::
   Enable to generate a new |LUT| *for each new frame* (i.e. uniform
   interleaver).

.. |factory::Interleaver_core::parameters::p+seed| replace::
   Select the seed used to initialize the |PRNG|.

.. |factory::Interleaver_core::parameters::p+read-order| replace::
   Change the read order of the ``COL_ROW`` and ``ROW_COL`` interleavers.

.. --------------------------------------------------- factory Noise parameters

.. |factory::Noise::parameters::p+noise-range,R| replace::
   Set the noise energy range to run in a |MATLAB| style vector.

.. |factory::Noise::parameters::p+noise-min,m| replace::
   Set the minimal noise energy value to simulate.

.. |factory::Noise::parameters::p+noise-max,M| replace::
   Set the maximal noise energy value to simulate.

.. |factory::Noise::parameters::p+noise-step,s| replace::
   Set the noise energy step between each simulation iteration.

.. |factory::Noise::parameters::p+pdf-path| replace::
   Give a file that contains |PDF| for different |ROP|.

.. |factory::Noise::parameters::p+noise-type,E| replace::
   Select the type of **noise** used to simulate.