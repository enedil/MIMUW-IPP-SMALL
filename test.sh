#!/usr/bin/env bash

function print_color {
    red=`tput setaf 1`
    green=`tput setaf 2`
    reset=`tput sgr0`
    case "$2" in
        red)
            printf "$red";;
        green)
            printf "$green";;
    esac

    printf "$1"
    printf "$reset\n"
}

function correct_mark {
    printf "\u2713"
}

function wrong_mark {
    printf "\u274C"
}

function test_file {
    out=$(mktemp --suffix .out)
    err=$(mktemp --suffix .err)

    executable="$1"
    in_test="$2"
    out_test="${in_test%%in}out"
    err_test="${in_test%%in}err"


    "$executable" < "$in_test" >"$out" 2>"$err"
    if [ "$?" != 0 ];
    then
        return 1
    fi

    cmp -s "$out" "$out_test"
    if [ "$?" != 0 ];
    then
        return 1
    fi

    cmp -s "$err" "$err_test"
    if [ "$?" != 0 ];
    then
        return 1
    fi
        
    return 0
}

function test_valgrind {
    executable="$1"
    in_test="$2"

    err=$(mktemp --suffix err)

    valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all "$executable" < "$in_test" 2>&1 > /dev/null | awk '/SUMMARY/{y=1}y'
}

function help {
    printf "Usage:\t$0 [binary] [directory_with_tests]\n"
    printf "if env variable VALGRIND_DISABLE is set, valgrind will not be run\n"
    exit 1
}

ulimit -s unlimited

if [ -z "$1" ]
then
    help
elif [ -z "$2" ]
then
    help
fi

executable="$1"
directory="$2"

if [[ !(-x "$executable") ]]
then
    print_color "Binary is not executable by this user!\n" red
    help
fi


for test_input in "${directory%%/}/"*.in
do
    test_file "$executable" "$test_input"
    status_code=$?
    if [[ $status_code -eq 0 ]]
    then
        correct_mark
        print_color " TEST ${test_input%%.in} PASSED" green
    else
        wrong_mark
        print_color " TEST ${test_input%%.in} FAILED" red
    fi

    [ -z "$VALGRIND_DISABLE" ] && test_valgrind "$executable" "$test_input"
done
