valgrind 2>valgrindtemp.txt --leak-check=full --show-leak-kinds=all ./minishell
python3 supp.py valgrindtemp.txt
cat valgrindtemp.txt 
rm valgrindtemp.txt