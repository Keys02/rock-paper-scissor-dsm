#include <iostream>

class Player {
    public:
    Player() {
        do {
            std::cout << "################################################################################################################\n"
                    << "****************************************************************************************************************\n"
                    << "                                                  How to play                                                   \n"
                    << "****************************************************************************************************************\n"
                    << "Shoot either one of Rock, Paper or Scissor"
                    << "\n"
                    << "\n"
                    << "Rock Paper Scissor shoot!!\n"
                    << "################################################################################################################\n";
                    std::string choice;
                    std::cout << "Player: ";
                    std::cin >> choice;
        } while (player_shoot != "Rock" && player_shoot != "Paper" && player_shoot != "Scissor");
    }

    void shoot(std::string hand) {
        player_shoot = hand;
    }

    private:
        std::string player_shoot;
};