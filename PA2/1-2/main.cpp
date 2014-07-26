#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

void addToVector(string course, string name, vector<string> & list)
{
	//cout << course << ": " << name << '\n';
	list.push_back(course + ": " + name);
}

bool nalnum(char foo)
{
	return !(isalnum(foo) || foo == ' ' || foo == ':' || foo == ',');
}

bool transform  ( const char * inFile,
                  const char * outFile )
{
   	// todo
	ifstream inFS;
	string line;
	vector<string> list;
	int sepInit, sep1, sep2;
	unsigned int i;
	
	inFS.open(inFile);
	
	if(!inFS || inFS.fail())
		return false;
	
	if (inFS.is_open()){
		while(getline(inFS,line)){
			if(line.substr(0,1) != "#" && line != ""){
				//kontrola validity
				if(find_if(line.begin(), line.end(), nalnum) != line.end())
					return false;//kontrola alfanumericity
				
				sep1 = line.substr(0,line.find(":")).find(" ");
				while(sep1 != -1){
					if((line.substr(sep1+1, 1) != " " && line.substr(sep1+1, 1) != ":") &&
					   (sep1 != 0 && (line.substr(sep1-1, 1) != " " /*&& line.substr(sep1-1, 1) != ":")*/)))
						return false;
					sep1 = line.substr(0,line.find(":")).find(" ", sep1+1);
				} //jmeno rozdelene mezerou
				sep1 = line.find(" ",line.find(":"));
				while(sep1 != -1){
					if((line.substr(sep1+1, 1) != " " && line.substr(sep1+1, 1) != ",") &&
					   (line.substr(sep1-1, 1) != " " && line.substr(sep1-1, 1) != ":" && line.substr(sep1-1, 1) != ","))
						return false;
					sep1 = line.find(" ", sep1+1);
				} //predmet rozdeleny mezerou
				if(line.find("#") != string::npos)
					return false; // spatne umisteny sharp
				
				line.erase(remove(line.begin(), line.end(),' '), line.end());
				sepInit = sep1 = line.find(":");
				if(sepInit == -1) return false;
				//cout << line << '\n';
				
				//kontrola validity
				if(line.find(":") != line.length()-1 && line.substr(line.find(":")+1, line.length()-1).find(":") != string::npos)
					return false; //vice dvojtecek v radku
				if(line.substr(0, line.find(":")).find(",") != string::npos)
					return false; //carka ve jmene
				
					
				
				
				
				//kontrola duplicit I.
				for(i=0;i<list.size();i++){
					//cout << "i-ty " << list.at(i).substr(0,list.at(i).find(":")) << '\n';
					//cout << "current " << line.substr(0, line.find(":")) << '\n';
				
					//if(list.at(i).substr(0,list.at(i).find(":")) == line.substr(0, line.find(":")))
					if(list.at(i).find(line.substr(0, line.find(":"))) != string::npos)
					/*unsigned int test = list.at(i).find(line.substr(0, line.find(":")));
					if(test != string::npos && list.at(i).substr(test - 1,1) == " " && (((test + line.substr(0, line.find(":")).length()) == list.at(i).length()) || (list.at(i).substr((test + line.substr(0, line.find(":")).length() + 1),1)) == ","))*/
						return false;
					// STACK LIMIT
					
				}
				
				while(1){
					sep2 = line.find(",",sep1+1);
					
					//kontrola validity
					if((unsigned)(sep2+1) == line.length() || line.substr(sep2+1,1) == ",")
						return false;
					
					if(sep2 == -1){
						//cout << line.substr(sep1+1, line.length()-sep1-1) << '\n';
					
						addToVector(line.substr(sep1+1, line.length()-sep1-1), line.substr(0, sepInit), list);	
												
						break;
					}
					//cout << '[' << line.substr(sep1+1, sep2-sep1-1) << ']' << '\n';
					
					addToVector(line.substr(sep1+1, sep2-sep1-1), line.substr(0, sepInit), list);
						
					sep1 = sep2;
				}				
			}
		}
		inFS.close();
		
		sort(list.begin(), list.end());
		i = list.size();
			
		//kontrola prazdnoty reloaded
		for(i=0;i<list.size();i++)
			//if(list.at(i).find(":") == 0)
				//return false;
			if(list.at(i).find(":") == list.at(i).length()-2)
				return false;
		
		
		//kontrola duplicit II.
		list.erase(unique(list.begin(), list.end()), list.end());
		if(i != list.size())
			return false;
		
		//for(i=0;i<list.size();i++)
		//	cout << list.at(i) << '\n';

		ofstream outFS;
		outFS.open(outFile);
		
		if(!outFS.is_open())
			return false;
		
		//kontrola prazdnoty
		if(i == 0){
			outFS.close();
			return true;
		}
		
		addToVector("","",list);
		
		line = list.at(0);
		
		for(i=1;i<list.size();i++){
			if(list.at(i-1).substr(0,list.at(i-1).find(":")) == list.at(i).substr(0,list.at(i).find(":")))
				line = line + "," + list.at(i).substr(list.at(i).find(":")+1, list.at(i).length()-1);
			else{
				if(line.find(":") != 0)
					outFS << line << '\n';
				line = list.at(i);
				if(i == list.size() - 1)
					if(line.find(":") != 0)
						outFS << line << '\n';
			}
		}
		
		outFS.close();
		if(outFS.fail())
			return false;
		return true;
 	}	
	else{
		return false;		
	}
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
   	// tests
	cout << transform("tests/in_0.txt","out_0.txt") << '\n';
	cout << transform("tests/in_1.txt","out_1.txt") << '\n';
	cout << transform("tests/in_2.txt","out_2.txt") << '\n';
	cout << transform("tests/in_3.txt","out_3.txt") << '\n';
	cout << "__________________" << '\n';
   	cout << '0' << transform("tests_extended/1.txt","tests_extended/out_1.txt") << '\n';
   	cout << '0' << transform("tests_extended/2.txt","tests_extended/out_2.txt") << '\n';
   	cout << '0' << transform("tests_extended/3.txt","tests_extended/out_3.txt") << '\n';
   	cout << '0' << transform("tests_extended/4.txt","tests_extended/out_4.txt") << '\n';
   	cout << '0' << transform("tests_extended/5.txt","tests_extended/out_5.txt") << '\n';
   	cout << '0' << transform("tests_extended/6.txt","tests_extended/out_6.txt") << '\n';
   	cout << '0' << transform("tests_extended/7.txt","tests_extended/out_7.txt") << '\n';
   	cout << '0' << transform("tests_extended/8.txt","tests_extended/out_8.txt") << '\n';
   	cout << '0' << transform("tests_extended/9.txt","tests_extended/out_9.txt") << '\n';
   	cout << '1' << transform("tests_extended/10.txt","tests_extended/out_10.txt") << '\n';
   	cout << '0' << transform("tests_extended/11.txt","tests_extended/out_11.txt") << '\n';
   	cout << '0' << transform("tests_extended/12.txt","tests_extended/out_12.txt") << '\n';
   	cout << '0' << transform("tests_extended/13.txt","tests_extended/out_13.txt") << '\n';
   	cout << '0' << transform("tests_extended/14.txt","tests_extended/out_14.txt") << '\n';
   	cout << '0' << transform("tests_extended/15.txt","tests_extended/out_15.txt") << '\n';
   	cout << '0' << transform("tests_extended/16.txt","tests_extended/out_16.txt") << '\n';
   	cout << '1' << transform("tests_extended/17.txt","tests_extended/out_17.txt") << '\n';
   	cout << '1' << transform("tests_extended/18.txt","tests_extended/out_18.txt") << '\n';
   	cout << '0' << transform("tests_extended/19.txt","tests_extended/out_19.txt") << '\n';
   	cout << '1' << transform("tests_extended/20.txt","tests_extended/out_20.txt") << '\n';
   	cout << '1' << transform("tests_hardcore/data.txt","tests_hardcore/result_out.txt") << '\n';
   	return 0;
}
#endif /* __PROGTEST__ */
