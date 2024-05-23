#!/usr/bin/bash

RED="\033[31m"
GREEN="\033[32m"
BASH_OUT="bash.txt"
PIPEX_OUT="pipex.txt"
PIPEX_ERR="pipex_err.txt"
BASH_ERR="bash_err.txt"
IN="./in.txt"

first_args=("cat" "ls" "ls -la" "cat -e" "mkdir test")
second_args=("wc -l" "wc" "grep 1" "grep" "rmdir test" "ls -a" "grep test")

for first_arg in "${first_args[@]}"
do
    for second_arg in "${second_args[@]}"
    do
        ./pipex "${IN}" "${first_arg}" "${second_arg}" "${PIPEX_OUT}" 2> ${PIPEX_ERR}
        if [ $? -ne 0 ]; then
            printf "${RED}[Error] ${first_arg} ${second_arg} [Error]\n"
        else
            < "${IN}" "${first_arg}" | "${second_arg}" > "${BASH_OUT}" 2> ${BASH_ERR}
            if ! diff "${PIPEX_OUT}" "${BASH_OUT}" &> /dev/null || ! diff "${PIPEX_ERR}" "${BASH_ERR}" &> /dev/null
            then
                printf "${GREEN}[OK] ${first_arg} ${second_arg} [OK]\n"
            else
                printf "${RED}[KO] ${first_arg} ${second_arg} [KO]\n"
				break
            fi
        fi
    done
done
