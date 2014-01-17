### Welcome to SoDA.

SoDA is a library and toolchain designed to make dependency analysis, test prioritization or test selection on your test suite.

The SoDA library contains the necessary data structures to effeciently store a testcase coverage matrix, the results of your test suite of multiple revisions and the changes in your program during multiple revisions. These data structures can be used to implement prioritization or other dependence analysis related algorithms. 

We are planning to release some evaluation tools that will measure the quality of the implemented algorithms. 

Visit our [wiki](https://github.com/sed-szeged/soda/wiki/SoDA) page to get more information about the project.

The library is released under LGPL license.

### Getting started

#### Dependencies

* cmake
* Boost 1.49 or higher
* pthread
* googletest (for unit tests)
 
#### Compile

```bash
mkdir build #Create a build directory next to the cloned repository.
cd build
cmake ../soda/SoDA
make
```

#### Sample application

You can find a sample prioritization algorithm in the SoDa/cl/SoDATools/sample directory. It demonstrates some of the features available in the SoDA library. The algorithm implements a general test case prioritization based on the coverage matrix. The test cases with higher coverage will be at the beginning of the prioritized list.

### Authors and Contributors

SoDA is created and maintained by the [Department of Software Engineering](http://www.sed.hu), [University of Szeged](http://www.u-szeged.hu). 

If you would like to contribute to the project, create an issue at our GitHub page or send a pull request. Every contribution is welcome!
