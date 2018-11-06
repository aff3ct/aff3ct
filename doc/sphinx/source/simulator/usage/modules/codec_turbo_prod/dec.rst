.. _dec-turbo_prod-decoder-parameters:

Decoder parameters
------------------

.. _dec-turbo_prod-dec-sub-cw-size:

``--dec-sub-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--dec-sub-cw-size 1``

The codeword size.

.. _dec-turbo_prod-dec-sub-info-bits:

``--dec-sub-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--dec-sub-info-bits 1``

Useful number of bit transmitted (information bits).

.. _dec-turbo_prod-dec-alpha:

``--dec-alpha``
"""""""""""""""

   :type: list of (real number):limited length [1;inf]
   :Examples: ``--dec-alpha TODO``

Weighting factor, one by half iteration (so twice more than number of iterations). If not enough given values, then automatically extends the last to all iterations.

.. _dec-turbo_prod-dec-beta:

``--dec-beta``
""""""""""""""

   :type: list of (real number:positive):limited length [1;inf]
   :Examples: ``--dec-beta TODO``

Reliability factor, one by half iteration (so twice more than number of iterations). If not enough given values, then automatically extends the last to all iterations. If not given, then computes beta dynamically from the least reliable position metrics.

.. _dec-turbo_prod-dec-c:

``--dec-c``
"""""""""""

   :type: integer
   :Examples: ``--dec-c 1``

Number of competitors (0 means equal to number of test vectors, 1 means only the decided word).

.. _dec-turbo_prod-dec-cp-coef:

``--dec-cp-coef``
"""""""""""""""""

   :type: list of (real number):limited length [5;5]
   :Examples: ``--dec-cp-coef TODO``

The 5 Chase Pyndiah constant coefficients "a,b,c,d,e".

.. _dec-turbo_prod-dec-flips:

``--dec-flips``
"""""""""""""""

   :type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-turbo_prod-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-turbo_prod-dec-implem:

``--dec-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``FAST`` ``NAIVE`` ``STD`` 
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``FAST``  | |dec-implem_descr_fast|  |
+-----------+--------------------------+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+

.. |dec-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_std| replace:: TODO VALUE STD


.. _dec-turbo_prod-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :type: integer
   :Examples: ``--dec-ite 1``

Maximal number of iterations in the turbo.

.. _dec-turbo_prod-dec-p:

``--dec-p``
"""""""""""

   :type: integer
   :Examples: ``--dec-p 1``

Number of least reliable positions.

.. _dec-turbo_prod-dec-sub-corr-pow:

``--dec-sub-corr-pow, -T``
""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--dec-sub-corr-pow 1``

Correction power of the BCH code.

.. _dec-turbo_prod-dec-sub-flips:

``--dec-sub-flips``
"""""""""""""""""""

   :type: integer
   :Examples: ``--dec-sub-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-turbo_prod-dec-sub-hamming:

``--dec-sub-hamming``
"""""""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-turbo_prod-dec-sub-implem:

``--dec-sub-implem``
""""""""""""""""""""

   :type: text
   :Allowed values: ``FAST`` ``GENIUS`` ``NAIVE`` ``STD`` 
   :Examples: ``--dec-sub-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+------------+-------------------------------+
| Value      | Description                   |
+============+===============================+
| ``FAST``   | |dec-sub-implem_descr_fast|   |
+------------+-------------------------------+
| ``GENIUS`` | |dec-sub-implem_descr_genius| |
+------------+-------------------------------+
| ``NAIVE``  | |dec-sub-implem_descr_naive|  |
+------------+-------------------------------+
| ``STD``    | |dec-sub-implem_descr_std|    |
+------------+-------------------------------+

.. |dec-sub-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-sub-implem_descr_genius| replace:: TODO VALUE GENIUS
.. |dec-sub-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-sub-implem_descr_std| replace:: TODO VALUE STD


.. _dec-turbo_prod-dec-sub-type:

``--dec-sub-type, -D``
""""""""""""""""""""""

   :type: text
   :Allowed values: ``ALGEBRAIC`` ``CHASE`` ``ML`` 
   :Examples: ``--dec-sub-type ALGEBRAIC``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+---------------+--------------------------------+
| Value         | Description                    |
+===============+================================+
| ``ALGEBRAIC`` | |dec-sub-type_descr_algebraic| |
+---------------+--------------------------------+
| ``CHASE``     | |dec-sub-type_descr_chase|     |
+---------------+--------------------------------+
| ``ML``        | |dec-sub-type_descr_ml|        |
+---------------+--------------------------------+

.. |dec-sub-type_descr_algebraic| replace:: TODO VALUE ALGEBRAIC
.. |dec-sub-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-sub-type_descr_ml| replace:: TODO VALUE ML


.. _dec-turbo_prod-dec-t:

``--dec-t``
"""""""""""

   :type: integer
   :Examples: ``--dec-t 1``

Number of test vectors (0 means equal to 2^p).

.. _dec-turbo_prod-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :type: text
   :Allowed values: ``CHASE`` ``CP`` ``ML`` 
   :Examples: ``--dec-type CHASE``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``CP``    | |dec-type_descr_cp|    |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+

.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_cp| replace:: TODO VALUE CP
.. |dec-type_descr_ml| replace:: TODO VALUE ML


