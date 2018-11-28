.. _dec-turbo_prod-decoder-parameters:

Decoder parameters
------------------

The |TPC| decoder first decodes columns once with the Chase-Pyndiah algorithm,
then rows, and columns again then rows again and so on.

Let's say :math:`C` is the :math:`N \times N` *a priori* matrix from the
demodulator.

Let's say :math:`R_{i+1}^c` is the :math:`N \times N` *a posteriori* matrix
computed by this decoder after the :math:`i^{th}` iteration on the columns.
Initially, :math:`R_0^c = C`.

Let's say :math:`R_{i+1}^r` is the :math:`N \times N` *a posteriori* matrix
computed by this decoder after the :math:`i^{th}` iteration on the rows,
with :math:`R_i^r = R_{i+1}^c`.


The process of the columns for the :math:`i^{th}` iteration gives:

   :math:`R_{i+1}^c = alpha_{2i+0}.W_i^c + C`

with :math:`W_i^c` the extrinsic from the Chase-Pyndiah decoder
computed on :math:`R_{i}^c`.

The process of the rows for the :math:`i^{th}` iteration gives:

   :math:`R_{i+1}^r = alpha_{2i+1}.W_i^r + C`

with :math:`W_i^r` the extrinsic from the Chase-Pyndiah decoder
computed on :math:`R_{i}^r`.

Parameter :math:`alpha` is set with the argument :ref:`dec-turbo_prod-dec-alpha`.



.. _dec-turbo_prod-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``CHASE`` ``CP`` ``ML``
   :Default: ``CP``
   :Examples: ``--dec-type CP``

Select the algorithm to decode each column and row of the |TPC|.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``CP``    | |dec-type_descr_cp|    |
+-----------+------------------------+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+

.. |dec-type_descr_cp|    replace:: Decode with the Chase-Pyndiah algorithm of
   the |TPC|
.. |dec-type_descr_chase| replace:: Decode with the Chase algorithm
.. |dec-type_descr_ml|    replace:: Decode with the |ML| algorithm


.. rubric:: The ``CP`` algorithm is the implementation of
   :cite:`Pyndiah1998` but in a more generic way in order to let the user
   chose its configuration:

* Chase step: find the more reliable codeword :math:`D`:
   * Take hard decision :math:`H` on input :math:`R`.
   * Select the :math:`p` (set with :ref:`dec-turbo_prod-dec-p`) least reliable
     positions from :math:`R` to get a metric set :math:`P` of :math:`p`
     elements.
   * Create :math:`t` (set with :ref:`dec-turbo_prod-dec-t`) test vectors
     from test patterns.
   * Hard decode with the sub-decoder to get the competitors with good syndrome
     set :math:`C`.
   * Remove competitors from :math:`C` to keep :math:`c` of them
     (set with :ref:`dec-turbo_prod-dec-c`).
   * Compute the metrics :math:`C_m` (euclidean distance) of each
     competitor compared to :math:`H`.
   * Select the competitors with the smallest metric
     to get the decided word :math:`D` with a
     metric :math:`D_m` and where :math:`D_j = \begin{cases}
     +1 & \text{when } H_j = 0 \\
     -1 & \text{when } H_j = 1
     \end{cases}`

* Pyndiah step: compute reliabilities of each bit of :math:`D`
   * :math:`a, b, c, d` and :math:`e` are simulation constants changeable by
     the user with :ref:`dec-turbo_prod-dec-cp-coef`
   * Compute the reliability :math:`F` of :math:`D` for each bit
     :math:`D_j` of the word:

     * Find :math:`C^s` the competitor with the smallest metric :math:`C_m`
       that have :math:`C_j^s \neq D_j`.
     * when :math:`C^s` exists:

        :math:`F_j = b . D_j . [C_m - D_m]`
     * when :math:`C^s` does not exist and if
       :ref:`dec-turbo_prod-dec-beta` is given:

        :math:`F_j = D_j . beta`
     * else:

        :math:`F_j = D_j . \left[ \displaystyle\sum_{i=0}^{e}
        P_i - c . D_m + d . |R_j| \right]`
        where :math:`P` is considered sorted, :math:`0 < e < p`,
        and when :math:`e == 0 \implies e = p - 1`.

   * Compute extrinsic :math:`W = F - a . R`


.. _dec-turbo_prod-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``NAIVE`` ``STD``
   :Examples: ``--dec-implem STD``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+

