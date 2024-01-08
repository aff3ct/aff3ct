.. _dec-rsc_db-decoder-parameters:

|RSC| |DB| Decoder parameters
-----------------------------

.. _dec-rsc_db-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``BCJR`` ``CHASE`` ``ML``
   :Default: ``BCJR``
   :Examples: ``--dec-type BCJR``

|factory::Decoder::p+type,D|

Description of the allowed values:

+-----------+------------------------------------------------------------------+
| Value     | Description                                                      |
+===========+==================================================================+
| ``BCJR``  | Select the |BCJR| |DB| decoder :cite:`Bahl1974a`.                |
+-----------+------------------------------------------------------------------+
| ``CHASE`` | See the common :ref:`dec-common-dec-type` parameter.             |
+-----------+------------------------------------------------------------------+
| ``ML``    | See the common :ref:`dec-common-dec-type` parameter.             |
+-----------+------------------------------------------------------------------+

.. _dec-rsc_db-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``GENERIC`` ``DVB-RCS1`` ``DVB-RCS2``
   :Default: ``DVB-RCS1``
   :Examples: ``--dec-implem DVB-RCS1``

|factory::Decoder::p+implem|

Description of the allowed values:

+--------------+---------------------------------------------------------------+
| Value        | Description                                                   |
+==============+===============================================================+
| ``GENERIC``  | Select a generic implementation that works on any trellis.    |
+--------------+---------------------------------------------------------------+
| ``DVB-RCS1`` | Select an implementation dedicated to the |DVB-RCS1| trellis  |
|              | (faster than the ``GENERIC`` implementation).                 |
+--------------+---------------------------------------------------------------+
| ``DVB-RCS2`` | Select an implementation dedicated to the |DVB-RCS2| trellis  |
|              | (faster than the ``GENERIC`` implementation).                 |
+--------------+---------------------------------------------------------------+

.. _dec-rsc_db-dec-max:

``--dec-max``
"""""""""""""

   :Type: text
   :Allowed values: ``MAXS`` ``MAXL`` ``MAX``
   :Examples: ``--dec-max MAX``

|factory::Decoder_RSC_DB::p+max|

Description of the allowed values:

+----------+----------------------+
| Value    | Description          |
+==========+======================+
| ``MAXS`` | |dec-max_descr_maxs| |
+----------+----------------------+
| ``MAXL`` | |dec-max_descr_maxl| |
+----------+----------------------+
| ``MAX``  | |dec-max_descr_max|  |
+----------+----------------------+

.. |dec-max_descr_maxs|  replace:: :math:`\max^*(a,b) = \max(a,b) +
   \log(1 + \exp(-|a - b|))`.
.. |dec-max_descr_maxl|  replace:: :math:`\max^*(a,b) \approx \max(a,b) +
   \max(0, 0.301 - (0.5 |a - b|))`.
.. |dec-max_descr_max|   replace:: :math:`\max^*(a,b) \approx \max(a,b)`.

``MAXS`` for *Max Star* is the exact :math:`\max^*` operator. ``MAXL`` for
*Max Linear* is a linear approximation of the :math:`\max^*` function. ``MAX``
for *Max* is the simplest :math:`\max^*` approximation with only a
:math:`\max` function.

.. note:: The |BCJR| with the :math:`\max` approximation is also called the
   max-log-|MAP| algorithm.

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Rsdb-
