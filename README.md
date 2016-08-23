This repository contains the [Illumos][illumos] make source code (in branch
`illumos`) and based on that a port to Linux (branch `master`).

The Illumos make is based on Sun's dmake, which is compatible to
Sun make.

Motivation: compile old Solaris projects (that use Sun make)
under Linux without much effort. Think: the C code needs serious
fixing because it includes metric tons of non-portable constructs
such that you want to postpone the porting of the existing build
system (which heavily uses Sun make specific extensions).

Another motivation to use a Sun compatible make might be the
inclination to test one of its extensions, e.g. target groups,
command and hidden dependencies.

2016, Georg Sauthofff <mail@georg.so>


## Build Instructions

    git clone https://github.com/gsauthof/somake.git
    mkdir somake-build
    cd somake-build
    cmake ../somake -DCMAKE_BUILD_TYPE=Release
    make

Or to use Ninja instead of GNU Make:

    git clone https://github.com/gsauthof/somake.git
    mkdir somake-build
    cd somake-build
    cmake ../somake -DCMAKE_BUILD_TYPE=Release -G Ninja
    ninja-build

## Examples

The following assumes that the repository was cloned to the
directory `somake` and that `somake-build` is the build
directory.

Copy the example files and the basic rule file for testing:

    $ cd ../somake-build
    $ cp -r ../somake/example/ .
    $ cp ../somake/bin/make.rules.file example/make.rules
    $ cd example
    $ ln -s ../somake

Compile the hello world program:

    $ ./somake -m serial helloworld
    cc    -o helloworld helloworld.c

