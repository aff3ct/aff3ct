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
   :Allowed values: ``BFER`` ``BFERI`` ``EXIT``
   :Default: ``BFER``
   :Examples: ``--sim-type BFERI``

Select the type of simulation (or communication chain skeleton).

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``BFER``  | |sim-type_descr_bfer|  |
+-----------+------------------------+
| ``BFERI`` | |sim-type_descr_bferi| |
+-----------+------------------------+
| ``EXIT``  | |sim-type_descr_exit|  |
+-----------+------------------------+

.. |sim-type_descr_bfer|  replace:: The standard |BFER| chain (:numref:`fig_bfer`).
.. |sim-type_descr_bferi| replace:: The iterative |BFER| chain (:numref:`fig_bferi`).
.. |sim-type_descr_exit|  replace:: The EXIT chart simulation chain (not documented at this time).

.. _fig_bfer:

.. figure:: images/bfer.png
   :figwidth: 90 %
   :align: center

   The standard |BFER| chain.

.. _fig_bferi:

.. figure:: images/bferi.png
   :figwidth: 90 %
   :align: center

   The iterative |BFER| chain.

.. _sim-sim-cde-type:

``--sim-cde-type, -C`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: text
   :Allowed values: ``BCH`` ``LDPC`` ``POLAR`` ``RA`` ``REP`` ``RS`` ``RSC``
                    ``RSC_DB`` ``TURBO`` ``TURBO_DB`` ``TURBO_PROD`` ``UNCODED``
   :Examples: ``-C BCH``

Select the code type you want to simulate.

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
|                | :cite:`Gallager1963,MacKay1995`.                            |
+----------------+-------------------------------------------------------------+
| ``POLAR``      | The `Polar`_ codes :cite:`Arikan2009`.                      |
+----------------+-------------------------------------------------------------+
| ``RA``         | The `Repeat Accumulate`_ codes :cite:`Divsalar1998`.        |
+----------------+-------------------------------------------------------------+
| ``REP``        | The `Repetition`_ codes :cite:`Ryan2009`.                   |
+----------------+-------------------------------------------------------------+
| ``RS``         | The `Reed-Solomon`_ codes :cite:`Reed1960`.                 |
+----------------+-------------------------------------------------------------+
| ``RSC``        | The `Recursive Systematic Convolutional`_ codes             |
|                | :cite:`Ryan2009`.                                           |
+----------------+-------------------------------------------------------------+
| ``RSC_DB``     | The `Recursive Systematic Convolutional`_ codes with double |
|                | binary symbols :cite:`Ryan2009`.                            |
+----------------+-------------------------------------------------------------+
| ``TURBO``      | The `Turbo`_ codes :cite:`Berrou1993`.                      |
+----------------+-------------------------------------------------------------+
| ``TURBO_DB``   | The `Turbo`_ codes with double binary symbols               |
|                | :cite:`Berrou1993`.                                         |
+----------------+-------------------------------------------------------------+
| ``TURBO_PROD`` | The `Turbo Product`_ codes :cite:`Ryan2009`.                |
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

Specify the representation of the real numbers in the receiver part of the
chain. 64-bit and 32-bit precisions imply a floating-point representation of the
real numbers. 16-bit and 8-bit imply a fixed-point representation of the real
numbers (see the :ref:`qnt-quantizer-parameters` to configure the quantization).

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

Select the type of **noise** used to simulate.

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

Give the minimal noise energy value to simulate.

.. attention:: This argument is another way to set the noise range to simulate.
   It is ignored or not required if the :ref:`sim-sim-noise-range` argument is
   given, so you may find other piece of information in its description.


.. _sim-sim-noise-max:

``--sim-noise-max, -M`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: real number
   :Examples: ``-M 5.0``

Give the maximal noise energy value to simulate.

.. attention:: This argument is another way to set the noise range to simulate.
   It is ignored or not required if the :ref:`sim-sim-noise-range` argument is
   given, so you may find other piece of information in its description.

.. _sim-sim-noise-step:

``--sim-noise-step, -s``
""""""""""""""""""""""""

   :Type: real number
   :Default:  0.1
   :Examples: ``-s 1.0``

Give the noise energy step between each simulation iteration.

.. attention:: This argument is another way to set the noise range to simulate.
   It is ignored or not required if the :ref:`sim-sim-noise-range` argument is
   given, so you may find other piece of information in its description.

.. _sim-sim-noise-range:

