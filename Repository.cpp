#include "Repository.h"

LinkedList::LinkedList() {
	listSize = 0;
	head = nullptr;
}

void LinkedList::Insert(string str) {
	ListNode* newNode = new ListNode(str);
	listSize++;
	if (head == nullptr) {
		head = newNode;
	}
	else {
		ListNode* temp = head;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
}

void LinkedList::Delete(string str) {
	if (head == nullptr) { return; }
	if (head->data == str) {
		ListNode* temp = head;
		head = head->next;
		listSize--;
		delete temp;
	}
	else {
		ListNode* curr = head;
		ListNode* prev = head;
		while (curr != nullptr && curr->data != str) {
			prev = curr;
			curr = curr->next;
		}
		if (curr == nullptr) { return; }

		prev->next = curr->next;
		listSize--;
		delete curr;
	}
}

int LinkedList::getSize() {
	return listSize;
}

string LinkedList::getDataAt(int index) {
	if (head == nullptr) { return ""; }
	if (index == 0) { return head->data; }
	int i = 0;
	ListNode* curr = head;
	while (curr != nullptr && index != i) {
		curr = curr->next;
		i++;
	}
	if (curr == nullptr) { return ""; }
	return curr->data;
}

int Repository::getHeight(Node* node) {
	if (node == nullptr)
		return 0;
	return node->height;
}

int Repository::getBalance(Node* node) {
	if (node == nullptr)
		return 0;
	return getHeight(node->left) - getHeight(node->right);
}

void Repository::setPrivateStatus(Node* node, string repo_name, bool status) {
	if (node == nullptr) {
		return;
	}
	else {
		if (node->Repository_Name == repo_name) {
			node->isPrivate = status;
		}
		else if (node->Repository_Name < repo_name) {
			setPrivateStatus(node->right, repo_name, status);
		}
		else {	//node->Repo_Name > name
			setPrivateStatus(node->left, repo_name, status);
		}
	}
}

Node* Repository::rightRotate(Node* y) {
	Node* x = y->left;
	Node* T = x->right;
	x->right = y;
	y->left = T;
	y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
	x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
	return x;
}

Node* Repository::leftRotate(Node* x) {
	Node* y = x->right;
	Node* T = y->left;
	y->left = x;
	x->right = T;
	x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
	y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
	return y;
}

Node* Repository::Add_File(Node* node, string repName, string fileName, string commit_msg) {
	if (node == nullptr) {
		Node* newRepo = new Node(repName);
		if (fileName != "") {
			newRepo->files->Insert(fileName);
		}
		if (commit_msg != "") {
			newRepo->commits->Insert(commit_msg);
		}
		return newRepo;
	}
	if (repName < node->Repository_Name) {
		node->left = Add_File(node->left, repName, fileName, commit_msg);
	}
	else if (repName > node->Repository_Name) {
		node->right = Add_File(node->right, repName, fileName, commit_msg);
	}
	else {
		//Repo Exists append into it
		if (fileName != "") {
			//File Name can be "" when only creating a new Repo
			//Add Into Linked List
			node->files->Insert(fileName);
		}
		if (commit_msg != "") {
			//Add Into Commits
			node->commits->Insert(commit_msg);
		}
		return node;
	}

	node->height = 1 + max(getHeight(node->left), getHeight(node->right));
	int balance = getBalance(node);

	if (balance > 1 && repName < node->left->Repository_Name)
		return rightRotate(node);
	if (balance < -1 && repName > node->right->Repository_Name)
		return leftRotate(node);
	if (balance > 1 && repName > node->left->Repository_Name) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}
	if (balance < -1 && repName < node->right->Repository_Name) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
	return node;
}

Node* Repository::minValueNode(Node* node) {
	Node* current = node;
	while (current->left != nullptr)
		current = current->left;
	return current;
}

bool Repository::ForkRepository(Node* node, string repo_name, Repository& repo) {
	if (node == nullptr) {
		cout << "Repository Not Found!" << endl;
		return false;
	}
	else {
		if (node->Repository_Name == repo_name) {
			if (node->isPrivate) {
				cout << "Cannot Fork a Private Repository!" << endl;
				return false;
			}
			repo.CreateANewRepo(repo_name);
			int files = node->files->getSize();
			for (int i = 0; i < files; i++) {
				repo.Add_File(repo_name, node->files->getDataAt(i));
			}
			int commits = node->commits->getSize();
			for (int i = 0; i < commits; i++) {
				repo.Add_File(repo_name, node->commits->getDataAt(i));
			}
			cout << "Repository " << repo_name << " forked!" << endl;
			return true;
		}
	}
	return false;
}

Node* Repository::deleteRepo(Node* root, string repoName) {
	if (root == nullptr) {
		return root;
	}
	if (repoName < root->Repository_Name) {
		root->left = deleteRepo(root->left, repoName);
	}
	else if (repoName > root->Repository_Name) {
		root->right = deleteRepo(root->right, repoName);
	}
	else {
		if (root->left == nullptr || root->right == nullptr) {
			Node* temp = root->left ? root->left : root->right;
			if (temp == nullptr) {
				temp = root;
				root = nullptr;
			}
			else {
				*root = *temp;
			}
			delete temp;
		}
		else
		{
			Node* temp = minValueNode(root->right);
			root->Repository_Name = temp->Repository_Name;
			root->files = temp->files;
			root->commits = temp->commits;
			root->right = deleteRepo(root->right, temp->Repository_Name);
		}
	}

	if (root == nullptr)
		return root;

	root->height = 1 + max(getHeight(root->left), getHeight(root->right));
	int balance = getBalance(root);

	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);
	if (balance > 1 && getBalance(root->left) < 0) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);
	if (balance < -1 && getBalance(root->right) > 0) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	return root;
}

