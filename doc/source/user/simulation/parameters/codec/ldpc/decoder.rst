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

|factory::Decoder_LDPC::p+h-path|

This argument is not required if the encoder type :ref:`enc-ldpc-enc-type`
is ``LDPC_DVBS2``.

For the AList format, an |ASCII| file composed by positive integers is expected:

.. code-block:: bash

   # -- Part 1 --
   # 'nVN' is the total number of variable nodes and 'nCN' is the total number of check nodes
   nVN nCN
   # 'dmax_VN' is the higher variable node degree and 'dmax_CN' is the higher check node degree
   dmax_VN dmax_CN
   # list of the degrees for each variable nodes
   d_VN_{1} d_VN_{2} [...] d_VN_{nVN}
   # list of the degrees for each check nodes
   d_CN_{1} d_CN_{2} [...] d_CN_{nCN}
   #
   # -- Part 2 --
   # each following line describes the check nodes connected to a variable node, the first
   # check node index is '1' (and not '0')
   # variable node '1'
   VN_{1}_CN_{idx_1} [...] VN_{1}_CN_{idx_d_VN_{1}}
   # variable node '2'
   VN_{2}_CN_{idx_1} [...] VN_{2}_CN_{idx_d_VN_{2}}
   [...]
   # variable node 'nVN'
   VN_{nVN}_CN_{idx_1} [...] VN_{nVN}_CN_{idx_d_VN_{nVN}}
   #
   # -- Part 3 --
   # each following line describes the variables nodes connected to a check node, the first
   # variable node index is '1' (and not '0')
   # check node '1'
   CN_{1}_VN_{idx_1} [...] CN_{1}_VN_{idx_d_CN_{1}}
   # check node '2'
   CN_{2}_VN_{idx_1} [...] CN_{2}_VN_{idx_d_CN_{2}}
   [...]
   # check node 'nCN'
   CN_{nCN}_VN_{idx_1} [...] CN_{nCN}_VN_{idx_d_CN_{nCN}}

In the part 2 and 3, it is possible to pad, at the end of the indexes list, with
zeros when the current node degree is smaller than the maximum node degree.
|AFF3CT| will be able to read the file even if it is padded with zeros.

For the |QC| format, an |ASCII| file composed by integers is expected:

.. code-block:: bash

   # 'C' is the number of columns (there is 'C * Z' variable nodes)
   # 'R' is the number of rows (there is 'R * Z' check nodes)
   # 'Z' is the expansion factor
   C R Z

   # each 'B_r_{y}_c_{x}' is a sub-matrix bloc of size 'Z * Z'
   # 'B_r_{y}_c_{x} = -1' means a zero matrix
   # 'B_r_{y}_c_{x} = 0' means an identity matrix
   # 'B_r_{y}_c_{x} = s' with 's' between '1' and 'Z-1' means an identity matrix shifted 's' times
   #                     to the right
   B_r_{1}_c_{1} B_r_{1}_c_{2} [...] B_r_{1}_c_{C}
   B_r_{2}_c_{1} B_r_{2}_c_{2} [...] B_r_{2}_c_{C}
   [...]
   B_r_{R}_c_{1} B_r_{R}_c_{2} [...] B_r_{R}_c_{C}

   # puncturing pattern (optional)
   # 'T_c_{x}' can be '0' or '1'
   #  - if 'T_c_{x} = 0', does not transmit the 'Z' consecutive bits
   #  - if 'T_c_{x} = 1', transmits the 'Z' consecutive bits
   T_c_{1} T_c_{2} [...] T_c_{C}

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

|factory::Decoder::p+type,D|

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
   :Allowed values: ``STD`` ``GALA`` ``GALB`` ``GALE`` ``WBF`` ``MWBF`` ``PPBF``
                    ``SPA`` ``LSPA`` ``AMS`` ``MS`` ``NMS`` ``OMS``
   :Default: ``SPA``
   :Examples: ``--dec-implem AMS``

|factory::Decoder::p+implem|

Description of the allowed values:

