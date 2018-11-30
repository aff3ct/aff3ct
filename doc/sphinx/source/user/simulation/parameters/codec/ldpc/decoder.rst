.. _dec-ldpc-decoder-parameters:

Decoder parameters
------------------

.. _dec-ldpc-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``BIT_FLIPPING`` ``BP_FLOODING`` ``BP_HORIZONTAL_LAYERED``
                    ``BP_PEELING`` ``BP_VERTICAL_LAYERED`` ``CHASE`` ``ML``
   :Default: ``BP_FLOODING``
   :Examples: ``--dec-type BP_HORIZONTAL_LAYERED``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+---------------------------+---------------------------------------------------------------+
| Value                     | Description                                                   |
+===========================+===============================================================+
| ``BIT_FLIPPING``          | The Bit Flipping algorithm.                                   |
+---------------------------+---------------------------------------------------------------+
| ``BP_FLOODING``           | The Belief Propagation Flooding algorithm.                    |
+---------------------------+---------------------------------------------------------------+
| ``BP_HORIZONTAL_LAYERED`` | The Belief Propagation Horizontal Layered algorithm.          |
+---------------------------+---------------------------------------------------------------+
| ``BP_VERTICAL_LAYERED``   | The Belief Propagation Vertical Layered algorithm.            |
+---------------------------+---------------------------------------------------------------+
| ``BP_PEELING``            | The Belief Propagation Peeling algorithm :cite:`Peeling2001`. |
+---------------------------+---------------------------------------------------------------+
| ``CHASE``                 | See the common :ref:`dec-common-dec-type` parameter.          |
+---------------------------+---------------------------------------------------------------+
| ``ML``                    | See the common :ref:`dec-common-dec-type` parameter.          |
+---------------------------+---------------------------------------------------------------+


.. TODO: BP_HORIZONTAL_LAYERED_LEGACY and __cpp_aligned_new


.. _dec-ldpc-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``AMS`` ``GALA`` ``LSPA`` ``MS`` ``NMS`` ``OMS``
                    ``SPA`` ``STD`` ``WBF``
   :Default: ``SPA``
   :Examples: ``--dec-implem AMS``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``AMS``   | |dec-implem_descr_ams|   |
+-----------+--------------------------+
| ``GALA``  | |dec-implem_descr_gala|  |
+-----------+--------------------------+
| ``MS``    | |dec-implem_descr_ms|    |
+-----------+--------------------------+
| ``NMS``   | |dec-implem_descr_nms|   |
+-----------+--------------------------+
| ``OMS``   | |dec-implem_descr_oms|   |
+-----------+--------------------------+
| ``SPA``   | |dec-implem_descr_spa|   |
+-----------+--------------------------+
| ``LSPA``  | |dec-implem_descr_lspa|  |
+-----------+--------------------------+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+
| ``WBF``   | |dec-implem_descr_wbf|   |
+-----------+--------------------------+

.. |dec-implem_descr_ams|   replace:: The Approximate Min Star algorithm.
.. |dec-implem_descr_gala|  replace:: The Gallager A algorithm.
.. |dec-implem_descr_ms|    replace:: The Min-Sum algorithm.
.. |dec-implem_descr_nms|   replace:: The Normalized Min-Sum algorithm.
.. |dec-implem_descr_oms|   replace:: The Offset Min-Sum algorithm.
.. |dec-implem_descr_spa|   replace:: The Sum-Product Algorithm.
.. |dec-implem_descr_lspa|  replace:: The Log Sum-Product Algorithm.
.. |dec-implem_descr_std|   replace:: A standard implementation of the decoder.
.. |dec-implem_descr_wbf|   replace:: The Weighted Bit Flipping algorithm


The ``MS``, ``NMS`` and ``OMS`` implementations come from the *Offset-Normalized
Min-Sum* algorithm. The ``MS`` force the norm to 1 and the offset to 0.
The ``NMS`` leaves the norm free to the user to choose, when it is the offset
for the ``OMS``.



The following table shows the different decoder types and their matching
implementation when no |SIMD| strategy is used:

+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| Value                     | ``AMS``|``GALA``|``MS``|``NMS``|``OMS``|``SPA``|``LSPA``|``STD``|``WBF`` |
+===========================+========+========+======+=======+=======+=======+========+=======+========+
| ``BIT_FLIPPING``          |        |        |      |       |       |       |        |       |  |OK|  |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| ``BP_FLOODING``           | |OK|   |  |OK|  | |OK| | |OK|  | |OK|  | |OK|  |  |OK|  |       |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| ``BP_HORIZONTAL_LAYERED`` | |OK|   |        | |OK| | |OK|  | |OK|  | |OK|  |  |OK|  |       |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| ``BP_VERTICAL_LAYERED``   | |OK|   |        | |OK| | |OK|  | |OK|  | |OK|  |  |OK|  |       |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| ``BP_PEELING``            |        |        |      |       |       |       |        |  |OK| |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+

The following table shows the different decoder types and their matching
implementation when the ``INTER`` |SIMD| strategy is used:

+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| Value                     | ``AMS``|``GALA``|``MS``|``NMS``|``OMS``|``SPA``|``LSPA``|``STD``|``WBF`` |
+===========================+========+========+======+=======+=======+=======+========+=======+========+
| ``BIT_FLIPPING``          |        |        |      |       |       |       |        |       |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| ``BP_FLOODING``           |        |        |      |       |       |       |        |       |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| ``BP_HORIZONTAL_LAYERED`` |        |        | |OK| | |OK|  | |OK|  |       |        |       |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| ``BP_VERTICAL_LAYERED``   |        |        |      |       |       |       |        |       |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+
| ``BP_PEELING``            |        |        |      |       |       |       |        |       |        |
+---------------------------+--------+--------+------+-------+-------+-------+--------+-------+--------+

