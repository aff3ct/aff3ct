.. _itl-interleaver-parameters:

Interleaver parameters
----------------------

The interleaving process is frequent in coding schemes. It can be found directly
in the code definition (for instance in Turbo or Turbo Product codes) or in
larger schemes like for the turbo demodulation in the receiver (see the
iterative |BFER| chain in :numref:`fig_bferi`).

.. _itl-itl-type:

``--itl-type``
""""""""""""""

   :Type: text
   :Allowed values: ``CCSDS`` ``COL_ROW`` ``DVB-RCS1`` ``DVB-RCS2`` ``GOLDEN``
                    ``LTE`` ``NO`` ``RANDOM`` ``RAND_COL`` ``ROW_COL`` ``USER``
   :Default: ``RANDOM``
   :Examples: ``--itl-type RANDOM``

Select the interleaver type.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``NO``       | |itl-type_descr_no|       |
+--------------+---------------------------+
| ``COL_ROW``  | |itl-type_descr_col_row|  |
+--------------+---------------------------+
| ``ROW_COL``  | |itl-type_descr_row_col|  |
+--------------+---------------------------+
| ``RANDOM``   | |itl-type_descr_random|   |
+--------------+---------------------------+
| ``RAND_COL`` | |itl-type_descr_rand_col| |
+--------------+---------------------------+
| ``GOLDEN``   | Select the interleaver    |
|              | described in              |
|              | :cite:`Crozier1999`.      |
+--------------+---------------------------+
| ``CCSDS``    | |itl-type_descr_ccsds|    |
+--------------+---------------------------+
| ``LTE``      | |itl-type_descr_lte|      |
+--------------+---------------------------+
| ``DVB-RCS1`` | |itl-type_descr_dvb-rcs1| |
+--------------+---------------------------+
| ``DVB-RCS2`` | |itl-type_descr_dvb-rcs2| |
+--------------+---------------------------+
| ``USER``     | |itl-type_descr_user|     |
+--------------+---------------------------+

.. |itl-type_descr_no|       replace:: Disable the interleaving process: the
   output is the input (:numref:`fig_itl_no`).
.. |itl-type_descr_col_row|  replace:: Fill the interleaver by column, read it
   by row (can be customized with the :ref:`itl-itl-read-order` parameter)
   (:numref:`fig_itl_col_row`).
.. |itl-type_descr_row_col|  replace:: Fill the interleaver by row, read it by
   column (can be customized with the :ref:`itl-itl-read-order` parameter)
   (:numref:`fig_itl_row_col`)
.. |itl-type_descr_random|   replace:: Generate a random sequence for the entire
   frame (based on the |MT 19937| |PRNG|) (:numref:`fig_itl_random`).
.. |itl-type_descr_rand_col| replace:: Generate multiple random sequences
   decomposed in independent columns (based on the |MT 19937| |PRNG|)
   (:numref:`fig_itl_rand_col`).
.. |itl-type_descr_ccsds|    replace:: Select the interleaver defined in the
   |CCSDS| standard.
.. |itl-type_descr_lte|      replace:: Select the interleaver defined in the
   |LTE| standard.
.. |itl-type_descr_dvb-rcs1| replace:: Select the interleaver defined in the
   |DVB-RCS1| standard.
.. |itl-type_descr_dvb-rcs2| replace:: Select the interleaver defined in the
   |DVB-RCS2| standard.
.. |itl-type_descr_user|     replace:: Select the interleaver sequence (|LUT|)
   from an external file (to use with the :ref:`itl-itl-path` parameter)
   (:numref:`fig_itl_user`).

.. _fig_itl_no:

.. figure:: images/itl_no.png
   :align: center

   Interleaver ``NO``.

.. _fig_itl_col_row:

.. figure:: images/itl_col_row.png
   :align: center

   Interleaver ``COL_ROW``.

.. _fig_itl_row_col:

.. figure:: images/itl_row_col.png
   :align: center

   Interleaver ``ROW_COL``.

.. _fig_itl_random:

.. figure:: images/itl_random.png
   :align: center

   Interleaver ``RANDOM``.

.. _fig_itl_rand_col:

.. figure:: images/itl_rand_col.png
   :align: center

   Interleaver ``RAND_COL``.

