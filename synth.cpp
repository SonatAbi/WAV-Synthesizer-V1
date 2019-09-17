#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <string>
#include <math.h>
#include <cmath>
#include <iomanip>

using namespace std;

void readSamples(ifstream &in, string &Input, double &NUMSAMPLES);
void readNotes(ifstream &in, string &Input, double &delta, int16_t *& Data, bool &fail);

int main(int argc, char *argv[])
{
	string input;
	string wave;
	std::vector<std::string>  arguments;
	for (int i = 0; i < argc; ++i) arguments.push_back(argv[i]);
	if (argc == 3) {
		input = argv[1];
		wave = argv[2];
	}
	else {
		std::cerr << "Error: Wrong Command Line Arguments" << endl;
		return EXIT_FAILURE;
	}

	double samples = 0;
	ifstream in;
	int16_t  *Data;
	readSamples(in, input, samples);
	cout << samples << "\n";

	int32_t  SampleRate = 44100;       // samples per second, 44100
	int32_t  NUMSAMPLES =(int32_t) (samples * SampleRate);  /**************************************************/
	int32_t   ChunkID = 0x46464952;        // "RIFF"
	int32_t  Subchunk1Size = 16;    // 16
	int16_t  NumChannels = 1;      // Mono = 1
	int16_t  BitsPerSample = 16;    // 8 bits = 8, 16 bits = 16 <=
	int32_t  Subchunk2Size = NUMSAMPLES * NumChannels * BitsPerSample / 8;    // NumSamples * NumChannels * BitsPerSample/8
	int32_t  ChunkSize = 20 + Subchunk1Size +Subchunk2Size;         // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	int32_t   Format = 0x45564157;         // "WAVE"
	int32_t   Subchunk1ID = 0x20746d66;   // "fmt "
	int16_t  AudioFormat = 1;      // 1
	int32_t  ByteRate = SampleRate * NumChannels * BitsPerSample / 8;         // SampleRate * NumChannels * BitsPerSample/8
	int16_t  BlockAlign = NumChannels * BitsPerSample / 8;       // NumChannels * BitsPerSample/8
	int32_t   Subchunk2ID = 0x61746164;   // "data"
	Data = new int16_t[NUMSAMPLES]; // the Pulse Code Modulated data
	double delta = 0.0000226757369614512471655328798186;
	
	cout << NUMSAMPLES<<"\n";
	
	ofstream out(wave, std::ios::binary);
	out.write(reinterpret_cast<std::fstream::char_type*>(&ChunkID), sizeof ChunkID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&ChunkSize), sizeof ChunkSize);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Format), sizeof Format);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk1ID), sizeof Subchunk1ID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk1Size), sizeof Subchunk1Size);
	out.write(reinterpret_cast<std::fstream::char_type*>(&AudioFormat), sizeof AudioFormat);
	out.write(reinterpret_cast<std::fstream::char_type*>(&NumChannels), sizeof NumChannels);
	out.write(reinterpret_cast<std::fstream::char_type*>(&SampleRate), sizeof SampleRate);
	out.write(reinterpret_cast<std::fstream::char_type*>(&ByteRate), sizeof ByteRate);
	out.write(reinterpret_cast<std::fstream::char_type*>(&BlockAlign), sizeof BlockAlign);
	out.write(reinterpret_cast<std::fstream::char_type*>(&BitsPerSample), sizeof BitsPerSample);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk2ID), sizeof Subchunk2ID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk2Size), sizeof Subchunk2Size);
	bool fail = false;
	readNotes(in, input, delta, Data, fail);
	if (fail == true) {
		return EXIT_FAILURE;
	}
	int j = 0;
	while (j < NUMSAMPLES) {
		out.write(reinterpret_cast<std::fstream::char_type*>(&Data[j]), sizeof Data[j]);
		j++;
	}

	return EXIT_SUCCESS;
}

void readNotes(ifstream &in, string &Input, double &delta, int16_t *& Data, bool &fail) {
	string note;
	string amp;
	string samp;
	int count = 0;
	
	double freq;
	int amplitude;
	double duration;
	double result;

	int i = 0;
	double t = 0;
	int error = 0;

	in.open(Input);
	while (!in.eof())
	{
		getline(in, note, ',');
		getline(in, amp, ',');
		getline(in, samp, '\n');
		duration = stod(samp);
		amplitude = stoi(amp);
		if (amplitude < 0) {
			fail = true;
		}
		if (duration > 0 ) 
		{
			if (note == "A" || note == "B" || note == "C" || note == "D" || note == "E" || note == "F" || note == "G") 
			{
				if (note == "A") {
					freq = 440;
				}
				else if (note == "B") {
					freq = 493.88;
				}
				else if (note == "C") {
					freq = 261.63;
				}
				else if (note == "D") {
					freq = 293.66;
				}
				else if (note == "E") {
					freq = 329.63;
				}
				else if (note == "F") {
					freq = 349.63;
				}
				else if (note == "G") {
					freq = 392;
				}
	
				error++;
				bool check = false;

				while (t < duration)
				{
					result = amplitude * sin(2 * M_PI * freq * t);
					int16_t temp = (int16_t) result;/********************************/
					Data[i] = temp;
					i++;
					if (error > 1 && amplitude == 0 && check == false) {
						t = t + delta;
						check = true;
					}
					t = t + delta;
				}
				check = false;

				count++;
				cout << count << "\n";
				t = 0;
			}
			else 
			{
				if (note == "") 
				{
					continue;
				}
				else 
				{
					fail = true;
				}
			}
		}
		else 
		{
			fail = true;
		}
	}
}

void readSamples(ifstream &in, string &Input, double &NUMSAMPLES) {
	string note;
	string amp;
	string samples;
	in.open(Input);
	while (!in.eof())
	{
		getline(in,note,',');
		getline(in, amp, ',');
		getline(in, samples, '\n');

		if (note == "") {
			continue;
		}
		else {
			double duration = stod(samples);
			if (duration > 0) {
				NUMSAMPLES = NUMSAMPLES + duration;
			}
		}
		
		cout << note << "\n" << amp << "\n" << samples << "\n";
		cout << NUMSAMPLES << "\n";
	}
	in.close();
}