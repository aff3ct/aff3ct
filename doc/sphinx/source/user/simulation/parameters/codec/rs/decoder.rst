.. _dec-rs-decoder-parameters:

|RS| Decoder parameters
-----------------------

.. _dec-rs-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``ALGEBRAIC`` ``CHASE`` ``ML``
   :Default: ``ALGEBRAIC``
   :Examples: ``--dec-type ALGEBRAIC``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+---------------+----------------------------+
| Value         | Description                |
+===============+============================+
| ``ALGEBRAIC`` | |dec-type_descr_algebraic| |
+---------------+----------------------------+
| ``CHASE``     | |dec-type_descr_chase|     |
+---------------+----------------------------+
| ``ML``        | |dec-type_descr_ml|        |
+---------------+----------------------------+

.. _Berlekamp–Massey: https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm
.. _Chien search: https://en.wikipedia.org/wiki/Chien_search

.. |dec-type_descr_algebraic| replace:: Decoding with the `Berlekamp–Massey`_
   algorithm followed by a `Chien search`_.
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

The Berlekamp–Massey : :cite:`Berlekamp1968,Massey1969`

.. _dec-rs-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``GENIUS`` ``STD``
   :Examples: ``--dec-implem GENIUS``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+------------+---------------------------+
| Value      | Description               |
+============+===========================+
| ``GENIUS`` | |dec-implem_descr_genius| |
+------------+---------------------------+
| ``STD``    | |dec-implem_descr_std|    |
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

Set the correction power of the |RS| decoder. This value corresponds to the
number of symbols errors that the decoder is able to correct.

References
""""""""""

.. bibliography:: references.bib