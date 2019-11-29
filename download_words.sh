#!/bin/bash

# This script is simply to reduce .git filesize by downloading the
# wordlists off of the internet

WGET=/usr/bin/wget

$WGET 'https://raw.githubusercontent.com/first20hours/google-10000-english/master/20k.txt'
$WGET 'https://raw.githubusercontent.com/dolph/dictionary/master/enable1.txt'
