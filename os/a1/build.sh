g++ uShell.cpp -Wall -Werror main.cpp -o uShell.out
./uShell.out -v < input.txt > output.txt
./uShell_ref -v < input.txt > output_ref.txt
