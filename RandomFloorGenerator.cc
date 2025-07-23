export module RandomFloorGenerator;
import <string>;
import <vector>;
import <fstream>;

// Chamber types for random generation
export enum class ChamberType {
    SMALL,
    MEDIUM,
    LARGE
};

// Floor pattern templates 
export enum class FloorPattern {
    CLASSIC_5_CHAMBER,
    LINEAR_CHAMBERS,
    CROSS_PATTERN,
    CIRCULAR_PATTERN,
    MAZE_LIKE
};

export class RandomFloorGenerator {
private:
    static const int FLOOR_WIDTH = 79;
    static const int FLOOR_HEIGHT = 25;
    
    unsigned int seed;
    char floorMap[FLOOR_HEIGHT][FLOOR_WIDTH + 1];
    
    // Pattern definitions and chamber templates
    struct ChamberTemplate {
        int width, height;
        std::vector<std::string> pattern;
        std::vector<std::pair<int, int>> doorPositions; // Relative positions for doors
    };
    
    std::vector<ChamberTemplate> smallChambers;
    std::vector<ChamberTemplate> mediumChambers;
    std::vector<ChamberTemplate> largeChambers;
    
    // Floor generation state
    std::vector<std::pair<int, int>> passagePoints; // Connection points between chambers
    std::vector<std::pair<int, int>> chamberPositions; // Where chambers are placed
    
public:
    RandomFloorGenerator(unsigned int seed = 0);
    ~RandomFloorGenerator() = default;
    
    // Main generation methods
    bool generateFloor(FloorPattern pattern = FloorPattern::CLASSIC_5_CHAMBER);
    bool saveToFile(const std::string& filename) const;
    
    // Configuration
    void setSeed(unsigned int newSeed);
    unsigned int getSeed() const { return seed; }
    
    // Chamber management
    void initializeChamberTemplates();
    ChamberTemplate createSmallChamber(int variant = 0) const;
    ChamberTemplate createMediumChamber(int variant = 0) const;
    ChamberTemplate createLargeChamber(int variant = 0) const;
    
    // Pattern generation
    bool generateClassic5Chamber();
    bool generateLinearChambers();
    bool generateCrossPattern();
    bool generateCircularPattern();
    bool generateMazeLike();
    
    // Utility methods
    void clearFloor();
    bool placeChamber(const ChamberTemplate& chamber, int x, int y);
    void connectChambers(int x1, int y1, int x2, int y2);
    void drawPassage(int x1, int y1, int x2, int y2);
    
    // Item and enemy spawning (for later implementation)
    void spawnEnemies();
    void spawnItems();
    void spawnStairs();
    void spawnPlayerStart();
    void spawnRandomEntities(); // Add this new method for testing
    
    // Validation and debugging
    bool validateFloor() const;
    void printFloor() const;
    void printDebugInfo() const;
    
    // Static utility methods for external use
    static std::string generateFloorFile(FloorPattern pattern, unsigned int seed, const std::string& filename);
    static bool isValidFloorFile(const std::string& filename);
};
