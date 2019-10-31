.. _mnt-monitor-parameters:

Monitor parameters
------------------

The monitor is the last module in the chain: **it compares the decoded
information bits with the initially generated ones from the source**.
Furthermore, it can also compute **the mutual information** (|MI|) from the
demodulator output.

.. _mnt-mnt-max-fe:

``--mnt-max-fe, -e``
""""""""""""""""""""

   :Type: integer
   :Default: 100
   :Examples: ``--mnt-max-fe 25``

|factory::Monitor_BFER::p+max-fe,e|

.. _mnt-mnt-err-hist:

``--mnt-err-hist``
""""""""""""""""""

   :Type: integer
   :Examples: ``--mnt-err-hist 0``

|factory::Monitor_BFER::p+err-hist|

The histogram is saved in |CSV| format:

.. code-block:: bash

   "Number of error bits per wrong frame"; "Histogram (noise: 5.000000dB, on 10004 frames)"
   0; 0
   1; 7255
   2; 2199
   3; 454
   4; 84
   5; 11
   6; 12

.. _mnt-mnt-err-hist-path:

``--mnt-err-hist-path``
"""""""""""""""""""""""

   :Type: file
   :Rights: write only
   :Default: :file:`./hist`
   :Examples: ``--mnt-err-hist-path my/histogram/root/path/name``

|factory::Monitor_BFER::p+err-hist-path|

An output filename example is :file:`hist_2.000000.txt` for a noise value of
2 dB. For `Gnuplot <http://www.gnuplot.info/>`_ users you can then simply
display the histogram with the following command:

.. code-block:: bash

   gnuplot -e "set key autotitle columnhead; plot 'hist_2.000000.txt' with lines; pause -1"

.. _mnt-mnt-mutinfo:

``--mnt-mutinfo``
"""""""""""""""""

|factory::BFER::p+mutinfo|

.. note:: Only available on ``BFER`` simulation types (see the
   :ref:`sim-sim-type` parameter for more details).

.. _mnt-mnt-red-lazy:

``--mnt-red-lazy``
"""""""""""""""""""""""

|factory::BFER::p+red-lazy|

Using this parameter can significantly reduce the simulation time, especially
for short frame sizes when the monitor synchronizations happen very often.

.. note:: This parameter is not available if the code has been compiled with
   |MPI|.

.. note:: By default, if the :ref:`mnt-mnt-red-lazy-freq` parameter is not
   specified, the interval/frequency is set to the same value than the
   :ref:`ter-ter-freq` parameter.

.. warning:: Be careful, this parameter is known to alter the behavior of the
   :ref:`sim-sim-max-fra` parameter.

.. _mnt-mnt-red-lazy-freq:

``--mnt-red-lazy-freq``
"""""""""""""""""""""""""""

   :Type: integer
   :Default: 1000
   :Examples: ``--mnt-red-lazy-freq 200``

|factory::BFER::p+red-lazy-freq|

.. note:: This parameter automatically enables the :ref:`mnt-mnt-red-lazy`
   parameter.

.. note:: This parameter is not available if the code has been compiled with
   |MPI|.

.. _mnt-mnt-mpi-comm-freq:

``--mnt-mpi-comm-freq``
"""""""""""""""""""""""

   :Type: integer
   :Default: 1000
   :Examples: ``--mnt-mpi-comm-freq 1``

|factory::BFER::p+mpi-comm-freq|

.. note:: Available only when compiling with the |MPI| support
   :ref:`compilation_cmake_options`.

.. note:: When this parameter is specified, the :ref:`ter-ter-freq` parameter
   is automatically set to the same value except if the :ref:`ter-ter-freq` is
   explicitly defined.

.. TODO: add link to MPI use