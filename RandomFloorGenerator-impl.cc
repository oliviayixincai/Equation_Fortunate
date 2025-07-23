module RandomFloorGenerator;
import <iostream>;
import <fstream>;
import <random>;
import <algorithm>;
import <ctime>;

RandomFloorGenerator::RandomFloorGenerator(unsigned int seed) : seed(seed) {
    if (seed == 0) {
        this->seed = static_cast<unsigned int>(std::time(nullptr));
    }
    
    clearFloor();
    initializeChamberTemplates();
}

void RandomFloorGenerator::setSeed(unsigned int newSeed) {
    seed = newSeed;
}

void RandomFloorGenerator::clearFloor() {
    // Initialize with walls
    for (int row = 0; row < FLOOR_HEIGHT; ++row) {
        for (int col = 0; col < FLOOR_WIDTH; ++col) {
            floorMap[row][col] = ' '; // Empty space (will be walls)
        }
        floorMap[row][FLOOR_WIDTH] = '\0';
    }
    
    // Add border walls
    for (int col = 0; col < FLOOR_WIDTH; ++col) {
        floorMap[0][col] = '|';
        floorMap[FLOOR_HEIGHT - 1][col] = '|';
    }
    for (int row = 0; row < FLOOR_HEIGHT; ++row) {
        floorMap[row][0] = '|';
        floorMap[row][FLOOR_WIDTH - 1] = '|';
    }
    
    passagePoints.clear();
    chamberPositions.clear();
}

void RandomFloorGenerator::initializeChamberTemplates() {
    // Small chamber templates based on chamber-impl.cc
    
    // Small Chamber 1 - L-shaped
    smallChambers.push_back({
        13, 6,
        {"|-------------|",
         "|.............|",
         "|.............|",
         "|.......|-----|",
         "|.......|     ",
         "|-------|     "},
        {{10, 3}} // Door position
    });
    
    // Small Chamber 2 - Rectangle with internal walls
    smallChambers.push_back({
        13, 6,
        {"|-------------|",
         "|.............|",
         "|....|---|....|",
         "|....|---|....|",
         "|.............|",
         "|-------------|"},
        {{0, 2}, {12, 2}} // Door positions
    });
    
    // Small Chamber 3 - Zigzag
    smallChambers.push_back({
        13, 6,
        {"|----|        ",
         "|....|----|   ",
         "|.........|---|",
         "|---|.........|",
         "    |----|....|",
         "         |----|"},
        {{4, 0}, {12, 2}} // Door positions
    });
    
    // Medium chambers (larger versions)
    mediumChambers.push_back({
        20, 8,
        {"|------------------|",
         "|..................|",
         "|..................|",
         "|..................|",
         "|..................|",
         "|..................|",
         "|..................|",
         "|------------------|"},
        {{0, 3}, {19, 3}} // Door positions
    });
    
    // Large chambers
    largeChambers.push_back({
        30, 12,
        {"|----------------------------|",
         "|............................|",
         "|............................|",
         "|............................|",
         "|............................|",
         "|............................|",
         "|............................|",
         "|............................|",
         "|............................|",
         "|............................|",
         "|............................|",
         "|----------------------------|"},
        {{0, 5}, {29, 5}} // Door positions
    });
}

RandomFloorGenerator::ChamberTemplate RandomFloorGenerator::createSmallChamber(int variant) const {
    if (variant >= 0 && variant < smallChambers.size()) {
        return smallChambers[variant];
    }
    return smallChambers[0]; // Default
}

RandomFloorGenerator::ChamberTemplate RandomFloorGenerator::createMediumChamber(int variant) const {
    if (variant >= 0 && variant < mediumChambers.size()) {
        return mediumChambers[variant];
    }
    return mediumChambers[0]; // Default
}

RandomFloorGenerator::ChamberTemplate RandomFloorGenerator::createLargeChamber(int variant) const {
    if (variant >= 0 && variant < largeChambers.size()) {
        return largeChambers[variant];
    }
    return largeChambers[0]; // Default
}

bool RandomFloorGenerator::generateFloor(FloorPattern pattern) {
    clearFloor();
    
    switch (pattern) {
        case FloorPattern::CLASSIC_5_CHAMBER:
            return generateClassic5Chamber();
        case FloorPattern::LINEAR_CHAMBERS:
            return generateLinearChambers();
        case FloorPattern::CROSS_PATTERN:
            return generateCrossPattern();
        case FloorPattern::CIRCULAR_PATTERN:
            return generateCircularPattern();
        case FloorPattern::MAZE_LIKE:
            return generateMazeLike();
        default:
            return generateClassic5Chamber();
    }
}

