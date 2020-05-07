# QtCaPso

A Qt desktop application that simulates the prey-predator model described in
[[1]](#1).

The model describes the interactions between a sessile prey and its predator,
such interactions are local in nature and occur on a two dimensional lattice,
where periodic boundaries have been implemented. Each site of the lattice
may be occupied by a prey, a predator, both or be empty. The movement of
predators is governed by a local particle swarm optimization algorithm.

## Getting started

### Dependencies

  * A compiler with C++ 14, (MinGW 7.3.0 64 bit on Windows, and gcc 7.4.0 on
    Linux have been tested).
  * Qt 5.12.2

### Bulding with QtCreator

  1. Clone the repository to a local directory:
```sh
$ git clone https://github.com/mariomartinezm/QtCaPso
```

  2. Run QtCreator and open the project file *QtCaPso.pro*
```
File > Open File or Project...
```

  3. Configure the project's kit and build directories.

  4. Build the project.
```
Build > Build All
```

### Building from source

  1. Clone the repository to a local directory:
```sh
$ git clone https://github.com/mariomartinezm/QtCaPso
$ cd QtCaPso
```

  2. Generate Makefile using ```qmake```:
```sh
$ qmake QtCaPso.pro
```

  3. Build the project
```sh
$ make
```
  4. The resulting QtCaPso executable will be located inside the *release* directory.

### References
<a id="1">[1]</a>
Martínez Molina, M., Moreno Armendáriz, M. A., Tuoh Mora, J. C. S. (2013).
On the spatial dynamics and oscillatory behavior of a prey-predator model
based on cellular automata and particle swarm optimization.
Journal of Theoretical Biology, 336, 173-184.