export module observer;
import <string>;

// Forward declarations
export class Floor;
export class PlayerCharacter;

export class Observer {
public:
    virtual ~Observer() = default;
    virtual void notify() = 0;
    virtual void displayFloor(Floor* floor) = 0;
    virtual void displayGameInfo(PlayerCharacter* player, int floorNum) = 0;
    virtual void displayMessage(const std::string& message) = 0;
};

export class TextDisplay : public Observer {
private:
    static const int INFO_ROWS = 5; // Reserved rows for game info
    
public:
    TextDisplay();
    ~TextDisplay() = default;
    
    void notify() override;
    void displayFloor(Floor* floor) override;
    void displayGameInfo(PlayerCharacter* player, int floorNum) override;
    void displayMessage(const std::string& message) override;
    void clearScreen();
    void printHeader();
    void printFooter();
};