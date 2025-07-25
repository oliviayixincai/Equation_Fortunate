CXX = g++-13
CXXFLAGS = -std=c++20 -Wall -g -O2
EXEC = cc3k

# All source files
SOURCES = Position-impl.cc Character-impl.cc Item-impl.cc observer-impl.cc \
          Floor-impl.cc game-impl.cc characterpack1-impl.cc decorator-impl.cc \
          chamber-impl.cc RandomFloorGenerator-impl.cc main.cc

# Try to include PRNG if available
PRNG_FILES = $(wildcard RandomNumberGeneratorExamples/PRNG.cc)

# Object files
OBJECTS = $(patsubst %.cc, build/%.o, $(SOURCES))
PRNG_OBJECTS = $(patsubst %.cc, build/%.o, $(PRNG_FILES))
ALL_OBJECTS = $(OBJECTS) $(PRNG_OBJECTS)

# Create build dir
$(shell mkdir -p build)
$(shell mkdir -p build/RandomNumberGeneratorExamples)

.PHONY: all clean help test modules traditional

# Default target - try modules first, fall back to traditional
all: modules

# Try C++20 modules build
modules:
	@echo "🚀 Attempting C++20 modules build..."
	@$(MAKE) modules-build 2>/dev/null || $(MAKE) traditional-build

modules-build: CXXFLAGS += -fmodules-ts
modules-build: $(EXEC)
	@echo "✅ C++20 modules build successful!"

# Fallback to traditional build
traditional-build:
	@echo "⚠️  Modules failed, trying traditional build..."
	@$(MAKE) traditional

traditional: $(EXEC)
	@echo "✅ Traditional C++ build successful!"

$(EXEC): $(ALL_OBJECTS)
	@echo "🔗 Linking final executable..."
	$(CXX) $(CXXFLAGS) $(ALL_OBJECTS) -o $@
	@echo "✅ Build complete! Run with: ./$(EXEC)"

# Compile source files
build/%.o: %.cc
	@echo "🔧 Compiling: $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "🧹 Cleaning build artifacts..."
	@rm -rf build/ gcm.cache/
	@rm -f $(EXEC)
	@echo "✅ Clean complete!"

test: $(EXEC)
	@echo "🧪 Running quick test..."
	@if [ -f cc3kfloor.txt ]; then \
		echo "TestPlayer\ns\nq" | ./$(EXEC) cc3kfloor.txt; \
	else \
		echo "TestPlayer\ns\nq" | ./$(EXEC); \
	fi

help:
	@echo "🎮 ChamberCrawler3000 - Smart Build System"
	@echo "=========================================="
	@echo "  make          - Smart build (tries modules, falls back)"
	@echo "  make modules  - Force C++20 modules build"
	@echo "  make traditional - Force traditional build"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make test     - Build and run quick test"
	@echo "  make help     - Show this help"
	@echo ""
	@echo "🔧 Using GCC 13 - will try C++20 modules first"
	