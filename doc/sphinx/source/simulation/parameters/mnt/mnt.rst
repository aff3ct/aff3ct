.. _mnt-monitor-parameters:

Monitor parameters
------------------

The Monitor is the last module in the chain as it compares the decoded frame
with the initially generated one in the Source module. Furthermore, it can
also compute the mutual information at the output of the demodulator.
It has also the power to limit the currently running simulated noise point as it
can stop it when a given amount of frame errors is reached. An histogram of the
number of errors per frame for each noise point can be dumped, too.


.. _mnt-mnt-max-fe:

``--mnt-max-fe, -e``
""""""""""""""""""""

   :Type: integer
   :Default: 100
   :Examples: ``--mnt-max-fe 25``

Sets the maximum number of frame errors simulated for each noise point.

.. _mnt-mnt-err-hist:

``--mnt-err-hist``
""""""""""""""""""

   :Type: integer
   :Examples: ``--mnt-err-hist 1``

Activates the histogram of the number of errors per frame. Sets also the maximum
number of bit error per frame included in the histogram (0 is no limit).

.. _mnt-mnt-err-hist-path:

``--mnt-err-hist-path``
"""""""""""""""""""""""

   :Type: file
   :Rights: write only
   :Default: :file:`./hist`
   :Examples: ``--mnt-err-hist-path my/histogram/root/path/name``

Path to the output histogram. When files are dumped, the current noise value
is added to this name with the *.txt* extension.

An output filename example is :file:`hist_2.000000.txt` for a noise value of
:math:`2\ dB`.
For `Gnuplot <http://www.gnuplot.info/>`_ users you can then simply display the
histogram with a simple command:

.. code-block:: bash

   gnuplot -e "set key autotitle columnhead; plot 'hist_2.000000.txt' with lines; pause -1"

.. _mnt-mnt-mutinfo:

``--mnt-mutinfo``
"""""""""""""""""

Activates the computation of the mutual information.

.. note:: Available only on ``BFER`` simulation types (:ref:`sim-sim-type`).