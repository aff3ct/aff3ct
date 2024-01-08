.. _dec-bch-decoder-parameters:

|BCH| Decoder parameters
------------------------

.. _dec-bch-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``ALGEBRAIC`` ``CHASE`` ``ML``
   :Default: ``ALGEBRAIC``
   :Examples: ``--dec-type ALGEBRAIC``

|factory::Decoder::p+type,D|

Description of the allowed values:

.. _Berlekamp-Massey algorithm: https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm
.. _Chien search: https://en.wikipedia.org/wiki/Chien_search

+---------------+--------------------------------------------------------------+
| Value         | Description                                                  |
+===============+==============================================================+
| ``ALGEBRAIC`` | Select the `Berlekamp-Massey algorithm`_                     |
|               | :cite:`Berlekamp1968,Massey1969` followed by a               |
|               | `Chien search`_ :cite:`Chien1964`.                           |
+---------------+--------------------------------------------------------------+
| ``CHASE``     | See the common :ref:`dec-common-dec-type` parameter.         |
+---------------+--------------------------------------------------------------+
| ``ML``        | See the common :ref:`dec-common-dec-type` parameter.         |
+---------------+--------------------------------------------------------------+

.. _dec-bch-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``GENIUS`` ``STD``
   :Default: ``STD``
   :Examples: ``--dec-implem FAST``

|factory::Decoder::p+implem|

Description of the allowed values:

+------------+---------------------------+
| Value      | Description               |
+============+===========================+
| ``STD``    | |dec-implem_descr_std|    |
+------------+---------------------------+
| ``FAST``   | |dec-implem_descr_fast|   |
+------------+---------------------------+
| ``GENIUS`` | |dec-implem_descr_genius| |
+------------+---------------------------+

.. |dec-implem_descr_std|    replace:: A standard implementation of the |BCH|.
.. |dec-implem_descr_fast|   replace:: Select the fast implementation optimized
   for |SIMD| architectures.
.. |dec-implem_descr_genius| replace:: A really fast implementation that compare
   the input to the original codeword and correct it only when the number of
   errors is less or equal to the |BCH| correction power.

.. note::
   In the ``STD`` implementation, the Chien search finds roots of the
   error location polynomial. If the number of found roots does not match the
   number of found errors by the |BM| algorithm, then the frame is not modified.

   However, in the ``FAST`` implementation the correction of the bits
   is done at the same time as the execution of the Chien search. Then when the
   latter fails, the frame can be modified.

.. note::
   When a frame is very corrupted and when the above ``STD`` and ``FAST``
   implementations can be wrong in the correction by converging to another
   codeword, the ``GENIUS`` implementation cannot fail. Results may then differ
   from a real word implementation.


.. _dec-bch-dec-corr-pow:

``--dec-corr-pow, -T``
""""""""""""""""""""""

   :Type: integer
   :Default: 5
   :Examples: ``--dec-corr-pow 18``

|factory::Decoder_BCH::p+corr-pow,T|

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Bch-
