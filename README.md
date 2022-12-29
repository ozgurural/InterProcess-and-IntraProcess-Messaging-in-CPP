# 360T - Technical challenge C++ position

### Project Description
This project is a simple implementation of a messaging system between two players. The players can communicate with each other in two different ways:

Same process messaging: In this type of messaging, both players reside in the same process.

Separate process messaging: In this type of messaging, both players reside in separate processes and communicate using pipes.

The messages exchanged between the players are simple string messages. The players send and receive messages in an iterative manner until a predefined message count is reached.

### Requirements

- C++20
- CMake 3.22

### Directory Structure

The project consists of the following directories:

- include - This directory contains the header files for the project.
- src - This directory contains the source files for the project.
- test - This directory contains the unit tests for the project.

### Class Definitions

#### Player

This class represents a player in the messaging system. It has the following responsibilities:

- Initialize the player with a name, message count, pipes, and send state (for separate process messaging).
- Implement a method for sending messages to another player in the same process (SameProcessSendMessage).
- Implement a method for receiving messages from another player in the same process (SameProcessReceiveMessage).
- Implement a method for writing to a pipe (WritePipeThread) in separate process messaging.
- Implement a method for reading from a pipe (ReadPipeThread) in separate process messaging.
- Implement a method to check if the termination condition has been reached (ShouldTerminate).

#### PlayerManager
This class is responsible for managing the players and coordinating their communication. It has the following responsibilities:

- Implement methods for starting same process messaging (startSameProcessMessaging) and separate process messaging (startSeparateProcessMessaging).
- Create instances of Player objects and pass them to the above mentioned methods for starting messaging.
- Create pipes and processes for seperate process messaging.

### File Descriptions

The project consists of the following components:

- main.cpp: The entry point of the program. It creates an instance of PlayerManager and calls the appropriate methods for starting same process or separate process messaging.
- Logger.h: A simple logger class that can be used to print log messages to the console.
- Player.h: The header file for the Player class.
- Player.cpp: The implementation file for the Player class.
- PlayerManager.h: The header file for the PlayerManager class.
- PlayerManager.cpp: The implementation file for the PlayerManager class.

### Building and Running the Project

To build and run the project, create a build directory and run CMake in it:

```sh
mkdir build
cd build
cmake ..
make
./player
```

A sample output of unit_tests is as the following:

