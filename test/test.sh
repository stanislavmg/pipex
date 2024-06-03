#!/bin/bash
# colors
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BOLD="\033[1m"
RESET='\033[0m'
# out files
BASH_OUT="out/bash.txt"
PIPEX_OUT="out/pipex.txt"
PIPEX_ERR="out/pipex_err.txt"
BASH_ERR="out/bash_err.txt"
FILES=("${PIPEX_ERR}" "${PIPEX_OUT}" "${BASH_ERR}" "${BASH_OUT}")
# pipex FILES
PIPEX="../pipex"
IN="in.txt"
OUT="out.txt"
TEST=0
LEAKS=1

rm -f tmp/*
chmod 644 ${FILES[@]} ${IN}
# leaks
MEMLEAKS=""
VG_ERR=42
if ! command -v valgrind > /dev/null 2>&1
then
	printf "${RED}Valgrind is not installed. Memory leaks detection is not enabled...${RESET}\n"
	LEAKS=0
elif [[ "$OSTYPE" == "darwin"* ]]
then
	printf "${RED}Memory leaks detection has been disabled on Darwin plateforms...${RESET}\n"
	LEAKS=0
else
	if [ $LEAKS -gt 0 ]
	then
		MEMLEAKS="valgrind --leak-check=full --show-leak-kinds=all --undef-value-errors=no --error-exitcode=$VG_ERR --errors-for-leak-kinds=all" --log-file=tmp/vlg_${TEST}
	fi
fi

function wait_for_timeout()
{
	sleep 2
	if kill -0 $1 > /dev/null 2>&1
	then
		kill $1
	fi
}

function run_test() {
	TEST=$((TEST + 1))
    ${MEMLEAKS} ${PIPEX} "${IN}" "${1}" "${2}" "${PIPEX_OUT}" 2>> ${PIPEX_ERR}
	if [ $? -eq $VG_ERR ]
	then
		print_aligned "${TEST}" "./pipex \"$1\" \"$2\"" "[LK]" "${YELLOW}"
	else
		rm tmp/vlg_${TEST} &> /dev/null && rm tmp/*.core* &> /dev/null
    fi
    2>> ${BASH_ERR} < ${IN} ${1} 2>> ${BASH_ERR} | ${2} 2>> ${BASH_ERR} > ${BASH_OUT}
}

function cleanf() {
	for file in "${FILES[@]}"
	do
		> "${file}"
	done
}

function print_aligned() {
    local line_number=$1
    local command=$2
    local status=$3
    printf "${4}%-3s %-28s %5s\n${RESET}" "$line_number" "$command" "$status"
}

function cmp_out() {
	if diff ${PIPEX_OUT} ${BASH_OUT} &> /dev/null && diff ${PIPEX_ERR} ${BASH_ERR} &> /dev/null
        then
			print_aligned "${TEST}" "./pipex \"$1\" \"$2\"" "[OK]" "${GREEN}"
        else
			print_aligned "${TEST}" "./pipex \"$1\" \"$2\"" "[KO]" "${RED}"
			printf "BASH OUTPUT:\n\n" >> tmp/${TEST}
			cat ${BASH_OUT} >> tmp/${TEST}
			printf "\nBASH ERROR:\n\n" >> tmp/${TEST}
			cat ${BASH_ERR} >> tmp/${TEST}
			printf "\n=======================================\n\n" >> tmp/${TEST}
			printf "\nPIPEX OUTPUT:\n\n" >> tmp/${TEST}
			cat ${PIPEX_OUT} >> tmp/${TEST}
			printf "\nPIPEX ERROR:\n\n" >> tmp/${TEST}
			cat ${PIPEX_ERR} >> tmp/${TEST}
    fi
	cleanf
}

function str_infile(){
    if grep "$1" "${BASH_ERR}" &> /dev/null; then
        local count_bash=$(grep -c "$1" "${BASH_ERR}")
        local count_pipex=$(grep -c "$1" "${PIPEX_ERR}")

        if [ "$count_bash" -eq "$count_pipex" ]; then
            echo "1" > "${BASH_ERR}"
            echo "1" > "${PIPEX_ERR}"
        fi
    fi
}

# simple test
printf "${BOLD}\n  SIMPLE TEST\n\n${RESET}"
first_args=("ls" "cat -e" "wc -l" "pwd" "/bin/cat")
second_args=("wc -l" "wc" "grep -ee" "/bin/ls" "head" "cat")
cleanf
for first_arg in "${first_args[@]}"
do
    for second_arg in "${second_args[@]}"
    do
		run_test "${first_arg}" "${second_arg}"
		cmp_out "${first_arg}" "${second_arg}"
    done
done
# check errors
printf "${BOLD}\n  ERROR TEST\n\n${RESET}"
first_args=("void" "" "    " "cat" "ls")
second_args=("ls" "cat" "void" "" "   ")
for first_arg in "${first_args[@]}"
do
    for second_arg in "${second_args[@]}"
    do
        run_test "${first_arg}" "${second_arg}"
		str_infile "command not found"
		cmp_out "${first_arg}" "${second_arg}"
    done
done

# check permission
printf "${BOLD}\n  CHECK FILE PERMISSION\n\n${RESET}"

chmod 0 ${IN}
run_test "cat" "ls"
chmod 644 ${IN}
str_infile "Permission denied"
cmp_out "cat" "ls"

chmod 0 ${PIPEX_OUT} ${BASH_OUT}
run_test "cat" "ls"
chmod 644 ${PIPEX_OUT} ${BASH_OUT}
str_infile "Permission denied"
cmp_out "cat" "ls"

chmod 0 ${IN} ${PIPEX_OUT} ${BASH_OUT}
run_test "cat" "ls"
chmod 644 ${IN} ${PIPEX_OUT} ${BASH_OUT}
str_infile "Permission denied"
cmp_out "cat" "ls"

# check timeout
printf "${BOLD}\n  TIMEOUT TEST\n\n${RESET}"
TEST=$((TEST + 1))
${PIPEX} "/dev/urandom" "cat" "head" "${PIPEX_OUT}" 2>> ${PIPEX_ERR} & bg_process=$!
wait_for_timeout ${bg_process} & wait ${bg_process}
if [ ${?} -eq 143 ]
then
	printf "${RED}${TEST}  ./pipex \"/dev/urandom\" \"cat\" \"head\" [KO]\n${RESET}"
else
	printf "${GREEN}${TEST}  ./pipex \"/dev/urandom\" \"cat\" \"head\" [OK]\n${RESET}"
fi

TEST=$((TEST + 1))
${PIPEX} ${IN} "yes" "head" "${PIPEX_OUT}" 2>> ${PIPEX_ERR} & bg_process=$!
wait_for_timeout ${bg_process} & wait ${bg_process}
if [ ${?} -eq 143 ]
then
	printf "${RED}${TEST}  ./pipex \"yes\" \"head\" [KO]\n${RESET}"
else
	printf "${GREEN}${TEST}  ./pipex \"yes\" \"head\" [OK]\n${RESET}"
fi