#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <typeinfo>
#include <cstring>
#include <iostream>

#define PORT 8080

std::string draw_hand(std::string choice) {
    std::string hand = "";
    if (choice == "Rock") {
        hand = R"(
    _______
---'   ____)
      (_____)
      (_____)
      (____)
---.__(___)
        )";
    }
    else if (choice == "Paper") {
        hand = R"(
    _______
---'   ____)____
          ______)
          _______)
         _______)
---.__________)
        )";
    }
    else if (choice == "Scissor") {
        hand = R"(
    _______
---'   ____)____
          ______)
       __________)
      (____)
---.__(___)
        )";
    }
    return hand;
}

std::string shoot_player() {
    std::string choice;
    std::cout << "Player: ";
    std::cin >> choice;
    std::cout << draw_hand(choice) << "\n";
    return choice;
}

std::string choose_winner(std::string player_one_choice, std::string player_two_choice) {
    char p_s  = player_one_choice[0];

    switch (p_s) {
        case 'R':
            if (player_two_choice == "Rock")
                return "It's a tie";
            else if (player_two_choice == "Paper")
                return "You lost";
            else if (player_two_choice == "Scissor")
                return "You won";
            break;
        case 'P':
            if (player_two_choice == "Rock")
                return "You won";
            else if (player_two_choice == "Paper")
                return "It's a tie";
            else if (player_two_choice == "Scissor")
                return "You lost";
            break;
        case 'S':
            if (player_two_choice == "Rock")
                return "You lost";
            else if (player_two_choice == "Paper")
                return "You won";
            else if (player_two_choice == "Scissor")
                return "It's a tie";
            break;
    }

    return "";
}

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

  // Create socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Attach socket to the port
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // Accept a connection
  new_socket =
      accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
  if (new_socket < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  // Read data
  read(new_socket, buffer, 1024);
  std::cout << draw_hand(buffer) << '\n';

  // Close socket
  close(new_socket);
  close(server_fd);
  return 0;
}

