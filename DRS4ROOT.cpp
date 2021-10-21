//C++ include
#include <iostream>
#include <BinDecode.h>

using namespace std;

int main(int argc, char** argv)
{
    BinDecode BD;

    string input,output;
    string line, path, file, makefolder;
    int limit=-1;

    std::fstream infile("./configfiles/filelist.txt", std::ios_base::in);

    if(infile.is_open())
    {
        while(getline(infile, input))
        {   
            line = input;
            std::size_t found_slash = line.find_last_of("/");
            path = line.substr(0,found_slash);
            file = line.substr(found_slash+1);

            makefolder = "mkdir -p " + path + "/ROOTFILES";
            system(makefolder.c_str());
            
            std::size_t found_dot = file.find_last_of(".");
            output = path + "/ROOTFILES/" + file.substr(0,found_dot) + ".root";

            if(argc>1)
            {
                limit = stoi(argv[1]);
            }
            BD.Decode(input,output,limit);
        }
  	    infile.close();
    }else
    {
        cout << ">>>>> File not found! Try again" << endl;
    }

    return 0;
}