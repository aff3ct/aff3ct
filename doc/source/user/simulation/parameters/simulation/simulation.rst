.. _sim-simulation-parameters:

Simulation parameters
---------------------

The simulation parameters allow to customize the communication chain from an
high level point of view. Various communication chain skeletons are available
and can be selected as well as the channel code family to simulate, it is also
possible to enable debug and benchmarking tools.

.. _sim-sim-type:

``--sim-type``
""""""""""""""

   :Type: text
   :Allowed values: ``BFER`` ``BFERI``
   :Default: ``BFER``
   :Examples: ``--sim-type BFERI``

|factory::Launcher::p+type|

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``BFER``  | |sim-type_descr_bfer|  |
+-----------+------------------------+
| ``BFERI`` | |sim-type_descr_bferi| |
+-----------+------------------------+

.. |sim-type_descr_bfer|  replace:: The standard |BFER| chain (:numref:`fig_bfer`).
.. |sim-type_descr_bferi| replace:: The iterative |BFER| chain (:numref:`fig_bferi`).

.. _fig_bfer:

.. figure:: images/bfer.svg
   :figwidth: 90 %
   :align: center

   The standard |BFER| chain.

.. _fig_bferi:

.. figure:: images/bferi.svg
   :figwidth: 90 %
   :align: center

   The iterative |BFER| chain.

.. _sim-sim-cde-type:

