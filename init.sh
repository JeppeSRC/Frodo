#!/bin/bash

PREMAKE_VERSION="5.0.0-alpha12"

uname_s=$(uname -s)
uname_m=$(uname -m)

Machine=""
OS="windows"

if [ "$uname_s" == "Linux" ]; then
    OS="linux"
fi
    
if [ "$uname_m" == "x86_64" ]; then
    Machine="x86_64"
elif [ "$uname_m" == "x86" ]; then
    Machine="x86"
else
    echo "Unknown platform $uname_m! Supported platforms are x86 and x86_64"
fi


$(mkdir -p "tmp" "bin")

if [ "$OS" == "windows" ]; then
    $(curl -L -o tmp/premake.zip https://github.com/premake/premake-core/releases/download/v$PREMAKE_VERSION/premake-$PREMAKE_VERSION-windows.zip)
    $(unzip -u  tmp/premake.zip -d bin)
elif [ "$OS" == "linux" ]; then 
    if [ "$Machine" == "x86_64" ]; then
		$(curl -L -o "tmp/premake5.tar.gz" "https://github.com/premake/premake-core/releases/download/v$PREMAKE_VERSION/premake-$PREMAKE_VERSION-linux.tar.gz")
		$(tar -xvzf "tmp/premake5.tar.gz" -C "bin")
	else
		$(curl -L -o "tmp/premake5-src.zip" "https://github.com/premake/premake-core/releases/download/v$PREMAKE_VERSION/premake-$PREMAKE_VERSION-src.zip")
		$(unzip -o "tmp/premake5-src.zip" -d "tmp")

		echo "Building premake from source.."
		$(make -C "tmp/premake-$PREMAKE_VERSION/build/gmake.unix/")
		$(cp "tmp/premake-$PREMAKE_VERSION/bin/release/premake5" "bin/")
	fi
fi

$(rm -r tmp)