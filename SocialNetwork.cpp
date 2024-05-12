#include "SocialNetwork.h"

SocialNetwork::SocialNetwork() {
	users_count = 0;
	UsersList = nullptr;
	UserFollowing = nullptr;
}

int SocialNetwork::getTotalUsers() {
	return users_count;
}

int SocialNetwork::getUserIndex(string usr) {
	for (int i = 0; i < users_count; i++) {
		if (UsersList[i] == usr) {
			return i;	// returns the index of user
		}
	}
	return -1;	// Return -1 if no such user is found
}

void SocialNetwork::loadData() {
	fstream file;
	file.open("users_followers.csv", ios::in);
	users_count = 0;
	if (!file.is_open()) {
		file.open("users_followers.csv", ios::out);
		file.close();
		return;
	}

	string line;
	while (getline(file, line)) {
		users_count++;
	}
	file.close();

	file.open("users_followers.csv", ios::in);
	if (!file.is_open()) { return; }

	delete[] UsersList;
	delete[] UserFollowing;

	UsersList = new string[users_count];
	UserFollowing = new vector<string>[users_count];

	int user_i = 0;

	while (!file.eof() && user_i < users_count) {
		string user, followers_line, follower;
		int i;
		try
		{
			getline(file, user, ':');
			getline(file, followers_line);
			if (user == "")
				break;
		}
		catch (const std::exception&)
		{
			cout << "Invalid Data in Load File (\"users_followers.csv\")\n";
			return;
		}

		UsersList[user_i] = user;

		for (int i = 0; i <= followers_line.length(); i++) {
			if (followers_line[i] == ',' || followers_line[i] == '\0') {
				if (follower != "")
					UserFollowing[user_i].push_back(follower);
				follower = "";
				continue;
			}
			follower += followers_line[i];
		}
		user_i++;
	}
	file.close();
}

vector<string> SocialNetwork::getFollowingList(string usr) {
	int index = getUserIndex(usr);
	if (index == -1) { vector<string> temp;  return temp; }

	return UserFollowing[index];
}

int SocialNetwork::getFollowersCount(string user) {
	int followers_count = 0;
	for (int i = 0; i < users_count; i++) {
		for (int j = 0; j < UserFollowing[i].size(); i++) {
			if (UserFollowing[i][j] == user)
				followers_count++;
		}
	}
	return followers_count;
}

void SocialNetwork::saveData() {
	fstream file;
	file.open("users_followers.csv", ios::out);

	if (!file.is_open()) { return; }

	for (int i = 0; i < users_count; i++) {
		file << UsersList[i] << ":";
		for (int j = 0; j < UserFollowing[i].size(); j++) {
			file << UserFollowing[i][j];
			if (j + 1 != UserFollowing[i].size()) {
				file << ",";
			}
		}
		file << endl;
	}
	file.close();
}

void SocialNetwork::addNewUsr(string usr) {
	fstream file;
	file.open("users_followers.csv", ios::out);
	bool is_new = true;
	if (!file.is_open()) { return; }

	for (int i = 0; i < users_count; i++) {
		file << UsersList[i] << ":";
		if (UsersList[i] == usr) { is_new = false; }
		for (int j = 0; j < UserFollowing[i].size(); j++) {
			file << UserFollowing[i][j];
			if (j + 1 != UserFollowing[i].size()) {
				file << ",";
			}
		}
		file << endl;
	}
	if (is_new)
		file << usr << ":" << endl;
	file.close();

	loadData();
}

bool SocialNetwork::IsFollowing(string usr, string follow_user) {
	int index = getUserIndex(usr);
	if (index == -1) { return false; }

	for (int i = 0; i < UserFollowing[index].size(); i++) {
		if (UserFollowing[index][i] == follow_user) {
			return true;
		}
	}
	return false;
}

void SocialNetwork::AddToFollowingList(string usr, string follow_user_name) {
	int index = getUserIndex(usr);
	if (index == -1) { return; }
	if (getUserIndex(follow_user_name) < 0) { return; }
	if (usr == follow_user_name) { return; }

	if (IsFollowing(usr, follow_user_name)) { return; }

	UserFollowing[index].push_back(follow_user_name);
}

void SocialNetwork::UnfollowUser(string usr, string following_user_name) {
	int index = getUserIndex(usr);
	if (index == -1) { return; }
	if (!IsFollowing(usr, following_user_name)) { return; }

	vector<string> newFollowingList;
	for (int i = 0; i < UserFollowing[index].size(); i++) {
		if (following_user_name != UserFollowing[index][i]) {
			newFollowingList.push_back(UserFollowing[index][i]);
		}
	}

	UserFollowing[index] = newFollowingList;
}

vector<string> SocialNetwork::getUserList() {
	vector<string> get_users_list;
	for (int i = 0; i < users_count; i++) {
		get_users_list.push_back(UsersList[i]);
	}
	return get_users_list;
}

SocialNetwork::~SocialNetwork() {
	delete[] UsersList;
	delete[] UserFollowing;
}
