#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <utility>
using namespace std;

// add,sub = 2 cycles
// mul = 10 cycles
// div = 40 cycles

vector<string> instFirst;
vector<string> instSecond;
vector<string> instThird;
vector<string> instName;
vector<string> regName;
vector<int> regValue;
vector<vector<string> > addRs;
vector<vector<string> > mulRs;
vector<string> rat;
string first, second, third;
int cycle = 0;
vector<string> addBuffer; // 0:RS, 1:operation, 2:result, 3:cycle, 4:tag
vector<string> mulBuffer;
vector<int> ratTag;
vector<int> addTag;
vector<int> mulTag;

void storeValue(vector<string> inputContent);
void printRF();
void printRAT();
void printRS();
bool addRSisEnough();
bool mulRSisEnough();
int returnAddRsloca();
int returnMulRsloca();
string getRFValue(string s);
int returnRATloca(string s);
string ifHaveRat(string s);
pair<bool, int> addRsisReady(int cycle);
pair<bool, int> mulRsisReady(int cycle);
void addRsValueUpdate();
void mulRsValueUpdate();
bool isFinished();
void printAll();

int main()
{
	ifstream input;
	vector<string> inputContent;
	string inp;
	input.open("example.txt", ios::in);
	if (!input) {
		cerr << "File could not be opened" << endl;
		exit(1);
	}

	while (!input.eof()) {
		while (getline(input, inp, '\n')) {
			inputContent.push_back(inp);
		}
	}

	storeValue(inputContent);
	for (int i = 0; i < regName.size(); i++) {
		regValue.push_back(i * 2);
		ratTag.push_back(-1);
	}
	vector<string> rs;
	for (int i = 0; i < 3; i++)
		rs.push_back("NUL");
	for (int i = 0; i < 3; i++) { // addRS
		addRs.push_back(rs);
		addTag.push_back(0);
	}
	for (int i = 0; i < 2; i++) { // mulRS
		mulRs.push_back(rs);
		mulTag.push_back(0);
	}
	for (int i = 0; i < regName.size(); i++) {
		rat.push_back("NUL");
	}
	for (int i = 0; i < 5; i++) {
		addBuffer.push_back("NUL");
		mulBuffer.push_back("NUL");
	}
	printAll();

	//for (int i = 0; i < instName.size(); i++) {
	int i = 0;
	while (i < instName.size()) {
		cycle++;
		bool valueUpdate = false;
		pair<bool, int> addPair, mulPair, addPair1;
		if (addBuffer[0] == "NUL") { // if buffer is empty
			addPair = addRsisReady(cycle);
		}
		else if (addBuffer[0] != "NUL") { // if buffer need to release
			if (stoi(addBuffer[3]) == cycle) {
				addRsValueUpdate();
				addPair = addRsisReady(cycle);
				valueUpdate = true;
			}
		}

		if (mulBuffer[0] == "NUL") {
			mulPair = mulRsisReady(cycle);
		}
		else if (mulBuffer[0] != "NUL") {
			if (stoi(mulBuffer[3]) == cycle) {
				mulRsValueUpdate();
				mulPair = mulRsisReady(cycle);
				addPair1 = addRsisReady(cycle);
				valueUpdate = true;
			}
		}

		if (instName[i] == "add" && addRSisEnough() == true) {
			int loca = returnAddRsloca();
			if (loca != -1) { // if inst cannot issue to RS (RS has no space to issue)
				addRs[loca][0] = "+";
				addRs[loca][1] = ifHaveRat(instSecond[i]);
				addRs[loca][2] = ifHaveRat(instThird[i]);

				string r = "RS";
				r = r + to_string(loca + 1);
				rat[returnRATloca(instFirst[i])] = r;
				ratTag[returnRATloca(instFirst[i])] = addTag[loca];

				printAll();
				i++;
			}
		}
		else if (instName[i] == "add" && addRSisEnough() == false) {
			if (addPair.first || mulPair.first || valueUpdate) {
				printAll();
			}
		}
		else if (instName[i] == "addi" && addRSisEnough() == true) {
			int loca = returnAddRsloca();
			if (loca != -1) { // if inst cannot issue to RS
				addRs[loca][0] = "+";
				addRs[loca][1] = ifHaveRat(instSecond[i]);
				addRs[loca][2] = instThird[i];

				string r = "RS";
				r = r + to_string(loca + 1);
				rat[returnRATloca(instFirst[i])] = r;
				ratTag[returnRATloca(instFirst[i])] = addTag[loca];

				printAll();
				i++;
			}
		}
		else if (instName[i] == "addi" && addRSisEnough() == false) {
			if (addPair.first || mulPair.first || valueUpdate) {
				printAll();
			}
		}
		else if (instName[i] == "sub" && addRSisEnough() == true) {
			int loca = returnAddRsloca();
			if (loca != -1) { // if inst cannot issue to RS
				addRs[loca][0] = "-";
				addRs[loca][1] = ifHaveRat(instSecond[i]);
				addRs[loca][2] = ifHaveRat(instThird[i]);

				string r = "RS";
				r = r + to_string(loca + 1);
				rat[returnRATloca(instFirst[i])] = r;
				ratTag[returnRATloca(instFirst[i])] = addTag[loca];

				printAll();
				i++;
			}
		}
		else if (instName[i] == "sub" && addRSisEnough() == false) {
			if (addPair.first || mulPair.first || valueUpdate) {
				printAll();
			}
		}
		else if (instName[i] == "subi" && addRSisEnough() == true) {
			int loca = returnAddRsloca();
			if (loca != -1) { // if inst cannot issue to RS
				addRs[loca][0] = "-";
				addRs[loca][1] = ifHaveRat(instSecond[i]);
				addRs[loca][2] = instThird[i];

				string r = "RS";
				r = r + to_string(loca + 1);
				rat[returnRATloca(instFirst[i])] = r;
				ratTag[returnRATloca(instFirst[i])] = addTag[loca];

				printAll();
				i++;
			}
		}
		else if (instName[i] == "subi" && addRSisEnough() == false) {
			if (addPair.first || mulPair.first || valueUpdate) {
				printAll();
			}
		}
		else if (instName[i] == "mul" && mulRSisEnough() == true) {
			int loca = returnMulRsloca();
			if (loca != -1) { // if inst cannot issue to RS
				mulRs[loca][0] = "x";
				mulRs[loca][1] = ifHaveRat(instSecond[i]);
				mulRs[loca][2] = ifHaveRat(instThird[i]);

				string r = "RS";
				r = r + to_string(loca + 4);
				rat[returnRATloca(instFirst[i])] = r;
				ratTag[returnRATloca(instFirst[i])] = mulTag[loca];

				printAll();
				i++;
			}
		}
		else if (instName[i] == "mul" && mulRSisEnough() == false) {
			if (addPair.first || mulPair.first || valueUpdate) {
				printAll();
			}
		}
		else if (instName[i] == "muli" && mulRSisEnough() == true) {
			int loca = returnMulRsloca();
			if (loca != -1) { // if inst cannot issue to RS
				mulRs[loca][0] = "x";
				mulRs[loca][1] = ifHaveRat(instSecond[i]);
				mulRs[loca][2] = instThird[i];

				string r = "RS";
				r = r + to_string(loca + 4);
				rat[returnRATloca(instFirst[i])] = r;
				ratTag[returnRATloca(instFirst[i])] = mulTag[loca];

				printAll();
				i++;
			}
		}
		else if (instName[i] == "muli" && mulRSisEnough() == false) {
			if (addPair.first || mulPair.first || valueUpdate) {
				printAll();
			}
		}
		else if (instName[i] == "div" && mulRSisEnough() == true) {
			int loca = returnMulRsloca();
			if (loca != -1) { // if inst cannot issue to RS
				mulRs[loca][0] = "/";
				mulRs[loca][1] = ifHaveRat(instSecond[i]);
				mulRs[loca][2] = ifHaveRat(instThird[i]);

				string r = "RS";
				r = r + to_string(loca + 4);
				rat[returnRATloca(instFirst[i])] = r;
				ratTag[returnRATloca(instFirst[i])] = mulTag[loca];

				printAll();
				i++;
			}
		}
		else if (instName[i] == "div" && mulRSisEnough() == false) {
			if (addPair.first || mulPair.first || valueUpdate) {
				printAll();
			}
		}
		else if (instName[i] == "divi" && mulRSisEnough() == true) {
			int loca = returnMulRsloca();
			if (loca != -1) { // if inst cannot issue to RS
				mulRs[loca][0] = "/";
				mulRs[loca][1] = ifHaveRat(instSecond[i]);
				mulRs[loca][2] = instThird[i];

				string r = "RS";
				r = r + to_string(loca + 4);
				rat[returnRATloca(instFirst[i])] = r;
				ratTag[returnRATloca(instFirst[i])] = mulTag[loca];

				printAll();
				i++;
			}
		}
		else if (instName[i] == "divi" && mulRSisEnough() == false) {
			if (addPair.first || mulPair.first || valueUpdate) {
				printAll();
			}
		}
		if (addPair.first) {
			for (int j = 0; j < 3; j++) {
				addRs[addPair.second][j] = "NUL";
			}
		}
		if (addPair1.first) {
			for (int j = 0; j < 3; j++) {
				addRs[addPair1.second][j] = "NUL";
			}
		}
		if (mulPair.first) {
			for (int j = 0; j < 3; j++) {
				mulRs[mulPair.second][j] = "NUL";
			}
		}

		//printAll();
	}

	bool finished = false;
	//bool finished = true;
	while (!finished) {
		cycle++;
		pair<bool, int> addPair, mulPair, addPair1;
		if (addBuffer[0] == "NUL") { // if buffer is empty
			addPair = addRsisReady(cycle);
			if (addPair.first) {
				for (int j = 0; j < 3; j++) {
					addRs[addPair.second][j] = "NUL";
				}
				printAll();
			}
		}
		else if (addBuffer[0] != "NUL") { // if buffer need to release
			if (stoi(addBuffer[3]) == cycle) {
				addRsValueUpdate();
				addPair = addRsisReady(cycle);
				if (addPair.first) {
					for (int j = 0; j < 3; j++) {
						addRs[addPair.second][j] = "NUL";
					}
				}
				printAll();
			}
		}

		if (mulBuffer[0] == "NUL") {
			mulPair = mulRsisReady(cycle);
			if (mulPair.first) {
				for (int j = 0; j < 3; j++) {
					mulRs[mulPair.second][j] = "NUL";
				}
				printAll();
			}
		}
		else if (mulBuffer[0] != "NUL") {
			if (stoi(mulBuffer[3]) == cycle) {
				mulRsValueUpdate();
				mulPair = mulRsisReady(cycle);
				if (mulPair.first) {
					for (int j = 0; j < 3; j++) {
						mulRs[mulPair.second][j] = "NUL";
					}
				}

				addPair1 = addRsisReady(cycle);
				if (addPair1.first) {
					for (int j = 0; j < 3; j++) {
						addRs[addPair1.second][j] = "NUL";
					}
				}
				printAll();
			}
		}

		//printAll();
		finished = isFinished();
	}

	system("pause");
}

