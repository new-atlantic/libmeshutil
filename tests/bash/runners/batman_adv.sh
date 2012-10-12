#!/bin/bash

PASSES=0
FAILURES=0

NETWORK_INTERFACE=$(ifconfig -s -a | head -n 2 | tail -n 1 | awk '{ print $1 }')

remove_kmod() {
    modprobe -r batman_adv

    if [ "$(grep -o "^batman_adv" /proc/modules)" == "batman_adv" ]; then
        echo "Removing batman_adv failed. Aborting..."
        exit 1
    fi
}

insert_kmod() {
    modprobe batman_adv

    if [ -z $(grep -o "^batman_adv" /proc/modules) ]; then
        echo "Inserting kernel module failed. Aborting..."
        exit 1
    fi
}

add_bat_if() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    batctl if add $NETWORK_INTERFACE
    if [ -z "$(batctl if | grep $NETWORK_INTERFACE)" ]; then
      echo "Enabling bat interface failed. Aborting..."
      exit 1
    fi
  else
    batctl -m "$INTERFACE_NAME" if add $NETWORK_INTERFACE
    if [ -z "$(batctl -m $INTERFACE_NAME if | grep $NETWORK_INTERFACE)" ]; then
      echo "Enabling bat interface failed. Aborting..."
      exit 1
    fi
  fi
}

del_bat_if() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    batctl if del $NETWORK_INTERFACE
  else
    batctl -m "$INTERFACE_NAME" if del $NETWORK_INTERFACE
  fi
}

test_case () {
    local TEST_NAME="$1"
    local TEST_BINARY="$2"
    local EXPECTED_RESULT="$3"
    local PRECEDING_TABS="$4"
    local MIDDLE_TABS="$5"

    local PASS="$(echo -n '['; tput setaf 2; echo -n 'pass'; tput sgr0; echo -n ']')"
    local FAIL="$(echo -n '['; tput setaf 1; echo -n 'fail'; tput sgr0; echo -n ']')"

    local DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

    echo -n "$TEST_NAME: "

    local ACTUAL_RESULT=$("$DIR/../bin/$TEST_BINARY")
    if [ "$EXPECTED_RESULT" == "$ACTUAL_RESULT" ]; then
        local TEST_RESULT=$PASS
        ((PASSES++))
    else
        local TEST_RESULT=$FAIL
        ((FAILURES++))
    fi

    echo -e "$PRECEDING_TABS\t$TEST_RESULT\t\t$EXPECTED_RESULT$MIDDLE_TABS\t$ACTUAL_RESULT"
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

test_case "Kernel Module Available" "kmod_available" "$EXPECTED" "\t" "\t\t"

if [ "$EXPECTED" == "FALSE" ]; then
    echo "batman_adv kernel module not available. Aborting..."
    exit 1
fi

########

remove_kmod
test_case "Kernel Module Loaded" "kmod_loaded" "FALSE" "\t\t" "\t\t"

####

insert_kmod
test_case "Kernel Module Loaded" "kmod_loaded" "TRUE" "\t\t" "\t\t"

########

test_case "Kernel Module Version" "kmod_version" "$(cat /sys/module/batman_adv/version)" "\t\t" "\t"

########

remove_kmod
test_case "Default bat interface available" "if_available_default" "FALSE" "" "\t\t"

####

insert_kmod
add_bat_if
test_case "Default bat interface available" "if_available_default" "TRUE" "" "\t\t"
del_bat_if

########

remove_kmod
test_case "Named bat interface available" "if_available_named" "FALSE" "\t" "\t\t"

####

insert_kmod
add_bat_if "not_default"
test_case "Named bat interface available" "if_available_named" "TRUE" "\t" "\t\t"
del_bat_if "not_default"

########

remove_kmod

echo ""
echo -e "Test cases run:\t\t$((PASSES + FAILURES))"
if [ $PASSES -gt 0 ]; then
    echo -en "Tests passed:\t\t"; tput setaf 2; echo -n "$PASSES"; tput sgr0; echo "";
fi;
if [ $FAILURES -gt 0 ]; then
    echo -en "Tests failed:\t\t"; tput setaf 1; echo -n "$FAILURES"; tput sgr0; echo "";
fi;

exit $FAILURES
