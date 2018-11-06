.. _dec-ldpc-decoder-parameters:

Decoder parameters
------------------

.. _dec-ldpc-dec-h-path:

``--dec-h-path`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""

   :type: file
   :Rights: read only
   :Examples: ``--dec-h-path example/path/to/the/right/file``

Path to the H matrix (AList or QC formated file).

.. _dec-ldpc-dec-flips:

``--dec-flips``
"""""""""""""""

   :type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-ldpc-dec-h-reorder:

``--dec-h-reorder``
"""""""""""""""""""

   :type: text
   :Allowed values: ``ASC`` ``DSC`` ``NONE`` 
   :Examples: ``--dec-h-reorder ASC``

Specify if the check nodes (CNs) from H have to be reordered, 'NONE': do nothing (default), 'ASC': from the smallest to the biggest CNs, 'DSC': from the biggest to the smallest CNs.

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

.. |dec-h-reorder_descr_asc| replace:: TODO VALUE ASC
.. |dec-h-reorder_descr_dsc| replace:: TODO VALUE DSC
.. |dec-h-reorder_descr_none| replace:: TODO VALUE NONE


.. _dec-ldpc-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-ldpc-dec-implem:

``--dec-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``AMS`` ``GALA`` ``LSPA`` ``MS`` ``NAIVE`` ``NMS`` ``OMS`` ``SPA`` ``STD`` ``WBF`` 
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
| ``LSPA``  | |dec-implem_descr_lspa|  |
+-----------+--------------------------+
| ``MS``    | |dec-implem_descr_ms|    |
+-----------+--------------------------+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+
| ``NMS``   | |dec-implem_descr_nms|   |
+-----------+--------------------------+
| ``OMS``   | |dec-implem_descr_oms|   |
+-----------+--------------------------+
| ``SPA``   | |dec-implem_descr_spa|   |
+-----------+--------------------------+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+
| ``WBF``   | |dec-implem_descr_wbf|   |
+-----------+--------------------------+

.. |dec-implem_descr_ams| replace:: TODO VALUE AMS
.. |dec-implem_descr_gala| replace:: TODO VALUE GALA
.. |dec-implem_descr_lspa| replace:: TODO VALUE LSPA
.. |dec-implem_descr_ms| replace:: TODO VALUE MS
.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_nms| replace:: TODO VALUE NMS
.. |dec-implem_descr_oms| replace:: TODO VALUE OMS
.. |dec-implem_descr_spa| replace:: TODO VALUE SPA
.. |dec-implem_descr_std| replace:: TODO VALUE STD
.. |dec-implem_descr_wbf| replace:: TODO VALUE WBF


.. _dec-ldpc-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :type: integer
   :Examples: ``--dec-ite 1``

Maximal number of iterations in the LDPC decoder.

.. _dec-ldpc-dec-min:

``--dec-min``
"""""""""""""

   :type: text
   :Allowed values: ``MIN`` ``MINL`` ``MINS`` 
   :Examples: ``--dec-min MIN``

The MIN implementation for the nodes (AMS decoder).

Description of the allowed values:

+----------+----------------------+
| Value    | Description          |
+==========+======================+
| ``MIN``  | |dec-min_descr_min|  |
+----------+----------------------+
| ``MINL`` | |dec-min_descr_minl| |
+----------+----------------------+
| ``MINS`` | |dec-min_descr_mins| |
+----------+----------------------+

.. |dec-min_descr_min| replace:: TODO VALUE MIN
.. |dec-min_descr_minl| replace:: TODO VALUE MINL
.. |dec-min_descr_mins| replace:: TODO VALUE MINS


.. _dec-ldpc-dec-mwbf:

``--dec-mwbf``
""""""""""""""

   :type: real number
   :Examples: ``--dec-mwbf 1.0``

Factor used in the modified WBF algorithm (works only with "\\-\\-dec-implem WBF"). Set 0 for basic WBF.

.. _dec-ldpc-dec-no-synd:

``--dec-no-synd``
"""""""""""""""""


Disable the syndrome detection (disable the stop criterion in the LDPC decoders).

.. _dec-ldpc-dec-norm:

``--dec-norm``
""""""""""""""

   :type: real number
   :Examples: ``--dec-norm 1.0``

Normalization factor used in the normalized min-sum BP algorithm (works only with "\\-\\-dec-implem NMS").

.. _dec-ldpc-dec-off:

``--dec-off``
"""""""""""""

   :type: real number
   :Examples: ``--dec-off 1.0``

Offset used in the offset min-sum BP algorithm (works only with "\\-\\-dec-implem NMS").

.. _dec-ldpc-dec-simd:

``--dec-simd``
""""""""""""""

   :type: text
   :Allowed values: ``INTER`` 
   :Examples: ``--dec-simd INTER``

The SIMD strategy you want to use.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``INTER`` | |dec-simd_descr_inter| |
+-----------+------------------------+

.. |dec-simd_descr_inter| replace:: TODO VALUE INTER


.. _dec-ldpc-dec-synd-depth:

``--dec-synd-depth``
""""""""""""""""""""

   :type: integer
   :Examples: ``--dec-synd-depth 1``

Successive number of iterations to validate the syndrome detection.

.. _dec-ldpc-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :type: text
   :Allowed values: ``BIT_FLIPPING`` ``BP_FLOODING`` ``BP_HORIZONTAL_LAYERED`` ``BP_PEELING`` ``BP_VERTICAL_LAYERED`` ``CHASE`` ``ML`` 
   :Examples: ``--dec-type BIT_FLIPPING``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+---------------------------+----------------------------------------+
| Value                     | Description                            |
+===========================+========================================+
| ``BIT_FLIPPING``          | |dec-type_descr_bit_flipping|          |
+---------------------------+----------------------------------------+
| ``BP_FLOODING``           | |dec-type_descr_bp_flooding|           |
+---------------------------+----------------------------------------+
| ``BP_HORIZONTAL_LAYERED`` | |dec-type_descr_bp_horizontal_layered| |
+---------------------------+----------------------------------------+
| ``BP_PEELING``            | |dec-type_descr_bp_peeling|            |
+---------------------------+----------------------------------------+
| ``BP_VERTICAL_LAYERED``   | |dec-type_descr_bp_vertical_layered|   |
+---------------------------+----------------------------------------+
| ``CHASE``                 | |dec-type_descr_chase|                 |
+---------------------------+----------------------------------------+
| ``ML``                    | |dec-type_descr_ml|                    |
+---------------------------+----------------------------------------+

.. |dec-type_descr_bit_flipping| replace:: TODO VALUE BIT_FLIPPING
.. |dec-type_descr_bp_flooding| replace:: TODO VALUE BP_FLOODING
.. |dec-type_descr_bp_horizontal_layered| replace:: TODO VALUE BP_HORIZONTAL_LAYERED
.. |dec-type_descr_bp_peeling| replace:: TODO VALUE BP_PEELING
.. |dec-type_descr_bp_vertical_layered| replace:: TODO VALUE BP_VERTICAL_LAYERED
.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML


