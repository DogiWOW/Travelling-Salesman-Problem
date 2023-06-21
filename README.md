### Travelling-Salesman-Problem
Program made in C++ that solves the Travelling Salesman Problem

## Functionality
The program solves the TSP problem using an evolutionary algorithm. 
It was tested for lots of different cases on Windows 10 OS in VS Code and Microsoft VS 2022.
Algorithm tries to find the shortest path that visits each and every city exactly once and comes back to the it started at.
User can choose different parameters (for example population count or mutation rate) to his liking using the terminal.

## Running the program
To run the program simply run the "main.exe" executable file.
If you encounter any problems make sure you have installed a c++ compiler on your device.

## Using the program
# Enter your simulation parameters (or hit ENTER for default):
- Population count -       pupulation count in each generation
- Elite population count - amount of individuals that makes it to the next generation
- Number of generations -  number of generations the program simulates
- Mutation frequency -     how often the genes mutate

# Choose your city generation method
- Randomized points -      set the number of cities with randomized coordinates
- From the terminal -      enter the cities and its coordinates from the terminal
- From a file -            choose a text file from the "cities" folder

After this the program calculates the shortest cycle and displays the route it found