``--sim-noise-range, -R`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: |MATLAB| style vector
   :Default:  step of 0.1
   :Examples: ``-R "0.5:1,1:0.05:1.2,1.21"``

Set the noise energy range to run in a |MATLAB| style vector.
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

Give a file that contains |PDF| for different |ROP|. To use with the ``OPTICAL``
:ref:`channel type <chn-chn-type>`. It sets the noise range from the given ones
in the file. However, it is overwritten by :ref:`sim-sim-noise-range` or limited
by :ref:`sim-sim-noise-min` and :ref:`sim-sim-noise-max` with a minimum step of
:ref:`sim-sim-noise-step` between two values.

.. _sim-sim-meta:

``--sim-meta``
""""""""""""""

   :Type: text
   :Examples: ``--sim-meta "TITLE"``

.. _INI: https://en.wikipedia.org/wiki/INI_file

Add meta-data at the beginning of the |AFF3CT| standard output (`INI`_ format is
used). The value of the parameter will be affected to the *title* meta-data and
the *command line* will be added.

.. note:: :ref:`PyBER <user_pyber_overview>`, our |GUI| tool, can take advantage
   of those meta-data to enhance the display of the simulations.

.. _sim-sim-coded:

``--sim-coded``
"""""""""""""""

Enable the coded monitoring. By default, in the simulation, the information bits
are extracted from the decoded codewords and then they are compared to the
initially generated information bits. When this parameter is enabled, the
decoded codewords are directly compared with the initially encoded codewords.

.. note:: This parameter can have a negative impact on the |BER| performance.

.. note:: In some rare cases, to enable this parameter can reduce the simulation
  time.

.. _sim-sim-coset:

``--sim-coset, -c``
"""""""""""""""""""

Enable the COSET approach. This methods flips the value of the input and of the
output of the decoder in function of the original codeword bits. The flipped
values are the matching bit at 1 in the codeword. Then the decoder works like on
an all zero codeword. This is useful when developing a decoder without
associated encoder. This activates automatically the ``COSET`` encoder and
vice-versa.

.. TODO : add a link to the COSET encoder.


.. _sim-sim-dbg:

``--sim-dbg``
"""""""""""""

Enable the debug mode. This prints the frame values after each module
step.

.. note:: Debug mode runs the simulation on one thread only. Then you may need
   to remove the :ref:`sim-sim-threads` from your command line if you use it.

.. hint:: To keep a readable debug log, use :ref:`mnt-mnt-max-fe` or
   :ref:`sim-sim-max-fra` to stop your
   simulation after that a given amount of frames have been played. You may also
   think about using :ref:`sim-sim-dbg-limit` when playing with too long
   frames (more than 32 bits in function of your screen size).

.. _sim-sim-dbg-hex:

``--sim-dbg-hex``
"""""""""""""""""

Enable the debug mode and *print values in the hexadecimal format*.

.. _sim-sim-dbg-limit:

``--sim-dbg-limit, -d``
"""""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-dbg-limit 1``

Enable the debug mode and set *the max number of elements* to display per
frame. A number of elements at 0 means there is no dump limit.

.. _sim-sim-dbg-fra:

``--sim-dbg-fra``
"""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-dbg-fra 10``

Enable the debug mode and set *the max number of frames* to display per module.
A number of elements at 0 means there is no dump limit.

.. _sim-sim-dbg-prec:

``--sim-dbg-prec``
"""""""""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--sim-dbg-prec 1``

Enable the debug mode and set *the decimal precision* (number of digits for
decimal part) of real elements.

.. _sim-sim-no-colors:

``--sim-no-colors``
"""""""""""""""""""

Disable the colors in the shell.

.. note:: |AFF3CT| uses the `rang <https://github.com/agauniyal/rang>`_ library
   that is portable on every OS as it only depends on C++ standard library.
   Furthermore, rang doesn't interfere when redirecting cout/cerr/clog to
   somewhere else and leaves the decision to the library user.

.. _sim-sim-seed:

``--sim-seed, -S``
""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-seed 42``

Set the seed used in the simulation to initialize the general pseudo random
generators. This last generator is used to generate the seeds of the different
random generators of all modules.

.. note:: When using **MPI**, each node has its own seed generated from this
   initial value to guarantee the generation of different values between the
   different computers.
.. TODO : add link to MPI use

.. note:: |AFF3CT| uses pseudo-random generators in order to be able to replay
   the same results from a run to another. It is helpful to debug source code.
   However, when simulating in multi-thread, computer load is not controllable
   and so results may differ as threads do not necessary run at the same speed.


