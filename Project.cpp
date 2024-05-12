#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <Windows.h>	//Sleep (mili-seconds)
#include "Hash_Table.h"
#include "SocialNetwork.h"
#include "Repository.h"

using namespace std;

HashTable AccountsList;
SocialNetwork socialnetwork;
Repository* repository;

string LoggedInUser = "";

/*
	Register
	Login - Logout
	Profile View

	Github Part:
	Create, Delete, <Copy>, Public/Private, commit_msg, view stats

	-->
		Add File
		Remove File
		Follow/Unfollow User

*/

void InitialLoading() {
	AccountsList.LoadData();
	socialnetwork.loadData();

	int size = AccountsList.getLength();

	for (int i = 0; i < size; i++) {
		string user = AccountsList.getUserAtIndex(i).username;
		if (user != DeletedUser.username && user != EmptyUser.username &&
			user != "") {
			if (socialnetwork.getUserIndex(user) == -1) {
				//Get the index of where the user is at.
				socialnetwork.addNewUsr(user);
			}
		}
	}

	vector<string> list = socialnetwork.getUserList();
	int list_size = list.size();
	repository = new Repository[list_size];

	for (int i = 0; i < list_size; i++) {
		repository[i].LoadData(list[i]);
	}
}

void DisplayUsersList(vector<string> userslist) {
	cout << "Users List:" << endl;
	int users = userslist.size();
	for (int i = 0; i < users; i++) {
		if (i % 2 == 0) {
			cout << setw(25) << left << userslist[i];
			cout << "\t";
		}
		if (i % 2 == 1) {
			cout << setw(25) << left << userslist[i];
			cout << endl;
		}
	}
	cout << endl;
}

void ViewMenu();
void RegisterAnAccount();
bool LoginIntoAnAccount();
void LoggedInUserOptions();

void ManageGitHub();
void ViewRepository(vector<string> repo_list, int index);
void ShowUserGitHub(string user);

