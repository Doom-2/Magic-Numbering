
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <regex>
#include <Windows.h>
#include <conio.h>
using namespace std;

DWORD WINAPI CheckEscape(LPVOID lpParam) {
	while (GetAsyncKeyState(VK_ESCAPE) == 0) {
		//sleep 
		Sleep(500);
	}
	exit(0);

}

// Checks for positive integers, throws an error othewise.
// Returns correct integer.
int cin_positive_int()
{
	int val;
	do {
		try {
			if (!(cin >> val)) throw 1;
			if (cin.good() && val <= 0 && cin.peek() == '\n') throw 2;
			if (cin.peek() != '\n') throw 3;
		}
		catch (int i) {
			switch (i)
			{
			case 1:
			case 3:
				cerr << "������������ ����. ����������� ������ ����� �����.\n" << ">";
				cin.clear();
				break;
			case 2:
				cerr << "������������ ����. ����� ����� ������ ���� �������������.\n" << ">";
				cin.clear();
				break;
			default:
				break;
			}
			while (cin.get() != '\n');
			continue;
		}
		break;
	} while (true);
	return val;
}

char cin_char_y_n() {
	char ch;
	while (1)
	{
		cin >> ch;
		if (ch == 'y' || ch == 'n') break;
		else {
			while (cin.get() != '\n');
			cin.clear();
			cout << "������������ ����. ������� y ��� n." << endl << ">";
		}
	}
	return ch;
}

int numWords(string wordnum)
{
	int counting = 0;
	string word;
	stringstream strm(wordnum);

	while (strm >> word)
		++counting;
	return counting;
}

int main()
{
	CreateThread(NULL, 0, CheckEscape, NULL, 0, NULL);

	SetConsoleCP(1251); // set the win-cp 1251 code page in the input stream
	SetConsoleOutputCP(1251); // set the win-cp 1251 code page in the output stream

	const string GREETINGS = "������� ������ ��� �����, ������� ����.\n"
							"����� ������������ �������� �������������� Drag-and-Drop\n"
							"��� ������ �� ��������� ������� Esc\n\n>";

	const string REQUEST_N_START = "\n������� ��������� ����� N-������������������:\n>";

	const string REQUEST_N_INCR = "\n������� ��������:\n>";

	const string REQUEST_NUM_DIGITS = "\n������� ���������� �������� ��� ���������:\n"
									"��������, ��� ������ N0026... ������� 4, ��� N00026..., ������� 5\n>";

	const string REQUEST_L_NUM = "\n��������� �� ��������� ������ � ������� �����������, ��������, L10016, L10000?\n"
								"���������: ��� ������������, ������� 'n', ��� ������� ��������� - 'y'\n"
								"���� ����� ����� ���, ������� ����� �� �������� 'y' ��� 'n' (��� �������)\n"
								"y / n ?\n>";
	ifstream file;
	ofstream outfile;
	stringstream sstream;
	string fname, line;
	int counter, incr, digit_num;
	char apply_to_L;

	// Enter the input data to set the program behavior
	cout << "**********************\n ���������� ���������\n**********************\n\n";
	cout << GREETINGS;
	while (getline(cin, fname))
	{
		// Exclude double quotes from the string if they exist. Need to put correct string into open() function
		if (fname == "") { cout << "��� ����� �� ����� ���� ������.\n" << ">"; continue;}
		if (fname.front() == '"') {
			fname.erase(0, 1); // erase the first character
			fname.erase(fname.size() - 1); // erase the last character
		}
		file.open(fname);
		if (!file)
		{
			cout << "���� �� ������." << endl << ">";
			continue;
		}

		// enter the input data
		cout << REQUEST_N_START;
		counter = cin_positive_int();
		cout << REQUEST_N_INCR;
		incr = cin_positive_int();
		cout << REQUEST_NUM_DIGITS;
		digit_num = cin_positive_int();
		cout << REQUEST_L_NUM;
		apply_to_L = cin_char_y_n();
		vector<string> buff; // initialize buffer for saving data line by line

		while (getline(file, line)) // to get you all the lines.
		{
			// First remove leading, trailing and extra spaces from line
			line = regex_replace(line, regex("^ +| +$|( ) +"), "$1");
			// Find line wich contains NxxxGxx/Mxx without space and remove frame number (Nxxx) from it
			try
			{
				if (line.starts_with("N") && isdigit(line[1]) && line.substr(line.find_first_not_of("0123456789", 1), 1) != " ")
					line = line.substr(line.find_first_not_of("0123456789", 1));
			}
			catch (out_of_range& ex)
			{
				continue;
			}
			// Skip line if it consists only of frame number (Nxxx)
			if (line.starts_with("N") && isdigit(line[1]) && numWords(line) == 1) continue;
			// Skip line if it is empty
			if (line.empty()) continue;
			// Remove Nxxxx and following space from each line if it exists
			if (line.starts_with("N") && isdigit(line[1])) line = line.substr(line.find_first_of(" \t") + 1);
			// Skip line starts with '%', '(' or ';'
			if (line.starts_with("%") || line.starts_with("(") || line.starts_with(";")) { buff.push_back(line); continue; }
			// Skip line starts with 'L' if needed
			if (apply_to_L == 'n' && line.starts_with("L")) { buff.push_back(line); continue; }

			sstream << setfill('0') << setw(digit_num) << counter;
			line = "N" + sstream.str() + " " + line;
			sstream.str(""); // clear stringstream
			buff.push_back(line); // push each modified line in vector object
			counter += incr;
		}

		file.close();
		// Open the same file and line by line write the data from vector into it
		outfile.open(fname);
		for (string line : buff)
			outfile << line << endl;
		outfile.close();
		system("CLS"); // console cleanup
		cout << endl << "���� " << fname << " ������� �����������." << endl << endl;
		while (cin.get() != '\n');
		cin.clear();
		cout << GREETINGS;
	}
	return 0;
}