void printAll() {
	printRF();
	printRAT();
	printRS();
}

bool isFinished() {
	bool finish = true;
	for (int i = 0; i < addRs.size(); i++) {
		for (int j = 0; j < addRs[i].size(); j++) {
			if (addRs[i][j] != "NUL") {
				finish = false;
			}
		}
	}

	for (int i = 0; i < mulRs.size(); i++) {
		for (int j = 0; j < mulRs[i].size(); j++) {
			if (mulRs[i][j] != "NUL") {
				finish = false;
			}
		}
	}

	if (addBuffer[0] != "NUL") {
		finish = false;
	}
	if (mulBuffer[0] != "NUL") {
		finish = false;
	}
	for (int i = 0; i < rat.size(); i++) {
		if (rat[i] != "NUL") {
			finish = false;
		}
	}

	return finish;
}

void addRsValueUpdate() {
	for (int i = 0; i < rat.size(); i++) {
		if (addBuffer[0] == rat[i] && ratTag[i] == stoi(addBuffer[4])) {
			regValue[i] = stoi(addBuffer[2]);
			rat[i] = "NUL";
		}
	}

	for (int i = 0; i < addRs.size(); i++) {
		for (int j = 1; j < addRs[i].size(); j++) {
			if (addBuffer[0] == addRs[i][j]) {
				addRs[i][j] = addBuffer[2];
			}
		}
	}

	for (int i = 0; i < mulRs.size(); i++) {
		for (int j = 1; j < mulRs[i].size(); j++) {
			if (addBuffer[0] == mulRs[i][j]) {
				mulRs[i][j] = addBuffer[2];
			}
		}
	}

	for (int i = 0; i < addBuffer.size(); i++) {
		addBuffer[i] = "NUL";
	}
}

