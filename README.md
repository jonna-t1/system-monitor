# CppND-System-Monitor

A Linux based System monitor that provides text-based graphical output in the terminal.

Object Oriented Programming Project of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Within the Udacity Workspace, `.student_bashrc` automatically installs ncurses every time you launch the Workspace.

If you are not using the Workspace, install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/jonna-t1/system-monitor`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
<<<<<<< HEAD
![Starting System Monitor](images/monitor.png)
=======
![Starting System Monitor](images/monitor.png)
>>>>>>> 19a73753c054cafa5db656d5d724b7f437546bea
