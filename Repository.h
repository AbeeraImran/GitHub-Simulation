#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct ListNode {
	string data;
	ListNode* next;
	ListNode(string s) {
		data = s;
		next = nullptr;
	}
};

class LinkedList {
private:
	ListNode* head;
	int listSize;
public:
	LinkedList();
	void Insert(string str);
	void Delete(string str);
	int getSize();
	string getDataAt(int index);
};

struct Node {
	string Repository_Name;
	LinkedList* files;
	LinkedList* commits;
	bool isPrivate;
	int height;
	Node* left;
	Node* right;
	Node(string RepoName) {
		Repository_Name = RepoName;
		files = new LinkedList;
		commits = new LinkedList;
		isPrivate = true;	//Private by default
		height = 1;
		left = nullptr;
		right = nullptr;
	}
};

class Repository
{
private:
	string OwnerName;	//User's Name
	Node* root;

	int getHeight(Node* node);
	int getBalance(Node* node);


	void setPrivateStatus(Node* node, string repo_name, bool status);
	Node* rightRotate(Node* y);
	Node* leftRotate(Node* x);

	Node* getRepoRecurr(Node* node, string name);

	Node* Add_File(Node* node, string repoName, string fileName = "", string Commit_Msg = "");
	Node* minValueNode(Node* node);

	bool ForkRepository(Node* node, string repo_name, Repository& repo);
	Node* deleteRepo(Node* root, string repoName);
	void deleteFileFromRepo(Node* node, string repoName, string fileName, string Commit_Msg);
	void saveData(Node*& node);
	void inorderTraversalSave(Node* node);
	void inorderTraversalRepoGrab(Node* node, vector<string>& vect);

public:
	Repository() {
		root = nullptr;
	}

	void CreateANewRepo(string repoName);
	void Add_File(string repoName, string fileName);
	void Add_Commit_Msg(string repoName, string commitMsg);
	bool ForkARepository(string repo_name, Repository& repo);
	void LoadData(string Owner);
	void SaveData();
	Node* getRepo(string name);
	vector<string> getRepoList();
};


