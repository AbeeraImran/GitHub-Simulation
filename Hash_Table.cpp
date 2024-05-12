#include "Hash_Table.h"

HashTable::HashTable(int num) {
	MaxLength = num;
	arr = new UserAcc[MaxLength];
	accounts_count = 0;
}

int HashTable::getUserCount() const {
	return accounts_count;
}

int HashTable::getLength() const {
	return MaxLength;
}

UserAcc HashTable::getUserAtIndex(int i) {
	if (i < 0 || i >= MaxLength) { return UserAcc(); }
	return arr[i];
}

bool HashTable::checkDataExists(UserAcc data) {
	int key = hashKey(data);

	if (CompareUsers(arr[key], data)) {
		return true;
	}

	for (int i = 1; i < MaxLength; i++) {
		int k = (key + i) % MaxLength;
		if (CompareUsers(arr[k], EmptyUser) || CompareUsers(arr[k], DeletedUser)) {
			return false;	// no such data found
		}
		if (CompareUsers(arr[k], data)) {
			return true;
		}
	}
	return false;
}

bool HashTable::checkUserExists(string usr_name) {
	int key = hashKey({ usr_name, "" });

	if (arr[key].username == usr_name
		&& !CompareUsers(arr[key], DeletedUser) && !CompareUsers(arr[key], EmptyUser)) {
		return true;
	}

	for (int i = 1; i < MaxLength; i++) {
		int k = (key + i) % MaxLength;
		if (arr[k].username == "") {
			return false;	// no such data found
		}
		if (arr[k].username == usr_name) {
			return true;
		}
	}
	return false;

}

bool HashTable::RegisterNewUser(UserAcc usr) {
	if (this->checkUserExists(usr.username)) { return false; }	//User already exists
	if (usr.username == "") { return false; }
	if (usr.password == "") { return false; }

	InsertData(usr);
	return true;
}

void HashTable::InsertData(UserAcc data) {
	int key = hashKey(data);

	if (CompareUsers(arr[key], EmptyUser) || CompareUsers(arr[key], DeletedUser)) {
		arr[key].username = data.username;
		arr[key].password = data.password;
		accounts_count++;
		IsNearFull();
		return;
	}

	for (int i = 1; i < MaxLength; i++) {
		int k = (key + i) % MaxLength;
		if (CompareUsers(arr[k], EmptyUser)) {
			arr[k].username = data.username;
			arr[k].password = data.password;
			accounts_count++;
			IsNearFull();
			return;
		}
	}
}

void HashTable::DeleteData(UserAcc data) {
	int key = hashKey(data);

	if (CompareUsers(arr[key], data)) {
		arr[key].username = DeletedUser.username;
		arr[key].password = DeletedUser.password;
		accounts_count--;
		return;
	}

	for (int i = 1; i < MaxLength; i++) {
		int k = (key + i) % MaxLength;
		if (CompareUsers(arr[k], EmptyUser)) {
			return;
		}
		if (CompareUsers(arr[k], data)) {
			arr[k].username = DeletedUser.username;
			arr[k].password = DeletedUser.password;
			accounts_count--;
			return;
		}
	}
}

void HashTable::LoadData() {
	fstream file;
	file.open("accounts_list.csv", ios::in);
	if (!file.is_open()) {
		cout << "Error Opening File" << endl;
		file.open("accounts_list.csv", ios::out);
		file.close();
		return;
	}

	accounts_count = 0;

	delete[] arr;
	arr = new UserAcc[MaxLength];

	string username, password;

	while (file.eof() == false) {
		try
		{
			getline(file, username, ',');
			getline(file, password);
			if (username == "" || password == "")
				continue;

			UserAcc user(username, password);
			InsertData(user);
		}
		catch (exception& ex)
		{
			cout << "Error: " << ex.what() << endl;
		}
	}


	file.close();
}

void HashTable::SaveData() const {
	fstream file;
	file.open("accounts_list.csv", ios::out);

	for (int i = 0; i < MaxLength; i++) {
		if (!CompareUsers(arr[i], EmptyUser) && !CompareUsers(arr[i], DeletedUser) &&
			!CompareUsers(arr[i], { "","" }))
			file << arr[i].username << "," << arr[i].password << endl;
	}

	file.close();
}

void HashTable::IsNearFull() {
	if ((accounts_count / ((double)MaxLength)) < 0.7) {
		return;
	}

	//extend size after saving data
	int tempArrSize = MaxLength;
	UserAcc* temp = new UserAcc[tempArrSize];
	for (int i = 0; i < MaxLength; i++) {
		temp[i].username = arr[i].username;
		temp[i].password = arr[i].password;
	}

	accounts_count = 0;
	delete[] arr;

	MaxLength *= 2;
	arr = new UserAcc[MaxLength];

	MaxLength *= 2;	//double the size
	for (int i = 0; i < tempArrSize; i++) {
		if (!CompareUsers(arr[i], EmptyUser) && !CompareUsers(arr[i], DeletedUser))
			InsertData(temp[i]);
	}
	delete[] temp;
}

int HashTable::hashKey(UserAcc data) const {
	string str = data.username;	//hashing based on username
	int key = 0;
	for (int i = 0; i < str.length(); i++) {
		key += str[i];
	}
	return key % MaxLength;
}

void HashTable::DisplayUserName(int index) {
	UserAcc user = getUserAtIndex(index);
	if (checkDataExists(user)) {
		cout << "User Name: " << user.username << endl;
	}
}

HashTable::~HashTable() {
	delete[] arr;
}

bool CompareUsers(UserAcc u1, UserAcc u2) {
	if (u1.username == u2.username && u1.password == u2.password) {
		return true;
	}
	return false;
}