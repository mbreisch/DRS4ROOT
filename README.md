# DRS4ROOT
Tool to convert DRS4 binary files to root trees. Used in a linux enviroment (tested in Ubuntu20.04).

# Prerequisites
Most of the used libraries are basic C++ libraries, but for the transform to ROOT files a version of root is neccessary.
To make the install easier the 
'''bash
./GetRoot.sh
'''
is used to install the latest stable version of root as a local install version. The script will install the basic prerequisites for root automatically, but extended or other system requirements have to be installed seperatly. 
If you don't have permission to run the command use 
'''bash
chmod +x ./GetRoot.h
'''
Once the script has finished a local root should be installed and a source command should be added to tha bashrc to make root accessible globally.

# How to install the DRS4ROOT tool
To install the tool change the directory to 
'''bash
/path/to/DRS4ROOT
'''
and run
'''bash
cmake .
'''
followed by 
'''bash
make -jN
'''
where N is the number of available CPU cores. Once done the DRS4ROOT folder should contain a 
'''bash
./DRS4ROOT
''' 
executable.

# How to use the DRS4ROOT tool
To run the just run 
'''bash
./DRS4ROOT
''' 
The prompt will then guide you to enter an input and an output file.
The inputfile has to be entered as full path-to-file (e.g. './RawFiles/Ex1/File1.dat', if you want to use the existing folder for raw files).
The outputfile will only need a name (e.g. 'SaveName1', no data extension) and is saved in the NewRootFiles folder. This will be changed to feature an user defined save folder.

# What does the root three look like? (BETA)
Each root file will contain as many trees as there were events in the original binary file.
Each tree is labeled 'Event_N;1' with N as the event number. 
Inside each tree are then several branches:
  - Waveform_BX_ChY
  - Time_BX_ChY
X represents the number of the connected DRS4 boards starting at 0 for one boards and incrementing for additional boards.
Y represents the channel on the DRS4 board going from 0 to 3 for each board.
The Waveform branches are in mV and the Time branches in ns.
There is already an additional tree clalled Boards which returns the amount of connected boards before events are evaluated.
The next update will include trees for triggertimestamp and other useful information.
