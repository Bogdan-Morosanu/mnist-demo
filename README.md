# Threads Demo

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