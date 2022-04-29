# Fabulous Fabrication System: The C/C++ package manager we've all been waiting for.

Building C/C++ software from source and managing dependencies is a huge hassle.
Why can't we have something simple and easy to use? Something like NodeJS's
package manager (NPM), Ruby's Gem or Python's pip? Well, wait no longer. FFS is
the package manager we've all been waiting for, for way too long.


## Installation

Clone the repo and run:
```sh
make install
```

The above command will install `ffs` in your system.


## Usage


### Building a package

To build a package, run:
```sh
ffs build
```

Any missing dependencies will be automatically installed.
This command will build the binaries specified in the `build` file and place
them in the `bin` directory.


### Installing packages

To install a specific package, run:
```sh
ffs install <package-name>
```

This will add the package to the `dependencies` file and install it.

To install all missing packages, run:
```sh
ffs install
```


### `dependencies` File

The `dependencies` file is a list of packages that are required to build the
project. It's a yaml file of the form:
```yaml
# name: git url
foo: https://github.com/foo/foo.git
bar: https://github.com/bar/bar.git
```

Each dependency is cloned into the `lib` directory and built if needed.
In the future, I plan to add support for a registry of packages, so that
a package can simply be referenced by its name.


### `build` File

The `build` file specifies the binaries that should be built and what source
files they are composed of. It's a yaml file of the form:
```yaml
my_binary:
  - src/main.cpp
  - src/other.cpp
  - src/third.cpp

my_other_binary:
  - src/other_main.cpp
  - src/other.cpp
```

The binaries are built in the `bin` directory.