void mulRsValueUpdate() {
	for (int i = 0; i < rat.size(); i++) {
		if (mulBuffer[0] == rat[i] && ratTag[i] == stoi(mulBuffer[4])) {
			regValue[i] = stoi(mulBuffer[2]);
			rat[i] = "NUL";
		}
	}

	for (int i = 0; i < addRs.size(); i++) {
		for (int j = 1; j < addRs[i].size(); j++) {
			if (mulBuffer[0] == addRs[i][j]) {
				addRs[i][j] = mulBuffer[2];
			}
		}
	}

	for (int i = 0; i < mulRs.size(); i++) {
		for (int j = 1; j < mulRs[i].size(); j++) {
			if (mulBuffer[0] == mulRs[i][j]) {
				mulRs[i][j] = mulBuffer[2];
			}
		}
	}

	for (int i = 0; i < mulBuffer.size(); i++) {
		mulBuffer[i] = "NUL";
	}
}

string ifHaveRat(string s) {
	for (int i = 0; i < rat.size(); i++) {
		if (s == regName[i]) {
			if (rat[i] != "NUL") {
				return rat[i];
			}
			else {
				return getRFValue(s);
			}
		}
	}
}

pair<bool, int> addRsisReady(int cycle) {
	pair<bool, int> isReady;
	for (int i = 0; i < addRs.size(); i++) {
		if (addRs[i][0] != "NUL") {
			if (addRs[i][1][0] != 'R'&&addRs[i][2][0] != 'R') {
				isReady.first = true;
				string s = "RS";
				s = s + to_string(i + 1);
				string ss;
				ss = addRs[i][1] + addRs[i][0] + addRs[i][2];
				string sss;
				if (addRs[i][0] == "+") {
					sss = to_string(stoi(addRs[i][1]) + stoi(addRs[i][2]));
				}
				else if (addRs[i][0] == "-") {
					sss = to_string(stoi(addRs[i][1]) - stoi(addRs[i][2]));
				}
				addBuffer[0] = s;
				addBuffer[1] = ss;
				addBuffer[2] = sss;
				addBuffer[3] = to_string(cycle + 2);
				addBuffer[4] = to_string(addTag[i]);
				addTag[i]++;

				//for (int j = 0; j < 3; j++){
				//	addRs[i][j] = "NUL";
				//}
				isReady.second = i;
				break;
			}
		}
	}

	return isReady;
}

