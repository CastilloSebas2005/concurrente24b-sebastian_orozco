// Copyright 2021,2023 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

/**
@mainpage

This source code implements two scenarios where concurrency is needed: a
computer network simulation and a server process.

## Network simulation

The network simulation mimics a producer machine that sends a number of packages
targeted to several consuming machines. The producer does not know the
consumers. A intermediary machine, called a dispatcher, redistributes the
messages to their targets. Network links are modeled as thread-safe queues.
Machines are represented by execution threads acting according to the stated
roles: Producer, Consumer, Dispatcher. Base clases are provided for each role.

To build the network simulation:

```sh
make clean
make -j4 DEFS=-DSIMULATION APPNAME=netsim
```

To run the executable, you provide some arguments to the simulation. For example
the following command ask the producer to send 1000 packages randomly targeted
to 3 consumers. First consumer takes 1 millisecond to process each message,
second consumer takes 2 milliseconds, and third consumer does not take extra
time to process each message received.

```sh
bin/netsim 1000 3 1 2 0
```

The implementation of the simulation is complete. You can use it to get an
example how the base classes of the consumer-producer pattern are derived and
used. The simulation code is in the `simulation/` folder.


## Web server

The webserver code implements a basic HTTP server. However it is not complete.
The provided code is serial, which is abnormal. Web servers, as any other types
of servers (e.g database servers, email servers, ...), are concurrent.
Therefore, the provided code is a starting point to practice the concurrent
programming abilities. This may be considered as legacy code, and it provides a
lot of value to you, because the probability that you work using legacy code is
higher than starting zero-code projects from scratch.

This webserver code is arguably well documented, it contains TODO comments given
you hints about what you have to change in order to get the server working
concurrently, and there are available videos explaining this code. These
amenities are unlikely to be found when you work in a real-world environment. To
build the webserver, run these commands:

```sh
make clean
make -j4 DEFS=-DWEBSERVER APPNAME=webserver
```

To run the web server provide a network port as the first argument. It is an
integer number higher than 1024 (otherwise you require administrador
privileges). If you omit this argument, 8080 port will be assumed.

```sh
bin/webserver 8080
```

The webserver process will be running in your server machine. You can make
requests to it by launching a web browser and connecting to the web address
http://localhost:8080/. If you plan to access your web server from another
computer, get first its network address running the command:

```sh
ip addr | grep -E '\b\d+(\.\d+){3}\b'
```

Ignore the 127.0.0.1 result. For example, if you get 10.1.137.117, you can
access the web server from any other computer of your local network using a web
browser accessing http://10.1.137.117:8080/.

@remark This software is for educative purposes. Use it at your own risk.
*/
