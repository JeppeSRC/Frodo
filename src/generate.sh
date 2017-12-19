#!/bin/bash

if [ ! -f "../bin/premake5" ]; then
    echo "Run 'init.sh' to download premake executable"
fi

action="$1"

if [ "$action" == "" ]; then
    action="vs2017"
fi

OS=$(uname -s)

if [ "$OS" == "Linux" ]; then
    echo "OS=Linux, generating project files!"
    ../bin/premake5 $action Linux
else
    echo "OS=$OS assuming windows, generating project files!"
    ../bin/premake5 $action Windows
fi