pair<bool, int> mulRsisReady(int cycle) {
	pair<bool, int> isReady;
	for (int i = 0; i < mulRs.size(); i++) {
		if (mulRs[i][0] != "NUL") {
			if (mulRs[i][1][0] != 'R'&&mulRs[i][2][0] != 'R') {
				isReady.first = true;
				string s = "RS";
				s = s + to_string(i + 4);
				string ss;
				ss = mulRs[i][1] + mulRs[i][0] + mulRs[i][2];
				string sss;
				if (mulRs[i][0] == "x") {
					sss = to_string(stoi(mulRs[i][1]) * stoi(mulRs[i][2]));
					mulBuffer[3] = to_string(cycle + 10);
				}
				else if (mulRs[i][0] == "/") {
					sss = to_string(stoi(mulRs[i][1]) / stoi(mulRs[i][2]));
					mulBuffer[3] = to_string(cycle + 40);
				}
				mulBuffer[0] = s;
				mulBuffer[1] = ss;
				mulBuffer[2] = sss;
				mulBuffer[4] = to_string(mulTag[i]);
				mulTag[i]++;

				//for (int j = 0; j < 3; j++){
				//	mulRs[i][j] = "NUL";
				//}
				isReady.second = i;
				break;
			}
		}
	}

	return isReady;
}

int returnRATloca(string s) {
	for (int i = 0; i < regName.size(); i++) {
		if (s == regName[i]) {
			return i;
		}
	}
}

