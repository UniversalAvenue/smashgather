# Smashgather Client

This is an OpenCV program that detects win screens, extracts the winning character name, and records games to the Smashgather server.

## Getting Started
Smashgather needs to be built from source, and it probably only works on OSX (sorry!). You'll need some libraries...

### OpenCV
Follow the instructions [here](http://www.pyimagesearch.com/2015/06/15/install-opencv-3-0-and-python-2-7-on-osx/), but ignore the bits about Python. When you're done, you should have a bunch of opencv libs installed, check out at `ls /usr/local/lib/libopencv*`.

### Submodules
The "C++ Requests" library we use is installed as a submodule (see the [github](https://github.com/whoshuu/cpr)). You should be able to sync the submodule files with:
`git submodule update --init --recursive`

### CMake
At this point, we need to figure out where all your libraries are installed and setup the build step. CMake does all that work, just run:
`cmake .`

Now you should have a `Makefile`.

## Building
Run `make` to build the `smashgather` and `smashgather_tests` binaries.

## Running
Run `./smashgather` to start the client. It'll try to connect to a local Smashgather server; to connect to a cloud server, set the `SERVER_URL` environment variable:
```
~/git/smashgather/client $ SERVER_URL=http://smashgather.herokuapp.com/graphql ./smashgather
==================================================================================
   _____ __  __           _____ _    _  _____       _______ _    _ ______ _____
  / ____|  \/  |   /\    / ____| |  | |/ ____|   /\|__   __| |  | |  ____|  __ \
 | (___ | \  / |  /  \  | (___ | |__| | |  __   /  \  | |  | |__| | |__  | |__) |
  \___ \| |\/| | / /\ \  \___ \|  __  | | |_ | / /\ \ | |  |  __  |  __| |  _  /
  ____) | |  | |/ ____ \ ____) | |  | | |__| |/ ____ \| |  | |  | | |____| | \ \
 |_____/|_|  |_/_/    \_\_____/|_|  |_|\_____/_/    \_\_|  |_|  |_|______|_|  \_\
==================================================================================
Connecting to Smashgather server...
Start POST http://smashgather.herokuapp.com/graphql: query { viewer { games(first: 1) { edges { node { createdAt, character { name } } } } } }
Finished POST http://smashgather.herokuapp.com/graphql
Response (code: 200):
  {
  "data": {
    "viewer": {
      "games": {
        "edges": [
          {
            "node": {
              "createdAt": "Sat Mar 12 2016 00:25:00 GMT+0000 (UTC)",
              "character": {
                "name": "Ness"
              }
            }
          }
        ]
      }
    }
  }
}
Connected! Running Smashgather (use CTRL+C to exit)...
```

## Tests
Run `./smashgather_tests` to run the test images through the win detection logic, with no validations... It's not a good test suite.
