#!/bin/bash

echo "🔨 Building ChamberCrawler3000..."
echo "=================================="

# Set compiler flags for C++20 modules
CXX_FLAGS="-std=c++20 -fmodules-ts -Wall -Wextra -O2"

# Check if compiler supports modules
echo "🔍 Checking compiler support..."
if ! g++ --version | grep -q "g++"; then
    echo "❌ GCC compiler not found. Please install GCC 11+ with module support."
    exit 1
fi

echo "✅ Compiler found"

# Create build directory
mkdir -p build
cd build

echo "🏗️  Compiling modules..."

# Compile interface modules first
echo "  📦 Compiling position module..."
g++ $CXX_FLAGS -c ../Position.cc -o position.o || { echo "❌ Failed to compile position"; exit 1; }

echo "  📦 Compiling item module..."
g++ $CXX_FLAGS -c ../Item.cc -o item.o || { echo "❌ Failed to compile item"; exit 1; }

echo "  📦 Compiling character module..."
g++ $CXX_FLAGS -c ../Character.cc -o character.o || { echo "❌ Failed to compile character"; exit 1; }

echo "  📦 Compiling observer module..."
g++ $CXX_FLAGS -c ../observer.cc -o observer.o || { echo "❌ Failed to compile observer"; exit 1; }

echo "  📦 Compiling Floor module..."
g++ $CXX_FLAGS -c ../floor.cc -o floor.o || { echo "❌ Failed to compile floor"; exit 1; }

echo "  📦 Compiling RandomFloorGenerator module..."
g++ $CXX_FLAGS -c ../RandomFloorGenerator.cc -o randomfloor.o || { echo "❌ Failed to compile randomfloor"; exit 1; }

echo "  📦 Compiling game module..."
g++ $CXX_FLAGS -c ../game.cc -o game.o || { echo "❌ Failed to compile game"; exit 1; }

echo "🔗 Compiling implementations..."

# Compile implementation files
echo "  🔧 Compiling Position-impl..."
g++ $CXX_FLAGS -c ../Position-impl.cc -o position-impl.o || { echo "❌ Failed to compile position-impl"; exit 1; }

echo "  🔧 Compiling Item-impl..."
g++ $CXX_FLAGS -c ../Item-impl.cc -o item-impl.o || { echo "❌ Failed to compile item-impl"; exit 1; }

echo "  🔧 Compiling Character-impl..."
g++ $CXX_FLAGS -c ../Character-impl.cc -o character-impl.o || { echo "❌ Failed to compile character-impl"; exit 1; }

echo "  🔧 Compiling observer-impl..."
g++ $CXX_FLAGS -c ../observer-impl.cc -o observer-impl.o || { echo "❌ Failed to compile observer-impl"; exit 1; }

echo "  🔧 Compiling Floor-impl..."
g++ $CXX_FLAGS -c ../Floor-impl.cc -o floor-impl.o || { echo "❌ Failed to compile floor-impl"; exit 1; }

echo "  🔧 Compiling RandomFloorGenerator-impl..."
g++ $CXX_FLAGS -c ../RandomFloorGenerator-impl.cc -o randomfloor-impl.o || { echo "❌ Failed to compile randomfloor-impl"; exit 1; }

echo "  🔧 Compiling game-impl..."
g++ $CXX_FLAGS -c ../game-impl.cc -o game-impl.o || { echo "❌ Failed to compile game-impl"; exit 1; }

echo "  🔧 Compiling main..."
g++ $CXX_FLAGS -c ../main.cc -o main.o || { echo "❌ Failed to compile main"; exit 1; }

echo "🔗 Linking main executable..."
g++ $CXX_FLAGS *.o -o cc3k || { echo "❌ Failed to link main executable"; exit 1; }

echo "🔗 Compiling demo programs..."
echo "  🎲 Compiling floor generator demo..."
g++ $CXX_FLAGS -c ../demo_floor_generator.cc -o demo-floor.o || { echo "⚠️  Demo compilation failed (this is optional)"; }
if [ -f demo-floor.o ]; then
    g++ $CXX_FLAGS demo-floor.o randomfloor.o randomfloor-impl.o -o demo_floor_generator || { echo "⚠️  Demo linking failed"; }
fi

cd ..

echo ""
echo "✅ Build complete!"
echo "🎮 Run the game with: ./build/cc3k"
echo "📁 Or with a floor file: ./build/cc3k cc3kfloor.txt"
echo "🎲 Or run the demo: ./build/demo_floor_generator"
echo ""
echo "💡 Available floor files:"
ls -la *.txt 2>/dev/null || echo "   (No .txt files found - use RandomFloorGenerator demo to create some!)" 