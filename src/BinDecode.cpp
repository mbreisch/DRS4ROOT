#include <BinDecode.h>

BinDecode::BinDecode()
{
    system("mkdir -p NewRootFiles");
}

int BinDecode::Decode(string filename, string savename, int limit)
{
	CreateRootFile(savename);

	FHEADER  fh;
	THEADER  th;
	BHEADER  bh;
	EHEADER  eh;
	TCHEADER tch;
	CHEADER  ch;

    	Savename = savename;
	unsigned short voltage[1024];
	float bin_width[16][4][1024];
	int i, j, b, chn, n, chn_index;
	double t1, t2, dt;

	// open the binary waveform file
	FILE *f = fopen(filename.c_str(), "rb");
	if (f == NULL) 
    	{
		printf("Cannot find file \'%s\'\n", filename.c_str());
		return 0;
	}

	// read file header
	fread(&fh, sizeof(fh), 1, f);
	if (fh.tag[0] != 'D' || fh.tag[1] != 'R' || fh.tag[2] != 'S') 
    	{
		printf("Found invalid file header in file \'%s\', aborting.\n", filename.c_str());
		return 0;
	}

	if (fh.version != '2') 
    	{
		printf("Found invalid file version \'%c\' in file \'%s\', should be \'2\', aborting.\n", fh.version, filename.c_str());
		return 0;
	}

	// read time header
	fread(&th, sizeof(th), 1, f);
	if (memcmp(th.time_header, "TIME", 4) != 0) 
    	{
		printf("Invalid time header in file \'%s\', aborting.\n", filename.c_str());
		return 0;
	}

	for (b = 0 ; ; b++) 
    	{
		// read board header
		fread(&bh, sizeof(bh), 1, f);
		if (memcmp(bh.bn, "B#", 2) != 0) 
        	{
			// probably event header found
			fseek(f, -4, SEEK_CUR);
			break;
		}

		printf("Found data for board #%d\n", bh.board_serial_number);

		// read time bin widths
		memset(bin_width[b], sizeof(bin_width[0]), 0);
		for (chn=0 ; chn<5 ; chn++) 
        	{
			fread(&ch, sizeof(ch), 1, f);
			if (ch.c[0] != 'C') 
            		{
				// event header found
				fseek(f, -4, SEEK_CUR);
				break;
			}
			i = ch.cn[2] - '0' - 1;
			printf("Found timing calibration for channel #%d\n", i+1);
			fread(&bin_width[b][i][0], sizeof(float), 1024, f);
		}
	}
	n_boards = b;

	// loop over all events in the data file
	for (n=0 ; ; n++) 
    	{
		// read event header
		i = (int)fread(&eh, sizeof(eh), 1, f);
		if (i < 1)
        	{
			break;
		}
		printf("Found event #%d %d %d\n", eh.event_serial_number, eh.second, eh.millisecond);
		glob_timestamp_y = eh.year;
		glob_timestamp_m = eh.month;
		glob_timestamp_d = eh.day;
		glob_timestamp_hh = eh.hour;
		glob_timestamp_mm = eh.minute;
		glob_timestamp_ss = eh.second;
		glob_timestamp_mils = eh.millisecond;
		glob_range = eh.range;

		// loop over all boards in data file
		for (b=0 ; b<n_boards ; b++) 
        	{
			// read board header
			fread(&bh, sizeof(bh), 1, f);
			if (memcmp(bh.bn, "B#", 2) != 0) 
            		{
				printf("Invalid board header in file \'%s\', aborting.\n", filename.c_str());
				return 0;
			}

			// read trigger cell
			fread(&tch, sizeof(tch), 1, f);
			if (memcmp(tch.tc, "T#", 2) != 0) 
            		{
				printf("Invalid trigger cell header in file \'%s\', aborting.\n", filename.c_str());
				return 0;
			}
			glob_triggercell = tch.trigger_cell;

			if (n_boards > 1)
            		{
				printf("Found data for board #%d\n", bh.board_serial_number);
			}

			// reach channel data
			for (chn=0 ; chn<4 ; chn++) 
            		{
				// read channel header
				fread(&ch, sizeof(ch), 1, f);
				if (ch.c[0] != 'C') 
                		{
					// event header found
					fseek(f, -4, SEEK_CUR);
					break;
				}
				chn_index = ch.cn[2] - '0' - 1;
                		fread(&scaler, sizeof(int), 1, f);
				fread(voltage, sizeof(short), 1024, f);

				for (i=0 ; i<1024 ; i++) 
                		{
					// convert data to volts
					waveform[b][chn_index][i] = (voltage[i] / 65536. + eh.range/1000.0 - 0.5);

					// calculate time for this cell
					for (j=0,time[b][chn_index][i]=0 ; j<i ; j++)
                    			{
						time[b][chn_index][i] += bin_width[b][chn_index][(j+tch.trigger_cell) % 1024];
					}				
				}	
			}	
			// align cell #0 of all channels
         		t1 = time[b][0][(1024-tch.trigger_cell) % 1024];
         		for (chn=1 ; chn<4 ; chn++) 
			{
            			t2 = time[b][chn][(1024-tch.trigger_cell) % 1024];
            			dt = t1 - t2;
            			for (i=0 ; i<1024 ; i++)
				{
               				time[b][chn][i] += dt;
				}
         		} 
		}
       		ToRoot(n);
		if(n+1>=limit && limit !=-1)
		{
			break;
		}
	}
	CloseRootFile(n);
	cout << "DONE!" << endl;
	return 1;
}