+-----------+------------------------------------------------------------------------+
| Value     | Description                                                            |
+===========+========================================================================+
| ``STD``   | Select the |STD| implementation.                                       |
+-----------+------------------------------------------------------------------------+
| ``GALA``  | Select the |GALA| algorithm :cite:`Ryan2009`.                          |
+-----------+------------------------------------------------------------------------+
| ``GALB``  | Select the |GALB| algorithm :cite:`Declerq2014` with majority vote.    |
+-----------+------------------------------------------------------------------------+
| ``GALE``  | Select the |GALE| algorithm :cite:`Declerq2014` with extended alphabet.|
+-----------+------------------------------------------------------------------------+
| ``PPBF``  | Select the |PPBF| algorithm :cite:`LeGhaffari2019`.                    |
+-----------+------------------------------------------------------------------------+
| ``WBF``   | Select the |WBF| algorithm :cite:`Wadayama2010`.                       |
+-----------+------------------------------------------------------------------------+
| ``MWBF``  | Select the |MWBF| algorithm :cite:`Wadayama2010`.                      |
+-----------+------------------------------------------------------------------------+
| ``SPA``   | Select the |SPA| update rules :cite:`MacKay1995`.                      |
+-----------+------------------------------------------------------------------------+
| ``LSPA``  | Select the |LSPA| update rules :cite:`MacKay1995`.                     |
+-----------+------------------------------------------------------------------------+
| ``AMS``   | Select the |AMS| update rule.                                          |
+-----------+------------------------------------------------------------------------+
| ``MS``    | Select the |MS| update rule :cite:`Fossorier1999`.                     |
+-----------+------------------------------------------------------------------------+
| ``NMS``   | Select the |NMS| update rule :cite:`Chen2002`.                         |
+-----------+------------------------------------------------------------------------+
| ``OMS``   | Select the |OMS| update rule :cite:`Chen2002`.                         |
+-----------+------------------------------------------------------------------------+

:numref:`tab_ldpc_dec_implem` shows the different decoder types and their
corresponding available implementations.

.. _tab_ldpc_dec_implem:

.. table:: |LDPC| decoder types and available implementations.

   +---------+-----+------+------+------+------+-----+------+-----+------+-----+----+-----+-----+
   | Decoder ||STD|||GALA|||GALB|||GALE|||PPBF|||WBF|||MWBF|||SPA|||LSPA|||AMS|||MS|||NMS|||OMS||
   +=========+=====+======+======+======+======+=====+======+=====+======+=====+====+=====+=====+
   | |BF|    |     |      |      |      ||K|   ||K|  ||K|   |     |      |     |    |     |     |
   +---------+-----+------+------+------+------+-----+------+-----+------+-----+----+-----+-----+
   | |BP-P|  ||K|  |      |      |      |      |     |      |     |      |     |    |     |     |
   +---------+-----+------+------+------+------+-----+------+-----+------+-----+----+-----+-----+
   | |BP-F|  |     ||K|   ||K|   ||K|   |      |     |      ||K3| ||K2|  ||K2| ||K2|||K2| ||K2| |
   +---------+-----+------+------+------+------+-----+------+-----+------+-----+----+-----+-----+
   | |BP-HL| |     |      |      |      |      |     |      ||K2| ||K2|  ||K2| ||K1|||K1| ||K1| |
   +---------+-----+------+------+------+------+-----+------+-----+------+-----+----+-----+-----+
   | |BP-VL| |     |      |      |      |      |     |      ||K2| ||K2|  ||K2| ||K2|||K2| ||K2| |
   +---------+-----+------+------+------+------+-----+------+-----+------+-----+----+-----+-----+

.. |K|  replace:: :math:`\checkmark`
.. |K1| replace:: :math:`\checkmark^{*}`
.. |K2| replace:: :math:`\checkmark^{**}`
.. |K3| replace:: :math:`\checkmark^{**+}`

:math:`^{*}/^{**}`: compatible with the :ref:`dec-ldpc-dec-simd`
``INTER`` parameter.

:math:`^{**}`: require the C++ compiler to support the **dynamic memory
allocation for over-aligned data**, see the
`P0035R4 paper <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0035r4.html>`_.
This feature is a part of the C++17 standard (working on the C++ GNU compiler
version 8.1.0). When compiling with the GNU compiler in C++11 mode, the
``-faligned-new`` option enables specifically the required feature.

:math:`^{+}`: compatible with the :ref:`dec-ldpc-dec-simd` ``INTRA`` parameter.

.. _dec-ldpc-dec-simd:

``--dec-simd``
""""""""""""""

   :Type: text
   :Allowed values: ``INTER``, ``INTRA``
   :Examples: ``--dec-simd INTER``

