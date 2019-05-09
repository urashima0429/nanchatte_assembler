#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <bitset>
#include <typeinfo>

using namespace std;
const char delimiter = ' ';
const int C_COMMAND = 0;
const int J_COMMAND = 1; //label
const int L_COMMAND = 2; //label
const string DC4 = bitset<4>(0).to_string();
const string DC3 = bitset<3>(0).to_string();
const int ADD = 0;
const int SUB = 1;
const int AND = 2;
const int OR  = 3;
const int XOR = 4;
const int CMP = 5;
const int MOV = 6;
//
const int SLL = 8;
const int SLR = 9;
const int SRL = 10;
const int SRA = 11;
const int IN  = 12;
const int OUT = 13;
//
const int HLT = 15;
const int LD  = 16;
const int ST  = 17;
const int LI  = 18;
//
//
//
const int B   = 22;
//
//
const int BE  = 25;
const int BLT = 26;
const int BLE = 27;
const int BNE = 28;




const map<string, int> OP_ID = {
	make_pair("ADD",ADD),
	make_pair("SUB",SUB),
	make_pair("AND",AND),
	make_pair("OR" ,OR ),
	make_pair("XOR",XOR),
	make_pair("CMP",CMP),
	make_pair("MOV",MOV),
	//
	make_pair("SLL",SLL),
	make_pair("SLR",SLR),
	make_pair("SRL",SRL),
	make_pair("SRA",SRA),
	make_pair("IN" ,IN ),
	make_pair("OUT",OUT),
	//
	make_pair("HLT",HLT),
	make_pair("LD" ,LD ),
	make_pair("ST" ,ST ),
	make_pair("LI" ,LI ),
	//
	//
	//
	make_pair("B"  ,B  ),
	//
	//
	make_pair("BE" ,BE ),
	make_pair("BLT",BLT),
	make_pair("BLE",BLE),
	make_pair("BNE",BNE),
	//
	//
	//
	//
};
const map<string, int> R_ID = {
	make_pair("R0", 0),
	make_pair("R1", 1),
	make_pair("R2", 2),
	make_pair("R3", 3),
	make_pair("R4", 4),
	make_pair("R5", 5),
	make_pair("R6", 6),
	make_pair("R7", 7),

};
const map<int, int> OP2 = {
	make_pair(LI , 0),
	make_pair(B  , 4),
};
const map<int, int> COND = {
	make_pair(BE , 0),
	make_pair(BLT, 1),
	make_pair(BLE, 2),
	make_pair(BNE, 3),
};
const map<int, int> OP3 = {
	make_pair(ADD, 0),
	make_pair(SUB, 1),
	make_pair(AND, 2),
	make_pair(OR , 3),
	make_pair(XOR, 4),
	make_pair(CMP, 5),
	make_pair(MOV, 6),
	//make_pair((r), 7),
	make_pair(SLL, 8),
	make_pair(SLR, 9),
	make_pair(SRL, 10),
	make_pair(SRA, 11),
	make_pair(IN , 12),
	make_pair(OUT, 13),
	// make_pair((r), 14),
	make_pair(HLT, 15),
};





int address = -1;
vector<string> mnimonic;
vector<string> commands;

////////////////////////////
//  Parser module
////////////////////////////
class Parser {
private:
	vector<string>::iterator itr;
	string target;
public:
	void init();
	bool hasMoreCommands();
	void advance();
	string getCommand();
	void parse();
	int commandType();
	string getLabel();
};

void Parser::init(){
	itr = commands.begin();
}
bool Parser::hasMoreCommands(){
	return itr != commands.end();
}

void Parser::advance(){
	target = *itr;
	++itr;
	++address;
};

string Parser::getCommand(){
	return target;
};

void Parser::parse(){
	// split command at space
	mnimonic.clear();
	string s;
	istringstream stream(target);
	while(getline(stream, s, delimiter)){
		mnimonic.push_back(s);
	}
};

int Parser::commandType(){
	// split command at space
	if (mnimonic[0][0] == '(') return L_COMMAND;
	int op_id = OP_ID.find(mnimonic[0])->second;
	switch(op_id){
		case ADD : 
		case SUB : 
		case AND : 
		case OR : 
		case XOR : 
		case CMP : 
		case MOV : 
		case SLL : 
		case SLR : 
		case SRL : 
		case SRA : 
		case IN : 
		case OUT : 
		case HLT : 
		case LD : 
		case ST : 
		case LI : 
			return C_COMMAND;

		case B : 
		case BE : 
		case BLT : 
		case BLE : 
		case BNE : 
			return J_COMMAND;
	}
}

string Parser::getLabel(){
	string label = mnimonic[0];
	return label.substr(1, label.length() - 2);
}


////////////////////////////
//  SymbolTable module
////////////////////////////

class SymbolTable {
private:
	map<string, int> table;

public:
	void addEntry(string symbol, int address);
	int getAddress(string symbol);
	void _print();
};

void SymbolTable::addEntry(string symbol, int address){
	table.insert(make_pair(symbol, address));
}

int SymbolTable::getAddress(string symbol){
	auto itr = table.find(symbol);
	return (itr == table.end() ? -1 : (*itr).second);
}

void SymbolTable::_print(){
	for (auto x : table){
		cout << x.second << endl;
	}
}


////////////////////////////
//  Code module
////////////////////////////

class Code {
public:
	string translate();
	string translate2(int adr);
};


