#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
string FileName = "MyFile.txt";
enum eProcesses{Show = 1,Add = 2,Delete = 3,UpdateInfo = 4,FindClient = 5,Transaction = 6,Exit = 7};
struct stClientInfo{
    string AccountNumber,Name,Pin,Age,PhoneNumber;
    int AccountBalance;
    bool MarkForDelete = false;
};
string Readstring(string Message){
    string S1;
    cout << Message << endl;
    getline(cin >> ws, S1);
    return S1;
}
char ReadCharacter(string Message){
    char c1;
    cout << Message << endl;
    cin >> c1;
    return c1;
}
vector<string> Split(string S1,string diameter=" "){
    string sWord;
    int Counter=0;
    vector<string> vString;
    int pos=0;
    while((pos = S1.find(diameter)) != std::string::npos){
        sWord= S1.substr(0,pos);
        if(sWord != ""){
            vString.push_back(sWord);
        }
        S1.erase(0,pos+diameter.length());
    }
    if(S1 != ""){
        vString.push_back(S1);
    }
    return vString;
}
string MakeRecordLine(stClientInfo Info,string Delim){
    string S1 = Info.AccountNumber + Delim + Info.Name + Delim + Info.Age + 
        Delim + Info.PhoneNumber + Delim + Info.Pin + Delim + to_string(Info.AccountBalance);
    return S1;
}
stClientInfo DecryptDataLine(string DataLine, string Delim = "#//#"){
    vector<string> vStrings;
    stClientInfo Info;
    vStrings = Split(DataLine,Delim);
    Info.AccountNumber = vStrings[0];
    Info.Name = vStrings[1];
    Info.Age = vStrings[2];
    Info.PhoneNumber = vStrings[3];
    Info.Pin = vStrings[4];
    Info.AccountBalance = stoi(vStrings[5]);
    return Info;
}
vector<stClientInfo> ReadData(string FileName){
    fstream MyFile;
    vector<stClientInfo> Clients;
    stClientInfo Data;
    MyFile.open(FileName,ios::in);
    if(MyFile.is_open()){
        string sa;
        while(getline(MyFile,sa)){
            Data = DecryptDataLine(sa);
            Clients.push_back(Data);
        }
    }
    return Clients;
}
void FillFileWithData(string FileName,vector<stClientInfo> Data){
    fstream File;
    File.open(FileName,ios::out);
    if(File.is_open()){
        for(stClientInfo &Client: Data){
            File << MakeRecordLine(Client,"#//#") << endl;
        }
        File.close();
    }
}
void PrintScreen(){
    cout << "===========================================" << endl;
    cout << "               Main Screen                 " << endl;
    cout << "===========================================" << endl;
    cout << setw(10) << "[1]Show Client List." << endl;
    cout << setw(10) << "[2]Add Client." << endl;
    cout << setw(10) << "[3]Delete Client." << endl;
    cout << setw(10) << "[4]Update Client." << endl;
    cout << setw(10) << "[5]Find Client." << endl;
    cout << setw(10) << "[6]Transaction." << endl;
    cout << setw(10) << "[7]Exit." << endl;
    cout << "===========================================" << endl;
}
void PrintClientInfo(stClientInfo Client){
    cout << setw(16) << std::left << "Account Number: " << Client.AccountNumber << endl;
    cout << setw(16) << std::left <<"Name: " << Client.Name << endl;
    cout << setw(16) << std::left << "Age: " << Client.Age << endl;
    cout << setw(16) << std::left << "Phone Number: " << Client.PhoneNumber << endl;
    cout << setw(16) << std::left << "Pin: " << Client.Pin << endl;
    cout << setw(16) << std::left << "Account Balance: " << Client.AccountBalance << endl;
}
void ShowClientList(vector<stClientInfo> Clients){
    short Counter = 1;
    for(stClientInfo &Client: Clients){
        cout << "Client " << Counter << " :" << endl;
        PrintClientInfo(Client);
        cout << endl << endl;
        Counter++;
    }
}
bool ClientExists(string AccountNumber,string FileName){
    fstream File;
    File.open(FileName,ios::in);
    if(File.is_open()){
        stClientInfo Client;
        string sa;
        while(getline(File,sa)){
            Client = DecryptDataLine(sa);
            if(Client.AccountNumber == AccountNumber){
                File.close();
                return true;
            }
        }
        File.close();
    }
    return false;
}
stClientInfo FillInfo(){
    stClientInfo Info;
    cout << "Please Enter Your Account Number: " << endl;
    getline(cin >> ws,Info.AccountNumber);
    while(ClientExists(Info.AccountNumber,FileName)){
        cout << "The Account Number you entered already exists" << endl;
        getline(cin >> ws, Info.AccountNumber);
    }
    cout << "Please Enter Your Name: " << endl;
    getline(cin,Info.Name);
    cout << "Please Enter Your Pin: " << endl;
    cin >> Info.Pin;
    cin.ignore(); // clear input buffer
    cout << "Please Enter Your PhoneNumber: " << endl;
    getline(cin,Info.PhoneNumber);
    cout << "Please Enter Your Age: " << endl;
    cin >> Info.Age;
    cout << "Enter Your Account Balance: " << endl;
    cin >> Info.AccountBalance;
    return Info;
}
void UpdateClientInfo(stClientInfo& Info){
    cout << "Please Enter Your Name: " << endl;
    getline(cin >> ws,Info.Name);
    cout << "Please Enter Your Pin: " << endl;
    cin >> Info.Pin;
    cin.ignore(); // clear input buffer
    cout << "Please Enter Your PhoneNumber: " << endl;
    getline(cin,Info.PhoneNumber);
    cout << "Enter Your Account Balance: " << endl;
    cin >> Info.AccountBalance;
}
void AddClient(stClientInfo Client,vector<stClientInfo> & Clients){
    Clients.push_back(Client);
    FillFileWithData(FileName,Clients);
}
bool FindUser(vector<stClientInfo> Clients,string AccountNumber){
    for(stClientInfo&Client: Clients){
        if(Client.AccountNumber == AccountNumber){
            cout << "We Found User !"  << endl << endl;
            PrintClientInfo(Client);
            return true;
        }
    }
    return false;
}
void MarkForDelete(vector<stClientInfo>&Clients,string AccountNumber){
    for(stClientInfo &Client:Clients){
        if(Client.AccountNumber == AccountNumber){
            Client.MarkForDelete = true;
        }
    }
}
void DeleteAccount(string FileName,vector<stClientInfo> &Clients){
    fstream File;
    File.open(FileName,ios::trunc | ios::out);
    if(File.is_open()){
        for(stClientInfo Client:Clients){
            if(Client.MarkForDelete != true){
                File << MakeRecordLine(Client,"#//#") << endl;
            }
        }
        Clients = ReadData(FileName);
        File.close();
    }
}
void UpdateClient(vector<stClientInfo>&Clients , string AccountNumber){
    for(stClientInfo &Client : Clients){
        if(Client.AccountNumber == AccountNumber){
            UpdateClientInfo(Client);
        }
    }
    FillFileWithData(FileName,Clients);
}
void TransactionMode(vector<stClientInfo> &Clients,string AccountSender, string AccountReciver){
    short Amount;
    cout << "How Much Amount Do you Need to send: " << endl;
    cin >> Amount;
    for(stClientInfo &Client:Clients){
        if(Client.AccountNumber == AccountSender){
            Client.AccountBalance = Client.AccountBalance - Amount;
        }
        if(Client.AccountNumber == AccountReciver){
            Client.AccountBalance = Client.AccountBalance + Amount;
        }
    }
    FillFileWithData(FileName,Clients);
}
eProcesses ChooseMode()
{   
    int Number;
    PrintScreen();
    cout << "Choose The Operation From [1-7]. " << endl;
    cin >> Number;
    
    return (eProcesses)Number;
}
void StartProcess(){
    eProcesses Mode;
    vector<stClientInfo> Clients = ReadData(FileName);
    stClientInfo ClientInfo;
    do{
        system("cls");
        Mode = ChooseMode();
        switch (Mode)
        {
        case eProcesses::Transaction:
        {
            system("cls");
            string AccountSender = Readstring("Enter your account number: ");
            string AccountReciver = Readstring("Enter the account number you want to send the money: ");
            TransactionMode(Clients,AccountSender,AccountReciver);
            system("pause");
            break;
        }
        case eProcesses::Show:
            system("cls");
            ShowClientList(Clients);
            system("pause");
            break;
        case eProcesses::Add:
        {
            system("cls");
            char Confirm = ReadCharacter("Are you sure you want to add an account ? ");
            if(Confirm == 'Y' || Confirm == 'y'){
                ClientInfo = FillInfo();
                AddClient(ClientInfo,Clients);
            }
            system("pause");
            break;
        }
        case eProcesses::FindClient:
        {
            system("cls");
            string AccountNumber = Readstring("Enter The Account Number: ");
            if(FindUser(Clients,AccountNumber)){
                break;
            }else{
                cout << "We Couldn't find the user with the AccountNumber" << endl;
                break;
            }
            system("pause");
            break;
        }
        case eProcesses::Delete:
        {
            system("cls");
            string AccountNumber = Readstring("Enter The Account Number: ") ;
                if(FindUser(Clients,AccountNumber)){
                    char Confirm = ReadCharacter("Are you Sure you want to delete this client ? ");
                    if(Confirm == 'y' || Confirm == 'Y')
                    {
                        MarkForDelete(Clients,AccountNumber);
                        DeleteAccount(FileName,Clients);
                    }
                    cout << "Deleted Account successfuly !" << endl;
                }else{
                    cout << "We couldn't find a client with that account number" << endl;
                }
                system("pause");
            break;
        }
        case eProcesses::UpdateInfo:
        {
            system("cls");
            string AccountNumber = Readstring("Enter The Account Number: ");
            if(FindUser(Clients,AccountNumber)){
                char Confirm = ReadCharacter("Are you Sure you want to update this client ? ");
                if(Confirm == 'Y' || Confirm == 'y'){
                    UpdateClient(Clients,AccountNumber);
                    cout << "Updated The Client Sucessfuly !" << endl;
                }
            }else{
                cout << "We couldn't find a client with that account number" << endl;
            }
            system("pause");
        }
        
    }
}while( Mode != eProcesses::Exit);}
/// @brief 
/// @return 

int main(){
    StartProcess();
    return 0;
}