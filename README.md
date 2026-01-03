# TCP Client/Server Demo

This is a demo of a simple TCP client and server. It is written in
standards-conforming C17 and complies with the POSIX.1-2008 API.

## Building
To build, simply run `make`. You will get two programs:
`client` and `server`.

## Using
To test, run `./server`, and then in another window, run
`./client 127.0.0.1` to connect to t he server. The server will
echo any string it gets back to the client, up to 1023 characters.

You can exit the server with `CTRL-C`. To exit the client, simply
type `exit`, which will disconnect from the server cleanly and
exit the program.

## Copying
You are free to use and copy the code under the Apache License 2.0.

## Contributing
This is a simple demo, so I won't accept any feature requests. If
you have any bugfixes or changes due to standards updates, I will
be happy to review them.