string Code::translate(){
	int op_id = OP_ID.find(mnimonic[0])->second;

	switch (op_id){

		////////////////////
		// 演算／入出力命令
		////////////////////

		case ADD:
		case SUB:
		case AND:
		case OR :
		case XOR:
		case CMP:
		case MOV: {
			string op  = "11"; 
			string Rs  = bitset<3>(R_ID.find(mnimonic[2])->second).to_string();
			string Rd  = bitset<3>(R_ID.find(mnimonic[1])->second).to_string();
			string op3 = bitset<4>(OP3.find(op_id)->second).to_string();
			string d   = DC4;
			return op + Rs + Rd + op3 + d;
		}

		case SLL:
		case SLR:
		case SRL:
		case SRA: {
			string op  = "11"; 
			string Rs  = DC3;
			string Rd  = bitset<3>(R_ID.find(mnimonic[1])->second).to_string();
			string op3 = bitset<4>(OP3.find(op_id)->second).to_string();
			string d   = bitset<4>(mnimonic[2]).to_string();
			return op + Rs + Rd + op3 + d;
		}

		case IN: {
			string op  = "11"; 
			string Rs  = DC3;
			string Rd  = bitset<3>(R_ID.find(mnimonic[1])->second).to_string();
			string op3 = bitset<4>(OP3.find(op_id)->second).to_string();
			string d   = DC4;
			return op + Rs + Rd + op3 + d;
		}

		case OUT: {
			string op  = "11"; 
			string Rs  = bitset<3>(R_ID.find(mnimonic[1])->second).to_string();
			string Rd  = DC3;
			string op3 = bitset<4>(OP3.find(op_id)->second).to_string();
			string d   = DC4;
			return op + Rs + Rd + op3 + d;
		}

		case HLT: {
			string op  = "11"; 
			string Rs  = DC3;
			string Rd  = DC3;
			string op3 = bitset<4>(OP3.find(op_id)->second).to_string();
			string d   = DC4;
			return op + Rs + Rd + op3 + d;
		}


		////////////////////
		// ロード／ストア命令
		////////////////////

		case LD: {
			string op  = "00"; 
			string Ra  = mnimonic[1];
			// b, Rb <= d(Rb)
			istringstream stream(mnimonic[2]);
			string Rb, d;
			getline(stream, d , '(');
			getline(stream, Rb, ')');
			Rb = bitset<3>(Rb).to_string();
			d  = bitset<8>(d ).to_string();
			return op + Ra + Rb + d;
		}

		case ST: {
			string op  = "01"; 
			string Ra  = mnimonic[1];
			// b, Rb <= d(Rb)
			istringstream stream(mnimonic[2]);
			string Rb, d;
			getline(stream, d , '(');
			getline(stream, Rb, ')');
			Rb = bitset<3>(Rb).to_string();
			d  = bitset<8>(d ).to_string();
			return op + Ra + Rb + d;
		}

		case LI: {
			string op  = "10";
			string op2 = bitset<3>(OP2.find(op_id)->second).to_string();
			string Rb  = bitset<3>(R_ID.find(mnimonic[1])->second).to_string();
			string d   = bitset<8>(mnimonic[2]).to_string();
			return op + op2 + Rb + d;
		}

		case B: {
			string op  = "10";
			string op2 = bitset<3>(OP2.find(op_id)->second).to_string();
			string Rb  = DC3;
			string d   = bitset<8>(mnimonic[1]).to_string();
			return op + op2 + Rb + d;
		}

		case BE:
		case BLT:
		case BLE:
		case BNE: {
			string op   = "10";
			string op2  = "111";
			// cout << COND.find(op_id)->second << endl;
			string cond = bitset<3>(COND.find(op_id)->second).to_string();
			string d    = bitset<8>(mnimonic[1]).to_string();
			return op + op2 + cond + d;
		}
	}
}

string Code::translate2(int adr){
	int op_id = OP_ID.find(mnimonic[0])->second;

	switch (op_id){
		case B: {
			string op  = "10";
			string op2 = bitset<3>(OP2.find(op_id)->second).to_string();
			string Rb  = DC3;
			string d   = bitset<8>(adr).to_string();
			return op + op2 + Rb + d;
		}

		case BE:
		case BLT:
		case BLE:
		case BNE: {
			string op   = "10";
			string op2  = "111";
			cout << COND.find(op_id)->second << endl;
			string cond = bitset<3>(COND.find(op_id)->second).to_string();
			string d    = bitset<8>(adr).to_string();
			return op + op2 + cond + d;
		}
	}
}


int main() {
    // filename.asm
    string ifn, ofn;
    cin >> ifn >> ofn;
    Parser parser;
    Code code;
    SymbolTable symbolTable;
	
	ifstream ifs;
	ifs.open(ifn, ios::in);
	if(!ifs){
        cout << "failed" << endl;
    }else{
		cout << "reading " << ifn << "..." << endl;
    }
    while(!ifs.eof()){
    	string s;
    	getline(ifs, s);
    	commands.push_back(s);
    }
    ifs.close();

	ofstream ofs;
	ofs.open(ofn, ios::out);
	if(!ofs){
        cout << "failed" << endl;
    }else{
		cout << "reading " << ofn << "..." << endl;
    }

    parser.init();

    // make up symbol table
	while(parser.hasMoreCommands()){
    	parser.advance();
    	parser.parse();

    	switch (parser.commandType()){

	    	case L_COMMAND:
	    		symbolTable.addEntry(parser.getLabel(), address);
	    		break;
		}

    }
	// debug 
    // symbolTable._print();

    parser.init();
    while(parser.hasMoreCommands()){
    	parser.advance();
    	parser.parse();

    	switch (parser.commandType()){

    	// ADD ...
    	case C_COMMAND:
			ofs << code.translate() << endl;
    		break;

    	case J_COMMAND:
    		ofs << code.translate2(symbolTable.getAddress(mnimonic[0])) << endl;
    		break;
		}

    }
}