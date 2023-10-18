#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
const string FileName = "Clients.txt";
const string UserFileName = "Users.txt";
enum eProcesses{Show = 1,Add = 2,Delete = 3,UpdateInfo = 4,FindClient = 5,Transaction = 6, ManageUsers = 7,Exit = 8};
enum eManageUsers {ShowUser = 1, AddUser = 2, DeleteUser = 3, UpdateUser = 4, FindUserin = 5 , MainMenuUser = 6};
enum eTransactions{Deposit = 1,WithDraw = 2,Balance = 3,MainMenu = 4};
struct stClientInfo{
    string AccountNumber,Name,Pin,Age,PhoneNumber;
    int AccountBalance;
    bool MarkForDelete = false;
};
const short add = 2;
const short show = 4;
const short update = 8;
const short deletes = 16;
const short manage = 32;
const short finds = 64;
const short transactions = 128;
struct stPermissions{
    bool Add=false,Show=false,Update=false,Find=false,Manage=false,Delete=false,Transactions=false;
};
struct stUser{
    string UserName,Password;
    short permissions = 0;
    stPermissions Perms;
    bool Login = false;
    bool DeleteMark = false;;
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
short CalculatePermissions(stUser User){
    short num = 0;
    short Counter = 0;
    if(User.Perms.Add){
        Counter++;
        num+2;
    }
    if(User.Perms.Delete){
        num+16;
        Counter++;
    }
    if(User.Perms.Show){
        num+4;
        Counter++;
    }
    if(User.Perms.Find){
        num+64;
        Counter++;
    }
    if(User.Perms.Manage){
        num+32;
        Counter++;
    }
    if(User.Perms.Transactions){
        num+128;
        Counter++;
    }{
    if(User.Perms.Update)
        num+8;
        Counter++;
    }
    if (Counter == 7){
        num = -1;
    }
    return num;
}
string MakeUserRecordLine(stUser Info,string Delim){
    string S1 = Info.UserName + Delim + Info.Password + Delim + to_string(Info.permissions);
    return S1;
}
eManageUsers ChooseModeUsers(){
    short Choice;
    cout << "Choose from [1-6]: ";
    cin >> Choice;
    return (eManageUsers)Choice; 
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
void FillFileWithUsersData(string FileName,vector<stUser> Data){
    fstream File;
    File.open(FileName,ios::out);
    if(File.is_open()){
        for(stUser &User: Data){
            File << MakeUserRecordLine(User,"#//#") << endl;
        }
        File.close();
    }
}
stUser Login(vector<stUser> Users){
    string sa;
    stUser User;
    User.Login = false;
    cout << "Enter Username: " ;
    cin >> sa;
    for(stUser &User : Users){
        if(User.UserName == sa){
            cout << "Enter password: ";
            cin >> sa;
            if(sa == User.Password){
                User.Login = true;
                return User;
            }
        }
    }
    return User;
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
    cout << setw(10) << "[7]Manage Users." << endl;
    cout << setw(10) << "[8]Log out." << endl;
    cout << "===========================================" << endl;
}
void PrintNoPermission(){
    system("cls");
    cout << "===========================================" << endl;
    cout << "               Declined,                 " << endl;
    cout << "           Contact your Adminstator     " << endl;
    cout << "===========================================" << endl;
    system("pause>0");
}
void PrintManageScreen(){
    cout << "===========================================" << endl;
    cout << "               Manage Users                 " << endl;
    cout << "===========================================" << endl;
    cout << setw(10) << "[1]Show Users." << endl;
    cout << setw(10) << "[2]Add User." << endl;
    cout << setw(10) << "[3]Delete User." << endl;
    cout << setw(10) << "[4]Update User." << endl;
    cout << setw(10) << "[5]Find User." << endl;
    cout << setw(10) << "[6]MainMenu." << endl;
    cout << "===========================================" << endl;
}
void PrintTransactionMenu(){
    cout << "===========================================" << endl;
    cout << "               Transactions                 " << endl;
    cout << "===========================================" << endl;
    cout << setw(10) << left <<"[1]Depoist." << endl;
    cout << setw(10) << left << "[2]WithDraw." << endl;
    cout << setw(10) << left << "[3]Balances." << endl; 
    cout << setw(10) << left << "[4]Main Menu." << endl;
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
void AddUsers(stUser User,vector<stUser> & Users){
    Users.push_back(User);
    FillFileWithUsersData(UserFileName,Users);
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
void Deposits(vector<stClientInfo> &Clients,string AccountNumber){
    short Amount ;
    char Confirmation;
    cout << "Enter the deposit amount: " << endl;
    cin >> Amount;
    for(stClientInfo &Client:Clients){
        if(Client.AccountNumber == AccountNumber){
            Confirmation = ReadCharacter("Are you sure you want to depoist ? ");
            if(Confirmation == 'y' || Confirmation == 'Y'){
                Client.AccountBalance += Amount;
            }
        }
    }
    FillFileWithData(FileName,Clients);
    Clients = ReadData(FileName);
}
void WithDraws(vector<stClientInfo> &Clients,string AccountNumber){
    short Amount ;
    char Confirmation;
    cout << "Enter the WithDrawel amount: " << endl;
    cin >> Amount;
    for(stClientInfo &Client:Clients){
        if(Client.AccountNumber == AccountNumber){
            Confirmation = ReadCharacter("Are you sure to you want to withdraw ? ");
            if(Confirmation == 'y' || Confirmation == 'Y'){
                Client.AccountBalance = Client.AccountBalance - Amount;
            }
        }
    }
    FillFileWithData(FileName,Clients);
    Clients = ReadData(FileName);
}
void PrintBalances(vector<stClientInfo> Clients){
    cout << "=========================================================================================" << endl;
    cout << setw(25) << left << "|" << "Name" << "|" << setw(25) << left << "|" << "AccountBalance " << "|" << endl;
    cout << "=========================================================================================" << endl;
    for(stClientInfo &Client: Clients){
        cout << setw(25) << left << Client.Name << setw(25 ) << left << Client.AccountBalance << endl;
    }
}
eProcesses ChooseMode()
{   
    int Number;
    PrintScreen();
    cout << "Choose The Operation From [1-7]. " << endl;
    cin >> Number;
    
    return (eProcesses)Number;
}
eTransactions ChooseTransactionMode(){
    short Choice;
    cout << "Choose Between [1-4]: " << endl;
    cin >> Choice;
    return (eTransactions)Choice;
}
stUser DecryptUserDataLine(string DataLine, string Delim = "#//#"){
    vector<string> vStrings;
    stUser Info;
    vStrings = Split(DataLine,Delim);
    Info.UserName = vStrings.at(0);
    Info.Password = vStrings.at(1);
    Info.permissions = stoi(vStrings.at(2));
    return Info;
}
vector<stUser> ReadUsersData(string FileName){
    fstream MyFile;
    vector<stUser> Users;
    stUser User;
    MyFile.open(FileName, ios::in);
    if(MyFile.is_open()){
        string sa;
        while(getline(MyFile,sa)){
            User = DecryptUserDataLine(sa);
            Users.push_back(User);
        }
    }
    return Users;
}
stUser UserFillInfo(){
    stUser User;
    int Number =0;
    cout << "Enter Username: " << endl;
    getline(cin >> ws, User.UserName);
    cout << "Enter Password: " << endl;
    getline(cin, User.Password);
    cout << endl << endl;
    char Confirmation = ReadCharacter("Do you want to give him admin ? ");
    if(Confirmation == 'Y' || Confirmation == 'y'){
        User.permissions = -1;
    }else{
        Confirmation = ReadCharacter("Can he show clients ? ");
        if(Confirmation == 'y'){
            Number += show;
        }
        Confirmation = ReadCharacter("can he add clients ? ");
        if(Confirmation == 'y'){
            Number += add;
        }
        Confirmation = ReadCharacter("can he find clients ? ");
        if(Confirmation == 'y'){
            Number += finds;
        }
        Confirmation = ReadCharacter("can he update clients ? ");
        if(Confirmation == 'y'){
            Number += update;
        }
        Confirmation = ReadCharacter("can he delete clients ? ");
        if(Confirmation == 'y'){
            Number += deletes;
        }
        Confirmation = ReadCharacter("can he manage users ? ");
        if(Confirmation == 'y'){
            Number += manage;
        }
        Confirmation = ReadCharacter("can he do transactions ? ");
        if(Confirmation == 'y'){
            Number += transactions;
        }
    }
    User.permissions = Number;
    return User;
}
void CalculatePermission(stUser &User){
    if(User.permissions == -1){
        User.Perms.Add = true;
        User.Perms.Delete = true;
        User.Perms.Update = true;
        User.Perms.Find = true;
        User.Perms.Show = true;
        User.Perms.Transactions = true;
        User.Perms.Manage = true;
    }else{
        if(!!(User.permissions & add)){
            User.Perms.Add = true;
        }
        if(!!(User.permissions & deletes)){
            User.Perms.Delete = true;
        }
        if(!!(User.permissions & update)){
            User.Perms.Update = true;
        }
        if(!!(User.permissions & finds)){
            User.Perms.Find = true;
        }
        if(!!(User.permissions & show)){
            User.Perms.Show = true;
        }
        if(!!(User.permissions & transactions)){
            User.Perms.Transactions = true;
        }
        if(!!(User.permissions & manage)){
            User.Perms.Manage = true;
        }
    }
}
void ShowUsers(vector<stUser> Users){
    short Counter =0;
    for(stUser &User: Users){
        cout << "User Number " << Counter+1;
        cout << "UserName: " << User.UserName << endl;
        Counter++;
        cout << "Password: " << User.Password << endl;
        cout << "Permission Number: " << User.permissions  << endl;
    }
}
void MarkUserforDelete(vector<stUser> &Users,string Username){
    for(stUser &User: Users){
        if(User.UserName == Username){
            User.DeleteMark = true;
            break;
        }
    }
}
void PrintDatatoFile(string Filename,vector<stUser> Users){
    fstream file;
    file.open(Filename, ios::out);
    if(file.is_open()){
        for(stUser &User: Users){
            if(User.DeleteMark != true){
                file << MakeUserRecordLine(User,"#//#") << endl;
            }
        }
        file.close();
    }
}
void UpdateInfos(stUser &User){
    int Number = 0;
    int counter = 0;
    cout << "Enter new password: ";
    cin >> User.Password;
    char Confirmation = ReadCharacter("Do you wanna give him full Permission");
    if(Confirmation == 'Y' || Confirmation == 'y'){
        User.permissions = -1;
    }else{
        Confirmation = ReadCharacter("Can he show clients ? ");
        if(Confirmation == 'y'){
            Number += show;
            counter++;
        }
        Confirmation = ReadCharacter("can he add clients ? ");
        if(Confirmation == 'y'){
            Number += add;
            counter++;
        }
        Confirmation = ReadCharacter("can he find clients ? ");
        if(Confirmation == 'y'){
            Number += finds;
            counter++;
        }
        Confirmation = ReadCharacter("can he update clients ? ");
        if(Confirmation == 'y'){
            Number += update;
            counter++;
        }
        Confirmation = ReadCharacter("can he delete clients ? ");
        if(Confirmation == 'y'){
            Number += deletes;
            counter++;
        }
        Confirmation = ReadCharacter("can he manage users ? ");
        if(Confirmation == 'y'){
            Number += manage;
            counter++;
        }
        Confirmation = ReadCharacter("can he do transactions ? ");
        if(Confirmation == 'y'){
            Number += transactions;
            counter++;
        }
    }
    if(counter == 7){
        Number = -1;
    }
    User.permissions = Number;
}
void UpdateUsers(vector<stUser> &Users, string Username){
    for(stUser &User: Users){
        if(User.UserName == Username){
            UpdateInfos(User);
            break;
        }
    }
}
stUser FindUsers(vector<stUser> Users,string Username){
    stUser User;
    for(stUser &User: Users){
        if(User.UserName == Username){
            cout << "Found User! " << endl;
            return User;
        }
    }
    return User;
}
void PrintInfoForUser(stUser User){
    cout << "Username: " << User.UserName << endl;
    cout << "Password: " << User.Password << endl;
    cout << "Permissions: " << User.permissions << endl;
}
void StartProcess(){
    eProcesses Mode;
    eManageUsers Process;
    vector<stUser> Users = ReadUsersData(UserFileName);
    stUser User;
    vector<stClientInfo> Clients = ReadData(FileName);
    stClientInfo ClientInfo;
    while(true){
        system("cls");
        stUser LoginedUser = Login(Users);
        CalculatePermission(LoginedUser);
        if(LoginedUser.Login){
        
            MainMenus:do{
            
            system("cls");

            Mode = ChooseMode();
            switch (Mode)
            {
            case eProcesses::Transaction:
            {
                if(LoginedUser.Perms.Transactions){
                do{
                system("cls");
                PrintTransactionMenu();
                eTransactions TransMode = ChooseTransactionMode();
                switch(TransMode){
                    
                    case eTransactions::Deposit:
                    {
                        system("cls");
                        string AccountNumber = Readstring("Enter your account number: ");
                        if(FindUser(Clients,AccountNumber)){
                            Deposits(Clients,AccountNumber);
                        }else{
                            cout << "We couldn't find an account with that number.. ";
                            system("pause");
                        }
                        system("pause");
                        break;
                    }
                    
                    case eTransactions::WithDraw:
                    {
                        system("cls");
                        string AccountNumber = Readstring("Enter your account number: ");
                        if(FindUser(Clients,AccountNumber)){
                            WithDraws(Clients,AccountNumber);
                        }else{
                            cout << "We couldn't find an account with number.. " << endl;
                            system("pause");
                        }
                        system("pause");
                        break;
                    }
                    case eTransactions::Balance:
                    {
                        system("cls");
                        PrintBalances(Clients);
                        system("pause");
                        break;
                    }
                    case eTransactions::MainMenu:
                    {
                        system("cls");
                        goto MainMenus;
                        break;
                    }
                    }while(true);
                }
                
                }else{
                    PrintNoPermission();
                    
                }
                break;
            }
            case eProcesses::Show:
                if(LoginedUser.Perms.Show){
                    system("cls");
                    ShowClientList(Clients);
                    system("pause");
                    
                }else{
                    PrintNoPermission();
                }
                break;
            case eProcesses::ManageUsers:
            {
                if(LoginedUser.Perms.Manage){
                    do{
                        system("cls");
                        PrintManageScreen();
                        eManageUsers Process = ChooseModeUsers();
                        switch (Process)
                        {
                        case eManageUsers::AddUser:
                        {
                            system("cls");
                            char Confirmation = ReadCharacter("Are you sure you want to add a user: ");
                            if(Confirmation == 'Y' || Confirmation == 'y'){
                                User = UserFillInfo();
                                AddUsers(User,Users);
                            }
                            break;
                        }
                        case eManageUsers::ShowUser:
                        {
                            system("cls");
                            ShowUsers(Users);
                            system("pause>0");
                            break;
                        }
                        case eManageUsers::DeleteUser:
                        {
                            system("cls");
                            string Username = Readstring("Enter the username of the user you want to delete");
                            if(Username == "Admin"){
                                cout << "You cannot Delete Admin" << endl;
                                break;
                            }else{
                                MarkUserforDelete(Users,Username);
                                PrintDatatoFile(UserFileName,Users);
                                Users = ReadUsersData(UserFileName);
                                cout << "Deleted user Successfuly" << endl;
                                system("pause>0");
                            }
                            break;
                        }
                        case eManageUsers::UpdateUser:
                        {
                            system("cls");
                            string Username = Readstring("Enter the username of the user you want to edit: ");
                            if(Username == "Admin"){
                                cout << "You cannot edit the Admin user" << endl;
                                break;
                            }else{
                                UpdateUsers(Users,Username);
                                PrintDatatoFile(UserFileName,Users);
                                Users = ReadUsersData(UserFileName);
                                cout << "Edited User Sucessfuly" << endl;
                                system("pause>0");
                            }
                            break;
                        }
                        case eManageUsers::FindUserin:
                        {
                            system("cls");
                            string Username = Readstring("Enter the Username of he user you want to find: ");
                            User = FindUsers(Users,Username);
                            PrintInfoForUser(User);
                            system("pause>0");
                            break;
                        }
                        case eManageUsers::MainMenuUser:
                        {
                            goto MainMenus;
                            break;
                        }
                    }
                    }while(Process != eManageUsers::MainMenuUser);
                }else{
                    PrintNoPermission();
                }
                break;
            }
            case eProcesses::Add:
            {
                if(LoginedUser.Perms.Add){
                system("cls");
                char Confirm = ReadCharacter("Are you sure you want to add an account ? ");
                if(Confirm == 'Y' || Confirm == 'y'){
                    ClientInfo = FillInfo();
                    AddClient(ClientInfo,Clients);
                }else{
                    break;
                }
                system("pause");
                
                }else{
                    PrintNoPermission();
                }
                break;
            }
            case eProcesses::FindClient:
            {
                if(LoginedUser.Perms.Find){
                system("cls");
                string AccountNumber = Readstring("Enter The Account Number: ");
                if(FindUser(Clients,AccountNumber)){
                }else{
                    cout << "We Couldn't find the user with the AccountNumber" << endl;
                }
                system("pause");
                
                }else{
                    PrintNoPermission();
                }
                break;
            }
            case eProcesses::Delete:
            {
                if(LoginedUser.Perms.Delete){
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
                
                }else{
                    PrintNoPermission();
                }
                break;
            }
            case eProcesses::UpdateInfo:
            {
                if(LoginedUser.Perms.Update){
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
            }else{
                PrintNoPermission();
            }
            break;
            }
        }
    }while( Mode != eProcesses::Exit);
        }else{
            system("cls");
            cout << "Invalid, Tryagain." << endl;
        }
    
}
}
/// @brief 
/// @return 

int main(){
    StartProcess();
    return 0;
}