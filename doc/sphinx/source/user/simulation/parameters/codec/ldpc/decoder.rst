.. _dec-ldpc-decoder-parameters:

|LDPC| Decoder parameters
-------------------------

.. _dec-ldpc-dec-h-path:

``--dec-h-path`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--dec-h-path conf/dec/LDPC/AR4JA_4096_8192.qc``
              ``--dec-h-path conf/dec/LDPC/MACKAY_504_1008.alist``

Give the path to the :math:`H` parity matrix. Support the AList and |QC|
formats.

This argument is not required if the encoder type :ref:`enc-ldpc-enc-type`
is ``LDPC_DVBS2``.

.. TODO: info bits pos at the end of .alist file puncturer pattern at the end
   of QC file

.. _dec-ldpc-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``BIT_FLIPPING`` ``BP_PEELING`` ``BP_FLOODING``
                    ``BP_HORIZONTAL_LAYERED`` ``BP_VERTICAL_LAYERED``
                    ``CHASE`` ``ML``
   :Default: ``BP_FLOODING``
   :Examples: ``--dec-type BP_HORIZONTAL_LAYERED``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+---------------------------+--------------------------------------------------+
| Value                     | Description                                      |
+===========================+==================================================+
| ``BIT_FLIPPING``          | Select the |BF| category of algorithms.          |
+---------------------------+--------------------------------------------------+
| ``BP_PEELING``            | Select the |BP-P| algorithm from                 |
|                           | :cite:`Peeling2001`.                             |
+---------------------------+--------------------------------------------------+
| ``BP_FLOODING``           | Select the |BP-F| algorithm from                 |
|                           | :cite:`MacKay1995`.                              |
+---------------------------+--------------------------------------------------+
| ``BP_HORIZONTAL_LAYERED`` | Select the |BP-HL| algorithm from                |
|                           | :cite:`Yeo2001`.                                 |
+---------------------------+--------------------------------------------------+
| ``BP_VERTICAL_LAYERED``   | Select the |BP-VL| algorithm from                |
|                           | :cite:`Zhang2002`.                               |
+---------------------------+--------------------------------------------------+
| ``CHASE``                 | See the common :ref:`dec-common-dec-type`        |
|                           | parameter.                                       |
+---------------------------+--------------------------------------------------+
| ``ML``                    | See the common :ref:`dec-common-dec-type`        |
|                           | parameter.                                       |
+---------------------------+--------------------------------------------------+

.. TODO: BP_HORIZONTAL_LAYERED_LEGACY and __cpp_aligned_new

.. _dec-ldpc-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``STD`` ``GALA`` ``WBF`` ``SPA`` ``LSPA`` ``AMS`` ``MS``
                    ``NMS`` ``OMS``
   :Default: ``SPA``
   :Examples: ``--dec-implem AMS``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+------------------------------------------------------------------+
| Value     | Description                                                      |
+===========+==================================================================+
| ``STD``   | Select the |STD| implementation.                                 |
+-----------+------------------------------------------------------------------+
| ``GALA``  | Select the |GALA| algorithm :cite:`Ryan2009`.                    |
+-----------+------------------------------------------------------------------+
| ``WBF``   | Select the |WBF| algorithm :cite:`Wadayama2010`.                 |
+-----------+------------------------------------------------------------------+
| ``SPA``   | Select the |SPA| update rules :cite:`MacKay1995`.                |
+-----------+------------------------------------------------------------------+
| ``LSPA``  | Select the |LSPA| update rules :cite:`MacKay1995`.               |
+-----------+------------------------------------------------------------------+
| ``AMS``   | Select the |AMS| update rules.                                   |
+-----------+------------------------------------------------------------------+
| ``MS``    | Select the |MS| update rules :cite:`Fossorier1999`.              |
+-----------+------------------------------------------------------------------+
| ``NMS``   | Select the |NMS| update rules :cite:`Chen2002`.                  |
+-----------+------------------------------------------------------------------+
| ``OMS``   | Select the |OMS| update rules :cite:`Chen2002`.                  |
+-----------+------------------------------------------------------------------+

:numref:`tab_ldpc_dec_implem` shows the different decoder types and their
corresponding available implementations.

.. _tab_ldpc_dec_implem:

.. table:: |LDPC| decoder types and available implementations.
   :align: center

   +---------+-----+------+-----+-----+------+-----+----+-----+-----+
   | Decoder ||STD|||GALA|||WBF|||SPA|||LSPA|||AMS|||MS|||NMS|||OMS||
   +=========+=====+======+=====+=====+======+=====+====+=====+=====+
   | |BF|    |     |      ||K|  |     |      |     |    |     |     |
   +---------+-----+------+-----+-----+------+-----+----+-----+-----+
   | |BP-P|  ||K|  |      |     |     |      |     |    |     |     |
   +---------+-----+------+-----+-----+------+-----+----+-----+-----+
   | |BP-F|  |     ||K|   |     ||K2| ||K2|  ||K2| ||K2|||K2| ||K2| |
   +---------+-----+------+-----+-----+------+-----+----+-----+-----+
   | |BP-HL| |     |      |     ||K2| ||K2|  ||K2| ||K1|||K1| ||K1| |
   +---------+-----+------+-----+-----+------+-----+----+-----+-----+
   | |BP-VL| |     |      |     ||K2| ||K2|  ||K2| ||K2|||K2| ||K2| |
   +---------+-----+------+-----+-----+------+-----+----+-----+-----+

.. |K| replace:: :math:`\checkmark`
.. |K1| replace:: :math:`\checkmark^{*}`
.. |K2| replace:: :math:`\checkmark^{**}`

:math:`^{*}/^{**}`: compatible with the :ref:`dec-ldpc-dec-simd`
``INTER`` parameter.

:math:`^{**}`: require the C++ compiler to support the **dynamic memory
allocation for over-aligned data**, see the
`P0035R4 paper <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0035r4.html>`_.
This feature is a part of the C++17 standard (working on the C++ GNU compiler
version 8.1.0).

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