string getRFValue(string s) {
	for (int i = 0; i < regName.size(); i++) {
		if (s == regName[i]) {
			return to_string(regValue[i]);
		}
	}
}

int returnAddRsloca() {
	int loca = -1;
	for (int i = 0; i < addRs.size(); i++) {
		if (addRs[i][0] == "NUL") {
			loca = i;
			break;
		}
	}

	return loca;
}

int returnMulRsloca() {
	int loca = -1;
	for (int i = 0; i < mulRs.size(); i++) {
		if (mulRs[i][0] == "NUL") {
			loca = i;
			break;
		}
	}

	return loca;
}

bool addRSisEnough() {
	bool isEnough = false;
	for (int i = 0; i < addRs.size(); i++) {
		if (addRs[i][0] == "NUL") {
			isEnough = true;
		}
	}

	return isEnough;
}

bool mulRSisEnough() {
	bool isEnough = false;
	for (int i = 0; i < mulRs.size(); i++) {
		if (mulRs[i][0] == "NUL") {
			isEnough = true;
		}
	}

	return isEnough;
}

void printRF() {
	cout << "=================================" << endl;
	cout << "cycle:" << setw(3) << cycle << endl << endl;

	cout << "RF" << endl;
	for (int i = 0; i < regName.size(); i++) {
		cout << regName[i] << ":" << setw(4) << regValue[i] << endl;
	}
	cout << endl;
}

void printRAT() {
	cout << "RAT" << endl;
	for (int i = 0; i < rat.size(); i++) {
		cout << regName[i] << ":" << setw(4) << rat[i] << setw(4) << ratTag[i] << endl;
	}
	cout << endl;
}

void printRS() {
	cout << "ADD_RS" << endl;
	for (int i = 0; i < addRs.size(); i++) {
		cout << "RS" << setw(2) << i + 1 << ":";
		for (int j = 0; j < addRs[i].size(); j++) {
			cout << setw(4) << addRs[i][j];
		}
		cout << setw(4) << addTag[i];
		cout << endl;
	}
	//cout << endl;

	cout << "BUFFER:  ";
	for (int i = 0; i < addBuffer.size() - 2; i++) {
		cout << addBuffer[i] << " ";
	}
	cout << addBuffer[4];
	cout << endl << "release cycle:" << setw(3) << addBuffer[3] << endl;
	cout << endl << endl;

	cout << "MUL_RS" << endl;
	for (int i = 0; i < mulRs.size(); i++) {
		cout << "RS" << setw(2) << i + 4 << ":";
		for (int j = 0; j < mulRs[i].size(); j++) {
			cout << setw(4) << mulRs[i][j];
		}
		cout << setw(4) << mulTag[i];
		cout << endl;
	}
	//cout << endl;

	cout << "BUFFER:  ";
	for (int i = 0; i < mulBuffer.size() - 2; i++) {
		cout << mulBuffer[i] << " ";
	}
	cout << mulBuffer[4];
	cout << endl << "release cycle:" << setw(3) << mulBuffer[3] << endl;
	cout << endl;
	cout << "=================================" << endl << endl << endl;
}