``--sim-cde-type, -C`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: text
   :Allowed values: ``BCH`` ``LDPC`` ``POLAR`` ``POLAR_MK`` ``RA`` ``REP``
                    ``RS`` ``RSC`` ``RSC_DB`` ``TURBO`` ``TURBO_DB``
                    ``TURBO_PROD`` ``UNCODED``
   :Examples: ``-C BCH``

|factory::Launcher::p+cde-type,C|

Description of the allowed values:

.. _Bose–Chaudhuri–Hocquenghem: https://en.wikipedia.org/wiki/BCH_code
.. _Low-Density Parity-Check: https://en.wikipedia.org/wiki/Low-density_parity-check_code
.. _Polar: https://en.wikipedia.org/wiki/Polar_code_(coding_theory)
.. _Repeat Accumulate: https://en.wikipedia.org/wiki/Repeat-accumulate_code
.. _Repetition: https://en.wikipedia.org/wiki/Repetition_code
.. _Reed-Solomon: https://en.wikipedia.org/wiki/Reed%E2%80%93Solomon_error_correction
.. _Recursive Systematic Convolutional: https://en.wikipedia.org/wiki/Convolutional_code
.. _Turbo: https://en.wikipedia.org/wiki/Turbo_code
.. _Turbo Product: http://www.ieee802.org/16/tutorial/80216t-00_01.pdf

+----------------+-------------------------------------------------------------+
| Value          | Description                                                 |
+================+=============================================================+
| ``BCH``        | The `Bose–Chaudhuri–Hocquenghem`_ codes :cite:`Bose1960`.   |
+----------------+-------------------------------------------------------------+
| ``LDPC``       | The `Low-Density Parity-Check`_ codes                       |
|                | :cite:`Gallager1963,MacKay1995a`.                           |
+----------------+-------------------------------------------------------------+
| ``POLAR``      | The `Polar`_ codes :cite:`Arikan2009b`.                     |
+----------------+-------------------------------------------------------------+
| ``POLAR_MK``   | The Polar mono- or multi-kernel codes :cite:`Benammar2017`. |
+----------------+-------------------------------------------------------------+
| ``RA``         | The `Repeat Accumulate`_ codes :cite:`Divsalar1998`.        |
+----------------+-------------------------------------------------------------+
| ``REP``        | The `Repetition`_ codes :cite:`Ryan2009a`.                  |
+----------------+-------------------------------------------------------------+
| ``RS``         | The `Reed-Solomon`_ codes :cite:`Reed1960`.                 |
+----------------+-------------------------------------------------------------+
| ``RSC``        | The `Recursive Systematic Convolutional`_ codes             |
|                | :cite:`Ryan2009a`.                                          |
+----------------+-------------------------------------------------------------+
| ``RSC_DB``     | The `Recursive Systematic Convolutional`_ codes with double |
|                | binary symbols :cite:`Ryan2009a`.                           |
+----------------+-------------------------------------------------------------+
| ``TURBO``      | The `Turbo`_ codes :cite:`Berrou1993`.                      |
+----------------+-------------------------------------------------------------+
| ``TURBO_DB``   | The `Turbo`_ codes with double binary symbols               |
|                | :cite:`Berrou1993`.                                         |
+----------------+-------------------------------------------------------------+
| ``TURBO_PROD`` | The `Turbo Product`_ codes :cite:`Ryan2009a`.               |
+----------------+-------------------------------------------------------------+
| ``UNCODED``    | An uncoded simulation.                                      |
+----------------+-------------------------------------------------------------+

.. note:: Only ``POLAR``, ``RSC``, ``RSC_DB``, ``LDPC`` and ``UNCODED`` codes
   are available in ``BFERI`` simulation type.

.. _sim-sim-prec:

``--sim-prec, -p``
""""""""""""""""""

   :Type: integer
   :Default: ``32``
   :Allowed values: ``8`` ``16`` ``32`` ``64``
   :Examples: ``--sim-prec 8``

|factory::Launcher::p+prec,p|

64-bit and 32-bit precisions imply a floating-point representation of the real
numbers. 16-bit and 8-bit imply a fixed-point representation of the real numbers
(see the :ref:`qnt-quantizer-parameters` to configure the quantization).

Description of the allowed values:

+--------+---------------------+
| Value  | Description         |
+========+=====================+
| ``8``  | |sim-prec_descr_8|  |
+--------+---------------------+
| ``16`` | |sim-prec_descr_16| |
+--------+---------------------+
| ``32`` | |sim-prec_descr_32| |
+--------+---------------------+
| ``64`` | |sim-prec_descr_64| |
+--------+---------------------+

.. |sim-prec_descr_8|  replace:: 8-bit precision.
.. |sim-prec_descr_16| replace:: 16-bit precision.
.. |sim-prec_descr_32| replace:: 32-bit precision.
.. |sim-prec_descr_64| replace:: 64-bit precision.

.. _sim-sim-noise-type:

``--sim-noise-type, -E``
""""""""""""""""""""""""

   :Type: text
   :Allowed values: ``EBN0`` ``ESN0`` ``EP`` ``ROP``
   :Default: ``EBN0``
   :Examples: ``-E EBN0``

|factory::Noise::p+noise-type,E|

Description of the allowed values:

+----------+-----------------------------+
| Value    | Description                 |
+==========+=============================+
| ``EBN0`` | |sim-noise-type_descr_ebn0| |
+----------+-----------------------------+
| ``ESN0`` | |sim-noise-type_descr_esn0| |
+----------+-----------------------------+
| ``EP``   | |sim-noise-type_descr_ep|   |
+----------+-----------------------------+
| ``ROP``  | |sim-noise-type_descr_rop|  |
+----------+-----------------------------+

.. |sim-noise-type_descr_ebn0| replace:: |SNR| per **information bit**
.. |sim-noise-type_descr_esn0| replace:: |SNR| per **transmitted symbol**
.. |sim-noise-type_descr_ep|   replace:: Event Probability
.. |sim-noise-type_descr_rop|  replace:: Received Optical Power

``ESN0`` is automatically calculated from ``EBN0`` and vice-versa with
the following equation:

.. centered::
   :math:`\frac{E_S}{N_0} = \frac{E_B}{N_0} + 10.\log(R.bps),`

where :math:`R` is the bit rate and :math:`bps` the number of bits per symbol.

.. note:: When selecting ``EP`` the simulator runs in reverse order, ie. from
   the greatest event probability to the smallest one.

.. hint:: When selecting a ``BEC`` or ``BSC`` channel the ``EP`` noise type is
   automatically set except if you give another one. This is the same for the
   ``OPTICAL`` channel with the ``ROP`` noise type.
   The channel type is set with the :ref:`chn-chn-type` argument.

.. _sim-sim-noise-min:

``--sim-noise-min, -m`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: real number
   :Examples: ``-m 0.0``

