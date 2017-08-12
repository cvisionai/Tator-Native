Developer Info
==============
This application is maintained and developed by [CVision][CVision], but we welcome
contributions from individuals and other organizations.  Feel free to contact
us on our website if you have an idea, feature request, or contribution to make.

Coding style
------------
This project uses the [Google C++ style guide][StyleGuide].  Refer to this for
coding practices and naming conventions.

Running unit tests
------------------
When running cmake, the option BUILD_TESTS must be enabled.

After CMake has been run the ALL_BUILD or INSTALL target has been built,
unit tests can be run by typing:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target RUN_TESTS --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

or on linux/mac:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make test
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[CVision]: http://www.cvisionconsulting.com
[StyleGuide]: https://google.github.io/styleguide/cppguide.html

