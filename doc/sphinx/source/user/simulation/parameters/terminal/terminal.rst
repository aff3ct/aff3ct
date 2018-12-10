.. _ter-terminal-parameters:

Terminal parameters
-------------------

The terminal is an observer module that reads and display the monitor
informations in real time. The terminal displays two types of results:
**intermediate results** and **final results**. The intermediate results are
printed on the **error output** during the simulation of a noise point and
refreshed at a defined frequency (see the :ref:`ter-ter-freq` parameter). On the
other hand, the final results are printed on the **standard output** once the
simulation of the noise point is over.

.. _ter-ter-type:

``--ter-type``
""""""""""""""

   :Type: text
   :Allowed values: ``STD``
   :Default: ``STD``
   :Examples: ``--ter-type STD``

Select the terminal type (the format to display the results).

Description of the allowed values:

+---------+----------------------+
| Value   | Description          |
+=========+======================+
| ``STD`` | |ter-type_descr_std| |
+---------+----------------------+

.. |ter-type_descr_std| replace:: Select the standard format.

.. note:: For more details on the standard output format see the
   :ref:`user_simulation_overview_output` section).

.. _ter-ter-freq:

``--ter-freq``
""""""""""""""

   :Type: integer
   :Default: 500
   :Examples: ``--ter-freq 1``

Set the display frequency (refresh time) of the intermediate results in
milliseconds. Setting 0 disables the display of the intermediate results.

.. note:: When |MPI| is enabled, this value is by default set to the same value
   than the :ref:`sim-sim-mpi-comm` parameter.

.. _ter-ter-no:

``--ter-no``
""""""""""""

Disable completely the terminal report.