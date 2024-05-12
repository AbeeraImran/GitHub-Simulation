#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct UserAcc {
	string username;
	string password;
	UserAcc(string usr = "", string pwd = "") {
		username = usr;
		password = pwd;
	}
};

bool CompareUsers(UserAcc u1, UserAcc u2);

static UserAcc EmptyUser, DeletedUser(".", ".");

class HashTable
{
private:
	UserAcc* arr;
	int MaxLength;
	int accounts_count;
public:
	HashTable(int num = 30);

	int getUserCount() const;

	int getLength() const;

	UserAcc getUserAtIndex(int i);

	bool checkDataExists(UserAcc data);

	bool checkUserExists(string usr_name);

	bool RegisterNewUser(UserAcc usr);

	void InsertData(UserAcc data);

	void DeleteData(UserAcc data);

	void LoadData();

	void SaveData() const;

	void IsNearFull();

	int hashKey(UserAcc data) const;

	void DisplayUserName(int index);

	~HashTable();
};