```sh
ozgurural@ozgurural:~/Desktop/360T/build$ ./player 
[INFO][2022-12-29 01:39:38.083587]: Starting same process messaging
[INFO][2022-12-29 01:39:38.083655]: Both players are running in the same process.
[INFO][2022-12-29 01:39:38.083671]: Player 1 and Player 2 are created.
[INFO][2022-12-29 01:39:38.083682]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083692]: Player 1(Process Id: 65595): Sending message 1: Hello, world!
[INFO][2022-12-29 01:39:38.083701]: Player 2(Process Id: 65595): Received message 1: Hello, world!
[INFO][2022-12-29 01:39:38.083711]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083719]: Player 1(Process Id: 65595): Sending message 2: Hello, world! 1
[INFO][2022-12-29 01:39:38.083727]: Player 2(Process Id: 65595): Received message 2: Hello, world! 1
[INFO][2022-12-29 01:39:38.083735]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083742]: Player 1(Process Id: 65595): Sending message 3: Hello, world! 1 2
[INFO][2022-12-29 01:39:38.083750]: Player 2(Process Id: 65595): Received message 3: Hello, world! 1 2
[INFO][2022-12-29 01:39:38.083759]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083767]: Player 1(Process Id: 65595): Sending message 4: Hello, world! 1 2 3
[INFO][2022-12-29 01:39:38.083777]: Player 2(Process Id: 65595): Received message 4: Hello, world! 1 2 3
[INFO][2022-12-29 01:39:38.083787]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083795]: Player 1(Process Id: 65595): Sending message 5: Hello, world! 1 2 3 4
[INFO][2022-12-29 01:39:38.083803]: Player 2(Process Id: 65595): Received message 5: Hello, world! 1 2 3 4
[INFO][2022-12-29 01:39:38.083812]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083820]: Player 1(Process Id: 65595): Sending message 6: Hello, world! 1 2 3 4 5
[INFO][2022-12-29 01:39:38.083828]: Player 2(Process Id: 65595): Received message 6: Hello, world! 1 2 3 4 5
[INFO][2022-12-29 01:39:38.083840]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083849]: Player 1(Process Id: 65595): Sending message 7: Hello, world! 1 2 3 4 5 6
[INFO][2022-12-29 01:39:38.083857]: Player 2(Process Id: 65595): Received message 7: Hello, world! 1 2 3 4 5 6
[INFO][2022-12-29 01:39:38.083866]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083874]: Player 1(Process Id: 65595): Sending message 8: Hello, world! 1 2 3 4 5 6 7
[INFO][2022-12-29 01:39:38.083883]: Player 2(Process Id: 65595): Received message 8: Hello, world! 1 2 3 4 5 6 7
[INFO][2022-12-29 01:39:38.083892]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083900]: Player 1(Process Id: 65595): Sending message 9: Hello, world! 1 2 3 4 5 6 7 8
[INFO][2022-12-29 01:39:38.083908]: Player 2(Process Id: 65595): Received message 9: Hello, world! 1 2 3 4 5 6 7 8
[INFO][2022-12-29 01:39:38.083917]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083925]: Player 1(Process Id: 65595): Sending message 10: Hello, world! 1 2 3 4 5 6 7 8 9
[INFO][2022-12-29 01:39:38.083933]: Player 2(Process Id: 65595): Received message 10: Hello, world! 1 2 3 4 5 6 7 8 9
[INFO][2022-12-29 01:39:38.083945]: Lock acquired by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083953]: Termination condition reached by Player 1 (Process Id: 65595)
[INFO][2022-12-29 01:39:38.083967]: ---------------------------------
[INFO][2022-12-29 01:39:38.083976]: Starting separate process messaging
[INFO][2022-12-29 01:39:38.083984]: Each player is running in a separate process.
[INFO][2022-12-29 01:39:38.084071]: Player 2(Process Id: 65595): Pipes are initialized
[INFO][2022-12-29 01:39:38.084129]: Player 1(Process Id: 65596): Pipes are initialized
[INFO][2022-12-29 01:39:38.084299]: Player 1(Process Id: 65596): Sending message 1: Hello World 1 
[INFO][2022-12-29 01:39:38.084409]: Player 2(Process Id: 65595): Received message 1: Hello World 1
[INFO][2022-12-29 01:39:38.084461]: Player 2(Process Id: 65595): Sending message 2: Hello World 1 2 
[INFO][2022-12-29 01:39:38.084560]: Player 1(Process Id: 65596): Received message 2: Hello World 1 2
[INFO][2022-12-29 01:39:38.084598]: Player 1(Process Id: 65596): Sending message 3: Hello World 1 2 3 
[INFO][2022-12-29 01:39:38.084688]: Player 2(Process Id: 65595): Received message 3: Hello World 1 2 3
[INFO][2022-12-29 01:39:38.084718]: Player 2(Process Id: 65595): Sending message 4: Hello World 1 2 3 4 
[INFO][2022-12-29 01:39:38.084755]: Player 1(Process Id: 65596): Received message 4: Hello World 1 2 3 4
[INFO][2022-12-29 01:39:38.084780]: Player 1(Process Id: 65596): Sending message 5: Hello World 1 2 3 4 5 
[INFO][2022-12-29 01:39:38.084808]: Player 2(Process Id: 65595): Received message 5: Hello World 1 2 3 4 5
[INFO][2022-12-29 01:39:38.084827]: Player 2(Process Id: 65595): Sending message 6: Hello World 1 2 3 4 5 6 
[INFO][2022-12-29 01:39:38.084856]: Player 1(Process Id: 65596): Received message 6: Hello World 1 2 3 4 5 6
[INFO][2022-12-29 01:39:38.084880]: Player 1(Process Id: 65596): Sending message 7: Hello World 1 2 3 4 5 6 7 
[INFO][2022-12-29 01:39:38.084910]: Player 2(Process Id: 65595): Received message 7: Hello World 1 2 3 4 5 6 7
[INFO][2022-12-29 01:39:38.084933]: Player 2(Process Id: 65595): Sending message 8: Hello World 1 2 3 4 5 6 7 8 
[INFO][2022-12-29 01:39:38.084963]: Player 1(Process Id: 65596): Received message 8: Hello World 1 2 3 4 5 6 7 8
[INFO][2022-12-29 01:39:38.084983]: Player 1(Process Id: 65596): Sending message 9: Hello World 1 2 3 4 5 6 7 8 9 
[INFO][2022-12-29 01:39:38.084999]: Player 1(Process Id: 65596): WritePipeThread is terminating
[INFO][2022-12-29 01:39:38.085016]: Player 2(Process Id: 65595): Received message 9: Hello World 1 2 3 4 5 6 7 8 9
[INFO][2022-12-29 01:39:38.085035]: Player 2(Process Id: 65595): ReadPipeThread is terminating
[INFO][2022-12-29 01:39:38.085048]: Player 2(Process Id: 65595): Sending message 10: Hello World 1 2 3 4 5 6 7 8 9 10 
[INFO][2022-12-29 01:39:38.085074]: Player 2(Process Id: 65595): WritePipeThread is terminating
[INFO][2022-12-29 01:39:38.085093]: Player 1(Process Id: 65596): Received message 10: Hello World 1 2 3 4 5 6 7 8 9 10
[INFO][2022-12-29 01:39:38.085110]: Player 1(Process Id: 65596): ReadPipeThread is terminating
ozgurural@ozgurural:~/Desktop/360T/build$ 
```