.. _sim-sim-stats:

``--sim-stats``
"""""""""""""""

Display the statistics module by module noise point after noise point.
Statistics show the time used by each module in the simulation, the
throughput and latency (min, max and average) by task.

.. _sim-sim-threads:

``--sim-threads, -t``
"""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-threads 1``

Specify the number of threads used by the simulation.
A 0 value means the maximum number of thread supported by the processor, ie.
the number of core (physical + virtual).

..note:: The simulation throughput is linear with the number of physical cores
   used, but the contribution of the virtual cores is more limited.

.. _sim-sim-crc-start:

``--sim-crc-start``
"""""""""""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--sim-crc-start 1``

Set the number of simulation iteration needed before starting the CRC checking
in the turbo demodulation process.

.. note:: Available only for BFERI simulations.

.. _sim-sim-ite:

``--sim-ite, -I``
"""""""""""""""""

   :Type: integer
   :Default: 15
   :Examples: ``--sim-ite 10``

Set the number of global iterations between the demodulator and the decoder.

.. note:: Available only for BFERI simulations.

.. _sim-sim-max-fra:

``--sim-max-fra, -n`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-max-fra 1``

Set the maximum number of frames to play after what the current simulated noise
point is stopped. A 0 value means no limit. The simulation is also stopped
except if the :ref:`sim-sim-crit-nostop` option is used.

.. _sim-sim-stop-time:

``--sim-stop-time`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-stop-time 1``

Give the time in *[sec]* after what the current simulated noise point is
stopped. A 0 value means no limit. The simulation is also stopped except
if the :ref:`sim-sim-crit-nostop` option is used.

.. _sim-sim-crit-nostop:

``--sim-crit-nostop`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

Set the stop criteria arguments:ref:`sim-sim-max-fra` and
:ref:`sim-sim-stop-time` stop only the currently simulated noise point but not
the simulation.

.. _sim-sim-err-trk:

``--sim-err-trk`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""

Track the bad frames. When a wrong frame is detected, the source frame,
the encoded frame and the noise applied by the channel are dumped in
their respective file. Then use the :ref:`sim-sim-err-trk-rev` argument
to run only these bad frames.

.. _sim-sim-err-trk-rev:

``--sim-err-trk-rev`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

Automatically replay the saved frames while running with :ref:`sim-sim-err-trk`.

.. tip:: To play back the bad frames, just add **-rev** to the
   :ref:`sim-sim-err-trk` argument and change nothing else to your command line
   except the debug mode arguments (:ref:`sim-sim-dbg`).

.. _sim-sim-err-trk-path:

``--sim-err-trk-path`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: file
   :Rights: read/write
   :Default: :file:`error_tracker`
   :Examples: ``--sim-err-trk-path errors/err``

Set the base path for the files where the bad frames will be stored or read.
To this base path is added the noise point value and the matching module
extension. With the above example, you may get files such as:

   * :file:`errors/err_0.64.src`
   * :file:`errors/err_0.64.enc`
   * :file:`errors/err_0.64.chn`

.. note:: For |SNR| noise type, the value used to define the filename is the
   noise variance.

.. danger:: Intermediate folders are not automatically created but only the
   files. You won't get any warning message, so be careful to create them before
   running long simulations!

.. _sim-sim-err-trk-thold:

``--sim-err-trk-thold`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--sim-err-trk-thold 1``

Give the threshold value of bit error count per frame that the error tracker
has to dump.

.. _sim-sim-no-legend:

``--sim-no-legend`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""

Display strictly no legend when launching the simulation.

.. tip::
   Use this option when you want to complete an already existing
   simulation result file with new noise points. Pay attention to use ``>>``
   instead of ``>`` to redirect the standard output in order to add results at
   the end of the file and not overwriting it.

.. _sim-sim-mpi-comm:

``--sim-mpi-comm``
""""""""""""""""""

   :Type: integer
   :Default: 1000
   :Examples: ``--sim-mpi-comm 1``

Set the ``MPI`` communication frequency between the nodes in *[ms]*.
This corresponds to the frequency with which nodes will gather their results.
This operation takes some computation resources so a too fast frequency may be
sub-productive.

.. note:: Available only when compiling with the MPI support
   :ref:`compilation_cmake_options`.

.. TODO : add link to MPI use

References
""""""""""

.. bibliography:: references.bib