|factory::Noise::p+noise-min,m|

.. attention:: This argument is another way to set the noise range to simulate.
   It is ignored or not required if the :ref:`sim-sim-noise-range` argument is
   given, so you may find other piece of information in its description.


.. _sim-sim-noise-max:

``--sim-noise-max, -M`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: real number
   :Examples: ``-M 5.0``

|factory::Noise::p+noise-max,M|

.. attention:: This argument is another way to set the noise range to simulate.
   It is ignored or not required if the :ref:`sim-sim-noise-range` argument is
   given, so you may find other piece of information in its description.

.. _sim-sim-noise-step:

``--sim-noise-step, -s``
""""""""""""""""""""""""

   :Type: real number
   :Default:  0.1
   :Examples: ``-s 1.0``

|factory::Noise::p+noise-step,s|

.. attention:: This argument is another way to set the noise range to simulate.
   It is ignored or not required if the :ref:`sim-sim-noise-range` argument is
   given, so you may find other piece of information in its description.

.. _sim-sim-noise-range:

``--sim-noise-range, -R`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: |MATLAB| style vector
   :Default:  step of 0.1
   :Examples: ``-R "0.5:1,1:0.05:1.2,1.21"``

|factory::Noise::p+noise-range,R|

The above example will run the following noise points::

   0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.05, 1.1, 1.15, 1.2, 1.21

.. attention:: The numerical range for a noise point is
   :math:`\left[-214.748; 213.952 \right]` with a precision of :math:`10^{-7}`.

.. note:: If given, the :ref:`sim-sim-noise-min`, :ref:`sim-sim-noise-max`, and
   :ref:`sim-sim-noise-step` parameters are ignored. But it is not required
   anymore if :ref:`sim-sim-noise-min` and :ref:`sim-sim-noise-max` are set.

.. _sim-sim-pdf-path:

``--sim-pdf-path``
""""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--sim-pdf-path example/path/to/the/right/file``

|factory::Noise::p+pdf-path|

To use with the ``OPTICAL`` :ref:`channel <chn-chn-type>`. It sets the noise
range from the given ones in the file. However, it is overwritten by
:ref:`sim-sim-noise-range` or limited by :ref:`sim-sim-noise-min` and
:ref:`sim-sim-noise-max` with a minimum step of :ref:`sim-sim-noise-step`
between two values.

.. _sim-sim-meta:

``--sim-meta``
""""""""""""""

   :Type: text
   :Examples: ``--sim-meta "TITLE"``

|factory::Simulation::p+meta|

.. note:: :ref:`PyBER <user_pyber_overview>`, our |GUI| tool, can take advantage
   of those meta-data to enhance the display of the simulated curves.

.. _sim-sim-coded:

``--sim-coded``
"""""""""""""""

|factory::BFER::p+coded|

By default, in the simulation, the information bits are extracted from the
decoded codewords and then they are compared to the initially generated
information bits. When this parameter is enabled, the decoded codewords are
directly compared with the initially encoded codewords.

.. note:: This parameter can have a negative impact on the |BER| performance.

.. note:: In some rare cases, to enable this parameter can reduce the simulation
  time.

.. _sim-sim-coset:

``--sim-coset, -c``
"""""""""""""""""""

|factory::BFER::p+coset,c|

