#include "lib/error_functions.h"
#include "sockets/unix_socket.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 5

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;

  // Create a new server socket with domain: AF_UNIX, type: SOCK_STREAM, protocol: 0
  int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  printf("Server socket fd = %d\n", sfd);

  // Make sure socket's file descriptor is legit.
  if (sfd == -1) {
    errExit("socket");
  }

  // Make sure the address we're planning to use isn't too long.
  if (strlen(SV_SOCK_PATH) > sizeof(addr.sun_path) - 1) {
    fatal("Server socket path too long: %s", SV_SOCK_PATH);
  }

  // Delete any file that already exists at the address. Make sure the deletion
  // succeeds. If the error is just that the file/directory doesn't exist, it's fine.
  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
    errExit("remove-%s", SV_SOCK_PATH);
  }

  // Zero out the address, and set family and path.
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

  // Bind the socket to the address. Note that we're binding the server socket
  // to a well-known address so that clients know where to connect.
  if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
    errExit("bind");
  }

  // The listen call marks the socket as *passive*. The socket will subsequently
  // be used to accept connections from *active* sockets.
  // listen cannot be called on a connected socket (a socket on which a connect()
  // has been succesfully performed or a socket returned by a call to accept()).
  if (listen(sfd, BACKLOG) == -1) {
    errExit("listen");
  }

  ssize_t numRead;
  char buf[BUF_SIZE];
  for (;;) {          /* Handle client connections iteratively */

    // Accept a connection. The connection is returned on a NEW
    // socket, 'cfd'; the listening socket ('sfd') remains open
    // and can be used to accept further connections. */
    printf("Waiting to accept a connection...\n");
    // NOTE: blocks until a connection request arrives.
    int cfd = accept(sfd, NULL, NULL);
    printf("Accepted socket fd = %d\n", cfd);

    //
    // Transfer data from connected socket to stdout until EOF */
    //

    // Read at most BUF_SIZE bytes from the socket into buf.
    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
      // Then, write those bytes from buf into STDOUT.
      if (write(STDOUT_FILENO, buf, numRead) != numRead) {
        fatal("partial/failed write");
      }
    }

    if (numRead == -1) {
      errExit("read");
    }

    if (close(cfd) == -1) {
      errMsg("close");
    }
  }
}