void BinDecode::CreateRootFile(string savename)
{
    	string RootFile = "./NewRootFiles/" + savename + ".root";
	RFile = new TFile(RootFile.c_str(),"update");
}

void BinDecode::CloseRootFile(int n_evn)
{
	int events = n_evn;
	TTree *EvTree = new TTree("EventNr","Number of events in the tree");
	EvTree->SetEntries(1);
	TBranch* ev = EvTree->Branch("EventNr",&events,"events/I");	
	ev->Fill();	
	RFile->Write();
	RFile->Close();
}

int BinDecode::ToRoot(int evn)
{     
	if(evn==0)
	{
		int boards = n_boards;
	    	TTree *BoTree = new TTree("Boards","Number of chained DRS4 boards");
		BoTree->SetEntries(1);
    		TBranch* bo = BoTree->Branch("Boards",&boards,"boards/I");	
		bo->Fill();

		TTree *TimeTree = new TTree("TimeBinWidth","Time bin-width for the channels");
		TimeTree->SetEntries(1024);
		for(int b=0; b<n_boards; b++)
		{	
			for(int ch=0; ch<4; ch++)
			{
				string strb2 = "Time_B" + to_string(b) + "_Ch" + to_string(ch);
				TBranch* bT = TimeTree->Branch(strb2.c_str(),&TimeBin,"TimeBin/D");
				for(int smp=0; smp<1024; smp++)
				{
					TimeBin = time[b][ch][smp];
					bT->Fill();
				}
			}
		}
	}

	for(int b=0; b<n_boards; b++)
	{	
		for(int ch=0; ch<4; ch++)
		{
			CreateTreeAndBranches(b,ch,evn);
		}
	}
    return 1;
}


int BinDecode::CreateTreeAndBranches(int b, int ch, int evn)
{
    	string event = "Event_" + to_string(evn);
    	string description = "Tree representing event " + to_string(evn) + " of board " + to_string(b);
	TTree *NewTree;

	if(ch==0)
	{
		NewTree= new TTree(event.c_str(),description.c_str(),1);
		NewTree->SetEntries(1024);
	}else
	{
		NewTree = (TTree*)RFile->Get(event.c_str());
	}

	string strb1 = "Waveform_B" + to_string(b) + "_Ch" + to_string(ch);
    	TBranch* b1 = NewTree->Branch(strb1.c_str(),&Sample,"Sample/D");
    	for(int smp=0; smp<1024; smp++)
    	{
       	 	Sample = waveform[b][ch][smp];
        	b1->Fill();
    	}
	string strb2 = "Scaler_B" + to_string(b) + "_Ch" + to_string(ch);
	TBranch* b2 = NewTree->Branch(strb2.c_str(),&Scaler,"Scaler/I");
	Scaler = scaler;
	b2->Fill();

	if(b==0 && ch==0)
	{
		TBranch* b3 = NewTree->Branch("Triggercell",&Triggercell,"Triggercell/I");
		Triggercell = glob_triggercell;
		b3->Fill();
		TBranch* b4 = NewTree->Branch("Timestamp_y",&Timestamp_y,"Timestamp_y/I");
		Timestamp_y = glob_timestamp_y;
		b4->Fill();
		TBranch* b5 = NewTree->Branch("Timestamp_m",&Timestamp_m,"Timestamp_m/I");
		Timestamp_m = glob_timestamp_m;
		b5->Fill();
		TBranch* b6 = NewTree->Branch("Timestamp_d",&Timestamp_d,"Timestamp_d/I");
		Timestamp_d = glob_timestamp_d;
		b6->Fill();
		TBranch* b7 = NewTree->Branch("Timestamp_hh",&Timestamp_hh,"Timestamp_hh/I");
		Timestamp_hh = glob_timestamp_hh;
		b7->Fill();
		TBranch* b8 = NewTree->Branch("Timestamp_mm",&Timestamp_mm,"Timestamp_mm/I");
		Timestamp_mm = glob_timestamp_mm;
		b8->Fill();
		TBranch* b9 = NewTree->Branch("Timestamp_ss",&Timestamp_ss,"Timestamp_ss/I");
		Timestamp_ss = glob_timestamp_ss;
		b9->Fill();
		TBranch* b10 = NewTree->Branch("Timestamp_mils",&Timestamp_mils,"Timestamp_mils/I");
		Timestamp_mils = glob_timestamp_mils;
		b10->Fill();
		TBranch* b11 = NewTree->Branch("Range",&Range,"Range/I");
		Range = glob_range;
		b11->Fill();	
	}

    	return 1;
}
