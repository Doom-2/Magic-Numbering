
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
			if (mode_sel_is_active && cin.good() && val > 6 && cin.peek() == '\n') throw 4;
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
				cerr << "Некорректный ввод. Введите число из диапазона 1-6.\n" << ">";
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

//asks for user input
//checks for input of only 'y' or 'n'
//not used in the current version
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

//Counts the number of words in string
int numWords(string str)
{
	int counting = 0;
	string word;
	stringstream strm(str);

	while (strm >> word)
		++counting;
	return counting;
}

//Removes a word from the string and returns a copy of the target string
string RemoveWordFromLine(string& line, string word)
{
	// \s* : extra optional spaces at the start
	// \b : word boundary
	regex reg("\\s*\\b" + word + "\\b");
	return regex_replace(line, reg, "");
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

	const string GREETINGS_COBURG_MAIN_PRG = "Выбран режим WALDRICH COBURG основная программа\n"
		"(для смены режима перезапустите программу)\n\n"
		"Стартовый номер кадра 2\n"
		"Приращение кадра 2\n"
		"Количество разрядов 4\n"
		"Вызов подпрограммы Lxxx нумеруется\n\n";

	const string GREETINGS_COBURG_SUB_PRG = "Выбран режим WALDRICH COBURG подпрограмма\n"
		"(для смены режима перезапустите программу)\n\n"
		"Стартовый номер кадра 2\n"
		"Приращение кадра 2\n"
		"Количество разрядов 4\n"
		"Номер подпрограммы Lxxx00 не нумеруется\n\n";

	const string GREETINGS_SKODA = "Выбран режим SKODA HCW3\n"
		"(для смены режима перезапустите программу)\n\n"
		"Стартовый номер кадра 5\n"
		"Приращение кадра 5\n"
		"Количество разрядов динамическое\n"
		"Из программы удаляются:\n"
		"- ненужные строки\n"
		"- повторяющиеся G54 и M08\n"
		"- RECALC_B, TRAORI и опасное перемещение после этих команд\n"
		"Литерал \"SAFETY_Y = 4900\" заменяется на \"SAFETY_Y = 1500\"\n\n";

	const string GREETINGS_JUARISTI = "Выбран режим JUARISTI\n"
		"(для смены режима перезапустите программу)\n\n"
		"Стартовый номер кадра 5\n"
		"Приращение кадра 5\n"
		"Количество разрядов динамическое\n"
		"Из программы удаляются ненужные строки\n"
		"Литерал \"G0 G153 Y3000\" заменяется на \"G0 G153 Y1500\"\n\n";

	const string GREETINGS_MANUAL = "Выбран ручной режим\n"
		"(для смены режима перезапустите программу)\n\n"
		"Введите данные самостоятельно\n\n";

	const string GREETINGS_REMOVE_N = "Выбран режим без нумерации кадров\n"
		"(для смены режима перезапустите программу)\n\n"
		"Данные номеров кадров будут удалены\n\n";

	const string FILE_NAME_INPUT = "Введите полное имя файла, включая путь.\n"
		"Можно использовать механизм перетаскивания Drag-and-Drop\n"
		"Для выхода нажмите Esc\n\n>";

	const string SELECT_MODE = "Выберите режим:\n"
		"1. Waldrich Coburg - основная программа\n"
		"2. Waldrich Coburg - подпрограмма\n"
		"3. Skoda HCW3\n"
		"4. JUARISTI\n"
		"5. Ручной ввод\n"
		"6. Удалить нумерацию\n\n"
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
	char isCoburgSubprg = 'n';
	bool isExtraFound{};

	vector<string> extra_strings_skoda = { 	// extra words for SKODA HCW3 that should be excluded
		"Program:",
		"Machine",
		"AO KG",
		"Operation",
		"DELTA_X",
		"DELTA_Z",
		"Tool:",
		"PRED_C800",
		"GOTOF N5",
		"G0 G153 X4000",
		"ANGLE TOLERANCE",
		"Please set tool",
		"**************************",
	};

	vector<string> extra_strings_juaristi = { 	// extra words for JUARISTI that should be excluded
	"MACHINE",
	"MOSCOW",
	//"DELTA_X",
	//"DELTA_Z",
	"GOTOF N7",
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
		cout << GREETINGS_COBURG_MAIN_PRG;
		break;
	case 2:
		cout << GREETINGS_COBURG_SUB_PRG;
		break;
	case 3:
		cout << GREETINGS_SKODA;
		break;
	case 4:
		cout << GREETINGS_JUARISTI;
		break;
	case 5:
		cout << GREETINGS_MANUAL;
		break;
	case 6:
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
				isCoburgSubprg = 'n';
				break;
			case 2:
				counter = 2;
				incr = 2;
				digit_num = 4;
				isCoburgSubprg = 'y';
				break;
			case 3:
			case 4:
				counter = 5;
				incr = 5;
				digit_num = 0;
				break;
			case 5:
				cout << REQUEST_N_START;
				counter = cin_positive_int();
				cout << REQUEST_N_INCR;
				incr = cin_positive_int();
				cout << REQUEST_NUM_DIGITS;
				digit_num = cin_positive_int();
				break;
			case 6:
				counter = 2;
				incr = 2;
				digit_num = 0;
				break;

			default:
				break;
		}

		vector<string> buff; // buffer for saving data line by line

		bool isG54Found{}, isRecalcBFound{}, isTraoriFound{}, isM6Found{}, isM08Found{};

		while (getline(file, line)) // to get you all the lines.
		{
			// First, remove leading, trailing and extra spaces from line
			line = regex_replace(line, regex("^ +| +$|( ) +"), "$1");

			// Skip lines containing certain substrings for Skoda HCW3 mode
			if (mode == 3) {
				if (line.find("TRAORI") != string::npos) { isTraoriFound = true; continue; }
				if (isTraoriFound) { isTraoriFound = false; continue; };
				if (line.find("RECALC_B") != string::npos) { isRecalcBFound = true; continue; }
				if (isRecalcBFound) { isRecalcBFound = false; continue; };
				for (string extra_string : extra_strings_skoda)
					if (line.find(extra_string) != string::npos) { isExtraFound = true; break; }
				if (isExtraFound) { isExtraFound = false; continue; };
			}

			// Skip lines containing certain substrings for JUARISTI mode
			else if (mode == 4) {
				for (string extra_string : extra_strings_juaristi)
					if (line.find(extra_string) != string::npos) { isExtraFound = true; break; }
				if (isExtraFound) { isExtraFound = false; continue; };
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
			if (line.starts_with(";") && line.size() == 1) continue;
			// Skip line if it's empty
			if (line.empty()) continue;
			// Remove Nxxxx and following space from each line if it exists
			if (line.starts_with("N") && isdigit(line[1])) line = line.substr(line.find_first_of(" \t") + 1);
			// Replace some literals and delete dublicates for SKODA HCW3 mode
			if (mode == 3) {
				if (line.find("SAFETY_Y = 4900") != std::string::npos) line = "SAFETY_Y = 1500";
				if (line.find("T=") != string::npos) line = ";T=\"\"";
				if (line.find("M6") != string::npos && isM6Found) continue;
				if (line.find("M6") != string::npos && !isM6Found) isM6Found = true;
				if (line.find("G54 G17") != string::npos && isG54Found) continue;
				if (line.find("G54 G17") != string::npos && !isG54Found) isG54Found = true;
				if (line.find("G54 D1") != string::npos) line = "D1";
				if (line.find("M08") != string::npos && isM08Found)	line = RemoveWordFromLine(line, "M08");
				if (line.find("M08") != string::npos && !isM08Found) isM08Found = true;
			}
			// Replace literal "G0 G153 Y3000" to "G0 G153 Y1500" for JUARISTI mode
			if (mode == 4 && line.find("G0 G153 Y3000") != std::string::npos) line = "G0 G153 Y1500";
			// Skip line if it's a label eg. 'LABEL1:', 'NO_MOVE:', etc.)
			if (line.ends_with(":")) { buff.push_back(line); continue; }
			// Skip line starting with '%', '(' or ';'
			if (line.starts_with("%") || line.starts_with("(") || line.starts_with(";")) { buff.push_back(line); continue; }
			// Skip line starting with "L" for Waldrich Coburg subprogram
			if (isCoburgSubprg == 'y' && line.starts_with("L")) { buff.push_back(line); continue; }
			// Add frame number to the beginning of current line
			if (mode != 6) {
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
		cout << endl << "Файл " << fname << " успешно перезаписан" << endl;
		cout << string(fname.size() + 25, '^') << endl << endl;
		switch (mode)
		{
		case 1:
			cout << GREETINGS_COBURG_MAIN_PRG;
			break;
		case 2:
			cout << GREETINGS_COBURG_SUB_PRG;
			break;
		case 3:
			cout << GREETINGS_SKODA;
			break;
		case 4:
			cout << GREETINGS_JUARISTI;
			break;
		case 5:
			cout << GREETINGS_MANUAL;
			break;
		case 6:
			cout << GREETINGS_REMOVE_N;
			break;

		default:
			break;
		}
		cout << FILE_NAME_INPUT;
	}
	return 0;
}
