
# Makefile to compile and run day1.c

# Set variables
CC = clang
TARGET = day1
SRC = day1.c

# The default target is to build and run the program
all: $(TARGET)
	./$(TARGET)

# Compile the C source file into an executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean up the generated files
clean:
	rm -f $(TARGET)
