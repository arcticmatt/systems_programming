# Sockets

## Unix Domain Sockets
Note: you need [buck](https://buck.build/) to run this demo.

To run the demo, run this one in terminal window/tab:

```
buck run //sockets:unix_server_socket
```

Then run this in another terminal window/tab:

```
buck run //sockets:unix_client_socket
```

Once both are running, you can enter input into the client process, and it should be echoed by the server.