bool RandomFloorGenerator::generateClassic5Chamber() {
    // Recreate the classic CC3k 5-chamber layout with random chamber types
    std::mt19937 rng(seed);
    
    // Define positions for 5 chambers (approximate positions from cc3kfloor.txt)
    std::vector<std::pair<int, int>> positions = {
        {3, 3},   // Top-left chamber
        {40, 3},  // Top-right chamber  
        {55, 8},  // Middle-right chamber
        {3, 15},  // Bottom-left chamber
        {25, 19}  // Bottom chamber
    };
    
    // Place chambers with random types
    for (size_t i = 0; i < positions.size(); ++i) {
        ChamberTemplate chamber;
        int chamberType = rng() % 3; // 0=small, 1=medium, 2=large
        
        switch (chamberType) {
            case 0:
                chamber = createSmallChamber(rng() % smallChambers.size());
                break;
            case 1:
                chamber = createMediumChamber(rng() % mediumChambers.size());
                break;
            case 2:
                chamber = createLargeChamber(rng() % largeChambers.size());
                break;
        }
        
        if (!placeChamber(chamber, positions[i].first, positions[i].second)) {
            std::cerr << "Failed to place chamber " << i << std::endl;
            return false;
        }
        
        chamberPositions.push_back(positions[i]);
    }
    
    // Connect chambers with passages (simplified for now)
    connectChambers(chamberPositions[0].first + 10, chamberPositions[0].second + 3,
                   chamberPositions[1].first, chamberPositions[1].second + 3);
    
    connectChambers(chamberPositions[1].first + 15, chamberPositions[1].second + 5,
                   chamberPositions[2].first, chamberPositions[2].second + 2);
    
    connectChambers(chamberPositions[0].first + 5, chamberPositions[0].second + 6,
                   chamberPositions[3].first + 5, chamberPositions[3].second);
    
    connectChambers(chamberPositions[3].first + 15, chamberPositions[3].second + 3,
                   chamberPositions[4].first, chamberPositions[4].second);
    
    // Add stairs and player spawn
    spawnStairs();
    spawnPlayerStart();
    
    return validateFloor();
}

bool RandomFloorGenerator::generateLinearChambers() {
    // Generate chambers in a line
    std::mt19937 rng(seed);
    
    int numChambers = 4 + (rng() % 3); // 4-6 chambers
    int spacing = FLOOR_WIDTH / (numChambers + 1);
    
    for (int i = 0; i < numChambers; ++i) {
        ChamberTemplate chamber = createSmallChamber(rng() % smallChambers.size());
        int x = spacing * (i + 1) - chamber.width / 2;
        int y = FLOOR_HEIGHT / 2 - chamber.height / 2;
        
        if (placeChamber(chamber, x, y)) {
            chamberPositions.push_back({x, y});
        }
    }
    
    // Connect adjacent chambers
    for (size_t i = 0; i < chamberPositions.size() - 1; ++i) {
        connectChambers(chamberPositions[i].first + 12, chamberPositions[i].second + 3,
                       chamberPositions[i + 1].first, chamberPositions[i + 1].second + 3);
    }
    
    spawnStairs();
    spawnPlayerStart();
    return validateFloor();
}

bool RandomFloorGenerator::generateCrossPattern() {
    // TODO: Implement cross pattern
    return generateClassic5Chamber(); // Fallback for now
}

bool RandomFloorGenerator::generateCircularPattern() {
    // TODO: Implement circular pattern
    return generateClassic5Chamber(); // Fallback for now
}

bool RandomFloorGenerator::generateMazeLike() {
    // TODO: Implement maze-like pattern
    return generateClassic5Chamber(); // Fallback for now
}

bool RandomFloorGenerator::placeChamber(const ChamberTemplate& chamber, int x, int y) {
    // Check bounds
    if (x < 1 || y < 1 || x + chamber.width >= FLOOR_WIDTH - 1 || y + chamber.height >= FLOOR_HEIGHT - 1) {
        return false;
    }
    
    // Place chamber pattern
    for (int row = 0; row < chamber.height; ++row) {
        for (int col = 0; col < chamber.width; ++col) {
            if (col < chamber.pattern[row].length()) {
                floorMap[y + row][x + col] = chamber.pattern[row][col];
            }
        }
    }
    
    return true;
}

