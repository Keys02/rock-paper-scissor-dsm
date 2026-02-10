#include <netinet/in.h>   // sockaddr_in, htons, INADDR_ANY
#include <sys/socket.h>   // socket(), bind(), listen(), accept()
#include <unistd.h>      // read(), close()
#include <iostream>      // std::cout
#include <vector>        // std::vector
#include <string>        // std::string
#include <cstring>       // memset
#include "src/GameActions.cpp"

#define PORT 8080

// Tracks connected players
int client_count = 0;
int player_id = 0;

// Caches all data sent by clients
std::vector<std::string> player_shoots;

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int opt = 1;

    // -------------------- CREATE SOCKET --------------------
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }

    // -------------------- SOCKET OPTIONS --------------------
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // -------------------- SERVER ADDRESS --------------------
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // -------------------- BIND --------------------
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    // -------------------- LISTEN --------------------
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "\n";

    // -------------------- ACCEPT LOOP --------------------
    while (true) {

        client_socket = accept(server_fd,
                               (struct sockaddr *)&address,
                               &addrlen);

        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        client_count++;
        player_id++;

        std::cout << "\nClient connected"
                  << " | Active clients: " << client_count
                  << " | Player ID: " << player_id << "\n";

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';

            // Cache player move
            player_shoots.push_back(buffer);

            client_count--;

            std::cout << "Client disconnected"
                    << " | Active clients: " << client_count << "\n";

            // Display all cached player moves
            if (player_shoots.size() == 2) {
                for (size_t i = 0; i < player_shoots.size(); i++) {
                    std::cout << "\n";
                    std::cout << "Player " << i + 1 << ":\n";
                    std::cout << draw_hand(player_shoots[i]) << "\n";

                    if (i == 1)
                        std::cout << "Game result: " << choose_winner(player_shoots[i-1], player_shoots[i]) << '\n';
                }
            }
        }

        close(client_socket);
    }

    close(server_fd);
    return 0;
}


