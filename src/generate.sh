#!/bin/bash

if [ ! -f "../bin/premake5" ]; then
    echo "Run 'init.sh' to download premake executable"
fi

action="$1"

if [ "$action" == "" ]; then
    action="vs2017"
fi


../bin/premake5 $action