void RandomFloorGenerator::connectChambers(int x1, int y1, int x2, int y2) {
    // Simple L-shaped passage connection
    drawPassage(x1, y1, x2, y1); // Horizontal first
    drawPassage(x2, y1, x2, y2); // Then vertical
}

void RandomFloorGenerator::drawPassage(int x1, int y1, int x2, int y2) {
    // Draw horizontal passage
    if (y1 == y2) {
        int startX = std::min(x1, x2);
        int endX = std::max(x1, x2);
        for (int x = startX; x <= endX; ++x) {
            if (x >= 0 && x < FLOOR_WIDTH && y1 >= 0 && y1 < FLOOR_HEIGHT) {
                if (floorMap[y1][x] == ' ') {
                    floorMap[y1][x] = '#';
                }
            }
        }
    }
    
    // Draw vertical passage
    if (x1 == x2) {
        int startY = std::min(y1, y2);
        int endY = std::max(y1, y2);
        for (int y = startY; y <= endY; ++y) {
            if (x1 >= 0 && x1 < FLOOR_WIDTH && y >= 0 && y < FLOOR_HEIGHT) {
                if (floorMap[y][x1] == ' ') {
                    floorMap[y][x1] = '#';
                }
            }
        }
    }
}

void RandomFloorGenerator::spawnPlayerStart() {
    // Place player at the first chamber
    if (!chamberPositions.empty()) {
        int x = chamberPositions[0].first + 5;
        int y = chamberPositions[0].second + 2;
        if (x >= 0 && x < FLOOR_WIDTH && y >= 0 && y < FLOOR_HEIGHT) {
            floorMap[y][x] = '@';
        }
    }
}

void RandomFloorGenerator::spawnStairs() {
    // Place stairs at the last chamber
    if (!chamberPositions.empty()) {
        int x = chamberPositions.back().first + 3;
        int y = chamberPositions.back().second + 2;
        if (x >= 0 && x < FLOOR_WIDTH && y >= 0 && y < FLOOR_HEIGHT) {
            floorMap[y][x] = '\\';
        }
    }
}

void RandomFloorGenerator::spawnEnemies() {
    // TODO: Implement enemy spawning when teammate completes Enemy system
}

void RandomFloorGenerator::spawnItems() {
    // TODO: Implement item spawning when teammate completes Item system
}

bool RandomFloorGenerator::validateFloor() const {
    // Basic validation: check if we have chambers and connections
    bool hasPlayer = false;
    bool hasStairs = false;
    
    for (int row = 0; row < FLOOR_HEIGHT; ++row) {
        for (int col = 0; col < FLOOR_WIDTH; ++col) {
            if (floorMap[row][col] == '@') hasPlayer = true;
            if (floorMap[row][col] == '\\') hasStairs = true;
        }
    }
    
    return hasPlayer && hasStairs && !chamberPositions.empty();
}

bool RandomFloorGenerator::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file: " << filename << std::endl;
        return false;
    }
    
    // Write floor data
    for (int row = 0; row < FLOOR_HEIGHT; ++row) {
        file << floorMap[row] << std::endl;
    }
    
    file.close();
    std::cout << "Floor saved to: " << filename << std::endl;
    return true;
}

void RandomFloorGenerator::printFloor() const {
    for (int row = 0; row < FLOOR_HEIGHT; ++row) {
        std::cout << floorMap[row] << std::endl;
    }
}

void RandomFloorGenerator::printDebugInfo() const {
    std::cout << "=== RANDOM FLOOR GENERATOR DEBUG ===" << std::endl;
    std::cout << "Seed: " << seed << std::endl;
    std::cout << "Chambers placed: " << chamberPositions.size() << std::endl;
    std::cout << "Passage points: " << passagePoints.size() << std::endl;
    std::cout << "Floor validated: " << (validateFloor() ? "Yes" : "No") << std::endl;
}

std::string RandomFloorGenerator::generateFloorFile(FloorPattern pattern, unsigned int seed, const std::string& filename) {
    RandomFloorGenerator generator(seed);
    
    if (generator.generateFloor(pattern)) {
        if (generator.saveToFile(filename)) {
            return filename;
        }
    }
    
    return ""; // Failed
}

bool RandomFloorGenerator::isValidFloorFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    std::string line;
    int lineCount = 0;
    bool hasPlayer = false;
    bool hasStairs = false;
    
    while (std::getline(file, line) && lineCount < 30) {
        if (line.find('@') != std::string::npos) hasPlayer = true;
        if (line.find('\\') != std::string::npos) hasStairs = true;
        lineCount++;
    }
    
    return hasPlayer && hasStairs && lineCount >= 20;
} 