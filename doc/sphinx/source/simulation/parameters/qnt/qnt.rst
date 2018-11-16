.. _qnt-quantizer-parameters:

Quantizer parameters
--------------------

The Quantizer module is activated only when running quantified decoders, ie.
when the simulation precision is set to ``8`` or ``16``
(cf. :ref:`sim-sim-prec`).

It converts input floating points frames into output quantified fixed point
frames. The position of the fixed point is settable by the user.

.. Danger:: The total number of bits used for the quantification does not fix
   a saturation for the decoder itself that uses the whole available bits given
   by the simulation precision.

.. Warning:: All codes do not work with a quantification process.

.. TODO: list those codes that do not work with fixed point.

.. _qnt-qnt-type:

``--qnt-type``
""""""""""""""

   :Type: text
   :Allowed values: ``CUSTOM`` ``POW2``
   :Default: ``POW2``
   :Examples: ``--qnt-type CUSTOM``

Type of the quantizer to use in the simulation.




Description of the allowed values:

+------------+-------------------------+
| Value      | Description             |
+============+=========================+
| ``CUSTOM`` | |qnt-type_descr_custom| |
+------------+-------------------------+
| ``POW2``   | |qnt-type_descr_pow2|   |
+------------+-------------------------+

.. |qnt-type_descr_custom| replace:: :math:`Q_i = \begin{cases}
   +v_{sat} & \text{when } v_i > +v_{sat} \\
   -v_{sat} & \text{when } v_i < -v_{sat} \\
   v_i      & \text{else}
   \end{cases}` with :math:`v_i = \lfloor \frac{Y_i}{\Delta} \rceil
   \text{ and } v_{sat} = 2^{p_b - 1} - 1
   \text{ and } \Delta = \frac{|p_r|}{v_{sat}}`

.. |qnt-type_descr_pow2| replace:: :math:`Q_i = \begin{cases}
   +v_{sat} & \text{when } v_i > +v_{sat} \\
   -v_{sat} & \text{when } v_i < -v_{sat} \\
   v_i      & \text{else}
   \end{cases}` with :math:`v_i = \lfloor Y_i * F \rceil
   \text{ and } v_{sat} = 2^{p_b - 1} - 1
   \text{ and } F = 2^{p_d}`

Where :math:`p_r`, :math:`p_b` and :math:`p_d` are respectively given through
:ref:`qnt-qnt-range`, :ref:`qnt-qnt-bits` and :ref:`qnt-qnt-dec`.


.. _qnt-qnt-range:

``--qnt-range``
"""""""""""""""

   :Type: real number
   :Examples: ``--qnt-range 1.0``

The min/max bound for the tricky quantizer.


.. _qnt-qnt-bits:

``--qnt-bits``
""""""""""""""

   :Type: integer
   :Default: 8 else see :numref:`bits_default_table`
   :Examples: ``--qnt-bits 1``

Sets the number of bits used for the quantizer.


.. _bits_default_table:

.. table:: Default values for the total number of bits for the different codes.

   +----------+----------+-----------+---------+---------+------------+-----------+--------------+---------------+
   | Code     | ``LDPC`` | ``POLAR`` | ``REP`` | ``RSC`` | ``RSC_DB`` | ``TURBO`` | ``TURBO_DB`` | ``TURBO_PROD``|
   +==========+==========+===========+=========+=========+============+===========+==============+===============+
   | Value    |   6      |   6       |   6     |   6     |   6        |   6       |   6          ||bit_turboprod||
   +----------+----------+-----------+---------+---------+------------+-----------+--------------+---------------+

.. |bit_turboprod| replace:: 6 on 8 bits and 8 on 16 bits

.. _qnt-qnt-dec:

``--qnt-dec``
"""""""""""""

   :Type: integer
   :Default: 3 else see :numref:`dec_default_table`
   :Examples: ``--qnt-dec 1``

Sets the position of the fixed point in the quantified representation.

.. _dec_default_table:

.. table:: Default values for the fixed point position for the different codes.

   +----------+----------+-----------+---------+-----------+------------+-----------+--------------+---------------+
   | Code     | ``LDPC`` | ``POLAR`` | ``REP`` | ``RSC``   | ``RSC_DB`` | ``TURBO`` | ``TURBO_DB`` | ``TURBO_PROD``|
   +==========+==========+===========+=========+===========+============+===========+==============+===============+
   | Value    |   2      |   1       |   2     | |dec_RSC| | |dec_RSCDB|||dec_turbo|| |dec_turbodb|||dec_turboprod||
   +----------+----------+-----------+---------+-----------+------------+-----------+--------------+---------------+

.. |dec_RSC|       replace:: 1 on 8 bits and 3 on 16 bits
.. |dec_RSCDB|     replace:: 1 on 8 bits and 3 on 16 bits
.. |dec_turbo|     replace:: 2 on 8 bits and 3 on 16 bits
.. |dec_turbodb|   replace:: 2 on 8 bits and 3 on 16 bits
.. |dec_turboprod| replace:: 2 on 8 bits and 3 on 16 bits

.. _qnt-qnt-implem:

``--qnt-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``STD``
   :Default: ``STD``
   :Examples: ``--qnt-implem FAST``

Selects the implementation of the quantizer.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``FAST`` | |qnt-implem_descr_fast| |
+----------+-------------------------+
| ``STD``  | |qnt-implem_descr_std|  |
+----------+-------------------------+

.. |qnt-implem_descr_fast| replace:: A standard implementation
.. |qnt-implem_descr_std|  replace:: A much faster method using |SIMD| but *only
   for* ``POW2`` *type*.
