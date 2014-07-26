#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

using namespace std;

string strWord( int index , string line)
{
       int count = 0;
       string word;
       for ( unsigned int i = 0 ; i < line.length(); i++){
           if ( isspace(line[i]) != 0 ){
                if ( isspace(line[i+1]) == 0){
                    count ++;
                    if ( count == index) return word;
                    word ="";
                }
           }
           else
                word += line[i];
       }
       return word;
}

int main()
{
    string line;
    int cnt = 0;
    int attribover;
    vector<string> classNames;
    vector<string> extends;
    vector<string> implements;

    ifstream myfile ("sample.txt");
    if (myfile.is_open()){

    cout << "digraph g{\nnode [shape=record]\nedge [dir=back, arrowtail=empty]\n";
        while ( getline (myfile, line) ){

            while(isspace(line[0]) != 0){
                line = line.substr(1,line.length() - 1);
            }

            if ( line == "" ) continue;

            else if ( line.find("class") == 0 ){
                cout << cnt << " [label=\"{";
                cout << strWord(2,line) << "|";
                classNames.push_back(strWord(2,line));

                if( line.find("extends") != string::npos ){
                    int t = line.find("extends") + 8;
                    if( line.find("implements") != string::npos )
                        extends.push_back(line.substr(t,line.find("implements") - t - 1));
                    else
                        extends.push_back(line.substr(t,line.length() - t - 1));
                }
                else
                    extends.push_back("");

                if( line.find("implements") != string::npos ){
                    int t = line.find("implements") + 11;
                    implements.push_back(line.substr(t,line.length() - t - 1));
                }
                else
                    implements.push_back("");

                attribover = 0;
                cnt++;
            }

            else if ( line.find("interface") == 0 ){
                cout << cnt << " [label=\"{";
                cout << "\\<\\<interface\\>\\>\\n" << strWord(2,line) << "|";
                classNames.push_back(strWord(2,line));

                if( line.find("extends") != string::npos ){
                    int t = line.find("extends") + 8;
                    if( line.find("implements") != string::npos )
                        extends.push_back(line.substr(t,line.find("implements") - t - 1));
                    else
                        extends.push_back(line.substr(t,line.length() - t - 1));
                }
                else
                    extends.push_back("");

                if( line.find("implements") != string::npos ){
                    int t = line.find("implements") + 11;
                    implements.push_back(line.substr(t,line.length() - t - 1));
                }
                else
                    implements.push_back("");

                attribover = 0;
                cnt++;
            }

            else if ( line == "}" ){
                if(attribover == 0) cout << "|"; // HACK
                cout << "}\",fillcolor=\"yellow\",style=\"filled\"];\n";
            }

            else{
                line = '\t' + line;
                if ( attribover == 0 && line.find("(") != string::npos ){
                    cout << "|";
                    attribover = 1;
                }


                int pos = 3;
                if(attribover == 1){
                if( strWord(2,line).compare("public") == 0 )
                    cout << "+ ";
                else if( strWord(2,line).compare("protected") == 0 )
                    cout << "# ";
                else if( strWord(2,line).compare("private") == 0 )
                    cout << "- ";
                else
                    pos = 2;
                }

                else{
                pos = 2;
                if( strWord(2,line).compare("public") == 0 )
                    cout << "+ ";
                else if( strWord(2,line).compare("protected") == 0 )
                    cout << "# ";
                else if( strWord(2,line).compare("private") == 0 )
                    cout << "- ";
                else
                    pos = 1;
                }

                int postemp = 0;
                while(strWord(pos + postemp,line).compare("(") != 0
                   and strWord(pos + postemp,line).compare("()") != 0
                   and strWord(pos + postemp,line).compare(";") != 0)
                    postemp++;
                // pokud je navratova hodnota ... 2
                // pokud neni navratova hod.  ... 1

                cout << strWord(pos + postemp - 1,line);

                if ( attribover == 1 ){
                    if ( line.find("()") != string::npos )
                        cout << "( )";

                    else{
                        cout << "(";
                        string templine = line.substr(line.find("(") + 1, line.find(")") - line.find("(") - 1);
                        //cout << "\n(" << templine << ")\n";

                        string temp = "";
                        for ( unsigned int i = 0 ; i < templine.length(); i++){
                            if(templine[i] == ','){
                                cout << strWord(3,temp) << " : " << strWord(2,temp) << ", ";
                                temp = "";
                            }
                            else
                                temp += templine[i];
                        }
                        cout << strWord(3,temp) << " : " << strWord(2,temp) << ")";

                    }
                }

                if(postemp == 2)
                    cout << " : " << strWord(pos + postemp - 2,line);

                cout << "\\l";
            }
        }
        //myfile.close();

        for (unsigned int te = 0; te < extends.size(); te++){
            string temp = "";
            for ( unsigned int i = 0 ; i < extends[te].length(); i++){
                if(extends[te][i] == ','){
                    if( temp.compare("") != 0 )
                        for ( unsigned int tem = 0; tem < classNames.size(); tem++ )
                            if( temp == classNames[tem] )
                                cout << tem << " -> " << te << "\n";
                    temp = "";
                }
                else
                    if(extends[te][i] != ' ')
                        temp += extends[te][i];
            }
            if( temp.compare("") != 0 )
                for ( unsigned int tem = 0; tem < classNames.size(); tem++ )
                    if( temp == classNames[tem] )
                        cout << tem << " -> " << te << "\n";
        }
        for (unsigned int te = 0; te < implements.size(); te++){
            string temp = "";
            for ( unsigned int i = 0 ; i < implements[te].length(); i++){
                if(implements[te][i] == ','){
                    if( temp.compare("") != 0 )
                        for ( unsigned int tem = 0; tem < classNames.size(); tem++ )
                            if( temp == classNames[tem] )
                                cout << tem << " -> " << te << " [style = dashed]" << "\n";
                    temp = "";
                }
                else
                    if(implements[te][i] != ' ')
                        temp += implements[te][i];
            }
            if( temp.compare("") != 0 )
                for ( unsigned int tem = 0; tem < classNames.size(); tem++ )
                    if( temp == classNames[tem] )
                        cout << tem << " -> " << te << " [style = dashed]" << "\n";
        }
        cout << "}\n";
    }
    return 0;
}
