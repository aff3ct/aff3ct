.. _itl-interleaver-parameters:

Interleaver parameters
----------------------

.. _itl-itl-cols:

``--itl-cols``
""""""""""""""

   :type: integer
   :Examples: ``--itl-cols 1``

Specify the number of columns used for the RAND_COL, ROW_COL or COL_ROW interleaver.

.. _itl-itl-path:

``--itl-path``
""""""""""""""

   :type: file
   :Rights: read only
   :Examples: ``--itl-path example/path/to/the/right/file``

Specify the path to the interleaver file (to use with "\\-\\-itl-type USER").

.. _itl-itl-read-order:

``--itl-read-order``
""""""""""""""""""""

   :type: text
   :Allowed values: ``BOTTOM_LEFT`` ``BOTTOM_RIGHT`` ``TOP_LEFT`` ``TOP_RIGHT`` 
   :Examples: ``--itl-read-order BOTTOM_LEFT``

Read order of the COL_ROW and ROW_COL interleavers.

Description of the allowed values:

+------------------+-------------------------------------+
| Value            | Description                         |
+==================+=====================================+
| ``BOTTOM_LEFT``  | |itl-read-order_descr_bottom_left|  |
+------------------+-------------------------------------+
| ``BOTTOM_RIGHT`` | |itl-read-order_descr_bottom_right| |
+------------------+-------------------------------------+
| ``TOP_LEFT``     | |itl-read-order_descr_top_left|     |
+------------------+-------------------------------------+
| ``TOP_RIGHT``    | |itl-read-order_descr_top_right|    |
+------------------+-------------------------------------+

.. |itl-read-order_descr_bottom_left| replace:: TODO VALUE BOTTOM_LEFT
.. |itl-read-order_descr_bottom_right| replace:: TODO VALUE BOTTOM_RIGHT
.. |itl-read-order_descr_top_left| replace:: TODO VALUE TOP_LEFT
.. |itl-read-order_descr_top_right| replace:: TODO VALUE TOP_RIGHT


.. _itl-itl-type:

``--itl-type``
""""""""""""""

   :type: text
   :Allowed values: ``CCSDS`` ``COL_ROW`` ``DVB-RCS1`` ``DVB-RCS2`` ``GOLDEN`` ``LTE`` ``NO`` ``RANDOM`` ``RAND_COL`` ``ROW_COL`` ``USER`` 
   :Examples: ``--itl-type CCSDS``

Type of the interleaver to use in the simulation.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``CCSDS``    | |itl-type_descr_ccsds|    |
+--------------+---------------------------+
| ``COL_ROW``  | |itl-type_descr_col_row|  |
+--------------+---------------------------+
| ``DVB-RCS1`` | |itl-type_descr_dvb-rcs1| |
+--------------+---------------------------+
| ``DVB-RCS2`` | |itl-type_descr_dvb-rcs2| |
+--------------+---------------------------+
| ``GOLDEN``   | |itl-type_descr_golden|   |
+--------------+---------------------------+
| ``LTE``      | |itl-type_descr_lte|      |
+--------------+---------------------------+
| ``NO``       | |itl-type_descr_no|       |
+--------------+---------------------------+
| ``RANDOM``   | |itl-type_descr_random|   |
+--------------+---------------------------+
| ``RAND_COL`` | |itl-type_descr_rand_col| |
+--------------+---------------------------+
| ``ROW_COL``  | |itl-type_descr_row_col|  |
+--------------+---------------------------+
| ``USER``     | |itl-type_descr_user|     |
+--------------+---------------------------+

.. |itl-type_descr_ccsds| replace:: TODO VALUE CCSDS
.. |itl-type_descr_col_row| replace:: TODO VALUE COL_ROW
.. |itl-type_descr_dvb-rcs1| replace:: TODO VALUE DVB-RCS1
.. |itl-type_descr_dvb-rcs2| replace:: TODO VALUE DVB-RCS2
.. |itl-type_descr_golden| replace:: TODO VALUE GOLDEN
.. |itl-type_descr_lte| replace:: TODO VALUE LTE
.. |itl-type_descr_no| replace:: TODO VALUE NO
.. |itl-type_descr_random| replace:: TODO VALUE RANDOM
.. |itl-type_descr_rand_col| replace:: TODO VALUE RAND_COL
.. |itl-type_descr_row_col| replace:: TODO VALUE ROW_COL
.. |itl-type_descr_user| replace:: TODO VALUE USER


.. _itl-itl-uni:

``--itl-uni``
"""""""""""""


Enable the regeneration of the interleaver at each new frame.

