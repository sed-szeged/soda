### Welcome to SoDA.

SoDA is a library and toolchain designed to make dependency analysis, test prioritization or test selection on your test suite.

The SoDA library contains the necessary data structures to efficiently store a coverage matrix for multiple test cases, the results of your test suite execution of multiple revisions and the changes in your program during multiple revisions. These data structures can be used to implement prioritization or other dependence analysis related algorithms. 

We are planning to release evaluation tools that will measure the quality of the implemented algorithms. 

Visit our [wiki](https://github.com/sed-szeged/soda/wiki/) page to get more information about the project.

The library is released under LGPLv3 license.

### Getting started

#### Dependencies

* cmake 2.8.0 or higher
* Boost 1.49 or higher
* pthread
* googletest (for unit tests)
 
#### Compile

```bash
mkdir build #Create a build directory next to the cloned repository.
cd build
cmake ../soda/src  # Pass -Dtest=ON argument if you want to execute the unit tests.
make
```

#### Command line programs

The library contains several command line programs:
* instrumentserver
 * Creates SoDA coverage matrix when you executing your instrumented code.
* test-suite-selections
 * Runs one or more selection algorithm on the test suite an computes the precision, inclusiveness of the selected test cases according to the results of the test executions.
* test-selection-statistics
 * Computes several statistics of the test suite like density of coverage matrix, average changes per revision, etc.
* test-suite-metrics
 * Computes the FL and FD metric values of the test suite.
* test-suite-score
 * Computes the fault detection score and fault localization score of a test suite based on actual faults.
* test-suite-reduction
 * Reduces the given coverage data with specified reduction algorithms.
* binaryDump
 * Reads and saves data from the binary formats in a human readable format.
* rawDataReader
 * Create SoDA coverage, results and changeset matrices from raw data sources.

#### Sample application

You can find a sample prioritization algorithm in the src/cl/SoDATools/sample directory. It demonstrates some of the features available in the SoDA library. The algorithm implements a general test case prioritization based on the coverage matrix. The test cases with higher coverage will be at the beginning of the prioritized list.

### Authors and Contributors

SoDA is created and maintained by the [Department of Software Engineering](http://www.sed.hu), [University of Szeged](http://www.u-szeged.hu). 

#### Contribution
If you would like to contribute to the project, create an issue at our GitHub page or send a pull request. Every contribution is welcome!

We are using [git flow](http://danielkummer.github.io/git-flow-cheatsheet/) to manage our repository.
The *origin/develop* is our main development branch. *origin/master* is always in production ready state.
