class Game {
    Floor *theFloor;
    PlayerCharacter *p1;
    long seed;
public: 
    void nextFloor();
    void gameover();
    void quit();
    void displayMessage();
};