valgrind 2>valgrindtemp.txt --track-fds=yes --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell
python3 supp.py valgrindtemp.txt
cat valgrindtemp.txt 
rm valgrindtemp.txt