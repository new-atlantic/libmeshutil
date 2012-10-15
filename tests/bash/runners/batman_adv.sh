#!/bin/bash

PASSES=0
FAILURES=0

NETWORK_INTERFACE="$1"

if [ -z "$NETWORK_INTERFACE" ]; then
   NETWORK_INTERFACE=$(ifconfig -s -a | head -n 2 | tail -n 1 | awk '{ print $1 }')
fi

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
    INTERFACE_NAME="bat0"
  fi

  batctl -m "$INTERFACE_NAME" if add $NETWORK_INTERFACE
  if [ -z "$(batctl -m $INTERFACE_NAME if | grep $NETWORK_INTERFACE)" ]; then
    echo "Enabling bat interface failed. Aborting..."
    exit 1
  fi

}

del_bat_if() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    INTERFACE_NAME="bat0"
  fi

  batctl -m "$INTERFACE_NAME" if del $NETWORK_INTERFACE
}

enable_bat_if() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    INTERFACE_NAME="bat0"
  fi

  ifconfig "$NETWORK_INTERFACE" up

  ifconfig "$INTERFACE_NAME" up
}

bat_if_address() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    INTERFACE_NAME="bat0"
  fi

  ifconfig $INTERFACE_NAME | grep HWaddr | egrep -o "([[:alnum:]][[:alnum:]][:]){5}[[:alnum:]][[:alnum:]]"
}

mount_debug_fs() {
  if [ -z "$(mount | grep debugfs)" ]; then
    mount -t debugfs none /sys/kernel/debug
  fi
}

n_bat_nodes() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    INTERFACE_NAME="bat0"
  fi

  if [ -n "$(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | grep 'No batman nodes in range...')" ]; then
    echo -n "1"
  else
    echo -n $(($(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | wc | awk '{ print $1 }') - 1))
  fi
}

bat_node_addresses() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    INTERFACE_NAME="bat0"
  fi

  if [ -z "$(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | egrep -o '^([[:alnum:]][[:alnum:]][:]){5}[[:alnum:]][[:alnum:]]')" ]; then
    echo -n "NO NODES"
  else
    echo -n "$(($(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | wc | awk '{ print $1 }') - 2)) "
    addresses="$(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | egrep -o '^([[:alnum:]][[:alnum:]][:]){5}[[:alnum:]][[:alnum:]]')"
    for addr in $addresses; do
      tmp=" $addr$tmp"
    done
    echo -n $tmp
  fi
}

bat_actual_next_hop_addresses() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    INTERFACE_NAME="bat0"
  fi

  if [ -z "$(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | tail -n +3 | grep -o ').*' | cut -c 3-19)" ]; then
    echo -n "NO NODES"
  else
    echo -n "$(($(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | tail -n +3 | grep -o ').*' | cut -c 3-19 | wc | awk '{ print $1 }'))) "
    addresses="$(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | tail -n +3 | grep -o ').*' | cut -c 3-19)"
    for addr in $addresses; do
      tmp=" $addr$tmp"
    done
    echo -n $tmp
  fi
}

