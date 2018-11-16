.. _ter-terminal-parameters:

Terminal parameters
-------------------

The Terminal is an observer module that reads the information to display
from the monitor in real time.

.. hint:: The intermediate results are dumped on the **error output** of the
   program when the final display is made on the **standard output**.
   So when launching AFF3CT you can redirect the last one to a file to save
   results and still seeing the intermediate reports on your terminal.
   See :ref:`simulation_overview` for more details about how to launch AFF3CT.

.. note:: Intermediate results of a same noise point are always displayed on the
   same line. They finish with a carrier return **\\r** character.

.. _ter-ter-freq:

``--ter-freq``
""""""""""""""

   :Type: integer
   :Default: 500
   :Examples: ``--ter-freq 1``

Sets the display frequency (refresh time) in *[ms]*, for each iteration.
Setting 0 disables the refresh to dump only the final result.

.. _ter-ter-no:

``--ter-no``
""""""""""""

Disables completely the terminal report.

.. _ter-ter-type:

``--ter-type``
""""""""""""""

   :Type: text
   :Allowed values: ``STD``
   :Default: ``STD``
   :Examples: ``--ter-type STD``

Type of the terminal to use to format the displayed results.

Description of the allowed values:

+---------+----------------------+
| Value   | Description          |
+=========+======================+
| ``STD`` | |ter-type_descr_std| |
+---------+----------------------+

.. |ter-type_descr_std| replace:: This is the standard format readable by
   :ref:`pyber`