The *coset* approach is a "trick" to simulate |BFER| performance **without an
encoder**. It is based on the |AZCW| technique (see the :ref:`src-src-type`
parameter). In the specific case of modulation with memory effect, |AZCWs| can
lead to erroneous |BFER| performance. The *coset* approach solves this problem
by randomly generating N bits, those bits represent a frame but there are
**certainly not** a codeword. Then, those random bits (or symbols) can be
modulated avoiding |AZCW| unexpected effects. On the receiver side, the idea is
to force the decoder to work on an |AZCW| (because the N received |LLRs| are not
a valid codeword). Before the decoding process, knowing the initial bits
sequence, when a bit is 1 then the corresponding input |LLR| is replaced by its
opposite. This way, the decoder "believe" it is decoding an |AZCW| which is a
valid codeword. After the decoding process, knowing the initial bits sequence,
when a bit is 1, then the corresponding output bit is flipped.

.. TODO : add a link to the COSET encoder.

.. _sim-sim-dbg:

``--sim-dbg``
"""""""""""""

|factory::Simulation::p+dbg|

.. code-block:: bash

   aff3ct -C "REP" -K 4 -N 8 -m 1.0 -M 1.0 --sim-dbg
   # [...]
   # Source_random::generate(int32 U_K[4])
   # {OUT} U_K = [    1,     1,     0,     1]
   # Returned status: 0
   #
   # Encoder_repetition_sys::encode(const int32 U_K[4], int32 X_N[8])
   # {IN}  U_K = [    1,     1,     0,     1]
   # {OUT} X_N = [    1,     1,     0,     1,     1,     1,     0,     1]
   # Returned status: 0
   #
   # Modem_BPSK::modulate(const int32 X_N1[8], float32 X_N2[8])
   # {IN}  X_N1 = [    1,     1,     0,     1,     1,     1,     0,     1]
   # {OUT} X_N2 = [-1.00, -1.00,  1.00, -1.00, -1.00, -1.00,  1.00, -1.00]
   # Returned status: 0
   #
   # Channel_AWGN_LLR::add_noise(const float32 X_N[8], float32 Y_N[8])
   # {IN}  X_N = [-1.00, -1.00,  1.00, -1.00, -1.00, -1.00,  1.00, -1.00]
   # {OUT} Y_N = [-0.29, -0.24,  1.55, -0.58, -0.33, -0.51,  0.80, -2.88]
   # Returned status: 0
   #
   # Modem_BPSK::demodulate(const float32 Y_N1[8], float32 Y_N2[8])
   # {IN}  Y_N1 = [-0.29, -0.24,  1.55, -0.58, -0.33, -0.51,  0.80, -2.88]
   # {OUT} Y_N2 = [-0.73, -0.61,  3.91, -1.45, -0.84, -1.28,  2.01, -7.26]
   # Returned status: 0
   #
   # Decoder_repetition_std::decode_siho(const float32 Y_N[8], int32 V_K[4])
   # {IN}  Y_N = [-0.73, -0.61,  3.91, -1.45, -0.84, -1.28,  2.01, -7.26]
   # {OUT} V_K = [    1,     1,     0,     1]
   # Returned status: 0
   #
   # Monitor_BFER::check_errors(const int32 U[4], const int32 V[4])
   # {IN}  U = [    1,     1,     0,     1]
   # {IN}  V = [    1,     1,     0,     1]
   # Returned status: 0
   # [...]

.. note:: By default, the debug mode runs the simulation on one thread.
   It is strongly advise to remove the :ref:`sim-sim-threads` parameter from
   your command line if you use it.

.. hint:: To limit the size of the debug trace, use the :ref:`mnt-mnt-max-fe` or
   :ref:`sim-sim-max-fra` parameters to reduce the number of simulated frames.
   You may also think about using :ref:`sim-sim-dbg-limit` when the frame size
   is too long to be display on a screen line.

.. _sim-sim-dbg-hex:

