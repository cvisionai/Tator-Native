Building documentation
======================

Building the documentation requires installing [doxygen][doxysite] and
[LaTeX][latexsite].

After CMake has been run, documentation can be built by typing:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target doc --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

or on linux/mac:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make doc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from the build subdirectory.  This will generate doxygen documentation
in the doxygen subdirectory of the top level repository.  For the html
output, simply open index.html in your favorite browser.  For the pdf
output from latex, open refman.pdf.

A standalone user manual can be built by typing:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target manual --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

or on linux/mac:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make manual
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from the build subdirectory.  This will generate a user manual at
doxygen/manual/latex/refman.pdf.

[doxysite]: https://sourceforge.net/projects/doxygen/
[latexsite]: https://miktex.org/

