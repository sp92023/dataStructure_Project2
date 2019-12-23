# dataStructure_Project2
(in linux)
command:
1. g++ -std=c++11 project1.cpp -o project1
2. ./project1 // 執行

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
