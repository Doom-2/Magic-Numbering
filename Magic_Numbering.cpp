
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
// Takes a single argument indicating that the mode selection is active
int cin_positive_int(bool mode_sel_is_active=false)
{
	int val;
	do {
		try {
			if (!(cin >> val)) throw 1;
			if (mode_sel_is_active && cin.good() && (val <= 0 || val >4) && cin.peek() == '\n') throw 4;
			if (cin.good() && val <= 0 && cin.peek() == '\n') throw 2;
			if (cin.peek() != '\n') throw 3;
		}
		catch (int i) {
			switch (i)
			{
			case 1:
			case 3:
				cerr << "Некорректный ввод. Допускается только целое число.\n" << ">";
				cin.clear();
				break;
			case 2:
				cerr << "Некорректный ввод. Целое число должно быть положительным.\n" << ">";
				cin.clear();
				break;
			case 4:
				cerr << "Некорректный ввод. Введите число из диапазона 1-4.\n" << ">";
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
	while (cin.get() != '\n');
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
			cout << "Некорректный ввод. Введите y или n." << endl << ">";
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

	const string TITLE = "**********************\n"
						  "МАГИЧЕСКАЯ НУМЕРАЦИЯ\n"
						 "**********************\n";

	const string DESCRIPTION = "Упорядочит последовательность кадров G-кода, уберет лишнее\n\n";

	const string GREETINGS_COBURG = "Выбран режим WALDRICH COBURG.\n"
		"(для смены режима перезапустите программу)\n\n"
		"Стартовый номер кадра 2\n"
		"Приращение кадра 2\n"
		"Количество разрядов 4\n\n";

	const string GREETINGS_SKODA = "Выбран режим SKODA HCW3.\n"
		"(для смены режима перезапустите программу)\n\n"
		"Стартовый номер кадра 5\n"
		"Приращение кадра 5\n"
		"Количество разрядов динамическое\n"
		"Из программы удаляются ненужные строки\n\n";

	const string GREETINGS_MANUAL = "Выбран ручной режим.\n"
		"(для смены режима перезапустите программу)\n\n"
		"Введите данные самостоятельно\n\n";

	const string GREETINGS_REMOVE_N = "Выбран режим без нумерации кадров.\n"
		"(для смены режима перезапустите программу)\n\n"
		"Данные номеров кадров будут удалены\n\n";

	const string FILE_NAME_INPUT = "Введите полное имя файла, включая путь.\n"
		"Можно использовать механизм перетаскивания Drag-and-Drop\n"
		"Для выхода нажмите Esc\n\n>";

	const string SELECT_MODE = "Выберите режим:\n"
		"1. Waldrich Coburg\n"
		"2. Skoda HCW3\n"
		"3. Ручной ввод\n"
		"4. Удалить нумерацию\n\n"
		"Введите цифру и нажмите Enter\n"
		"Для выхода нажмите Esc\n\n>";

	const string REQUEST_N_START = "\nВведите стартовый номер N-последовательности:\n>";

	const string REQUEST_N_INCR = "\nВведите инкремент:\n>";

	const string REQUEST_NUM_DIGITS = "\nВведите количество разрядов для нумерации:\n"
		"Например, для вывода N0026... введите 4, для N00026..., введите 5\n>";

	const string REQUEST_L_NUM = "\nЭто подпрограмма?\n"
		"Если да, то не будет пронумерован кадр с именем, например, L10000, L30000\n"
		"y / n ?\n>";
	ifstream file;
	ofstream outfile;
	stringstream sstream;
	string fname, line;
	int counter, incr, digit_num, mode=0;
	char is_coburg_subprg = 'n';
	bool is_extra_found{};

	vector<string> extra_strings = { 	// extra words that should be excluded
		"Machine",
		"AO KG",
		"Operation",
		"DELTA_X",
		"DELTA_Z",
		"Tool:",
		"PRED_C800",
		"RECALC_B",
		"GOTOF N5",
		"G0 G153 X4000",
		"ANGLE TOLERANCE",
		"Please set tool",
		"**************************",
	};

	cout << TITLE;
	cout << DESCRIPTION;
	cout << SELECT_MODE;
	mode = cin_positive_int(true);

	system("CLS"); // console cleanup
	cout << TITLE;

	switch (mode)
	{
	case 1:
		cout << GREETINGS_COBURG;
		break;
	case 2:
		cout << GREETINGS_SKODA;
		break;
	case 3:
		cout << GREETINGS_MANUAL;
		break;
	case 4:
		cout << GREETINGS_REMOVE_N;
		break;

	default:
		break;
	}
	cout << FILE_NAME_INPUT;
	while (getline(cin, fname))
	{
		// Exclude double quotes from the string if they exist. Need to put correct string into open() function
		if (fname == "") { cout << "Имя файла не может быть пустым.\n" << ">"; continue;}
		if (fname.front() == '"') {
			fname.erase(0, 1); // erase the first character
			fname.erase(fname.size() - 1); // erase the last character
		}
		file.open(fname);
		if (!file)
		{
			cout << "Файл не найден." << endl << ">";
			continue;
		}

		// Enter the input data to set the program behavior
		switch (mode)
		{
			case 1:
				counter = 2;
				incr = 2;
				digit_num = 4;
				cout << REQUEST_L_NUM;
				is_coburg_subprg = cin_char_y_n();
				break;
			case 2:
				counter = 5;
				incr = 5;
				digit_num = 0;
				break;
			case 3:
				cout << REQUEST_N_START;
				counter = cin_positive_int();
				cout << REQUEST_N_INCR;
				incr = cin_positive_int();
				cout << REQUEST_NUM_DIGITS;
				digit_num = cin_positive_int();
				break;
			case 4:
				counter = 2;
				incr = 2;
				digit_num = 0;
				break;

			default:
				break;
		}

		vector<string> buff; // buffer for saving data line by line

		while (getline(file, line)) // to get you all the lines.
		{
			// First, remove leading, trailing and extra spaces from line
			line = regex_replace(line, regex("^ +| +$|( ) +"), "$1");

			// Skip the lines containing certain substrings (for Skoda HCW3)
			if (mode == 2) {
				for (string extra_string : extra_strings)
					if (line.find(extra_string) != std::string::npos) { is_extra_found = true; break; };
				if (is_extra_found) { is_extra_found = false; continue; };
			}
			

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
			// Skip line if it consists only of frame number (Nxxx) or ;
			if (line.starts_with("N") && isdigit(line[1]) && numWords(line) == 1) continue;
			if (line.starts_with(";") && numWords(line) == 1) continue;
			// Skip line if it's empty
			if (line.empty()) continue;
			// Remove Nxxxx and following space from each line if it exists
			if (line.starts_with("N") && isdigit(line[1])) line = line.substr(line.find_first_of(" \t") + 1);
			// Skip line if it's a label eg. 'LABEL1:', 'NO_MOVE:', etc.)
			if (line.ends_with(":")) { buff.push_back(line); continue; }
			// Skip line starting with '%', '(' or ';'
			if (line.starts_with("%") || line.starts_with("(") || line.starts_with(";")) { buff.push_back(line); continue; }
			// Skip line starting with "L" if it is a Coburg subprogram
			if (is_coburg_subprg == 'y' && line.starts_with("L")) { buff.push_back(line); continue; }
			// Add frame number to the beginning of current line
			if (mode != 4) {
				sstream << setfill('0') << setw(digit_num) << counter;
				line = "N" + sstream.str() + " " + line;
				sstream.str(""); // clear stringstream
			}
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
		cout << TITLE;
		cout << endl << "Файл " << fname << " успешно перезаписан." << endl << endl;
		switch (mode)
		{
		case 1:
			while (cin.get() != '\n');
			cin.clear();
			cout << GREETINGS_COBURG;
			break;
		case 2:
			cout << GREETINGS_SKODA;
			break;
		case 3:
			cout << GREETINGS_MANUAL;
			break;
		case 4:
			cout << GREETINGS_REMOVE_N;
			break;

		default:
			break;
		}
		cout << FILE_NAME_INPUT;
	}
	return 0;
}
