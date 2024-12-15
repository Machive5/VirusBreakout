# This script is used to compile and run the program

# Run make to compile the program
make

# Check if the make command was successful
if [ $? -eq 0 ]; then
    # Run the compiled program
    ./virus-breakout
else
    echo "Compilation failed."
fi