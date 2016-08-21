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


## Background

Illumos is based on [OpenSolaris][osolaris], the now discontinued
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


## Related make Implementations

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
- generation of included files

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

There are also extension where GNU and Sun make just use
different syntax. For example, conditional macros:

- Sun make: `target := VAR = value`
- GNU make: `taget: VAR = value`

(Note that GNU make interprets `:=` as assignment with immediate
expansion.)

The distributed make that came with OpenOffice is [also named
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


## Naming

I avoided the names `dmake`, `imake` and `smake` because there
are already multiple published make versions that use those
names. There is a [dmake][dmake] that comes with Solaris Studio, a [dmake
bundled with OpenOffice][oodmake], a dmake that OpenOffice's
dmake is based on, an [imake][imake] to build old X versions and
the [Schily smake][smake].

Thus, to not add to the confusion I chose `somake`.


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

