# dataStructure_Project2
(in linux)
command:
1. g++ -std=c++11 project1.cpp -o project1
2. ./project1 // 執行

執行結果：(example1)
=================================================================
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

example1:
-----------------------------------------------------------------
		li R1,0
		li R2,4
	Loop:
		beq R1,R2,End
		subi R2,R2,1
		beq R0,R0,Loop
	End:

系統流程：
=================================================================
	1. 讀擋
	2. 讀擋完成後將資料存成以下結構
	vector<string> loadInstName; // 紀錄每個register的名字
	vector<int> loadInstNumber; // 紀錄每個register的值

	// 將instruction分別記錄以下結構 ex: add R1,R2,R3
	// instFirst會記錄R1
	// instSecond會記錄R2
	// instThird會記錄R3
	// instName會記錄add
	vector<string> instFirst;
	vector<string> instSecond;
	vector<string> instThird;
	vector<string> loopName;
	vector<string> instName;
	vector<int> instLocation;
	vector<int> loopLocation;
	string first, second, third;
	
	struct Entry { // prediction資料儲存結構
		vector<int> outcome;
		vector<int> state; // 0:00, 1:01, 2:10, 3:11
		vector<int> st; // 0:SN, 1:WN, 2:WT, 3:ST
		int count = 0;
	};

	3. 資料儲存好以後，開始模擬組合語言執行流程，對branch去做預測
	4. 輸出結果

Function：
=================================================================

* do2BitHistroy() // 做branch prediction
* returnLoadValue() // 回傳register的值
* returnLoop() // 回傳loop的位置，以便branch到那個位置
* isExist() // 判斷register是否存在
