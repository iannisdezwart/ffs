# Fabulous Fabrication System: The C/C++ package manager and build tool we've all been waiting for.

Building C/C++ software from source and managing dependencies is a huge hassle.
Why can't we have something simple and easy to use? Something like JavaScript's
NPM, Ruby's Gem or Python's pip? Well, wait no longer. FFS is the package
manager we've all been waiting for, for way too long.


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
This command will build the targets specified in the `ffs.toml` file and place
them in the `bin` directory.


### Installing packages

To install a specific package, run:
```sh
ffs install <package-name>
```

This will add the package as a dependency to the `ffs.toml` file and install it.

To install all missing packages, run:
```sh
ffs install
```


### `ffs.toml` File

The `ffs.toml` file stores information about the dependencies and target
binaries of the package. Take a look at the following example:
```toml
[package]
name = "my_package"
description = "Does things."
author = "me"

[dependencies]
some_package = "0.1.0" # Specific version from the centralised FFS registry.
another_package = "latest" # Latest version from the centralised FFS registry.
third_package = "https://github.com/user/repo.git" # A git repository.

[targets.ffs]
src = [
	"main.cpp",
	"other.cpp",
	"third.cpp"
]
```