.. _sim-simulation-parameters:

Simulation parameters
---------------------

The simulation parameters allow the selection of the code to simulate on a
selected chain, as well as the noise range to run.
They give also access to powerful tools to debug the simulated modules.


.. _sim-sim-cde-type:

``--sim-cde-type, -C`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: text
   :Allowed values: ``BCH`` ``LDPC`` ``POLAR`` ``RA`` ``REP`` ``RS`` ``RSC``
                    ``RSC_DB`` ``TURBO`` ``TURBO_DB`` ``TURBO_PROD`` ``UNCODED``
   :Examples: ``-C BCH``

Select the code type you want to use.

Description of the allowed values:

+----------------+---------------------------------+
| Value          | Description                     |
+================+=================================+
| ``BCH``        | |sim-cde-type_descr_bch|        |
+----------------+---------------------------------+
| ``LDPC``       | |sim-cde-type_descr_ldpc|       |
+----------------+---------------------------------+
| ``POLAR``      | |sim-cde-type_descr_polar|      |
+----------------+---------------------------------+
| ``RA``         | |sim-cde-type_descr_ra|         |
+----------------+---------------------------------+
| ``REP``        | |sim-cde-type_descr_rep|        |
+----------------+---------------------------------+
| ``RS``         | |sim-cde-type_descr_rs|         |
+----------------+---------------------------------+
| ``RSC``        | |sim-cde-type_descr_rsc|        |
+----------------+---------------------------------+
| ``RSC_DB``     | |sim-cde-type_descr_rsc_db|     |
+----------------+---------------------------------+
| ``TURBO``      | |sim-cde-type_descr_turbo|      |
+----------------+---------------------------------+
| ``TURBO_DB``   | |sim-cde-type_descr_turbo_db|   |
+----------------+---------------------------------+
| ``TURBO_PROD`` | |sim-cde-type_descr_turbo_prod| |
+----------------+---------------------------------+
| ``UNCODED``    | |sim-cde-type_descr_uncoded|    |
+----------------+---------------------------------+

.. _Bose–Chaudhuri–Hocquenghem: https://en.wikipedia.org/wiki/BCH_code
.. _Low-density parity-check: https://en.wikipedia.org/wiki/Low-density_parity-check_code
.. _Polar: https://en.wikipedia.org/wiki/Polar_code_(coding_theory)
.. _Repeat accumulate: https://en.wikipedia.org/wiki/Repeat-accumulate_code
.. _Repetition: https://en.wikipedia.org/wiki/Repetition_code
.. _Reed–Solomon: https://en.wikipedia.org/wiki/Reed%E2%80%93Solomon_error_correction
.. _Recursive Systematic Convolutional: https://en.wikipedia.org/wiki/Convolutional_code
.. _Turbo: https://en.wikipedia.org/wiki/Turbo_code
.. _Turbo Product: http://www.ieee802.org/16/tutorial/80216t-00_01.pdf

.. |sim-cde-type_descr_bch|        replace:: The `Bose–Chaudhuri–Hocquenghem`_ code
.. |sim-cde-type_descr_ldpc|       replace:: The `Low-density parity-check`_ codes
.. |sim-cde-type_descr_polar|      replace:: The `Polar`_ code
.. |sim-cde-type_descr_ra|         replace:: The `Repeat accumulate`_ code
.. |sim-cde-type_descr_rep|        replace:: The `Repetition`_ code
.. |sim-cde-type_descr_rs|         replace:: The `Reed–Solomon`_ code
.. |sim-cde-type_descr_rsc|        replace:: The `Recursive Systematic Convolutional`_ code
.. |sim-cde-type_descr_rsc_db|     replace:: The `Recursive Systematic Convolutional`_ code with Double Binary symbols
.. |sim-cde-type_descr_turbo|      replace:: The `Turbo`_ codes
.. |sim-cde-type_descr_turbo_db|   replace:: The `Turbo`_ codes Double Binary symbols
.. |sim-cde-type_descr_turbo_prod| replace:: The `Turbo Product`_ code
.. |sim-cde-type_descr_uncoded|    replace:: An uncoded simulation

.. note:: Only ``POLAR``, ``RSC``, ``RSC_DB``, ``LDPC`` and ``UNCODED`` codes
   are available in ``BFERI`` simulation type.


.. _sim-sim-noise-type:

``--sim-noise-type, -E``
""""""""""""""""""""""""

   :Type: text
   :Allowed values: ``EBN0`` ``ESN0`` ``EP`` ``ROP``
   :Default: ``EBN0``
   :Examples: ``--sim-noise-type EBN0``

Selects the type of **noise** used to simulate.

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

.. |sim-noise-type_descr_ebn0| replace:: Signal Noise Ratio per **information bit**
.. |sim-noise-type_descr_esn0| replace:: Signal Noise Ratio per **transmitted symbol**
.. |sim-noise-type_descr_ep|   replace:: Event Probability
.. |sim-noise-type_descr_rop|  replace:: Received Optical Power

.. note:: ``ESN0`` is automatically calculated from ``EBN0`` and vice-versa.

.. note:: When selecting ``EP`` the simulator runs in reverse order, ie. from
   the greatest probability to the smallest one.

.. hint:: When selecting a ``BEC`` or ``BSC`` channel the ``EP`` noise type is
   automatically set except if you give another one. This is the same for the
   ``OPTICAL`` channel with the ``ROP`` noise type.
   The channel type is set with the :ref:`chn-chn-type` argument.

.. _sim-sim-noise-min:

``--sim-noise-min, -m`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: real number
   :Examples: ``--sim-noise-min 0.0``

