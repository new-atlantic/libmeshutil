#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

PASS="$(echo -n '['; tput setaf 2; echo -n 'pass'; tput sgr0; echo -n ']')"
FAIL="$(echo -n '['; tput setaf 1; echo -n 'fail'; tput sgr0; echo -n ']')"

FAILURES=0

echo -e "\t\t\t\t\t TEST\t\tEXPECTED\t\tRESULT"
echo -e "\t\t\t\t\t ----\t\t--------\t\t------"

####

echo -n "Kernel_Module_Available: "

if [ -f "/lib/modules/$(uname -r)/kernel/net/batman-adv/batman-adv.ko" ]; then
    EXPECTED="TRUE"
else
    EXPECTED="FALSE"
fi;

VALUE=$($DIR/../bin/kmod_available)
if [ "$EXPECTED" == "$VALUE" ]; then
    RESULT=$PASS
else
    RESULT=$FAIL
    ((FAILURES++))
fi

echo -e "\t\t$RESULT\t\t$EXPECTED\t\t\t$VALUE"

if [ "$EXPECTED" == "FALSE" ]; then
    echo "batman_adv kernel module not available. Aborting..."
    exit 1
fi


####

exit $FAILURES
