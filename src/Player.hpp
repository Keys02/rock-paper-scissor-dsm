#include <iostream>

class Player {
    public:
        std::string shoot() {
            std::string choice;
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
                        std::cout << "Player: ";
                        std::cin >> choice;
            } while (choice != "Rock" && choice != "Paper" && choice != "Scissor");

            return choice;
        }
    private:
        std::string player_shoot;
};