.. | ``FAST``  | |dec-implem_descr_fast|  |
.. +-----------+--------------------------+

.. |dec-implem_descr_std|   replace:: A standard implementation
.. |dec-implem_descr_naive| replace:: A naive implementation only for the ``ML``
..
   .. |dec-implem_descr_fast|  replace:: TODO VALUE FAST


.. _dec-turbo_prod-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Default: 4
   :Examples: ``--dec-ite 5``

Set the number of iterations in the turbo.

.. _dec-turbo_prod-dec-alpha:

``--dec-alpha``
"""""""""""""""

   :Type: list of real numbers
   :Default: all at 0.5
   :Examples: ``--dec-alpha "0.1,0.1,0.2,0.25,0.3,0.35,.5,.5,1.2"``

Give the *weighting factor* alpha, one by half iteration
(so twice more than the number of iterations). The first one is for the first
columns process, the second for the first rows process, the third for the second
columns process, the fourth for the second rows process, and so on.

If there are not enough values, then the last one given is automatically
extended to the rest of the half-iterations.
Conversely, if there are too many, the surplus is truncated.

.. _dec-turbo_prod-dec-beta:

``--dec-beta``
""""""""""""""

   :Type: list of real numbers
   :Examples: ``--dec-beta "0.1,0.1,0.2,0.25,0.3,0.35,.5,.5,1.2"``

Give the *reliability factor* beta, one by half iteration
(so twice more than the number of iterations). The first one is for the first
columns process, the second for the first rows process, the third for the second
columns process, the fourth for the second rows process, and so on.

If there are not enough values, then the last one given is automatically
extended to the rest of the half-iterations.
Conversely, if there are too many, the surplus is truncated.

If not given, then beta is dynamically computed as described in
:ref:`dec-turbo_prod-dec-type`.

.. _dec-turbo_prod-dec-c:

``--dec-c``
"""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--dec-c 3``

Set the *number of competitors*. A value of 0 means that the latter is set
to the number of test vectors, 1 means only the decided word.

.. _dec-turbo_prod-dec-p:

``--dec-p``
"""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--dec-p 1``

Set the number of *least reliable positions*.

.. _dec-turbo_prod-dec-t:

``--dec-t``
"""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--dec-t 1``

Set the *number of test vectors*. A value of 0 means equal to :math:`2^p` where
:math:`p` is the number of least reliable positions.

.. _dec-turbo_prod-dec-cp-coef:

``--dec-cp-coef``
"""""""""""""""""

   :Type: list of real numbers
   :Default: ``"1,1,1,1,0"``
   :Examples: ``--dec-cp-coef "0,0.25,0,0,3"``

Give the 5 ``CP`` constant coefficients :math:`a, b, c, d, e` as described in
:ref:`dec-turbo_prod-dec-type`.

.. _dec-turbo_prod-dec-flips:

``--dec-flips``
"""""""""""""""

   :Type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-turbo_prod-dec-hamming:

``--dec-hamming``
"""""""""""""""""

Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-turbo_prod-dec-sub-corr-pow:

``--dec-sub-corr-pow, -T``
""""""""""""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``-T 1``

Give the correction power of the BCH sub-decoder.

.. _dec-turbo_prod-dec-sub-flips:

``--dec-sub-flips``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-sub-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-turbo_prod-dec-sub-hamming:

``--dec-sub-hamming``
"""""""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance
in the ML/CHASE decoders.

.. _dec-turbo_prod-dec-sub-implem:

``--dec-sub-implem``
""""""""""""""""""""

   :Type: text
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

   :Type: text
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


.. TODO : remove those arguments in the code

   .. _dec-turbo_prod-dec-sub-cw-size:

   ``--dec-sub-cw-size, -N`` |image_required_argument|
   """""""""""""""""""""""""""""""""""""""""""""""""""

      :Type: integer
      :Examples: ``--dec-sub-cw-size 1``

   The codeword size.

   .. _dec-turbo_prod-dec-sub-info-bits:

   ``--dec-sub-info-bits, -K`` |image_required_argument|
   """""""""""""""""""""""""""""""""""""""""""""""""""""

      :Type: integer
      :Examples: ``--dec-sub-info-bits 1``

   Useful number of bit transmitted (information bits).

References
""""""""""

.. bibliography:: references.bib

