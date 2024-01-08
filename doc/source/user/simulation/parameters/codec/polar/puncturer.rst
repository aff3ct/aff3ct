.. _pct-polar-puncturer-parameters:

Polar Puncturer parameters
--------------------------

.. _pct-polar-pct-fra-size:

``--pct-fra-size, -N`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--pct-fra-size 1``

|factory::Puncturer::p+fra-size,N|

.. _pct-polar-pct-info-bits:

``--pct-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--pct-info-bits 1``

|factory::Puncturer::p+info-bits,K|

.. _pct-polar-pct-type:

``--pct-type``
""""""""""""""

   :Type: text
   :Allowed values: ``NO`` ``SHORTLAST``
   :Default: ``NO``
   :Examples: ``--pct-type NO``

|factory::Puncturer::p+type|

Description of the allowed values:

+---------------+--------------------------------------------------------------+
| Value         | Description                                                  |
+===============+==============================================================+
| ``NO``        | Disable the puncturer.                                       |
+---------------+--------------------------------------------------------------+
| ``SHORTLAST`` | Select the short last puncturing strategy from               |
|               | :cite:`Niu2013,Wang2014,Miloslavskaya2015`.                  |
+---------------+--------------------------------------------------------------+

References
""""""""""

.. bibliography:: references_pct.bib
   :labelprefix: Plrp-