.. _fig_itl_user:

.. figure:: images/itl_user.png
   :align: center

   Interleaver ``USER``.

.. _itl-itl-cols:

``--itl-cols``
""""""""""""""

   :Type: integer
   :Default: 4
   :Examples: ``--itl-cols 1``

Specify the number of columns used for the ``RAND_COL``, ``ROW_COL`` or
``COL_ROW`` interleavers.

.. _itl-itl-path:

``--itl-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--itl-path ../conf/itl/GSM-LDPC_4224.itl``

Set the file path to the interleaver |LUT| (to use with the ``USER``
interleaver).

An |ASCII| file is expected:

.. code-block:: console

   1

   16

   15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0

The first value ``1`` is the number of |LUTs| contained in the file, here there
is only one interleaved sequence.
The second value ``16`` is the frame size.
The last line is the interleaved sequence description. Here the frame is
reversed, the element ``0`` will take the place of the element ``15``, the
element ``1`` will take the place of the element ``14``, etc.

If there is more than one interleaved sequence then for each new frame a new
|LUT| is used in the natural order given by the file. Here is an example with
two |LUTs|:

.. code-block:: console

   2

   16

   15 14 13 12 11 10 9 8  7  6  5  4  3  2 1 0
    7  6  5  4  3  2 1 0 15 14 13 12 11 10 9 8

.. note:: When the number of simulated frames exceeds the number of |LUT|
   contained in the files, the |LUTs| from the beginning of the file are reused
   and this is repeated until the end of the simulation.

.. _itl-itl-read-order:

``--itl-read-order``
""""""""""""""""""""

   :Type: text
   :Allowed values: ``BOTTOM_LEFT`` ``BOTTOM_RIGHT`` ``TOP_LEFT`` ``TOP_RIGHT``
   :Examples: ``--itl-read-order BOTTOM_LEFT``

Change the read order of the ``COL_ROW`` and ``ROW_COL`` interleavers.
The read starts from the given corner of the array to the diagonally opposite
one. The read is made row by row for the ``COL_ROW`` interleaver and column by
column for the ``ROW_COL`` one.

Description of the allowed values (see also the figures just bellow):

+------------------+-------------------------------------+
| Value            | Description                         |
+==================+=====================================+
| ``TOP_LEFT``     | |itl-read-order_descr_top_left|     |
+------------------+-------------------------------------+
| ``TOP_RIGHT``    | |itl-read-order_descr_top_right|    |
+------------------+-------------------------------------+
| ``BOTTOM_LEFT``  | |itl-read-order_descr_bottom_left|  |
+------------------+-------------------------------------+
| ``BOTTOM_RIGHT`` | |itl-read-order_descr_bottom_right| |
+------------------+-------------------------------------+

.. |itl-read-order_descr_top_left|     replace:: Read is down from the top left
   corner to the bottom right corner.
.. |itl-read-order_descr_top_right|    replace:: Read is down from the top right
   corner to the bottom left corner.
.. |itl-read-order_descr_bottom_left|  replace:: Read is down from the bottom
   left corner to the top right corner.
.. |itl-read-order_descr_bottom_right| replace:: Read is down from the bottom
   right corner to the top left corner.

:numref:`fig_itl_col_row_order` depicts the read order options on the
``COL_ROW`` interleaver.

.. _fig_itl_col_row_order:

.. figure:: images/itl_col_row_order.png
   :align: center
   :scale: 50

   Interleaver ``COL_ROW`` read orders.

:numref:`fig_itl_row_col_order` depicts the read order options on the
``ROW_COL`` interleaver.

.. _fig_itl_row_col_order:

.. figure:: images/itl_row_col_order.png
   :align: center
   :scale: 65

   Interleaver ``COL_ROW`` read orders.


.. _itl-itl-uni:

``--itl-uni``
"""""""""""""

Enable to generate a new |LUT| *for each new frame* (i.e. uniform interleaver).
By default, if this parameter is not used, the random interleavers generate the
|LUT| only once for the whole simulation.

.. note:: This parameter has no effect if the selected interleaver is not
   randomly generated.

References
""""""""""

.. bibliography:: references.bib