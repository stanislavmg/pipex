#!/usr/bin/bash

RED="\033[31m"
GREEN="\033[32m"
BASH_OUT="bash.txt"
PIPEX_OUT="pipex.txt"
PIPEX_ERR="pipex_err.txt"
BASH_ERR="bash_err.txt"
IN="./in.txt"

function clean_files() {
	local files=("${@}")

	for file in "${files[@]}"
	do
		> "${file}"
	done
}

first_args=("cat" "ls" "ls -la" "cat -e" "wc -l" "wc" "tail" "")
second_args=("wc -l" "wc" "grep 1" "grep" "mkdir test" "rmdir 1" "ls -a" "")
files=("${PIPEX_ERR}" "${PIPEX_OUT}" "${BASH_ERR}" "${BASH_OUT}")

clean_files "${files[@]}"
for first_arg in "${first_args[@]}"
do
    for second_arg in "${second_args[@]}"
    do
		if [ -f test ]; then
			rmdir test 2> /dev/null
		fi
        ./pipex "${IN}" "${first_arg}" "${second_arg}" "${PIPEX_OUT}" 2> ${PIPEX_ERR}
		if [ -f test ]; then
			rmdir test 2> /dev/null
		fi
        < ${IN} ${first_arg} | ${second_arg} > ${BASH_OUT} 2> ${BASH_ERR}
        if diff ${PIPEX_OUT} ${BASH_OUT} &> /dev/null && diff ${PIPEX_ERR} ${BASH_ERR} &> /dev/null
        then
        	printf "${GREEN}[OK] ${first_arg} ${second_arg} [OK]\n"
        else
            printf "${RED}[KO] ${first_arg} ${second_arg} [KO]\n"
			sleep 10
        fi
		clean_files "${files[@]}"
    done
done
