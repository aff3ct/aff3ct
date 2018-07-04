# AFF3CT Documentation Generation

**AFF3CT** uses [Doxygen](http://www.doxygen.org) to generate automatically the documentation.

Doxygen can easily be installed on Ubuntu/Debian systems like this:

    $ sudo apt-get install doxygen

To generate the documentation, you simply have to do:

    $ doxygen config.txt

The last command will create two folders: `html` and `latex`.
In the `html` folder you just have to open the `index.html` file to read the generated documentation.