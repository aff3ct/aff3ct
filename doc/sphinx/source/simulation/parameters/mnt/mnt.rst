.. _mnt-monitor-parameters:

Monitor parameters
------------------

.. _mnt-mnt-err-hist:

``--mnt-err-hist``
""""""""""""""""""

   :Type: integer
   :Examples: ``--mnt-err-hist 1``

Activate the histogram of the number of errors per frame. Set the max number of bit error per frame included in the histogram (0 is no limit).

.. _mnt-mnt-err-hist-path:

``--mnt-err-hist-path``
"""""""""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--mnt-err-hist-path example/path/to/the/right/file``

Path to the output histogram (default is './hist', add automatically the current noise value and the extension '.txt').

.. _mnt-mnt-max-fe:

``--mnt-max-fe, -e``
""""""""""""""""""""

   :Type: integer
   :Examples: ``--mnt-max-fe 1``

Max number of frame errors for each noise point simulation.

.. _mnt-mnt-mutinfo:

``--mnt-mutinfo``
"""""""""""""""""


Allow the computation of the mutual information.