void storeValue(vector<string> inputContent) {
	for (int i = 0; i < inputContent.size(); i++) {
		for (int j = 0; j < inputContent[i].size(); j++) {
			if (inputContent[i][j] >= 'a'&&inputContent[i][j] <= 'z') {
				string inst;
				int l = 0;
				for (int k = j; k < inputContent[i].size(); k++) {
					if (inputContent[i][k] == ' ') {
						//continue;
						l = ++k;
						break;
					}
					inst.push_back(inputContent[i][k]);
				}
				//cout << inst << endl;
				if (inst == "addi") {
					for (; l < inputContent[i].size(); l++) {
						if (inputContent[i][l] == ',') {
							for (int m = ++l; m < inputContent[i].size(); m++) {
								if (inputContent[i][m] == ',') {
									for (int n = ++m; n < inputContent[i].size(); n++) {
										third.push_back(inputContent[i][n]);
									}
									instThird.push_back(third);
									if (atoi(third.c_str()) == 0)
										regName.push_back(third);
									third.clear();
									m = inputContent[i].size();
									break;
								}
								second.push_back(inputContent[i][m]);
							}
							instSecond.push_back(second);
							if (atoi(third.c_str()) == 0)
								regName.push_back(second);
							second.clear();
							l = inputContent[i].size();
							break;
						}
						first.push_back(inputContent[i][l]);
					}
					instFirst.push_back(first);
					if (atoi(third.c_str()) == 0)
						regName.push_back(first);
					instName.push_back(inst);
					first.clear();
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
					sort(regName.begin(), regName.end());
					regName.erase(unique(regName.begin(), regName.end()), regName.end());
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
				}
				else if (inst == "add") {
					for (; l < inputContent[i].size(); l++) {
						if (inputContent[i][l] == ',') {
							for (int m = ++l; m < inputContent[i].size(); m++) {
								if (inputContent[i][m] == ',') {
									for (int n = ++m; n < inputContent[i].size(); n++) {
										third.push_back(inputContent[i][n]);
									}
									instThird.push_back(third);
									if (atoi(third.c_str()) == 0)
										regName.push_back(third);
									third.clear();
									m = inputContent[i].size();
									break;
								}
								second.push_back(inputContent[i][m]);
							}
							instSecond.push_back(second);
							if (atoi(third.c_str()) == 0)
								regName.push_back(second);
							second.clear();
							l = inputContent[i].size();
							break;
						}
						first.push_back(inputContent[i][l]);
					}
					instFirst.push_back(first);
					if (atoi(third.c_str()) == 0)
						regName.push_back(first);
					instName.push_back(inst);
					first.clear();
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
					sort(regName.begin(), regName.end());
					regName.erase(unique(regName.begin(), regName.end()), regName.end());
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
				}
				else if (inst == "sub") {
					for (; l < inputContent[i].size(); l++) {
						if (inputContent[i][l] == ',') {
							for (int m = ++l; m < inputContent[i].size(); m++) {
								if (inputContent[i][m] == ',') {
									for (int n = ++m; n < inputContent[i].size(); n++) {
										third.push_back(inputContent[i][n]);
									}
									instThird.push_back(third);
									if (atoi(third.c_str()) == 0)
										regName.push_back(third);
									third.clear();
									m = inputContent[i].size();
									break;
								}
								second.push_back(inputContent[i][m]);
							}
							instSecond.push_back(second);
							if (atoi(third.c_str()) == 0)
								regName.push_back(second);
							second.clear();
							l = inputContent[i].size();
							break;
						}
						first.push_back(inputContent[i][l]);
					}
					instFirst.push_back(first);
					if (atoi(third.c_str()) == 0)
						regName.push_back(first);
					instName.push_back(inst);
					first.clear();
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
					sort(regName.begin(), regName.end());
					regName.erase(unique(regName.begin(), regName.end()), regName.end());
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
				}
				else if (inst == "subi") {
					for (; l < inputContent[i].size(); l++) {
						if (inputContent[i][l] == ',') {
							for (int m = ++l; m < inputContent[i].size(); m++) {
								if (inputContent[i][m] == ',') {
									for (int n = ++m; n < inputContent[i].size(); n++) {
										third.push_back(inputContent[i][n]);
									}
									instThird.push_back(third);
									if (atoi(third.c_str()) == 0)
										regName.push_back(third);
									third.clear();
									m = inputContent[i].size();
									break;
								}
								second.push_back(inputContent[i][m]);
							}
							instSecond.push_back(second);
							if (atoi(third.c_str()) == 0)
								regName.push_back(second);
							second.clear();
							l = inputContent[i].size();
							break;
						}
						first.push_back(inputContent[i][l]);
					}
					instFirst.push_back(first);
					if (atoi(third.c_str()) == 0)
						regName.push_back(first);
					instName.push_back(inst);
					first.clear();
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
					sort(regName.begin(), regName.end());
					regName.erase(unique(regName.begin(), regName.end()), regName.end());
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
				}
				else if (inst == "mul") {
					for (; l < inputContent[i].size(); l++) {
						if (inputContent[i][l] == ',') {
							for (int m = ++l; m < inputContent[i].size(); m++) {
								if (inputContent[i][m] == ',') {
									for (int n = ++m; n < inputContent[i].size(); n++) {
										third.push_back(inputContent[i][n]);
									}
									instThird.push_back(third);
									if (atoi(third.c_str()) == 0)
										regName.push_back(third);
									third.clear();
									m = inputContent[i].size();
									break;
								}
								second.push_back(inputContent[i][m]);
							}
							instSecond.push_back(second);
							if (atoi(third.c_str()) == 0)
								regName.push_back(second);
							second.clear();
							l = inputContent[i].size();
							break;
						}
						first.push_back(inputContent[i][l]);
					}
					instFirst.push_back(first);
					if (atoi(third.c_str()) == 0)
						regName.push_back(first);
					instName.push_back(inst);
					first.clear();
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
					sort(regName.begin(), regName.end());
					regName.erase(unique(regName.begin(), regName.end()), regName.end());
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
				}
				else if (inst == "muli") {
					for (; l < inputContent[i].size(); l++) {
						if (inputContent[i][l] == ',') {
							for (int m = ++l; m < inputContent[i].size(); m++) {
								if (inputContent[i][m] == ',') {
									for (int n = ++m; n < inputContent[i].size(); n++) {
										third.push_back(inputContent[i][n]);
									}
									instThird.push_back(third);
									if (atoi(third.c_str()) == 0)
										regName.push_back(third);
									third.clear();
									m = inputContent[i].size();
									break;
								}
								second.push_back(inputContent[i][m]);
							}
							instSecond.push_back(second);
							if (atoi(third.c_str()) == 0)
								regName.push_back(second);
							second.clear();
							l = inputContent[i].size();
							break;
						}
						first.push_back(inputContent[i][l]);
					}
					instFirst.push_back(first);
					if (atoi(third.c_str()) == 0)
						regName.push_back(first);
					instName.push_back(inst);
					first.clear();
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
					sort(regName.begin(), regName.end());
					regName.erase(unique(regName.begin(), regName.end()), regName.end());
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
				}
				else if (inst == "div") {
					for (; l < inputContent[i].size(); l++) {
						if (inputContent[i][l] == ',') {
							for (int m = ++l; m < inputContent[i].size(); m++) {
								if (inputContent[i][m] == ',') {
									for (int n = ++m; n < inputContent[i].size(); n++) {
										third.push_back(inputContent[i][n]);
									}
									instThird.push_back(third);
									if (atoi(third.c_str()) == 0)
										regName.push_back(third);
									third.clear();
									m = inputContent[i].size();
									break;
								}
								second.push_back(inputContent[i][m]);
							}
							instSecond.push_back(second);
							if (atoi(third.c_str()) == 0)
								regName.push_back(second);
							second.clear();
							l = inputContent[i].size();
							break;
						}
						first.push_back(inputContent[i][l]);
					}
					instFirst.push_back(first);
					if (atoi(third.c_str()) == 0)
						regName.push_back(first);
					instName.push_back(inst);
					first.clear();
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
					sort(regName.begin(), regName.end());
					regName.erase(unique(regName.begin(), regName.end()), regName.end());
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
				}
				else if (inst == "divi") {
					for (; l < inputContent[i].size(); l++) {
						if (inputContent[i][l] == ',') {
							for (int m = ++l; m < inputContent[i].size(); m++) {
								if (inputContent[i][m] == ',') {
									for (int n = ++m; n < inputContent[i].size(); n++) {
										third.push_back(inputContent[i][n]);
									}
									instThird.push_back(third);
									if (atoi(third.c_str()) == 0)
										regName.push_back(third);
									third.clear();
									m = inputContent[i].size();
									break;
								}
								second.push_back(inputContent[i][m]);
							}
							instSecond.push_back(second);
							if (atoi(third.c_str()) == 0)
								regName.push_back(second);
							second.clear();
							l = inputContent[i].size();
							break;
						}
						first.push_back(inputContent[i][l]);
					}
					instFirst.push_back(first);
					if (atoi(third.c_str()) == 0)
						regName.push_back(first);
					instName.push_back(inst);
					first.clear();
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
					sort(regName.begin(), regName.end());
					regName.erase(unique(regName.begin(), regName.end()), regName.end());
					//for (int m = 0; m < regName.size(); m++) {
					//	cout << regName[m] << " ";
					//}
					//cout << endl;
				}
				inst.clear();
				break;
			}
		}
	}
}
