#include <netinet/in.h>   // sockaddr_in, htons, INADDR_ANY
#include <sys/socket.h>   // socket(), bind(), listen(), accept()
#include <unistd.h>      // read(), close(), fork()
#include <iostream>      // std::cout
#include <signal.h>      // signal(), SIGCHLD
#include <sys/wait.h>    // waitpid()
#include "src/GameActions.cpp"

#define PORT 8080        // Port the server will listen on

// Keeps track of how many clients are currently connected
// IMPORTANT: This variable is meaningful ONLY in the parent process
int client_count = 0;
int player_count = 0;

/*
  This function is called automatically by the OS
  whenever a child process terminates (SIGCHLD signal).

  Each child process represents one connected client.
  When a child exits, that client is considered disconnected.
*/
void handle_sigchld(int) {

    // Reap all finished child processes (non-blocking)
    while (waitpid(-1, nullptr, WNOHANG) > 0) {
        client_count--;  // One client has disconnected

        std::cout << "Client disconnected. Active clients: "
                  << client_count << std::endl;
    }
}

int main() {
    int server_fd;       // Listening socket (server)
    int new_socket;      // Socket for a connected client
    struct sockaddr_in address; // Stores server address info
    int opt = 1;         // Option value for setsockopt
    int addrlen = sizeof(address);

    // Register signal handler for SIGCHLD
    // This allows the parent process to know when a client (child) exits
    signal(SIGCHLD, handle_sigchld);

    // -------------------- CREATE SOCKET --------------------
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // -------------------- SOCKET OPTIONS --------------------
    // Allows reusing the same port immediately after restarting the server
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // -------------------- SERVER ADDRESS SETUP --------------------
    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections on any network interface
    address.sin_port = htons(PORT);       // Convert port to network byte order

    // -------------------- BIND SOCKET --------------------
    // Bind the socket to the specified IP address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // -------------------- LISTEN --------------------
    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // -------------------- ACCEPT LOOP --------------------
    // This loop keeps the server running forever
    while (true) {

        // accept() blocks until a client connects
        // It returns a NEW socket dedicated to that client
        new_socket = accept(server_fd,
                            (struct sockaddr *)&address,
                            (socklen_t *)&addrlen);

        if (new_socket < 0) {
            perror("accept");
            continue; // Keep server running even if accept fails
        }

        // Increase the player count
        player_count++; 

        // A new client has successfully connected
        client_count++;
        std::cout << "Client connected. Active clients: "
                  << client_count << std::endl;

        // -------------------- FORK --------------------
        // Create a new process to handle this client
        if (fork() == 0) {
            // -------- CHILD PROCESS --------

            // Child does NOT accept new clients
            close(server_fd);

            char buffer[1024] = {0};

            // Read data sent by the client
            ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
            // std::vector<std::string> player_shoots; // Vector for storing all the players hand gestures

            if (bytes_read > 0) {
                buffer[bytes_read] = '\0'; // Null-terminate string
                std::cout << "Player " << player_count << ": " << buffer << "\n";
                std::cout << draw_hand(buffer) << '\n';
                // player_shoots.push_back(buffer); // Cache all players hand gestures

                // for(int i = 0; i < player_shoots.size(); i++) {
                //   std::cout << "Player " << i << '\n';
                //   std::cout << draw_hand(player_shoots[i]) << '\n';
                // }
            }

            // Close connection with this client
            close(new_socket);

            // Exit child process
            // This triggers SIGCHLD in the parent
            exit(0);
        }

        // -------- PARENT PROCESS --------
        // Parent does not communicate with the client
        // It only accepts new connections
        close(new_socket);
    }

    return 0;
}