|factory::Decoder_LDPC::p+simd|

:numref:`tab_ldpc_dec_implem` shows the decoders and implementations that
support |SIMD|.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``INTRA`` | |dec-simd_descr_intra| |
+-----------+------------------------+
| ``INTER`` | |dec-simd_descr_inter| |
+-----------+------------------------+

.. |dec-simd_descr_intra| replace:: Select the intra-frame strategy.
.. |dec-simd_descr_inter| replace:: Select the inter-frame strategy.

.. note:: In **the intra-frame strategy**, |SIMD| units process several LLRs in
   parallel within a single frame decoding. In **the inter-frame strategy**,
   SIMD units decodes several independent frames in parallel in order to
   saturate the |SIMD| unit. This approach improves the throughput of the
   decoder but requires to load several frames before starting to decode,
   increasing both the decoding latency and the decoder memory footprint.

.. note:: When the inter-frame |SIMD| strategy is set, the simulator will run
   with the right number of frames depending on the |SIMD| length. This number
   of frames can be manually set with the :ref:`sim-sim-inter-fra` parameter. Be
   aware that running the simulator with the :ref:`sim-sim-inter-fra` parameter
   set to 1 and the :ref:`dec-polar-dec-simd` parameter set to ``INTER`` will
   completely be counterproductive and will lead to no throughput improvements.

.. _dec-ldpc-dec-h-reorder:

``--dec-h-reorder``
"""""""""""""""""""

   :Type: text
   :Allowed values: ``ASC`` ``DSC`` ``NONE``
   :Default: ``NONE``
   :Examples: ``--dec-h-reorder ASC``

|factory::Decoder_LDPC::p+h-reorder|

The degree of a |CN| is the number of |VNs| that are connected to it.

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

.. |dec-h-reorder_descr_asc|  replace:: Reorder from the smallest |CNs| degree
   to the biggest |CNs| degree.
.. |dec-h-reorder_descr_dsc|  replace:: Reorder from the biggest |CNs| degree to
   the smallest |CNs| degree.
.. |dec-h-reorder_descr_none| replace:: Do not change the order.

.. _dec-ldpc-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Default: 10
   :Examples: ``--dec-ite 30``

|factory::Decoder_LDPC::p+ite,i|

.. note:: By default, in order to speedup the decoding time, the decoder can
   stop the decoding process if all the parity check equations are verified
   (also called **the syndrome detection**). In that case the decoder can
   perform less decoding iterations than the given number. To force the decoder
   to make all the iterations, use the :ref:`dec-ldpc-dec-no-synd` parameter.

.. _dec-ldpc-dec-min:

``--dec-min``
"""""""""""""

   :Type: text
   :Allowed values: ``MIN`` ``MINL`` ``MINS``
   :Default: ``MINL``
   :Examples: ``--dec-min MIN``

|factory::Decoder_LDPC::p+min|

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

|factory::Decoder_LDPC::p+norm|

.. _dec-ldpc-dec-off:

``--dec-off``
"""""""""""""

   :Type: real number
   :Default: 0.0
   :Examples: ``--dec-off 0.25``

|factory::Decoder_LDPC::p+off|

.. _dec-ldpc-dec-mwbf-factor:

``--dec-mwbf-factor``
"""""""""""""""""""""

   :Type: real number
   :Default: 0.0
   :Examples: ``--dec-mwbf-factor 1.0``

|factory::Decoder_LDPC::p+mwbf-factor|

.. _dec-ldpc-dec-synd-depth:

``--dec-synd-depth``
""""""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--dec-synd-depth 2``

|factory::Decoder_LDPC::p+synd-depth|

.. _dec-ldpc-dec-ppbf-proba:

``--dec-ppbf-proba``
""""""""""""""""""""

   :Type: list of real numbers
   :Examples: ``--dec-ppbf-proba "0,0.001,0.1,0.3,1,1,1"``

|factory::Decoder_LDPC::p+ppbf-proba|

Thus, with a parity matrix that has its largest variable node at 5, you must
give 7 values. Each value corresponds to an energy level as described in
:cite:`LeGhaffari2019`.

.. _dec-ldpc-dec-no-synd:

``--dec-no-synd``
"""""""""""""""""

|factory::Decoder_LDPC::p+no-synd|

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Ldpc-