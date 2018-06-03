#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

//struct to treat encoded/decoded messages as unique objects
struct morseCode{

    string msg;

    string getMsg(){
        return msg;
    }

    void setMsg(string s){
        msg = s;
    }   


    friend ostream& operator<<(ostream& output, morseCode& m){
        output << m.getMsg() << endl;
        return output;
    }

}; //end of morseCode

//forward declarations
void fillMaps(unordered_map<char, string>& e, unordered_map<string, char>& d);
morseCode encode(string& input, unordered_map<char, string>& e);
morseCode decode(string& input, unordered_map<string, char>& d);
vector<morseCode> parse(unordered_map<char, string>& e, unordered_map<string, char>& d, const string& s);
void toFile(vector<morseCode>& m);

int main(){

    
    //let's populate our key-value maps
    unordered_map<char, string> encode;
    unordered_map<string, char> decode;

    fillMaps(encode, decode);

    cout << "Welcome to Morse-code Encoder/Decoder!" << endl;
    cout << "Select input method:" << endl;
    cout << endl;
    cout << ">> file" << endl;
    cout << ">> user" << endl;
    cout << ">> exit" << endl;
    cout << endl;
    cout << "method: ";

    string userFlag;
    getline(cin, userFlag);
   
    //let's keep track of our encoded/decoded messages 
    vector<morseCode> msgs;

    do{
        //let's parse our input
        msgs = parse(encode, decode, userFlag);
        cout << endl;
        cout << "input method: ";
        getline(cin, userFlag);

    }while(userFlag != "exit");

    //let's write everything to a file before terminating
    toFile(msgs);
    cout << "Press enter to exit..." << endl;

    cin.ignore();
    return 0;
}

void fillMaps(unordered_map<char, string>& e, unordered_map<string, char>& d){

    //open filestream
    ifstream ist{"morsecode.txt"};

    //does file exist?
    if(!ist){
        cout << "Error - fillMaps(), file does not exist" << endl;
        return;
    }
    
    //get line by line input from file
    string input;
    while(getline(ist, input)){
        //break into tokens
        stringstream ss(input);
        char letter;
        string morse;
        //store key, value pairs in their respective maps
        while(ss >> letter >> morse){
            e.insert(std::pair<char, string>(letter, morse));
            d.insert(std::pair<string, char>(morse, letter));
        }        
    }

}

morseCode encode(string& input, unordered_map<char, string>& e){

    //input should be a string of letters
    //create stringstream to break into words
    stringstream ss(input);
    string msg;
    char word;
    while(ss >> word){
        //have we found a terminator?
        //if(word == '/'){
            //msg += " ";
            //continue;
        //}
        if(word == '$'){
            morseCode m;
            m.setMsg(msg);
            return m;
        }
        //use word to retrieve it's counter-part, build msg
        string tmp_str = e[word];
        msg += tmp_str;
        msg += " ";
    }

}

morseCode decode(string& input, unordered_map<string, char>& d){

    //input should be a string of characters
    //create stringstream to break into words
    stringstream ss(input);
    string msg, word;
    char spc = 32;
    while (ss >> word){
        //have we found a terminator?
        if(word == "/"){
            msg += spc;
            continue;
        }
        if(word == "$"){
            morseCode m;
            m.setMsg(msg);
            return m;
        }
        //use word to retrieve its counter-part, build msg
        char tmp_char = d[word];
        msg += string(1, tmp_char);
    }

}

vector<morseCode> parse(unordered_map<char, string>& e, unordered_map<string, char>& d, const string& userFlag){

    
    //tmp vector for objects
    vector<morseCode> msgs;

    //input method: from file

    if (userFlag == "file"){
        //open up input file
        ifstream ist{"msgs.txt"};

        //does file exist?
        if(!ist){
            cout << "Error - parse(), file does not exist" << endl;
        }

        //grab input line by line
        string input, sentence;
        while(getline(ist, input)){

            //is it a complete sentence already?
            char tmp_char = input[input.length()-2];
            if(tmp_char == '$'){
                //are we working with a previous string?
                if(sentence.length() == 0){
                    sentence = input;
                }
                else{
                    sentence += input;
                    sentence += " ";
                }
            }
            else{
                sentence += input;
                sentence += " ";
                continue;
            }

            //string is commplete, are we encoding or decoding?
            if(isalpha(sentence.at(0))){
                //encoding
                morseCode m;
                m = encode(sentence, e);
                msgs.push_back(m);
            }
            else{
                //decoding
                morseCode m;
                m = decode(sentence, d);
                msgs.push_back(m);
            }
            //remove contents of string so we don't concatenate duplicates
            sentence.clear();
        }
    
        return msgs;
    }
    else if(userFlag == "user"){

        cout << "enter input, terminate input with: $\n>";
        string input;
        getline(cin, input);

        //string is commplete, are we encoding or decoding?
        if(isalpha(input.at(0))){
            //encoding
            morseCode m;
            m = encode(input, e);
            cout << endl;
            cout << "plain-text: " << input << endl;
            cout << "cypher text: " << m << endl;
            cout << endl;
            msgs.push_back(m);
        }
        else{
            //decoding
            morseCode m;
            m = decode(input, d);
            cout << endl;
            cout << "cypher text: " << input << endl;
            cout << "plain text: " << m << endl;
            cout << endl; 
            msgs.push_back(m);
        }
        
        return msgs;

    }
}

void toFile(vector<morseCode>& m){

    //create file
    cout << "Name of file you wish to write to?\n>";
    string filename;
    getline(cin, filename);

    //intialize stream
    ofstream ost{filename};
    
    //does it exist?
    if(!ost){
        cout << "Error: toFile(), " << filename << " does not exist." << endl;
        return;
    }

    //write to it
    for(morseCode& i : m){
        ost << i;
    }

}
