Building documentation
======================

Building the documentation requires installing [doxygen][doxysite] and
[LaTeX][latexsite].

When cmake is run, the option BUILD_DOCS must be enabled.

After CMake has been run, documentation can be built by typing:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target doc --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

or on linux/mac:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make doc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from the build subdirectory.  This will generate doxygen documentation
in the build/documentation subdirectory.  For the html
output, simply open index.html in your favorite browser.  For the pdf
output from latex, navigate to the latex output directory and run 
make or make.bat on Windows.

[doxysite]: https://sourceforge.net/projects/doxygen/
[latexsite]: https://miktex.org/

