# brew install readline

# starts an x86_64 terminal using rosetta on apple silicon:
# arch -x86_64 zsh
gcc -shared -fPIC -o liblexer.so lexer.test.c ../src/lexer.c -DSHARED_LIBRARY