void Repository::deleteFileFromRepo(Node* node, string repoName, string fileName, string commit_msg) {
	if (node == nullptr) {
		return;
	}
	if (node->Repository_Name == repoName) {
		node->files->Delete(fileName);
		node->commits->Insert(commit_msg);
	}
	else if (node->Repository_Name < repoName) {
		deleteFileFromRepo(node->right, repoName, fileName, commit_msg);
	}
	else {// node->Repo_Name > repoName
		deleteFileFromRepo(node->left, repoName, fileName, commit_msg);
	}
}

void Repository::saveData(Node*& node) {
	if (node == nullptr) { return; }	//Just in case
	if (OwnerName == "") { return; }

	fstream file;
	string file_name = "Repository_" + OwnerName + ".csv";

	file.open(file_name, ios::app);
	if (!file.is_open()) { return; }
	string isprivt = node->isPrivate ? "1" : "0";
	string line = node->Repository_Name + ":" + isprivt + ":";
	int f_count = node->files->getSize();
	int c_count = node->commits->getSize();

	for (int i = 0; i < f_count; i++) {
		line += node->files->getDataAt(i);
		if (i + 1 != f_count)
			line += ",";
	}
	line += ":";
	for (int i = 0; i < c_count; i++) {
		line += node->commits->getDataAt(i);
		if (i + 1 != c_count)
			line += ",";
	}
	file << line << endl;
	file.close();
}

void Repository::inorderTraversalSave(Node* node) {
	if (node == nullptr) { return; }

	inorderTraversalSave(node->left);
	saveData(node);
	inorderTraversalSave(node->right);

}

void Repository::inorderTraversalRepoGrab(Node* node, vector<string>& vect) {
	if (node != nullptr) {
		inorderTraversalRepoGrab(node->left, vect);
		vect.push_back(node->Repository_Name);
		inorderTraversalRepoGrab(node->right, vect);
	}
}

Node* Repository::getRepoRecurr(Node* node, string name) {
	if (node == nullptr) {
		return nullptr;
	}
	else if (node->Repository_Name == name) {
		return node;
	}
	else if (node->Repository_Name < name) {
		return getRepoRecurr(node->right, name);
	}
	else {	//node->Repo_Name > name
		return getRepoRecurr(node->left, name);
	}
}

void Repository::CreateANewRepo(string repoName) {
	root = this->Add_File(root, repoName);
}

void Repository::Add_File(string repoName, string fileName) {
	root = this->Add_File(root, repoName, fileName);
}

void Repository::Add_Commit_Msg(string repoName, string commitMsg) {
	root = this->Add_File(root, repoName, "", commitMsg);
}

bool Repository::ForkARepository(string repo_name, Repository& repo) {
	return ForkRepository(root, repo_name, repo);
}

void Repository::LoadData(string Owner) {
	fstream file;
	OwnerName = Owner;
	string file_name = "Repository_" + OwnerName + ".csv";
	file.open(file_name, ios::in);
	if (!file.is_open()) {
		file.open(file_name, ios::out);	//Create a new File
		file.close();
		return;
	}

	string line;

	while (getline(file, line, '\n')) {
		string repo_name;
		string fileName;
		string commit_msg;
		if (line == "") { break; }

		int i;
		for (i = 0; i < line.length() && line[i] != ':'; i++) {
			repo_name += line[i];
		}
		i++;	//Ingore the ':'
		if (repo_name != "")
			CreateANewRepo(repo_name);
		//Get Private Status
		char isprivt = line[i]; i += 2; //Ignore the '0'/'1' AND ':'
		this->setPrivateStatus(root, repo_name, isprivt - '0');

		while (i <= line.length() && line[i] != ':') {
			if (line[i] == ',' || line[i] == '\0' || line[i] == '\n') {
				if (fileName != "")
					this->Add_File(repo_name, fileName);
				fileName = "";
				i++; continue;
			}
			fileName += line[i];
			i++;
		}
		i++; //Ignore the ':'
		this->Add_File(repo_name, fileName);
		while (i < line.length()) {
			if (line[i] == ',' || line[i] == '\0' || line[i] == '\n') {
				if (commit_msg != "")
					this->Add_Commit_Msg(repo_name, commit_msg);
				commit_msg = "";
				i++; continue;
			}
			commit_msg += line[i];
			i++;
		}
		this->Add_Commit_Msg(repo_name, commit_msg);
	}
	file.close();
}

void Repository::SaveData() {
	if (OwnerName == "") { return; }
	fstream file;
	string file_name = "Repository_" + OwnerName + ".csv";

	//This part of code clears the file contents
	file.open(file_name, ios::out | ios::trunc);	//Create a new File
	file.close();
	//

	inorderTraversalSave(root);
}

Node* Repository::getRepo(string name) {
	return getRepoRecurr(root, name);
}

vector<string> Repository::getRepoList() {
	vector<string> temp;
	inorderTraversalRepoGrab(root, temp);
	return temp;
}