Gives the minimal noise energy point to simulate.

.. attention:: This argument is another way to set the noise. It is ignored or
   not required if :ref:`sim-sim-noise-range` is given, so you may find other
   piece of information in its description.


.. _sim-sim-noise-max:

``--sim-noise-max, -M`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: real number
   :Examples: ``--sim-noise-max 5.0``

Gives the maximal noise energy to simulate.

.. attention:: This argument is another way to set the noise. It is ignored or
   not required if :ref:`sim-sim-noise-range` is given, so you may find other
   piece of information in its description.

.. _sim-sim-noise-step:

``--sim-noise-step, -s``
""""""""""""""""""""""""

   :Type: real number
   :Default:  0.1
   :Examples: ``--sim-noise-step 1.0``

Gives the noise energy step between each simulation iteration.

.. attention:: This argument is another way to set the noise. It is ignored or
   not required if :ref:`sim-sim-noise-range` is given, so you may find other
   piece of information in its description.

.. _sim-sim-noise-range:

``--sim-noise-range, -R`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: Matlab style vector
   :Default:  step of 0.1
   :Examples: ``--sim-noise-range "0.5:1,1:0.05:1.2,1.21"``

Set the noise energy range to run in a Matlab style vector.
The given example will run the following noise points::

   0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.05, 1.1, 1.15, 1.2, 1.21

.. attention:: The numerical limit for a noise point is a maximum of about
   :math:`\pm 214` with a precision :math:`10^{-7}`.

.. note:: If given, :ref:`sim-sim-noise-min`, :ref:`sim-sim-noise-max`, and
   :ref:`sim-sim-noise-step` are ignored. But it is not required anymore if
   :ref:`sim-sim-noise-min` and :ref:`sim-sim-noise-max` are set.

.. _sim-sim-coded:

``--sim-coded``
"""""""""""""""

Enable the coded monitoring (extends the monitored bits to the entire codeword).

.. _sim-sim-coset:

``--sim-coset, -c``
"""""""""""""""""""

Enable the coset approach.

.. _sim-sim-crc-start:

``--sim-crc-start``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--sim-crc-start 1``

Iteration number to start the CRC checking in the turbo demodulation process.

.. note::

   Available only for BFERI simulations

.. _sim-sim-debug:

``--sim-debug``
"""""""""""""""

Enable debug mode: print array values after each step.

.. _sim-sim-debug-hex:

``--sim-debug-hex``
"""""""""""""""""""

Debug mode prints values in the hexadecimal format.

.. _sim-sim-debug-limit:

``--sim-debug-limit, -d``
"""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--sim-debug-limit 1``

Set the max number of elements to display in the debug mode.

.. _sim-sim-debug-prec:

``--sim-debug-prec``
""""""""""""""""""""

   :Type: integer
   :Examples: ``--sim-debug-prec 1``

Set the precision of real elements when displayed in debug mode.

.. _sim-sim-ite:

``--sim-ite, -I``
"""""""""""""""""

   :Type: integer
   :Examples: ``--sim-ite 1``

Number of global iterations between the demodulator and the decoder.

.. note::

   Available only for BFERI simulations

.. _sim-sim-meta:

``--sim-meta``
""""""""""""""

   :Type: text
   :Examples: ``--sim-meta "TODO CHECK VALUE"``

Print the output with metadata, takes the simulation title.

.. _sim-sim-no-colors:

``--sim-no-colors``
"""""""""""""""""""


Disable the colors in the shell.


.. _sim-sim-pdf-path:

``--sim-pdf-path``
""""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--sim-pdf-path example/path/to/the/right/file``

A file that contains PDF for different SNR. Set the SNR range from the given ones. Overwritten by -R or limited by -m and -M with a minimum step of -s.

.. _sim-sim-prec:

``--sim-prec, -p``
""""""""""""""""""

   :Type: integer
   :Allowed values: ``8`` ``16`` ``32`` ``64``
   :Examples: ``--sim-prec 8``

The simulation precision in bits.

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

.. |sim-prec_descr_8| replace:: TODO VALUE 8
.. |sim-prec_descr_16| replace:: TODO VALUE 16
.. |sim-prec_descr_32| replace:: TODO VALUE 32
.. |sim-prec_descr_64| replace:: TODO VALUE 64


.. _sim-sim-seed:

``--sim-seed, -S``
""""""""""""""""""

   :Type: integer
   :Examples: ``--sim-seed 1``

Seed used in the simulation to initialize the pseudo random generators in general.

.. _sim-sim-stats:

``--sim-stats``
"""""""""""""""


Display statistics module by module.

.. _sim-sim-threads:

``--sim-threads, -t``
"""""""""""""""""""""

   :Type: integer
   :Examples: ``--sim-threads 1``

Enable multi-threaded mode and specify the number of threads (0 means the maximum supported by the core.

.. _sim-sim-type:

``--sim-type``
""""""""""""""

   :Type: text
   :Allowed values: ``BFER`` ``BFERI`` ``EXIT``
   :Examples: ``--sim-type BFER``

Select the type of simulation to launch (default is BFER).

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

.. |sim-type_descr_bfer| replace:: TODO VALUE BFER
.. |sim-type_descr_bferi| replace:: TODO VALUE BFERI
.. |sim-type_descr_exit| replace:: TODO VALUE EXIT


.. _sim-sim-crit-nostop:

``--sim-crit-nostop`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""


The stop criteria arguments -stop-time or -max-frame kill the current simulatated noise point but not the simulation.

.. _sim-sim-err-trk:

``--sim-err-trk`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""


Enable the tracking of the bad frames (by default the frames are stored in the current folder).

.. _sim-sim-err-trk-path:

``--sim-err-trk-path`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: file
   :Rights: read/write
   :Examples: ``--sim-err-trk-path example/path/to/the/right/file``

Base path for the files where the bad frames will be stored or read.

.. _sim-sim-err-trk-rev:

``--sim-err-trk-rev`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""


Automatically replay the saved frames.

.. _sim-sim-err-trk-thold:

``--sim-err-trk-thold`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--sim-err-trk-thold 1``

Dump only frames with a bit error count above or equal to this threshold.

.. _sim-sim-max-frame:

``--sim-max-frame, -n`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--sim-max-frame 1``

Maximum number of frames to play after what the current simulatated noise stops (0 is infinite).

.. _sim-sim-no-legend:

``--sim-no-legend`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""


Do not display any legend when launching the simulation.

.. _sim-sim-stop-time:

``--sim-stop-time`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--sim-stop-time 1``

Time in sec after what the current simulatated noise stops (0 is infinite).