``--sim-dbg-hex``
"""""""""""""""""

|factory::Simulation::p+dbg-hex|

.. code-block:: bash

   aff3ct -C "REP" -K 4 -N 8 -m 1.0 -M 1.0 --sim-dbg-hex
   # [...]
   # Modem_BPSK::modulate(const int32 X_N1[8], float32 X_N2[8])
   # {IN}  X_N1 = [0x1, 0x1, 0x0, 0x1, 0x1, 0x1, 0x0, 0x1]
   # {OUT} X_N2 = [-0x1p+0, -0x1p+0, 0x1p+0, -0x1p+0, -0x1p+0, -0x1p+0, 0x1p+0, -0x1p+0]
   # Returned status: 0
   #
   # Channel_AWGN_LLR::add_noise(const float32 X_N[8], float32 Y_N[8])
   # {IN}  X_N = [-0x1p+0, -0x1p+0, 0x1p+0, -0x1p+0, -0x1p+0, -0x1p+0, 0x1p+0, -0x1p+0]
   # {OUT} Y_N = [-0x1.28be1cp-2, -0x1.ec1ec8p-3, 0x1.8d242cp+0, -0x1.268a8p-1, -0x1.54c3ccp-2, -0x1.04df9ap-1, 0x1.9905f8p-1, -0x1.71132cp+1]
   # Returned status: 0
   # [...]

.. _sim-sim-dbg-limit:

``--sim-dbg-limit, -d``
"""""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-dbg-limit 1``

|factory::Simulation::p+dbg-limit,d|

.. code-block:: bash

   aff3ct -C "REP" -K 4 -N 8 -m 1.0 -M 1.0 --sim-dbg-limit 3
   # [...]
   # Modem_BPSK::modulate(const int32 X_N1[8], float32 X_N2[8])
   # {IN}  X_N1 = [    1,     1,     0, ...]
   # {OUT} X_N2 = [-1.00, -1.00,  1.00, ...]
   # Returned status: 0
   #
   # Channel_AWGN_LLR::add_noise(const float32 X_N[8], float32 Y_N[8])
   # {IN}  X_N = [-1.00, -1.00,  1.00, ...]
   # {OUT} Y_N = [-0.29, -0.24,  1.55, ...]
   # Returned status: 0
   # [...]

.. _sim-sim-dbg-fra:

``--sim-dbg-fra``
"""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-dbg-fra 10``

|factory::Simulation::p+dbg-fra|

This behavior can be overridden with the :ref:`sim-sim-inter-fra` parameter and
a task can be executed on many frames. In that case, you may want to reduce the
number of displayed frames on screen:

.. code-block:: bash

   aff3ct -C "REP" -K 4 -N 8 -m 1.0 -M 1.0 -F 8 --sim-dbg-fra 4
   # [...]
   # Modem_BPSK::modulate(const int32 X_N1[8x8], float32 X_N2[8x8])
   # {IN}  X_N1 = [f1(    1,     1,     0,     1,     1,     1,     0,     1),
   #               f2(    0,     1,     1,     0,     0,     1,     1,     0),
   #               f3(    1,     0,     1,     1,     1,     0,     1,     1),
   #               f4(    1,     0,     0,     0,     1,     0,     0,     0),
   #               f5->f8:(...)]
   # {OUT} X_N2 = [f1(-1.00, -1.00,  1.00, -1.00, -1.00, -1.00,  1.00, -1.00),
   #               f2( 1.00, -1.00, -1.00,  1.00,  1.00, -1.00, -1.00,  1.00),
   #               f3(-1.00,  1.00, -1.00, -1.00, -1.00,  1.00, -1.00, -1.00),
   #               f4(-1.00,  1.00,  1.00,  1.00, -1.00,  1.00,  1.00,  1.00),
   #               f5->f8:(...)]
   # Returned status: 0
   #
   # Channel_AWGN_LLR::add_noise(const float32 X_N[8x8], float32 Y_N[8x8])
   # {IN}  X_N = [f1(-1.00, -1.00,  1.00, -1.00, -1.00, -1.00,  1.00, -1.00),
   #              f2( 1.00, -1.00, -1.00,  1.00,  1.00, -1.00, -1.00,  1.00),
   #              f3(-1.00,  1.00, -1.00, -1.00, -1.00,  1.00, -1.00, -1.00),
   #              f4(-1.00,  1.00,  1.00,  1.00, -1.00,  1.00,  1.00,  1.00),
   #              f5->f8:(...)]
   # {OUT} Y_N = [f1(-0.29, -0.24,  1.55, -0.58, -0.33, -0.51,  0.80, -2.88),
   #              f2( 0.15, -0.71, -1.85,  1.69, -0.02, -0.50,  0.07,  0.79),
   #              f3(-1.03,  1.39, -1.03, -2.03, -0.67,  0.91, -0.45, -0.88),
   #              f4(-0.37, -1.07,  1.49,  0.94, -0.21,  1.35,  1.06,  0.97),
   #              f5->f8:(...)]
   # Returned status: 0
   # [...]

.. _sim-sim-dbg-prec:

``--sim-dbg-prec``
""""""""""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--sim-dbg-prec 1``

