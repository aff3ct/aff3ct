.. _dec-turbo_prod-decoder-parameters:

|TPC| Decoder parameters
------------------------

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

.. _dec-turbo_prod-dec-sub-cw-size:

``--dec-sub-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-sub-cw-size 1``

|factory::Decoder::p+cw-size,N|

.. _dec-turbo_prod-dec-sub-info-bits:

``--dec-sub-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-sub-info-bits 1``

|factory::Decoder::p+info-bits,K|

.. _dec-turbo_prod-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``CHASE`` ``CP`` ``ML``
   :Default: ``CP``
   :Examples: ``--dec-type CP``

|factory::Decoder::p+type,D|

This algorithm will decode each column and row of the |TPC|.

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
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml|    replace:: See the common :ref:`dec-common-dec-type`
   parameter.

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
   :Allowed values: ``STD``
   :Default: ``STD``
   :Examples: ``--dec-implem STD``

|factory::Decoder::p+implem|

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+

.. |dec-implem_descr_std|   replace:: A standard implementation

.. _dec-turbo_prod-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Default: 4
   :Examples: ``--dec-ite 8``

|factory::Decoder_turbo_product::p+ite,i|

.. _dec-turbo_prod-dec-alpha:

``--dec-alpha``
"""""""""""""""

   :Type: list of real numbers
   :Default: all at 0.5
   :Examples: ``--dec-alpha "0.1,0.1,0.2,0.25,0.3,0.35,.5,.5,1.2"``

|factory::Decoder_turbo_product::p+alpha|

The first one is for the first columns process, the second for the first rows
process, the third for the second columns process, the fourth for the second
rows process, and so on.

If there are not enough values, then the last one given is automatically
extended to the rest of the half-iterations.
Conversely, if there are too many, the surplus is truncated.

.. _dec-turbo_prod-dec-beta:

``--dec-beta``
""""""""""""""

   :Type: list of real numbers
   :Examples: ``--dec-beta "0.1,0.1,0.2,0.25,0.3,0.35,.5,.5,1.2"``

|factory::Decoder_turbo_product::p+beta|

The first one is for the first columns process, the second for the first rows
process, the third for the second columns process, the fourth for the second
rows process, and so on.

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

|factory::Decoder_turbo_product::p+c|

.. _dec-turbo_prod-dec-p:

``--dec-p``
"""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--dec-p 1``

|factory::Decoder_turbo_product::p+p|

.. _dec-turbo_prod-dec-t:

``--dec-t``
"""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--dec-t 1``

|factory::Decoder_turbo_product::p+t|

.. _dec-turbo_prod-dec-cp-coef:

``--dec-cp-coef``
"""""""""""""""""

   :Type: list of real numbers
   :Default: ``"1,1,1,1,0"``
   :Examples: ``--dec-cp-coef "0,0.25,0,0,3"``

|factory::Decoder_turbo_product::p+cp-coef|

See the :ref:`dec-turbo_prod-dec-type` parameter.

.. _dec-turbo_prod-dec-sub-type:

``--dec-sub-type, -D``
""""""""""""""""""""""

Please refer to the |BCH| :ref:`dec-bch-dec-type` parameter.

.. _dec-turbo_prod-dec-sub-corr-pow:

``--dec-sub-corr-pow, -T``
""""""""""""""""""""""""""

Please refer to the |BCH| :ref:`dec-bch-dec-corr-pow` parameter.

.. _dec-turbo_prod-dec-sub-implem:

``--dec-sub-implem``
""""""""""""""""""""

Please refer to the |BCH| :ref:`dec-bch-dec-implem` parameter.

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Trbp-