bat_potential_next_hop_addresses() {
  local INTERFACE_NAME="$1"

  if [ -z "$INTERFACE_NAME" ]; then
    INTERFACE_NAME="bat0"
  fi

  if [ -z "$(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | tail -n +3 | grep -o ').*' | cut -c 3-19)" ]; then
    echo -n "NO NODES"
  else
    echo -n "$(($(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | tail -n +3 | grep -o ').*' | cut -c 3-19 | wc | awk '{ print $1 }'))) "
    addresses="$(cat /sys/kernel/debug/batman_adv/$INTERFACE_NAME/originators | tail -n +3 | grep -o ']:.*' | cut -c 4- | egrep -o '([[:alnum:]][[:alnum:]][:]){5}[[:alnum:]][[:alnum:]]' | awk ' !x[$0]++' )"
    for addr in $addresses; do
      tmp=" $addr$tmp"
    done
    echo -n $tmp
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

###############################################################################

################################
##                            ##
##  Section 1: Kernel module  ##
##                            ##
################################

##########################################################
# 1.1 Check if the batman_adv kernel module is available #
##########################################################

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

################################################################################
# 1.2.a Check that the kernel module is correctly being reported as not loaded #
################################################################################

remove_kmod
test_case "Kernel Module Loaded" "kmod_loaded" "FALSE" "\t\t" "\t\t"

############################################################################
# 1.2.b Check that the kernel module is correctly being reported as loaded #
############################################################################

insert_kmod
test_case "Kernel Module Loaded" "kmod_loaded" "TRUE" "\t\t" "\t\t"

############################################
# 1.3 Test kernel module version retrieval #
############################################

test_case "Kernel Module Version" "kmod_version" "$(cat /sys/module/batman_adv/version)" "\t\t" "\t"

######################################
##                                  ##
##  Section 2: Bat Interface tests  ##
##                                  ##
######################################

################################
# 2.1 Default Interface (bat0) #
################################

##################################################################################
# 2.1.1.a Check that interface is reported as not available when kmod not loaded #
##################################################################################

remove_kmod
test_case "Default bat interface available" "if_available_default" "FALSE" "" "\t\t"

##############################################################################
# 2.1.1.b Check that interface is reported as available when it is available #
##############################################################################

insert_kmod
add_bat_if
test_case "Default bat interface available" "if_available_default" "TRUE" "" "\t\t"
del_bat_if

#################################################################################
# 2.1.2.a Check that interface is reported as not being up when kmod not loaded #
#################################################################################

remove_kmod
test_case "Default bat interface up" "if_up_default" "FALSE" "\t" "\t\t"

##########################################################################
# 2.1.2.b Check that interface is reported as being up when interface up #
##########################################################################

insert_kmod
add_bat_if
enable_bat_if
test_case "Default bat interface up" "if_up_default" "TRUE" "\t" "\t\t"
del_bat_if

####################################################################################
# 2.1.3.a Check that an error is reported when asking for hwaddr when no interface #
####################################################################################

remove_kmod
test_case "Default bat interface MAC address" "if_hwaddr_default" "ERROR" "" "\t\t"

##################################################################################
# 2.1.3.a Check that the MAC address is reported when bat interface is available #
##################################################################################

insert_kmod
add_bat_if
BAT_IF_ADDR=$(bat_if_address)
test_case "Default bat interface MAC address" "if_hwaddr_default" "$BAT_IF_ADDR" "" ""
del_bat_if


#######################
# 2.2 Named Interface #
#######################

##################################################################################
# 2.2.1.a Check that interface is reported as not available when kmod not loaded #
##################################################################################

remove_kmod
test_case "Named bat interface available" "if_available_named" "FALSE" "\t" "\t\t"

##############################################################################
# 2.2.1.b Check that interface is reported as available when it is available #
##############################################################################

insert_kmod
add_bat_if "not_default"
test_case "Named bat interface available" "if_available_named" "TRUE" "\t" "\t\t"
del_bat_if "not_default"

#################################################################################
# 2.2.2.a Check that interface is reported as not being up when kmod not loaded #
#################################################################################

remove_kmod
test_case "Named bat interface up" "if_up_named" "FALSE" "\t" "\t\t"

##########################################################################
# 2.1.2.b Check that interface is reported as being up when interface up #
##########################################################################

insert_kmod
add_bat_if "not_default"
enable_bat_if "not_default"
test_case "Named bat interface up" "if_up_named" "TRUE" "\t" "\t\t"
del_bat_if "not_default"

####################################################################################
# 2.2.3.a Check that an error is reported when asking for hwaddr when no interface #
####################################################################################

remove_kmod
test_case "Named bat interface MAC address" "if_hwaddr_named" "ERROR" "" "\t\t"

##################################################################################
# 2.2.3.a Check that the MAC address is reported when bat interface is available #
##################################################################################

insert_kmod
add_bat_if "not_default"
BAT_IF_ADDR=$(bat_if_address "not_default")
test_case "Named bat interface MAC address" "if_hwaddr_named" "$BAT_IF_ADDR" "" ""
del_bat_if "not_default"

#######################
##                   ##
##  Section 3: Mesh  ##
##                   ##
#######################

################################
# 3.1 Kernel module not loaded #
################################

remove_kmod

##################################
# 3.1.1 Default interface (bat0) #
##################################

test_case "Number of nodes (default if)" "n_nodes_if_default" "0" "\t" "\t\t"
test_case "Node addresses (default if)" "node_addresses_if_default" "ERROR" "\t" "\t\t"

#########################
# 3.1.2 Named interface #
#########################

test_case "Number of nodes (named if)" "n_nodes_if_named" "0" "\t" "\t\t"
test_case "Node addresses (named if)" "node_addresses_if_named" "ERROR" "\t" "\t\t"

############################
# 3.2 Kernel module loaded #
############################

##################################
# 3.2.1 Default interface (bat0) #
##################################

insert_kmod

add_bat_if
enable_bat_if
mount_debug_fs

sleep 60

#########################################
# 3.2.1.1 Check number of nodes in mesh #
#########################################

NNODES=$(n_bat_nodes)
test_case "Number of nodes (default if)" "n_nodes_if_default" "$NNODES" "\t" "\t\t"

#####################################
# 3.2.1.2 Check mesh node addresses #
#####################################

ADDRESSES=$(bat_node_addresses)
test_case "Node addresses (default if)" "node_addresses_if_default" "$ADDRESSES" "\t" "\t"

################################################
# 3.2.1.3 Check mesh actual next hop addresses #
################################################

ADDRESSES=$(bat_actual_next_hop_addresses)
test_case "Actual next hop addresses (def if)" "actual_next_hop_addr_default" "$ADDRESSES" "" "\t"

###################################################
# 3.2.1.4 Check mesh potential next hop addresses #
###################################################

ADDRESSES=$(bat_potential_next_hop_addresses)
test_case "Potential nhop addresses (def if)" "potential_next_hop_addr_default" "$ADDRESSES" "" "\t"

del_bat_if
remove_kmod

#########################
# 3.2.2 Named interface #
#########################

insert_kmod

add_bat_if "not_default"
enable_bat_if "not_default"
mount_debug_fs

sleep 60

#########################################
# 3.2.2.1 Check number of nodes in mesh #
#########################################

NNODES=$(n_bat_nodes "not_default")
test_case "Number of nodes (named if)" "n_nodes_if_named" "$NNODES" "\t" "\t\t"

#####################################
# 3.2.2.2 Check mesh node addresses #
#####################################

ADDRESSES=$(bat_node_addresses "not_default")
test_case "Node addresses (named if)" "node_addresses_if_named" "$ADDRESSES" "\t" "\t"

################################################
# 3.2.2.3 Check mesh actual next hop addresses #
################################################

ADDRESSES=$(bat_actual_next_hop_addresses "not_default")
test_case "Actual next hop addresses (named if)" "actual_next_hop_addr_named" "$ADDRESSES" "" "\t"

###################################################
# 3.2.2.4 Check mesh potential next hop addresses #
###################################################

ADDRESSES=$(bat_potential_next_hop_addresses "not_default")
test_case "Potential nhop addresses (named if)" "potential_next_hop_addr_named" "$ADDRESSES" "" "\t"


del_bat_if "not_default"

##############################################################################

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
