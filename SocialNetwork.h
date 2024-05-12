#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class SocialNetwork
{
private:
	string* UsersList;
	vector<string>* UserFollowing;
	int users_count;
public:
	SocialNetwork();
	int getTotalUsers();
	int getUserIndex(string usr);
	void loadData();

	vector<string> getFollowingList(string usr);

	int getFollowersCount(string user);

	void saveData();
	void addNewUsr(string usr);

	bool IsFollowing(string usr, string follow_user);

	void AddToFollowingList(string usr, string follow_user_name);
	void UnfollowUser(string usr, string following_user_name);
	vector<string> getUserList();
	~SocialNetwork();
};

#pragma once
