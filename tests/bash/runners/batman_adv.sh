#!/bin/bash

FAILURES=0

#$1 Test name.
#$2 Test binary name.
#$3 Expected value.
#$4 Tabs
test_case () {
    local PASS="$(echo -n '['; tput setaf 2; echo -n 'pass'; tput sgr0; echo -n ']')"
    local FAIL="$(echo -n '['; tput setaf 1; echo -n 'fail'; tput sgr0; echo -n ']')"
    local DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

    echo -n "$1: "

    local VALUE=$("$DIR/../bin/$2")
    if [ "$3" == "$VALUE" ]; then
        local RESULT=$PASS
    else
        local RESULT=$FAIL
        ((FAILURES++))
    fi

    echo -e "$4\t$RESULT\t\t$3\t\t\t$VALUE"
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
