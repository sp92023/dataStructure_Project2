# dataStructure_Project2
## command:
	(in linux)
	1. g++ -std=c++11 project2.cpp -o project2
	2. ./project2 // 執行

## 執行結果：(example)
	cycle:  0
	
	RF
	F1:   0
	F2:   2
	F3:   4
	F4:   6
	F5:   8

	RAT
	F1: NUL  -1
	F2: NUL  -1
 	F3: NUL  -1
	F4: NUL  -1
	F5: NUL  -1

	ADD_RS
	RS 1: NUL NUL NUL   0
	RS 2: NUL NUL NUL   0
	RS 3: NUL NUL NUL   0
	BUFFER:  NUL NUL NUL NUL
	release cycle:NUL

	
	MUL_RS
	RS 4: NUL NUL NUL   0
	RS 5: NUL NUL NUL   0
	BUFFER:  NUL NUL NUL NUL
	release cycle:NUL

=================================
 	
	cycle:  1

	RF
	F1:   0
	F2:   2
	F3:   4
	F4:   6
	F5:   8

	RAT
	F1: RS1   0
	F2: NUL  -1
	F3: NUL  -1
	F4: NUL  -1
	F5: NUL  -1

	ADD_RS
	RS 1:   +   2   1   0
	RS 2: NUL NUL NUL   0
	RS 3: NUL NUL NUL   0
	BUFFER:  NUL NUL NUL NUL
	release cycle:NUL


	MUL_RS
	RS 4: NUL NUL NUL   0
	RS 5: NUL NUL NUL   0
	BUFFER:  NUL NUL NUL NUL
	release cycle:NUL
	
=================================
	
	cycle: 66

	RF
	F1:   4
	F2:  30
	F3:   4
	F4:   2
	F5:  64

	RAT
	F1: NUL   0
	F2: NUL   1
	F3: NUL  -1
	F4: NUL   1
	F5: NUL   1

	ADD_RS
	RS 1: NUL NUL NUL   2
	RS 2: NUL NUL NUL   2
	RS 3: NUL NUL NUL   1
	BUFFER:  NUL NUL NUL NUL
	release cycle:NUL


	MUL_RS
	RS 4: NUL NUL NUL   2
	RS 5: NUL NUL NUL   1
	BUFFER:  NUL NUL NUL NUL
	release cycle:NUL

### example:
	addi F1,F2,1
	sub F1,F3,F4
	div F1,F2,F3
	mul F2,F3,F4
	add F2,F4,F2
	addi F4,F1,2
	mul F5,F5,F5
	add F1,F4,F4

## 系統流程：
	1. 讀擋
	2. 讀擋完成後將資料存成以下結構
	// 將instruction分別記錄以下結構 ex: add R1,R2,R3
	// instFirst會記錄R1
	// instSecond會記錄R2
	// instThird會記錄R3
	// instName會記錄add
	vector<string> instFirst;
	vector<string> instSecond;
	vector<string> instThird;
	vector<string> instName;
	
	vector<string> regName; // 紀錄每個register的名字
	vector<int> regValue; // 紀錄每個register的值
	vector<vector<string> > addRs;
	vector<vector<string> > mulRs;
	vector<string> rat;
	string first, second, third;
	int cycle = 0;
	vector<string> addBuffer; // 0:RS, 1:operation, 2:result, 3:cycle, 4:tag
	// buffer內存五個值，第一個存RAT所對應的RS，第二個存式子，第三個存執行後的結果，第四個存在第幾個cycle會write back，第五個存tag
	vector<string> mulBuffer;
	vector<int> ratTag;
	vector<int> addTag;
	vector<int> mulTag;
	
	3. 資料儲存好以後，開始執行tomasulo
	4. 輸出結果

## Function：

* bool addRSisEnough(), bool mulRSisEnough() // 判斷RS裡面是否有空間可以issue
* int returnAddRsloca(), int returnMulRsloca() // 當RS裡面有空間可以issue時，回傳所要儲存的location資訊
* pair<bool, int> addRsisReady(), pair<bool, int> mulRsisReady() // 判斷RS裡面，是否可以做dispatch(沒有RSX在RS裡面)，如果有已經準備完成的RS，回傳true並回傳RS location
* void addRsValueUpdate(), void mulRsValueUpdate() // buffer釋出後，更新RAT和RS裡面的資訊
* string ifHaveRat() // 當RAT裡面有值，拿取RAT裡面資訊，否則直接使用RF裡面的值

#### 讀檔：
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
					...
				}
				inst.clear();
				break;
			}
		}
	}
	storeValue()
	資料紀錄方式如上所述
