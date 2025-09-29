# NGC - Nozzle Geometry Calculator Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
INCLUDES = -Iinclude
LIBS = -lm

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/ngc

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Build the main executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	@$(CC) $(OBJECTS) $(LIBS) -o $@
	@echo "Build completed successfully!"

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(OBJDIR) $(BINDIR)
	@rm -f *.dat *.gp nozzle_plot.png
	@echo "Clean completed!"

# Install (copy to system path - requires sudo)
install: $(TARGET)
	@echo "Installing NGC to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation completed!"

# Uninstall
uninstall:
	@echo "Removing NGC from /usr/local/bin..."
	@sudo rm -f /usr/local/bin/ngc
	@echo "Uninstallation completed!"

# Run with default parameters
run: $(TARGET)
	@echo "Running NGC with default parameters..."
	@./$(TARGET)

# Run with example parameters
example: $(TARGET)
	@echo "Running NGC with example parameters..."
	@./$(TARGET) -t 0.005 -e 0.025 -p 2000000 -T 3500 --data example_geometry.dat

# Debug build
debug: CFLAGS += -DDEBUG -g3
debug: directories $(TARGET)

# Help target
help:
	@echo "NGC - Nozzle Geometry Calculator"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install to system path"
	@echo "  uninstall- Remove from system path"
	@echo "  run      - Run with default parameters"
	@echo "  example  - Run with example parameters"
	@echo "  debug    - Build with debug symbols"
	@echo "  help     - Show this help message"

# Dependencies
$(OBJDIR)/main.o: $(INCDIR)/ngc.h
$(OBJDIR)/nozzle_geometry.o: $(INCDIR)/ngc.h
$(OBJDIR)/performance.o: $(INCDIR)/ngc.h
$(OBJDIR)/plotting.o: $(INCDIR)/ngc.h
$(OBJDIR)/utils.o: $(INCDIR)/ngc.h

.PHONY: all directories clean install uninstall run example debug help