Verify that make detects that no rebuild is necessary:

    $ ./somake -m serial helloworld
    `helloworld' is up to date.

Test command dependencies:

    $ ./somake -m serial helloworld CC=gcc
    gcc    -o helloworld helloworld.c

Also works for other variables:

    $ ./somake -m serial helloworld CC=gcc CFLAGS=-Wall
    gcc -Wall   -o helloworld helloworld.c

Test hidden dependency checks:

    $ ./somake -m serial hello
    cc   -c  hello.c
    cc   -c  world.c
    cc  hello.o world.o -o hello
    $ ./somake -m serial hello
    `hello' is up to date.
    $ touch world.h
    $ ./somake -m serial hello
    cc   -c  hello.c
    cc   -c  world.c
    cc  hello.o world.o -o hello
    $ ./somake -m serial hello
    `hello' is up to date.
    $ touch world.h
    $ ./somake -m serial world.o
    cc   -c  world.c
    $ ./somake -m serial hello
    cc   -c  hello.c
    cc  hello.o world.o -o hello

Note that command and hidden dependencies are enabled by declaring the
`.KEEP_STATE:` pseudo target.

Test target groups:

    $ ./somake -m serial main_foo
    ./gen_foo.sh
    cc   -c  main_foo.c
    cc   -c  foo.c
    cc  foo.h main_foo.o foo.o -o main_foo

Now remove the `+` in the generating rule

    foo.c + foo.h: gen_foo.sh
	    ./gen_foo.sh

and run again:

    $ ./somake -m serial clean
    $ rm .make.state
    $ ./somake -m serial main_foo
    ./gen_foo.sh
    cc   -c  main_foo.c
    ./gen_foo.sh
    cc   -c  foo.c
    cc  foo.h main_foo.o foo.o -o main_foo

Note that the `./gen_foo.sh` action is now executed twice because the rule

    foo.c foo.h: gen_foo.sh
	    ./gen_foo.sh

is equivalent to two separate rules:

    foo.c: gen_foo.sh
	    ./gen_foo.sh

    foo.h: gen_foo.sh
	    ./gen_foo.sh

This is how classic make behaves - also GNU make - although in
this toy example it is harder to trigger - because in sequential
execution GNU make uses a advantageous timestamp comparison
strategy:

    $ make main_foo
    ./gen_foo.sh
    cc    -c -o main_foo.o main_foo.c
    cc    -c -o foo.o foo.c
    cc  main_foo.o foo.o -o main_foo

But a parallel execution will do:

    $ make -j3 main_foo
    ./gen_foo.sh
    ./gen_foo.sh
    cc    -c -o main_foo.o main_foo.c
    cc    -c -o foo.o foo.c
    cc  main_foo.o foo.o -o main_foo

Note that GNU make supports target groups with pattern rules, though:

    $ make -j3 both
    echo x > foo.bar
    sed 's/x/y/' foo.bar > foo.one
    sed 's/x/z/' foo.bar > foo.two

With Sun make the same result is yielded iff a `+` is inserted between both
pattern targets:

    $ ./somake -m serial both
    echo x > foo.bar
    sed 's/x/y/' foo.bar > foo.one
    sed 's/x/z/' foo.bar > foo.two

The example makefile also contains an example for Sun make style conditional
macro assignments (with `:=`):

    $ ./somake -m serial warn-helloworld
    cc -Wall   -o helloworld helloworld.c

The combination with command dependencies makes this feature even more useful:

    $ ./somake -m serial warn-helloworld
    cc -Wall   -o helloworld helloworld.c
    $ ./somake -m serial helloworld
    cc    -o helloworld helloworld.c
    $ ./somake -m serial warn-helloworld
    cc -Wall   -o helloworld helloworld.c

In contrast, GNU make doesn't rebuild `helloworld` because it doesn't
consider changed variables.

## Background

[Illumos][illumos] is based on [OpenSolaris][osolaris], the now discontinued
open-source version of Solaris. OpenSolaris was started by Sun
after the Solaris 10 release and was closed down in 2010 after
Oracle bought Sun.  Sun used the [CDDL][cddl] license on its open
sourced code.

Traditionally, Solaris includes it own version of `make` under
`/usr/ccs/bin/make`, also known as 'Sun make'. It has some
extensions over other make versions, such as BSD make and
[POSIX][posix] make. It only supports sequential execution
(unlike e.g. GNU make), however, Sun also developed `dmake`
(think: distributed make), usually bundled with the [Solaris
compiler suite 'Solaris Studio'][solstudio]. Dmake is compatible
with the Sun make syntax and besides serial execution it supports
parallel execution on an SMP system and on a cluster (apparently
similar to [distcc][distcc]). It was also open-sourced by Sun
under the CDDL.

Illumos started with the code base that included the original Sun
make but later switched to dmake.


## Comparison with other make Implementations

[GNU make][gmake] is the most popular, portable and thus relevant
make. Similar to most GNU tools it contains many very useful
extension over the traditional make and it basically runs
everywhere.

Examples of GNU make features that are missing from Sun make:

- parallel execution (the Sun make compatible dmake supports it)
- conditional constructs
- many built-in function
- user-defined functions
- templates
- guile integration

Sun make features that aren't supported by GNU make:

- target groups - i.e. to specify that more than one target
  depends on a source file and that the one action generates
  those targets at once. GNU make only supports a group of
  targets with pattern rules.
- command dependencies - i.e. a target is also considered
  out-of-date if the command changes - i.e. due to changed values
  of variables like `$(CC)` or `$(CFLAGS)`
- hidden dependencies - automatic discovery of header
  dependencies

There are also features where GNU and Sun make just use
different syntax. For example, conditional macros:

- Sun make: `target := VAR = value`
- GNU make: `taget: VAR = value`

(Note that GNU make interprets `:=` as assignment with immediate
expansion.)

As always, neither GNU make nor Sun make/dmake are bug-free.  For
example, in versions released before 2016, the `include` file
generation in GNU make prints misleading messages (cf. e.g. bug
102). Sun make's `.make.state` file (that is used for hidden
dependency and command dependency tracking) sometimes causes more
harm than good - e.g. when outdated dependencies aren't removed
(e.g. when one converts a target group into a target list).

The distributed make that comes with OpenOffice is [also named
DMake][oodmake]. It seems that it was independently developed of Sun's
dmake - and that it uses a different syntax.


## Portability

The changes in the `master` branch fix portability issues in the
original code such that the resulting `make` compiles and runs
under Linux. Also, a [CMake][cmake] build file is included for portable
building.

Although those changes target Linux, they aren't really Linux
specific. That means that the ported make should also compile on
other modern [POSIXy][posix] operating systems.

So far, the port was successfully tested under:

- Fedora 23/x86-64
- Solaris 10/SPARC


## Naming

I avoided the names `dmake`, `imake` and `smake` because there
are already multiple published make versions that use those
names. There is a [dmake][dmake] that comes with Solaris Studio, a [dmake
bundled with OpenOffice][oodmake], a dmake that OpenOffice's
dmake is based on, an [imake][imake] to build old X versions and
the [Schily smake][smake].

Thus, to not add to the confusion I chose `somake`.


## Installation

The build file also contains an install target, e.g.:

    $ DESTDIR=dest ninja-build install
    [1/1] Install the project...
    -- Install configuration: "Release"
    -- Installing: dest/usr/local/bin/somake
    [..]

If you use the standard makefile generator with cmake, just
replace `ninja-build` with `make`. Also, omitting the `DESTDIR`
installs everything for real. The destdir mechanism is useful for
preparing a binary package and just to have a preview.

To change the default install prefix, you have to call `cmake`
differently, e.g.:

    $ cmake ../somake -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release -G Ninja
    $ DESTDIR=dest ninja-build install
    [..]
    -- Install configuration: "Release"
    -- Installing: dest/usr/bin/somake
    [..]

### Tweaks

If you are compiling on a legacy platform with ancient libraries
you may need to tweak the cmake call a little bit. For example,
to compile with GCC on a Solaris 10 system:

    $ CC=gcc CXX=g++ LDFLAGS='-lnsl /opt/csw/lib/libintl.so.8' \
        cmake -DCMAKE_BUILD_TYPE=Release ../somake

### Manual Installation

In case you prefer a manual installation, basically it is just:

- copy the created `somake` binary to a `bin/` directory under some prefix
- copy the man page into the related manpath
- copy the rule files that contain the built-in rules and are located
  in the `bin/` subdirectory of this repository to one of the directories
  searched by `somake` (and also remove the `.file` suffix)

### Rule Search Path

Using `make.rules` as an example, `somake` tries to open it in
the following order:

1. `make.rules`
2. `$ORIGIN/../share/somake/make.rules`   # added by this port
3. `$ORIGIN/../share/lib/make/make.rules`
4. `$ORIGIN/../../share/make.rules`
5. `/usr/share/lib/make/make.rules`
6. `/etc/default/make.rules`


## License

Perhaps not required by the CDDL, but I license my changes
also under the [CDDL][cddl].


[illumos]: https://en.wikipedia.org/wiki/Illumos
[osolaris]: https://en.wikipedia.org/wiki/OpenSolaris
[solaris]: https://en.wikipedia.org/wiki/Solaris_(operating_system)
[cddl]: https://en.wikipedia.org/wiki/Common_Development_and_Distribution_License
[distcc]: https://en.wikipedia.org/wiki/Distcc
[solstudio]: https://en.wikipedia.org/wiki/Oracle_Developer_Studio
[gmake]: https://www.gnu.org/software/make/
[oodmake]: https://www.openoffice.org/tools/dmake/
[cmake]: https://en.wikipedia.org/wiki/CMake
[posix]: https://en.wikipedia.org/wiki/POSIX#Mostly_POSIX-compliant
[smake]: https://sourceforge.net/projects/s-make/
[imake]: https://en.wikipedia.org/wiki/Imake
[dmake]: https://docs.oracle.com/cd/E24457_01/html/E25220/index.html

