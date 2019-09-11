# C++ Utils

This repository intends to provide common utils used in C++ coding, all in one place.  
Not everything is my original work. Please refer to the list below or the individual files for the authorship information.

## Components
* **Logging.hpp**  
This file uses [Boost.Log](https://www.boost.org/doc/libs/1_71_0/libs/log/doc/html/index.html) library for providing logging functionality.  
It requires the `boost_log`, `boost_system`, and `pthread` libraries.
* **Timer.hpp**  
This file uses C++ [std::chrono](http://en.cppreference.com/w/cpp/header/chrono) library for providing timing functionality.
* **prettyprint.hpp**  
This file is from the [cxx-prettyprint](https://github.com/louisdx/cxx-prettyprint) repository distributed under the [Boost License](https://github.com/louisdx/cxx-prettyprint/blob/master/LICENSE_1_0.txt).

## Requirements
* **gcc** (with C++11 support)  
This project has been test built only on Linux platform, using gcc with C++11 support.

## Building
This repository is not intended to be used in a stand-alone mode.