int main()
{
	InitialLoading();
	ViewMenu();

	//Save Data on Quit
	AccountsList.SaveData();
	socialnetwork.saveData();
	int c = socialnetwork.getTotalUsers();
	for (int i = 0; i < c; i++) {
		repository[i].SaveData();
	}

	return 0;
}
void ViewMenu() {
	char ch;

	do {
		system("cls");
		cout << "Press any Button to perform listed Tasks\n";
		cout << "1.Register a New Account\n2.Login into an Existing Account\n";
		cout << "3.Exit Program\n";
		cout << ">> ";
		ch = _getch();

		if (ch == '1') {
			RegisterAnAccount();
		}
		else if (ch == '2') {
			bool login = LoginIntoAnAccount();
			if (!login) {
				cout << "Wrong Username or Password!" << endl;
			}
			else {
				LoggedInUserOptions();
			}
		}
	} while (ch != '3');
}
void RegisterAnAccount() {
	system("cls");
	UserAcc NewUser;
	cout << "Register An Acount\n\n";
	bool check = false;
	do {
		cout << "Enter a Unique Username: ";
		getline(cin, NewUser.username);
		check = AccountsList.checkUserExists(NewUser.username);
	} while (check);

	if (NewUser.username == "") { return; }

	cout << "Enter a Password: ";
	getline(cin, NewUser.password);
	if (NewUser.password == "") { return; }

	cout << NewUser.username << "," << NewUser.password << endl;

	if (!AccountsList.RegisterNewUser(NewUser)) {
		cout << "An Error Occured!\nPress any key to continue...";
		while (!_kbhit());
		system("cls");
		return;
	}
	AccountsList.SaveData();
	socialnetwork.addNewUsr(NewUser.username);
	vector<string> list = socialnetwork.getUserList();
	int list_size = list.size();
	repository = new Repository[list_size];

	for (int i = 0; i < list_size; i++) {
		repository[i].LoadData(list[i]);
	}
	cout << "Account Created!\nPress any key to continue...";
	while (!_kbhit());
	system("cls");
}
bool LoginIntoAnAccount() {
	UserAcc user;
	char ch = '\0';

	cout << "\nLogin Into An Account\n";
	cout << "Enter Username: ";
	getline(cin, user.username);
	cout << "Enter Password: ";
	getline(cin, user.password);

	if (AccountsList.checkDataExists(user)) {
		cout << "Account Successfully Logged in!\nPress any key to continue...";
		LoggedInUser = user.username;
		while (!_kbhit());
		system("cls");
		return true;
	}

	return false;
}
void LoggedInUserOptions() {
	if (LoggedInUser == "") { return; }
	char ch;
	do {
		system("cls");
		cout << "Welcome, " << LoggedInUser << endl;
		cout << "Select one of the following Options:\n";
		cout << "1.View a User Profile\n2.Goto Your GitHub Page\n3.Log Out\n";
		cout << ">> ";
		cin.clear();
		ch = _getch();
		if (ch == '1') {
			string name;
			vector<string> userslist = socialnetwork.getUserList();
			DisplayUsersList(userslist);
			cout << "\nEnter Username you want to search: ";
			getline(cin, name);
			//Display Selected User in Depth: Need GitHub Structure
			ShowUserGitHub(name);
		}
		else if (ch == '2') {
			ManageGitHub();
		}
	} while (ch != '3');
	LoggedInUser = "";
}
void ManageGitHub() {
	if (LoggedInUser == "") { return; }
	char ch;
	int index = socialnetwork.getUserIndex(LoggedInUser);
	do {
		system("cls");
		cout << "Welcome, " << LoggedInUser << endl;
		cout << "Select one of the following Options:\n";
		cout << "1.View your Repository\n2.Create a Repository\n3.Edit Repository\n";
		cout << "4.View Following Users\n5.Go Back\n";
		cout << ">> ";
		ch = _getch();

		if (ch == '1') {
			//Show User's Repository
			if (index == -1) { cout << "No Such User Found!"; return; }
			vector<string> list = repository[index].getRepoList();

			cout << "Name: " << LoggedInUser << endl;
			cout << "Total Repositories: " << list.size() << endl;
			for (int i = 0; i < list.size(); i++) {
				string status = "";
				if (repository[index].getRepo(list[i])->isPrivate) {
					status = "(Private)";
				}
				else {
					status = "(Public)";
				}

				cout << i + 1 << ". " << list[i] << status << endl;
			}
			cout << endl;

			Sleep(500);

			cout << "Do you want to view a repository in detail?\n";
			cout << "('Y' to view): "; char view = _getch();
			if (view == 'y' || view == 'Y') {
				ViewRepository(list, index);
			}

		}
		else if (ch == '2') {
			//To-Do
			string repo;
			cout << "Enter name of your Repository: ";
			getline(cin, repo);
			if (repository[index].getRepo(repo) == nullptr) {
				repository[index].CreateANewRepo(repo);
				cout << "Repository Created!" << endl;
				repository[index].SaveData();

				Sleep(2000);
			}
			else {
				cout << "Repository named: " << repo << " already exists!" << endl;
				Sleep(2000);
			}
		}
		else if (ch == '3') {
			//To-Do
			string name;
			cout << "Enter Repository Name you want to Edit: ";
			getline(cin >> ws, name);
			Node* node = repository[index].getRepo(name);
			if (node == nullptr) { cout << "No Such Repository Exists.\n"; Sleep(2000); continue; }
			cout << "1. Add a File\n2. Remove a File\n3. Change Repository Status\nAny Other key to Go Back\n";
			cout << ">> "; char opt = _getch();
			if (opt == '1') {
				string file_name;
				cout << "Enter Name of File you want to Add: ";
				getline(cin, file_name);
				if (file_name == "") { continue; }
				node->files->Insert(file_name);
				cout << "Enter a Commit Message (leave blank for default msg): ";
				string cmt_msg = "";
				getline(cin, cmt_msg);
				if (cmt_msg == "") { cmt_msg = file_name + " was added."; }
				node->commits->Insert(cmt_msg);
				cout << "File was successfully added\n";
				Sleep(2000);
			}
			else if (opt == '2') {
				string file_name;
				cout << "Enter Name of File you want to Remove: ";
				getline(cin, file_name);
				if (file_name == "") { continue; }
				node->files->Delete(file_name);
				cout << "Enter a Commit Message (leave blank for default msg): ";
				string cmt_msg = "";
				getline(cin, cmt_msg);
				if (cmt_msg == "") { cmt_msg = file_name + " was removed."; }
				node->commits->Insert(cmt_msg);
				cout << "File was successfully removed\n";
				Sleep(2000);
			}
			else if (opt == '3') {
				if (node->isPrivate) {
					cout << "Status changed to: Public\n";
					node->isPrivate = false;
				}
				else {
					cout << "Status changed to: Private\n";
					node->isPrivate = true;
				}
			}
		}
		else if (ch == '4') {
			//
			string name;
			vector<string> list = socialnetwork.getFollowingList(LoggedInUser);
			DisplayUsersList(list);
			cout << "\nEnter Username you want to search: ";
			getline(cin, name);
			bool valid_user = false;
			for (int i = 0; i < list.size(); i++) {
				if (list[i] == name) {
					//Display Selected User in Depth: Need GitHub Structure
					valid_user = true;
					ShowUserGitHub(name);
					break;
				}
			}

			if (!valid_user) { cout << "Not Following this user.\n"; Sleep(1500); }
		}
	} while (ch != '5');
}
void ViewRepository(vector<string> repo_list, int index) {
	int size = repo_list.size();
	if (size <= 0) { cout << "No Repositories Found!\n"; Sleep(2000); return; }

	cout << "Select Repository Number(1.." << size << "): ";
	int num; cin >> num;
	if (num < 1 || num > size) { cout << "No Such Repository!\n"; }

	Node* node = repository[index].getRepo(repo_list[num - 1]);
	if (node->isPrivate && LoggedInUser != socialnetwork.getUserList()[index]) {
		cout << "Cannot View this Repository! Status = Private" << endl;
		Sleep(2000);
		return;
	}
	int files = node->files->getSize();
	int commits = node->commits->getSize();

	cout << "Files:\n";
	for (int i = 0; i < files; i++) {
		cout << "  - " << node->files->getDataAt(i) << endl;
	}
	cout << "Commits:\n";
	for (int i = 0; i < commits; i++) {
		cout << "  - " << node->commits->getDataAt(i) << endl;
	}
	cout << endl;
	cout << "Press any key to continue...\n";
	_getch();

}
void ShowUserGitHub(string user) {
	int index = socialnetwork.getUserIndex(user);
	if (index == -1) { cout << "No Such User Found!"; return; }
	vector<string> list = repository[index].getRepoList();

	cout << "Name: " << user << endl;
	cout << "Total Repositories: " << list.size() << endl;
	for (int i = 0; i < list.size(); i++) {
		string status = "";
		if (repository[index].getRepo(list[i])->isPrivate) {
			status = "(Private)";
		}
		else {
			status = "(Public)";
		}

		cout << i + 1 << ". " << list[i] << status << endl;
	}
	cout << endl;


	cout << "1. View a Repository\n2. Fork a Repository\n3. Follow/Unfollow the user\n";
	cout << "Press anyother key to go back.\n";
	cout << ">> ";
	char opt = _getch();

	if (opt == '1') {
		// View Repository
		//To-Do
		ViewRepository(list, index);
	}
	else if (opt == '2') {
		// Copy Repository
		//To-Do
		string repo_name;
		cout << "Enter name of repository you want to fork: ";
		getline(cin, repo_name);
		repository[index].ForkARepository(repo_name, repository[socialnetwork.getUserIndex(LoggedInUser)]);
		Sleep(1800);
	}
	else if (opt == '3') {
		// Follow/UnFollow User
		bool following = socialnetwork.IsFollowing(LoggedInUser, user);
		if (following) {
			cout << "Currently Following this User.\n";
			cout << "Do you want to unfollow this user? (y/n)\n";
			cout << ">> ";
			char s = _getch();
			if (s == 'y' || s == 'Y') {
				socialnetwork.UnfollowUser(LoggedInUser, user);
				cout << "Not following: " << user << ", any longer." << endl;
				socialnetwork.saveData();
			}
		}
		else {
			if (user != LoggedInUser) {
				cout << "Do you want to follow this user? (y/n)\n";
				cout << ">> ";
				char s = _getch();
				if (s == 'y' || s == 'Y') {
					socialnetwork.AddToFollowingList(LoggedInUser, user);
					cout << "Now following: " << user << endl;
					socialnetwork.saveData();
				}
			}
		}

		cout << "Press any key to continue...\n";
		_getch();
	}

}