|factory::Simulation::p+dbg-prec|

.. code-block:: bash

   aff3ct -C "REP" -K 4 -N 8 -m 1.0 -M 1.0 --sim-dbg-prec 4
   # [...]
   # Modem_BPSK::modulate(const int32 X_N1[8], float32 X_N2[8])
   # {IN}  X_N1 = [      0,       0,       1,       1,       0,       0,       1,       1]
   # {OUT} X_N2 = [ 1.0000,  1.0000, -1.0000, -1.0000,  1.0000,  1.0000, -1.0000, -1.0000]
   # Returned status: 0
   #
   # Channel_AWGN_LLR::add_noise(const float32 X_N[8], float32 Y_N[8])
   # {IN}  X_N = [ 1.0000,  1.0000, -1.0000, -1.0000,  1.0000,  1.0000, -1.0000, -1.0000]
   # {OUT} Y_N = [ 1.4260,  0.4301, -1.5119,  0.1559,  0.0784,  1.6980, -1.6501, -0.0769]
   # Returned status: 0
   # [...]

.. _sim-sim-seed:

``--sim-seed, -S``
""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-seed 42``

|factory::Simulation::p+seed,S|

.. note:: |AFF3CT| uses |PRNG| to simulate the random. As a consequence the
   simulator behavior is reproducible from a run to another. It can be helpful
   to debug source code. However, when simulating in multi-threaded mode, the
   threads running order is not deterministic and so results will most likely be
   different from one execution to another.

.. _sim-sim-stats:

``--sim-stats``
"""""""""""""""

|factory::Simulation::p+stats|

.. code-block:: bash

   aff3ct -C "POLAR" -K 1723 -N 2048 -m 4.2 -M 4.2 -t 1 --sim-stats
   # [...]
   # -------------------------------------------||------------------------------||--------------------------------||--------------------------------
   #        Statistics for the given task       ||       Basic statistics       ||       Measured throughput      ||        Measured latency
   #     ('*' = any, '-' = same as previous)    ||          on the task         ||   considering the last socket  ||   considering the last socket
   # -------------------------------------------||------------------------------||--------------------------------||--------------------------------
   # -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------
   #       MODULE |              TASK |   TIMER ||    CALLS |     TIME |   PERC ||  AVERAGE |  MINIMUM |  MAXIMUM ||  AVERAGE |  MINIMUM |  MAXIMUM
   #              |                   |         ||          |      (s) |    (%) ||   (Mb/s) |   (Mb/s) |   (Mb/s) ||     (us) |     (us) |     (us)
   # -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------
   #      Channel |         add_noise |       * ||    14909 |     0.72 |  37.59 ||    42.17 |    20.75 |    45.52 ||    48.56 |    44.99 |    98.69
   #       Source |          generate |       * ||    14909 |     0.60 |  31.13 ||    42.84 |     8.72 |    44.34 ||    40.22 |    38.86 |   197.63
   #      Encoder |            encode |       * ||    14909 |     0.37 |  19.06 ||    83.17 |    16.00 |    86.10 ||    24.62 |    23.79 |   127.97
   #      Decoder |       decode_siho |       * ||    14909 |     0.22 |  11.32 ||   117.80 |    36.67 |   126.75 ||    14.63 |    13.59 |    46.99
   #      Monitor |      check_errors |       * ||    14909 |     0.01 |   0.42 ||  3186.81 |   120.63 |  3697.42 ||     0.54 |     0.47 |    14.28
   #        Modem |        demodulate |       * ||    14909 |     0.00 |   0.25 ||  6350.57 |   160.24 |  7876.92 ||     0.32 |     0.26 |    12.78
   #        Modem |          modulate |       * ||    14909 |     0.00 |   0.23 ||  6962.61 |   184.84 |  8291.50 ||     0.29 |     0.25 |    11.08
   # -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------
   #        TOTAL |                 * |       * ||    14909 |     1.93 | 100.00 ||    13.34 |     3.38 |    14.10 ||   129.19 |   122.21 |   509.42
   # [...]

Each line corresponds to a task. The tasks are sorted by execution time in the
simulation (descending order).
The first column group **identifies the task**:

   * ``MODULE``: the module type,
   * ``TASK``: the task name,
   * ``TIMER``: the name of the current task timer (it is possible to put timers
     inside a task to measure sub-parts of this task), ``*`` indicates that the
     whole task execution time is considered.

The second column group gives **basic statistics**:

   * ``CALLS``: the number of times this task has been executed,
   * ``TIME``: the cumulative time of all the task executions,
   * ``PERC``: the percentage of time taken by the task in the simulation.

The third column group shows **the average, the minimum and the maximum
throughputs**. Those throughputs are calculated considering the size of the
output frames. If the task does not have outputs (c.f the *check_errors* routine
from the monitor) then the number of input bits is used instead. For instance,
the *encode* task takes :math:`K` input bits and produces :math:`N` output bits,
so :math:`N` bits will be considered in the throughput computations.

The last column group shows **the average, the minimum and the maximum
latencies**.

The ``TOTAL`` line corresponds to the full communication chain. For the
throughput computations, the last socket of the last task determines the number
of considered bits. In a standard |BFER| simulation the last task is the
*check_errors* routine from the monitor and consequently the number of
information bits (:math:`K`) is considered. However, if the :ref:`sim-sim-coded`
parameter is enabled, it becomes the codeword size (:math:`N`).

.. note:: Enabling the statistics can increase the simulation time due the
   measures overhead. This is especially true when short frames are simulated.

.. warning:: In multi-threaded mode the reported time is the cumulative time of
   all the threads. This time is bigger than the real simulation time because it
   does not consider that many tasks have been executed in parallel.

.. warning:: The task throughputs will not increase with the number of threads:
   the statistics consider the performance on one thread.

.. _sim-sim-threads:

``--sim-threads, -t``
"""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-threads 1``

|factory::Simulation::p+threads,t|

.. note:: **Monte Carlo methods are known to be embarrassingly parallel**, which
   is why, in most cases, the simulation throughput will increase linearly with
   the number of threads (unless this number exceeds the number of cores
   available). However, in some cases, especially for large frames, when the
   number of threads is high, the memory footprint can exceeds the size of the
   CPU caches and it becomes less interesting to use a large number of threads.

.. _sim-sim-inter-fra:

``--sim-inter-fra, -F``
"""""""""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--sim-inter-fra 3``

|factory::Simulation::p+inter-fra,F|

The default behavior is to generate one frame at a time. This parameter enables
to process more than one frame during the execution of a task.

The number of frames consumed and produced when a task is executed is called the
**inter frame level** or |IFL|. Setting the |IFL| will automatically affect the
|IFL| level in all the simulation modules (c.f. :numref:`fig_sim_fra_inter`).

.. _fig_sim_fra_inter:

.. figure:: images/sim_fra_inter.svg
   :figwidth: 70 %
   :align: center

   3-way inter frame level in the communication chain.

The |IFL| also allows multi-user configurations to be simulated (see
:numref:`fig_sim_fra_multi_user`). This configurations is used when using |SCMA|
modulation (see the :ref:`mdm-mdm-type` ``SCMA`` parameter).

.. _fig_sim_fra_multi_user:

.. figure:: images/sim_fra_multi_user.svg
   :figwidth: 70 %
   :align: center

   3-way inter frame level with multi-user channel in the communication chain.

.. note:: **For short frames**, increase the |IFL| can **increase the
  simulation throughput**, it can hide task call overheads.

.. note:: **For large frames**, increase the |IFL| can **decrease the
  simulation throughput** due the CPU cache size limitation.

.. _sim-sim-crc-earlyt:

``--sim-crc-earlyt``
""""""""""""""""""""

|factory::BFER_ite::p+crc-earlyt|

.. note:: Available only for ``BFERI`` simulation type (c.f. the
   :ref:`sim-sim-type` parameter).

.. _sim-sim-ite:

``--sim-ite, -I``
"""""""""""""""""

   :Type: integer
   :Default: 15
   :Examples: ``--sim-ite 10``

