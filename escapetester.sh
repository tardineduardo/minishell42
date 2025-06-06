#!/bin/bash

# Caminho para seu minishell
MINISHELL=./minishell

# Cria diretórios temporários
mkdir -p out/bash out/minishell diffs

# Lista de testes
tests=(
'echo a\\b\c'
'echo \\'
'echo \\\\'
'echo "\\\\"'
'echo '\$USER''
'echo "\\$USER"'
'echo \\$'
'echo \\\$'
'echo abc\ def'
'echo "abc\ def"'
'echo "a\\b\\c"'
'echo "a\\$USER"'
'echo "a\\\$USER"'
'echo 'a\\b\\c''
'echo '$USER''
'echo "'$USER'"'
'echo "\"$USER\""'
'echo "'\$USER'"'
'echo \$FOO'
'echo "$FOO"'
'echo "\$FOO"'
'echo \\$FOO'
'echo \\\$FOO'
'echo "\\\$FOO"'
'echo "\$FOOabc"'
'echo \$\{FOO\}'
'echo "${FOO}"'
'echo "\${FOO}"'
'echo "\\${FOO}"'
'echo "\\\${FOO}"'
)

# Define uma variável de ambiente de exemplo
export FOO="bar"

# Executa testes
for i in "${!tests[@]}"; do
    printf "%s\nexit\n" "${tests[$i]}" | bash > "out/bash/test_$i.txt"
    printf "%s\nexit\n" "${tests[$i]}" | $MINISHELL > "out/minishell/test_$i.txt"
    diff -u "out/bash/test_$i.txt" "out/minishell/test_$i.txt" > "diffs/test_$i.diff"
done

# Resumo
echo -e "\nResumo dos testes com diferenças:"
for file in diffs/*.diff; do
    if [ -s "$file" ]; then
        echo "$(basename "$file")"
    fi
done
