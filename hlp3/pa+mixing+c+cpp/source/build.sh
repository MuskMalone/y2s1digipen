clear
mkdir ../split
mkdir ../outputjoin

mkdir ../split/sj1
gcc -Wall -Werror -Wextra splitter.c driver.c -o sj1.out
./sj1.out -s 70000 -o ../split/sj1/split_ -i ../data/a > splitout1.txt
./sj1.out -j -o ../outputjoin/sj1output -i ../split/sj1/split_* >joinout1.txt
diff ../outputjoin/sj1output ../data/a

mkdir ../split/sj2
 g++ -Wall -Werror -Wextra splitter.cpp driver.cpp -o sj2.out
./sj2.out -s 70000 -o ../split/sj2/split_ -i ../data/a  > splitout2.txt
./sj2.out -j -o ../outputjoin/sj2output -i ../split/sj2/split_* >joinout2.txt
diff ../outputjoin/sj2output ../data/a

for file in ../split/sj1/split_*; do diff "$file" "../split/sj2/${file##*/}"; done