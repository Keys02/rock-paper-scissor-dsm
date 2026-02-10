// A program which plays the game rock paper scissors
#include <string>

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

std::string choose_winner(std::string player_one_choice, std::string player_two_choice) {
    char p_s  = player_one_choice[0];

    switch (p_s) {
        case 'R':
            if (player_two_choice == "Rock")
                return "It's a tie";
            else if (player_two_choice == "Paper")
                return "Player 2 won";
            else if (player_two_choice == "Scissor")
                return "Player 1 won";
            break;
        case 'P':
            if (player_two_choice == "Rock")
                return "Player 1 won";
            else if (player_two_choice == "Paper")
                return "It's a tie";
            else if (player_two_choice == "Scissor")
                return "Player 2 won";
            break;
        case 'S':
            if (player_two_choice == "Rock")
                return "Player 2 won";
            else if (player_two_choice == "Paper")
                return "Player 1 won";
            else if (player_two_choice == "Scissor")
                return "It's a tie";
            break;
    }
    return "";
}