.. |OK| replace:: OK

.. _dec-ldpc-dec-simd:

``--dec-simd``
""""""""""""""

   :Type: text
   :Allowed values: ``INTER``
   :Examples: ``--dec-simd INTER``

Select the |SIMD| strategy you want to use to accelerate significantly
the simulation.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``INTER`` | |dec-simd_descr_inter| |
+-----------+------------------------+

.. |dec-simd_descr_inter| replace:: The decoder processes several frames at the
   same time.


.. _dec-ldpc-dec-h-path:

``--dec-h-path`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--dec-h-path conf/dec/LDPC/AR4JA_4096_8192.qc``
              ``--dec-h-path conf/dec/LDPC/MACKAY_504_1008.alist``

Give the path to the H matrix. Supported format are AList and QC files.

This argument is not needed if the encoder type :ref:`enc-ldpc-enc-type`
is ``LDPC_DVBS2``.

.. TODO: info bits pos at the end of .alist file
         puncturer pattern at the end of QC file


.. _dec-ldpc-dec-h-reorder:

``--dec-h-reorder``
"""""""""""""""""""

   :Type: text
   :Allowed values: ``ASC`` ``DSC`` ``NONE``
   :Default: ``NONE``
   :Examples: ``--dec-h-reorder ASC``

Specify if the check nodes (CNs) from H have to be reordered.

Description of the allowed values:

+----------+----------------------------+
| Value    | Description                |
+==========+============================+
| ``ASC``  | |dec-h-reorder_descr_asc|  |
+----------+----------------------------+
| ``DSC``  | |dec-h-reorder_descr_dsc|  |
+----------+----------------------------+
| ``NONE`` | |dec-h-reorder_descr_none| |
+----------+----------------------------+

.. |dec-h-reorder_descr_asc|  replace:: Reorder from the smallest to the biggest
   CNs.
.. |dec-h-reorder_descr_dsc|  replace:: Reorder from the biggest to the smallest
   CNs.
.. |dec-h-reorder_descr_none| replace:: Do not change the order.

.. _dec-ldpc-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Default: 10
   :Examples: ``--dec-ite 30``

Set the maximal number of iterations in the LDPC decoder.

.. note:: The syndrome is used in order to accelerate the simulation
   by stopping the decoder process before the end of all its iterations.
   The decoder may then often not realize all the planned iterations.
   Use :ref:`dec-ldpc-dec-no-synd` to deactivate this behavior.

.. _dec-ldpc-dec-min:

``--dec-min``
"""""""""""""

   :Type: text
   :Allowed values: ``MIN`` ``MINL`` ``MINS``
   :Default: ``MINL``
   :Examples: ``--dec-min MIN``

Give the *MIN* implementation for the ``AMS`` decoder nodes.

Description of the allowed values:

+----------+----------------------+
| Value    | Description          |
+==========+======================+
| ``MINS`` | |dec-min_descr_mins| |
+----------+----------------------+
| ``MINL`` | |dec-min_descr_minl| |
+----------+----------------------+
| ``MIN``  | |dec-min_descr_min|  |
+----------+----------------------+

.. |dec-min_descr_mins| replace:: :math:`\min^*(a,b) = \min(a,b) +
   \log(1 + \exp(-(a + b))) - \log(1 + \exp(-|a - b|))`.
.. |dec-min_descr_minl| replace:: :math:`\min^*(a,b) \approx \min(a,b) +
   corr(a + b) - corr(|a + b|)` with :math:`corr(x) = \begin{cases}
   0                  & \text{if } x >= 2.625\\
   -0.375  x + 0.6825 & \text{if } x < 1.0 \\
   -0.1875 x + 0.5    & \text{else}
   \end{cases}`.
.. |dec-min_descr_min|  replace:: :math:`\min^*(a,b) \approx \min(a,b)`.

``MINS`` for *Min Star* is the exact :math:`\min^*` operator.
``MINL`` for *Min Linear* is a linear approximation of the :math:`\min^*`
function. ``MIN`` for *Min* is the simplest approximation with
only a :math:`\min` function.


.. _dec-ldpc-dec-norm:

``--dec-norm``
""""""""""""""

   :Type: real number
   :Default: 1.0
   :Examples: ``--dec-norm 0.75``

Set the normalization factor used in the ``NMS`` algorithm.

.. _dec-ldpc-dec-off:

``--dec-off``
"""""""""""""

   :Type: real number
   :Default: 0.0
   :Examples: ``--dec-off 0.25``

Set the offset used in the ``OMS`` algorithm.

.. _dec-ldpc-dec-mwbf:

``--dec-mwbf``
""""""""""""""

   :Type: real number
   :Default: 0.0
   :Examples: ``--dec-mwbf 1.0``

Give the factor used in the modified ``WBF`` algorithm. Set 0 for basic WBF.

.. _dec-ldpc-dec-synd-depth:

``--dec-synd-depth``
""""""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--dec-synd-depth 2``

It helps to avoid the detection of false positive syndrome by ensuring that it
is valid for several consecutive iterations.

.. _dec-ldpc-dec-no-synd:

``--dec-no-synd``
"""""""""""""""""

Disable the syndrome detection and so the stop criterion iteration after
iteration.


References
""""""""""

.. bibliography:: references.bib