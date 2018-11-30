.. _dec-turbo-decoder-parameters:

Turbo Decoder parameters
------------------------

.. _dec-turbo-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``TURBO`` ``CHASE`` ``ML``
   :Examples: ``--dec-type CHASE``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``TURBO`` | |dec-type_descr_turbo| |
+-----------+------------------------+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+

.. |dec-type_descr_turbo| replace:: TODO VALUE TURBO
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

.. _dec-turbo-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``STD`` ``FAST``
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+
| ``FAST``  | |dec-implem_descr_fast|  |
+-----------+--------------------------+

.. |dec-implem_descr_std| replace:: TODO VALUE STD
.. |dec-implem_descr_fast| replace:: TODO VALUE FAST

.. _dec-turbo-dec-sub-type:

``--dec-sub-type, -D``
""""""""""""""""""""""

   :Type: text
   :Allowed values: ``BCJR`` ``CHASE`` ``ML``
   :Examples: ``--dec-sub-type BCJR``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+----------------------------+
| Value     | Description                |
+===========+============================+
| ``BCJR``  | |dec-sub-type_descr_bcjr|  |
+-----------+----------------------------+
| ``CHASE`` | |dec-sub-type_descr_chase| |
+-----------+----------------------------+
| ``ML``    | |dec-sub-type_descr_ml|    |
+-----------+----------------------------+

.. |dec-sub-type_descr_bcjr| replace:: TODO VALUE BCJR
.. |dec-sub-type_descr_chase| replace:: See the common
   :ref:`dec-common-dec-type` parameter.
.. |dec-sub-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

.. _dec-turbo-dec-sub-implem:

``--dec-sub-implem``
""""""""""""""""""""

   :Type: text
   :Allowed values: ``GENERIC`` ``STD`` ``FAST`` ``VERY_FAST``
   :Examples: ``--dec-sub-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+---------------+----------------------------------+
| Value         | Description                      |
+===============+==================================+
| ``GENERIC``   | |dec-sub-implem_descr_generic|   |
+---------------+----------------------------------+
| ``STD``       | |dec-sub-implem_descr_std|       |
+---------------+----------------------------------+
| ``FAST``      | |dec-sub-implem_descr_fast|      |
+---------------+----------------------------------+
| ``VERY_FAST`` | |dec-sub-implem_descr_very_fast| |
+---------------+----------------------------------+

.. |dec-sub-implem_descr_generic| replace:: TODO VALUE GENERIC
.. |dec-sub-implem_descr_std| replace:: TODO VALUE STD
.. |dec-sub-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-sub-implem_descr_very_fast| replace:: TODO VALUE VERY_FAST

.. _dec-turbo-dec-sub-simd:

``--dec-sub-simd``
""""""""""""""""""

   :Type: text
   :Allowed values: ``INTER`` ``INTRA``
   :Examples: ``--dec-sub-simd INTER``

The |SIMD| strategy you want to use.

Description of the allowed values:

+-----------+----------------------------+
| Value     | Description                |
+===========+============================+
| ``INTER`` | |dec-sub-simd_descr_inter| |
+-----------+----------------------------+
| ``INTRA`` | |dec-sub-simd_descr_intra| |
+-----------+----------------------------+

.. |dec-sub-simd_descr_inter| replace:: TODO VALUE INTER
.. |dec-sub-simd_descr_intra| replace:: TODO VALUE INTRA

.. _dec-turbo-dec-fnc:

``--dec-fnc``
"""""""""""""

Enables the flip and check decoder (requires "\\-\\-crc-type").

.. _dec-turbo-dec-fnc-crc-ite:

``--dec-fnc-crc-ite``
"""""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-fnc-crc-ite 1``

Set the iteration to start the CRC checking.

.. _dec-turbo-dec-fnc-ite-max:

``--dec-fnc-ite-M``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-fnc-ite-M 1``

Set last iteration at which the fnc is used.

.. _dec-turbo-dec-fnc-ite-min:

``--dec-fnc-ite-m``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-fnc-ite-m 1``

Set first iteration at which the fnc is used.

.. _dec-turbo-dec-fnc-ite-s:

``--dec-fnc-ite-s``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-fnc-ite-s 1``

Set iteration step for the fnc algorithm.

.. _dec-turbo-dec-fnc-q:

``--dec-fnc-q``
"""""""""""""""

   :Type: integer
   :Examples: ``--dec-fnc-q 1``

Set the search's space for the fnc algorithm.

.. _dec-turbo-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Examples: ``--dec-ite 1``

Maximal number of iterations in the turbo.

.. _dec-turbo-dec-sc:

``--dec-sc``
""""""""""""

Enables the self corrected decoder (requires "\\-\\-crc-type").

.. _dec-turbo-dec-sf-type:

``--dec-sf-type``
"""""""""""""""""

   :Type: text
   :Allowed values: ``ADAPTIVE`` ``ARRAY`` ``CST`` ``LTE`` ``LTE_VEC``
   :Examples: ``--dec-sf-type ADAPTIVE``

Scaling factor type.

Description of the allowed values:

+--------------+------------------------------+
| Value        | Description                  |
+==============+==============================+
| ``ADAPTIVE`` | |dec-sf-type_descr_adaptive| |
+--------------+------------------------------+
| ``ARRAY``    | |dec-sf-type_descr_array|    |
+--------------+------------------------------+
| ``CST``      | |dec-sf-type_descr_cst|      |
+--------------+------------------------------+
| ``LTE``      | |dec-sf-type_descr_lte|      |
+--------------+------------------------------+
| ``LTE_VEC``  | |dec-sf-type_descr_lte_vec|  |
+--------------+------------------------------+

.. |dec-sf-type_descr_adaptive| replace:: TODO VALUE ADAPTIVE
.. |dec-sf-type_descr_array| replace:: TODO VALUE ARRAY
.. |dec-sf-type_descr_cst| replace:: TODO VALUE CST
.. |dec-sf-type_descr_lte| replace:: TODO VALUE LTE
.. |dec-sf-type_descr_lte_vec| replace:: TODO VALUE LTE_VEC

.. _dec-turbo-dec-sub-max:

``--dec-sub-max``
"""""""""""""""""

   :Type: text
   :Allowed values: ``MAX`` ``MAXL`` ``MAXS``
   :Examples: ``--dec-sub-max MAX``

The MAX implementation for the nodes.

Description of the allowed values:

+----------+--------------------------+
| Value    | Description              |
+==========+==========================+
| ``MAX``  | |dec-sub-max_descr_max|  |
+----------+--------------------------+
| ``MAXL`` | |dec-sub-max_descr_maxl| |
+----------+--------------------------+
| ``MAXS`` | |dec-sub-max_descr_maxs| |
+----------+--------------------------+

.. |dec-sub-max_descr_max| replace:: TODO VALUE MAX
.. |dec-sub-max_descr_maxl| replace:: TODO VALUE MAXL
.. |dec-sub-max_descr_maxs| replace:: TODO VALUE MAXS