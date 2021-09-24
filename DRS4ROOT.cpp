//C++ include
#include <iostream>
#include <BinDecode.h>

using namespace std;

int main(int argc, char** argv)
{
    BinDecode BD;

    string input,output;
    string exit("exit");
    int limit;

    while(input.compare(exit)!=0)
    {
        cout << "Enter the input filename: ";
        cin >> input;

		if(input.compare(exit)==0)
        {
			break;
		}

        cout << "Enter the save fileneme: ";
        cin >> output;

        if(argc>1)
        {
            limit = stoi(argv[1]);
        }else
        {
            limit = -1;
        }

        BD.Decode(input,output,limit);
	}

    return 0;
}