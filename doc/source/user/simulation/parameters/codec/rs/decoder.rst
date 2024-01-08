.. _dec-rs-decoder-parameters:

|RS| Decoder parameters
-----------------------

The RS decoder was described by Reed and Solomon in 1960 :cite:`ReedSolomon1960`.

.. _dec-rs-dec-type:

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
|               | :cite:`Berlekamp1968a,Massey1969a` followed by a             |
|               | `Chien search`_ :cite:`Chien1964a`.                          |
+---------------+--------------------------------------------------------------+
| ``CHASE``     | See the common :ref:`dec-common-dec-type` parameter.         |
+---------------+--------------------------------------------------------------+
| ``ML``        | See the common :ref:`dec-common-dec-type` parameter.         |
+---------------+--------------------------------------------------------------+

.. _dec-rs-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``STD`` ``GENIUS``
   :Default: ``STD``
   :Examples: ``--dec-implem GENIUS``

|factory::Decoder::p+implem|

Description of the allowed values:

+------------+---------------------------+
| Value      | Description               |
+============+===========================+
| ``STD``    | |dec-implem_descr_std|    |
+------------+---------------------------+
| ``GENIUS`` | |dec-implem_descr_genius| |
+------------+---------------------------+

.. |dec-implem_descr_std|    replace:: A standard implementation of the |RS|.
.. |dec-implem_descr_genius| replace:: A really fast implementation that compare
   the input to the original codeword and correct it only when the number of
   symbols errors is less or equal to the |RS| correction power.

.. note::
   In the ``STD`` implementation, the Chien search finds roots of the
   error location polynomial. If the number of found roots does not match the
   number of found errors by the Berlekamp–Massey algorithm, then the frame is
   not modified.

   When a frame is very corrupted and when the above algorithms can be
   wrong in the correction by converging to another codeword, the ``GENIUS``
   implementation cannot fail. Results may then differ from a real word
   implementation.

.. _dec-rs-dec-corr-pow:

``--dec-corr-pow, -T``
""""""""""""""""""""""

   :Type: integer
   :Default: 5
   :Examples: ``-T 18``

|factory::Decoder_RS::p+corr-pow,T|

It is automatically calculated from the input and codeword sizes. See also
the argument :ref:`enc-rs-enc-info-bits`.

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Rs-
