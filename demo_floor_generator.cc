import RandomFloorGenerator;
import <iostream>;
import <string>;

int main() {
    std::cout << "🎲 ChamberCrawler3000 - Random Floor Generator Demo" << std::endl;
    std::cout << "══════════════════════════════════════════════════" << std::endl;
    
    // Create generator with a fixed seed for reproducible results
    RandomFloorGenerator generator(42);
    
    std::cout << "🏗️  Generating floors with different patterns..." << std::endl;
    std::cout << std::endl;
    
    // Generate Classic 5-Chamber Pattern
    std::cout << "1️⃣  Generating Classic 5-Chamber Pattern:" << std::endl;
    if (generator.generateFloor(FloorPattern::CLASSIC_5_CHAMBER)) {
        generator.saveToFile("demo_classic.txt");
        std::cout << "✅ Saved to demo_classic.txt" << std::endl;
    }
    std::cout << std::endl;
    
    // Generate Linear Chambers Pattern
    std::cout << "2️⃣  Generating Linear Chambers Pattern:" << std::endl;
    if (generator.generateFloor(FloorPattern::LINEAR_CHAMBERS)) {
        generator.saveToFile("demo_linear.txt");
        std::cout << "✅ Saved to demo_linear.txt" << std::endl;
    }
    std::cout << std::endl;
    
    // Generate Cross Pattern
    std::cout << "3️⃣  Generating Cross Pattern:" << std::endl;
    if (generator.generateFloor(FloorPattern::CROSS_PATTERN)) {
        generator.saveToFile("demo_cross.txt");
        std::cout << "✅ Saved to demo_cross.txt" << std::endl;
    }
    std::cout << std::endl;
    
    // Test validation
    std::cout << "🔍 Testing floor validation..." << std::endl;
    if (RandomFloorGenerator::isValidFloorFile("demo_classic.txt")) {
        std::cout << "✅ demo_classic.txt is valid" << std::endl;
    }
    if (RandomFloorGenerator::isValidFloorFile("demo_linear.txt")) {
        std::cout << "✅ demo_linear.txt is valid" << std::endl;
    }
    
    // Generate with different seeds
    std::cout << std::endl;
    std::cout << "🎲 Testing different seeds:" << std::endl;
    for (int seed = 100; seed <= 102; ++seed) {
        RandomFloorGenerator genSeed(seed);
        genSeed.generateFloor(FloorPattern::LINEAR_CHAMBERS);
        std::string filename = "demo_seed_" + std::to_string(seed) + ".txt";
        genSeed.saveToFile(filename);
        std::cout << "🌱 Seed " << seed << " → " << filename << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "🎉 Demo complete! Check the generated .txt files." << std::endl;
    std::cout << "💡 You can load these files in the main game using:" << std::endl;
    std::cout << "   ./cc3k demo_classic.txt" << std::endl;
    
    return 0;
} 