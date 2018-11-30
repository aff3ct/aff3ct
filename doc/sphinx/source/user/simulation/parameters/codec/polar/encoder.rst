.. _enc-polar-encoder-parameters:

Polar Encoder parameters
------------------------

.. _enc-polar-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``POLAR`` ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``POLAR`` | |enc-type_descr_polar| |
+-----------+------------------------+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_polar| replace:: TODO VALUE POLAR
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-polar-enc-fb-awgn-path:

``--enc-fb-awgn-path``
""""""""""""""""""""""

   :Type: path
   :Rights: read only
   :Examples: ``--enc-fb-awgn-path example/path/to/the/right/place/``

Path to a file or a directory containing the best channels to use for
information bits.

.. _enc-polar-enc-fb-gen-method:

``--enc-fb-gen-method``
"""""""""""""""""""""""

   :Type: text
   :Allowed values: ``FILE`` ``GA`` ``TV``
   :Examples: ``--enc-fb-gen-method FILE``

Select the frozen bits generation method.

Description of the allowed values:

+----------+--------------------------------+
| Value    | Description                    |
+==========+================================+
| ``FILE`` | |enc-fb-gen-method_descr_file| |
+----------+--------------------------------+
| ``GA``   | |enc-fb-gen-method_descr_ga|   |
+----------+--------------------------------+
| ``TV``   | |enc-fb-gen-method_descr_tv|   |
+----------+--------------------------------+

.. |enc-fb-gen-method_descr_file| replace:: TODO VALUE FILE
.. |enc-fb-gen-method_descr_ga| replace:: TODO VALUE GA
.. |enc-fb-gen-method_descr_tv| replace:: TODO VALUE TV

.. _enc-polar-enc-fb-sigma:

``--enc-fb-sigma``
""""""""""""""""""

   :Type: real number
   :Examples: ``--enc-fb-sigma 1.0``

Sigma value for the polar codes generation (adaptive frozen bits if sigma is
not set).

.. _enc-polar-enc-no-sys:

``--enc-no-sys``
""""""""""""""""

Disable the systematic encoding.