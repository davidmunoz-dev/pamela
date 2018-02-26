
# pamela

Epitech third year project : pamela

Cryptography module project

Deadline : 1 week

Beginning of the project : 20/11/2017, 08h42

Group size : 2 persons

Contributor : 
  -onillo_l

# Description

PAM was created by Sun Microsystems in 1995 as an authentication framework.

It then was improved and widely spread among Unix-like systems.

The goal of this project is to write a fully working PAM-module in C that opens one or several user’s encrypted containers.

* Each user should have his own container(s).

* This encrypted container must be in the user’s home directory.

* When a user logs on, his container must be opened (decrypted). When the user logs out, his container must be closed

### Use a virtual machine for testing purposes, otherwise if you mess with your PAM configuration, you might be locked out of your system!

* [PAM](https://en.wikipedia.org/wiki/Pluggable_authentication_module)

## Getting started

These instructions will allow you to obtain a copy of the operational project on your local machine for development and testing purposes.

### Prerequisites

What do you need to install the software and how to install it?

```
gcc
cryptsetup 
libcryptsetup-dev 
libpam0g-dev
make
```

### Installation

Here's how to start the project on your computer

Clone and go in the directory pamela

Project compilation

```
make install
make check
```

Running project

```
Change to another account in your computer or sudo.
```

## Build with

* [C](https://en.wikipedia.org/wiki/C_(programming_language))

## Autor

* **David Munoz** - [DavidMunoz-dev](https://github.com/davidmunoz-dev)
