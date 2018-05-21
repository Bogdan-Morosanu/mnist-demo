# Threads Demo
Small demo of a command line interface controlling threads.

## Instalation
The easiest way to run this application is to build and run it portably from its
docker container. Once you have docker installed and the docker daemon running,
just run ./build.sh to build the container.

Once built, you run the container with ./run.sh


## Usage
./demo --threads <nb-threads> will run nb-threads, and start parsing commands from stdin.

Commands:
* status       - prints status and other information about each thread.
* pause <nb>   - pause thread number <nb>.
* restart <nb> - restart execution for thread number <nb>.
* stop <nb>    - permanently stop execution for thread number <nb> (cannot be undone).
* exit         - stop parsing command and exit once all threads have finished executing.

## Thread Types
Currently there are three thread types:
* Cout      - provided by the threads submodule for debug purposes, can print messages to cout.
* Warp      - applies affine warping to random point clouds.
* Integrate - integrates e^x^2 numerically on random intervals.

Currently the program starts the first <nb-threads>/2 as Warp threads and the rest as Integrate threads.

## Developing / Inspecting the Code
This project uses CMake to generate project files. The recommended work flow is that you generate your
project files for your own IDE using CMake and then start the docker image with ./run.sh. The source
files will be mapped inside the container, where you can build them with the 'make' command. Whatever
changes you make to the source from your Host OS are visible inside the container, so you can edit
the files in your Host and compile in the container. This workflow has been tested on Linux and Mac OS.

Alternatively you could try to build the application natively, though this has not been tested and is
probably much more of a hassle than using docker. To do so you need the following dependencies:
   * Boost 1.66.0
   * Eigen 3.3.4
   * C++14-compliant compiler (currently we build with gcc 8.1.0)