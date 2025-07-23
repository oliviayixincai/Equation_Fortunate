module observer;
import <iostream>;
import <iomanip>;
import Floor;
import character;

// ANSI color codes for better display (as mentioned in CC3k spec)
const std::string RESET = "\033[0m";
const std::string BLUE = "\033[34m";   // Player and stairs
const std::string RED = "\033[31m";    // Enemies  
const std::string YELLOW = "\033[33m"; // Treasure
const std::string GREEN = "\033[32m";  // Potions
const std::string WHITE = "\033[37m";  // Default

TextDisplay::TextDisplay() {
    // Constructor - nothing special needed
}

void TextDisplay::notify() {
    // This method is called by subjects when they change
    // For now, just refresh the display
    // The actual display will be handled by Game calling displayFloor()
}

void TextDisplay::displayFloor(Floor* floor) {
    if (!floor) {
        std::cout << "No floor to display!" << std::endl;
        return;
    }
    
    clearScreen();
    printHeader();
    
    // Display the floor map
    for (int row = 0; row < floor->getHeight(); ++row) {
        for (int col = 0; col < floor->getWidth(); ++col) {
            Position pos(col, row);
            char displayChar = floor->getDisplayChar(pos);
            
            // Apply colors based on character type
            std::string color = WHITE;
            switch (displayChar) {
                case '@': // Player
                case '\\': // Stairs
                    color = BLUE;
                    break;
                case 'H': case 'W': case 'E': case 'O': case 'M': case 'D': case 'L': // Enemies
                    color = RED;
                    break;
                case 'G': // Gold/Treasure
                    color = YELLOW;
                    break;
                case 'P': // Potions
                    color = GREEN;
                    break;
                default:
                    color = WHITE;
                    break;
            }
            
            std::cout << color << displayChar << RESET;
        }
        std::cout << std::endl;
    }
    
    printFooter();
}

void TextDisplay::displayGameInfo(PlayerCharacter* player, int floorNum) {
    if (!player) {
        std::cout << "Floor: " << floorNum << std::endl;
        std::cout << "Player: Not created" << std::endl;
        return;
    }
    
    // TODO: Display actual player stats when teammate implements getters
    std::cout << "Floor: " << floorNum;
    
    // TODO: Add when PlayerCharacter methods are available
    // std::cout << "   Race: " << player->getRace();
    // std::cout << "   Gold: " << player->getGold();
    // std::cout << std::endl;
    // std::cout << "HP: " << player->getHP() << "/" << player->getMaxHP();
    // std::cout << "   Atk: " << player->getAtk();
    // std::cout << "   Def: " << player->getDef();
    
    std::cout << "   Player: Created"; // Placeholder
    std::cout << std::endl;
    std::cout << std::endl; // Extra space for readability
}

void TextDisplay::displayMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void TextDisplay::clearScreen() {
    // Clear screen using ANSI escape codes
    std::cout << "\033[2J\033[H" << std::flush;
}

void TextDisplay::printHeader() {
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                           CHAMBERCRAWLER3000                                 ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
}

void TextDisplay::printFooter() {
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║ Commands: [no/so/ea/we/ne/nw/se/sw] move, [u dir] use potion, [a dir] attack ║" << std::endl;
    std::cout << "║           [f] freeze enemies, [r] restart, [q] quit                          ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
}
