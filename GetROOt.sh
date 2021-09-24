#!/bin/bash
CURRENT='pwd'
ROOTDIR="/Root_local/root_install/bin/thisroot.sh"
SOURCE="source "

if ! command -v git &> /dev/null
then
    echo "Installing git"
    sudo apt-get install git
fi
if ! command -v dpkg-dev &> /dev/null
then
    echo "Installing dpkg-dev"
    sudo apt-get install dpkg-dev
fi
if ! command -v cmake > /dev/null
then
    echo "Installing cmake"
    sudo apt-get install cmake
fi
if ! command -v g++ > /dev/null
then
    echo "Installing g++"
    sudo apt-get install g++
fi
if ! command -v gcc > /dev/null
then
    echo "Installing gcc"
    sudo apt-get install gcc
fi
if ! command -v binutils > /dev/null
then
    echo "Installing binutils"
    sudo apt-get install binutils
fi
if ! command -v libx11-dev > /dev/null
then
    echo "Installing libx11-dev"
    sudo apt-get install libx11-dev
fi
if ! command -v libxpm-dev > /dev/null
then
    echo "Installing libxpm-dev"
    sudo apt-get install libxpm-dev
fi
if ! command -v libxft-dev > /dev/null
then
    echo "Installing libxft-dev"
    sudo apt-get install libxft-dev
fi
if ! command -v libxext-dev > /dev/null
then
    echo "Installing libxext-dev"
    sudo apt-get install libxext-dev
fi
if ! command -v python > /dev/null
then
    echo "Installing python"
    sudo apt-get install python
fi
if ! command -v libssl-dev > /dev/null
then
    echo "Installing libssl-dev"
    sudo apt-get install libssl-dev
fi


mkdir -p Root_local && cd Root_local
git clone --branch latest-stable https://github.com/root-project/root.git root_src
mkdir -p root_build root_install && cd root_build
cmake -DCMAKE_INSTALL_PREFIX=../root_install ../root_src -DCMAKE_CXX_STANDARD=17
cmake --build . -- install -j4

cd .. 
cd ..

echo ""
echo ""
echo "----------------------------------------------------------------"
echo "Done with the install of Root"
echo "Installed version is already sourced in the bashrc in" 
echo "$SOURCE$CURRENT$ROOTDIR"


