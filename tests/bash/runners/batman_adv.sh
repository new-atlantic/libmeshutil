#!/bin/bash

FAILURES=0

#$2 Test binary name.
#$3 Expected value.
#$4 Tabs
test_case () {
    local TEST_NAME="$1"
    local TEST_BINARY="$2"
    local EXPECTED_RESULT="$3"
    local PRECEDING_TABS="$4"

    local PASS="$(echo -n '['; tput setaf 2; echo -n 'pass'; tput sgr0; echo -n ']')"
    local FAIL="$(echo -n '['; tput setaf 1; echo -n 'fail'; tput sgr0; echo -n ']')"

    local DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

    echo -n "$TEST_NAME: "

    local ACTUAL_RESULT=$("$DIR/../bin/$TEST_BINARY")
    if [ "$EXPECTED_RESULT" == "$ACTUAL_RESULT" ]; then
        local TEST_RESULT=$PASS
    else
        local TEST_RESULT=$FAIL
        ((FAILURES++))
    fi

    echo -e "$PRECEDING_TABS\t$TEST_RESULT\t\t$EXPECTED_RESULT\t\t\t$ACTUAL_RESULT"
}

echo -e "\t\t\t\t\t\t BATMAN Advanced test cases"
echo ""
echo -e "\t\t\t\t\t TEST\t\tEXPECTED\t\tRESULT"
echo -e "\t\t\t\t\t ----\t\t--------\t\t------"

########

if [ -f "/lib/modules/$(uname -r)/kernel/net/batman-adv/batman-adv.ko" ]; then
    EXPECTED="TRUE"
else
    EXPECTED="FALSE"
fi;

test_case "Kernel Module Available" "kmod_available" "$EXPECTED" "\t"

if [ "$EXPECTED" == "FALSE" ]; then
    echo "batman_adv kernel module not available. Aborting..."
    exit 1
fi

########

modprobe -r batman_adv

if [ "$(grep -o "^batman_adv" /proc/modules)" == "batman_adv" ]; then
    echo "Removing batman_adv failed. Aborting..."
    exit 1
else
    test_case "Kernel Module Loaded" "kmod_loaded" "FALSE" "\t\t"
fi;

####

modprobe batman_adv

if [ -z $(grep -o "^batman_adv" /proc/modules) ]; then
    echo "Inserting kernel module failed. Aborting..."
    exit 1
else
    test_case "Kernel Module Loaded" "kmod_loaded" "TRUE" "\t\t"
fi;

########

exit $FAILURES