|factory::BFER_ite::p+ite,I|

.. note:: Available only for ``BFERI`` simulation type (c.f. the
   :ref:`sim-sim-type` parameter).

.. _sim-sim-sequence-path:

``--sim-sequence-path`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: file
   :Rights: write
   :Default: :file:`sequence.dot`
   :Examples: ``--sim-sequence-path sequence.dot``

|factory::BFER::p+sequence-path|

.. _sim-sim-max-fra:

``--sim-max-fra, -n`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-max-fra 1``

|factory::Simulation::p+max-fra,n|

.. note:: The default behavior is to stop the simulator when the limit is
   reached but it is possible to override it with the :ref:`sim-sim-crit-nostop`
   parameter. In this case, the remaining noise points will also be simulated
   and the limit will be applied for each of them.

.. _sim-sim-stop-time:

``--sim-stop-time`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-stop-time 1``

|factory::Simulation::p+stop-time|

.. note:: The default behavior is to stop the simulator when the limit is
   reached but it is possible to override it with the :ref:`sim-sim-crit-nostop`
   parameter. In this case, the remaining noise points will also be simulated
   and the limit will be applied for each of them.

.. _sim-sim-crit-nostop:

``--sim-crit-nostop`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

|factory::Simulation::p+crit-nostop|

To combine with the :ref:`sim-sim-max-fra` and/or the :ref:`sim-sim-stop-time`
parameters.

.. _sim-sim-err-trk:

``--sim-err-trk`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""

|factory::BFER::p+err-trk|

.. tip:: When working on the design of a new decoder or when improving an
   existing one, it can be very interesting to have a database of erroneous
   frames to work on (especially if those errors occur at low |BFER| when the
   simulation time is important). This way it is possible to focus only on
   those erroneous frames and quickly see if the decoder improvements have an
   impact on them. It is also interesting to be able to easily extract the
   erroneous frames from the simulator to characterize the type of errors and
   better understand why the decoder fails.

.. note:: See the :ref:`sim-sim-err-trk-rev` argument to replay the erroneous
   dumped frames.

.. _sim-sim-err-trk-rev:

``--sim-err-trk-rev`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

|factory::BFER::p+err-trk-rev|


.. tip:: To play back the erroneous frames, just add ``-rev`` to the
   :ref:`sim-sim-err-trk` argument and change nothing else to your command line.

.. _sim-sim-err-trk-path:

``--sim-err-trk-path`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: file
   :Rights: read/write
   :Default: :file:`error_tracker`
   :Examples: ``--sim-err-trk-path errors/err``

|factory::BFER::p+err-trk-path|

For the above example, the dumped or read files will be:

   * :file:`errors/err_0.64.src`
   * :file:`errors/err_0.64.enc`
   * :file:`errors/err_0.64.chn`

.. note:: For |SNR| noise type, the value used to define the filename is the
   noise variance :math:`\sigma`.

.. danger:: Be careful, if you give a wrong path you will not have a warning
   message at the beginning of the simulation. It can be frustrating when
   running a very long simulation...

.. _sim-sim-err-trk-thold:

``--sim-err-trk-thold`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-err-trk-thold 1``

|factory::BFER::p+err-trk-thold|

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Sim-
