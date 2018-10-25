#include "iostream"
#include "time.h"   
#include<string> 
#include "fstream"

using namespace std;

#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif

int money = 10;
int ante = 1;

struct node
{
	int value;
	int suit;
	node* next;
	node* last;
};

struct linkedList
{
	node* head;
};

linkedList* CreateLinkedList()
{
	linkedList* list = new linkedList;
	list->head = nullptr;
	return list;
}

void AddFirst(linkedList* list, int value, int suit)
{
	node* n = new node;
	n->value = value;
	n->suit = suit;
	n->last = nullptr;
	n->next = list->head;
	list->head = n;
}

void AddLast(linkedList* list, int value, int suit)
{
	if (list->head == nullptr)
	{
		AddFirst(list, value, suit);
		return;
	}
	node* tail = list->head;
	while (tail->next != nullptr) {
		tail = tail->next;
	}
	node* n = new node;
	n->value = value;
	n->suit = suit;
	n->next = nullptr;
	n->last = tail;
	tail->next = n;
}

void DelLinkedList(linkedList* list)
{
	node* n = list->head;
	while (n != nullptr)
	{
		node* temp = n;
		n = n->next;
		delete temp;
	}
	delete list;
}

void InitDeck(linkedList* deck)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			AddLast(deck, j, i);
		}
	}
}

int CountItems(linkedList* list)
{
	int count = 0;
	node* n = list->head;
	while (n != nullptr)
	{
		++count;
		n = n->next;
	}
	return count;
}

int ShowList(linkedList* list) {
	node* n = list->head;
	int i = 0;
	int column = 0;
	if (CountItems(list) >= 15) {
		column = 3;
	}
	else {
		column = 1;
	}
	while (n != nullptr)
	{
		cout <<"\t"<< ++i << ": ";
		switch (n->suit)
		{
		case 0:
			//cout << (char)3;
			cout << "Clubs    ";
			break;
		case 1:
			//cout << (char)4;
			cout << "Diamonds ";
			break;
		case 2:
			//cout << (char)5;
			cout << "Hearts   ";
			break;
		case 3:
			//cout << (char)6;
			cout << "Spades   ";
			break;
		}
		switch (n->value)
		{
		case 0:
			cout << "A\t";
			break;
		case 10:
			cout << "J\t";
			break;
		case 11:
			cout << "Q\t";
			break;
		case 12:
			cout << "K\t";
			break;
		default:
			cout << n->value + 1<< "\t";
			break;
		}
		if (i % column == 0) {
			cout << "\n";
		}		
		n = n->next;
	}
	cout << "\n";
	return 0;
}

int ShowMainMenu() {
	int input = 0;
	fflush(stdout);
	cout.clear();
	system("cls");
	cout << "This is a Poker game.\n"
		<< "You have: $"
		<< money
		<< " now.\n"
		<< "You need to pay: $"
		<< ante
		<< " per turn to play.\n"
		<< "Enter 1 to play the game.\nEnter 2 to quit the game.\n"
		<< ">";

	while (true) {
		cin >> input;
		if (cin.fail() || input > 2 || input < 1) {
			cout << "Invalid input, please try again.\n";
			cin.clear();
			cin.ignore(10000, '\n');
			cout << ">";
			continue;
		}
		else {
			cin.clear();
			cin.ignore(10000, '\n');
			if (input == 2) {
				return 1;
			}
		}
		return 0;
	}
	return 0;
}

int Shuffle(linkedList* deck, linkedList* discard) {
	deck->head = discard->head;
	discard->head = nullptr;
	return 0;
}

int DrawHand(linkedList* deck, linkedList* hand, linkedList* discard) {
	int numToDraw = 5 - CountItems(hand);
	int index = -1;
	int countDeck = 0;
	node* n = deck->head;
	for (int i = 0; i < numToDraw; i++) {
		countDeck = CountItems(deck);
		if (countDeck == 0) {
			Shuffle(deck, discard);
			n = deck->head;
			countDeck = CountItems(deck);
		}
		index = rand() % countDeck;
		while (index > 0) {
			n = n->next;
			index--;
		}
		if (n->last != nullptr) {
			n->last->next = n->next;
		}
		else {
			deck->head = n->next;
		}
		if (n->next != nullptr) {
			n->next->last = n->last;
		}
		if (hand->head != nullptr) {
			hand->head->last = n;
		}		
		n->next = hand->head;
		hand->head = n;
		n->last = nullptr;
		n = deck->head;
	}
	return 0;
}

int SortList(linkedList* list) {
	node* n = list->head;
	node* n2;
	for (int i = 0; i < 5; i++) {
		while (n->next != nullptr) {
			if (n->value > n->next->value || (n->value == n->next->value && n->suit > n->next->suit)) {
				n2 = n->next;
				if (n->last == nullptr) {
					list->head = n->next;
				}
				else {
					n->last->next = n->next;
				}
				n->next->last = n->last;
				n->last = n->next;
				if (n->next->next != nullptr) {
					n->next->next->last = n;
				}
				n->next = n2->next;
				n2->next = n;
			}
			n = n->next;
			if (n == nullptr) {
				break;
			}
		}
		n = list->head;
	}
	return 0;
}

string GetCurTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[128] = { 0 };
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
	return string(buffer);
}

int WriteLog(int value, bool flag) {
	ofstream out("log.txt",ios::app);
	//ifstream in("log.txt");
	if (out.is_open())
	{
		out << GetCurTime();
		if (flag) {
			out << "\twin:\t$" << value << endl;
		}
		else {
			out << "\tlose:\t$" << value << endl;
		}
		out.close();
	}
	else {
		cout << "Open file failed.\n";
	}
	return 0;
}

int ReadLog() {
	char buffer[256];
	ifstream in("log.txt");
	if (!in.is_open())
	{
		cout << "Open file failed.\n"; 
		return 0;
	}
	while (!in.eof())
	{
		in.getline(buffer, 100);
		cout << buffer << endl;
	}
	return 0;
}

int CalAward(linkedList* hand) {
	bool flush = true;
	bool three = 0;
	bool four = 0;
	int pair = 0;
	bool validPair = false;
	bool straight = false;
	bool royal = false;
	node* n = hand->head;
	int a[13] = { 0 };
	while (n != nullptr) {
		a[n->value]++;
		if (n->next != nullptr) {
			if (n->suit != n->next->suit) {
				flush = false;
			}
		}
		n = n->next;
	}
	for (int i = 0; i < 13; i++) {
		if (a[i] == 2) {
			pair++;
			if (i >= 10 || i == 0) {
				validPair = true;
			}
		}
		if (a[i] == 3) {
			three = true;
		}
		if (a[i] == 4) {
			four = true;
		}
		if (a[i] == 1 && i<=9) {
			if (a[i+1] == 1 && a[i+2] == 1 && a[i+3] == 1 && a[(i+4)%13] == 1) {
				straight = true;
				if (i == 9) {
					royal = true;
				}
			}
		}
	}
	cout << "You have: ";
	if (flush && royal) {
		cout << "Royal Flush!\nYou won $800!\n";
		money += 800;
		WriteLog(800, 1);
	}
	else if (flush && straight) {
		cout << "Straight Flush!\nYou won $50!\n";
		money += 50;
		WriteLog(50, 1);
	}
	else if (flush) {
		cout << "Flush!\nYou won $6!\n";
		money += 6;
		WriteLog(6, 1);
	}
	else if (four) {
		cout << "Four of a Kind!\nYou won $25!\n";
		money += 25;
		WriteLog(25, 1);
	}
	else if (pair == 2) {
		cout << "Two pair!\nYou won $2!\n";
		money += 2;
		WriteLog(2, 1);
	}
	else if (three) {
		if (pair == 1) {
			cout << "Full House!\nYou won $9!\n";
			money += 9;
			WriteLog(9, 1);
		}
		else {
			cout << "Three of a Kind!\nYou won $3!\n";
			money += 3;
			WriteLog(3, 1);
		}
	}
	else if (pair == 1 && validPair) {
		cout << "One pair!\nYou won $1!\n";
		money += 1;
		WriteLog(1, 1);
	}
	else {
		cout << "Nothing!\n";
	}
	return 0;
}

int DiscardHand(linkedList* hand, linkedList* discard, int index) {
	node* n = hand->head;
	if (index >= CountItems(hand)) {
		return 1;
	}
	while (index > 0) {
		n = n->next;
		index--;
	}
	if (n->last == nullptr) {
		hand->head = n->next;
	}
	else {
		n->last->next = n->next;
	}
	if (n->next != nullptr) {
		n->next->last = n->last;
	}
	if (discard->head != nullptr) {
		discard->head->last = n;
	}
	n->next = discard->head;
	discard->head = n;
	n->last = nullptr;
	return 0;
}

int DiscardAllHand(linkedList* hand,linkedList* discard) {
	while (hand->head != nullptr)
	{
		DiscardHand(hand, discard, 0);
	}
	return 0;
}

int Swap(linkedList* hand, linkedList* deck, int handIndex, int deckIndex) {
	node* hn = hand->head;
	node* dn = deck->head;
	int temp = -1;
	while (handIndex > 0)
	{
		hn = hn->next;
		handIndex--;
	}
	while (deckIndex > 0) {
		dn = dn->next;
		deckIndex--;
	}
	temp = hn->value;
	hn->value = dn->value;
	dn->value = temp;
	temp = hn->suit;
	hn->suit = dn->suit;
	dn->suit = hn->suit;
	return 0;
}

int GameFunc(linkedList* deck, linkedList* hand, linkedList* discard) {
	string input;
	int swapHandIndex = 0;
	int swapDeckIndex = 0;
	int deckCount = 0;
	money -= ante;
	WriteLog(ante, 0);
	while (true) {
		system("cls");
		cout << "Your Money: $" << money << endl;
		cout << "Your Ante: $" << ante << endl;
		DrawHand(deck, hand, discard);
		SortList(hand);
		cout << "Your hand contains: \n";
		ShowList(hand);	
		cout << "The deck contains: " << CountItems(deck) << "card(s).\n"
			<< "Options:\n"
			<< "- Type the index for the cards you wish to keep. (i.e., \"124\")\n"
			<< "- Type \"deck\" to view the cards remaining in the deck.\n"
			<< "- Type \"none\" to discard all cards in your hand.\n"
			<< "- Type \"all\" to keep all cards in your hand.\n"
			<< "- Type \"exit\" to exit the game.\n"
			<< "- Type \"swap\" to CHEAT and swap a card in your hand with one in the deck.\n"
			<< "- Type \"log\" to see the log of win and lose.\n"
			<< ">";

		while (true) {
			getline(cin, input);
			if (cin.fail()) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			if (input.empty()) {
				break;
			}
			else if (input == "deck") {
				cout << "The deck contains: \n";
				ShowList(deck);
				system("pause");
				break;
			}
			else if (input == "log") {
				cout << "The log: \n";
				ReadLog();
				system("pause");
				break;
			}
			else if (input == "exit") {
				return 1;
			}
			else if (input == "all") {
				cout << "You kept all cards." << endl;
				ShowList(hand);
				CalAward(hand);
				cout << "Your Money: $" << money << endl;
				return 0;
			}
			else if (input == "none"){
				cout << "You discarded all cards." << endl;
				DiscardAllHand(hand, discard);
				DrawHand(deck, hand, discard);
				SortList(hand);
				ShowList(hand);
				CalAward(hand);
				cout << "Your Money: $" << money << endl;
				return 0;
			}
			else if (input == "swap") {
				cout << "Please enter the index of your hand card that you want to swap.\n>";
				while (true) {
					cin >> swapHandIndex;
					if (cin.fail() || swapHandIndex > 5 || swapHandIndex < 1) {
						cout << "Invalid input, please try again.\n";
						cin.clear();
						cin.ignore(10000, '\n');
						cout << ">";
					}
					else {
						break;
					}
				}
				cout << "The deck contains: \n";
				ShowList(deck);
				deckCount = CountItems(deck);
				cout << "Please enter the index of deck`s card that you want to swap.\n>";
				while (true) {
					cin >> swapDeckIndex;
					if (cin.fail() || swapDeckIndex > deckCount || swapDeckIndex < 1) {
						cout << "Invalid input, please try again.\n";
						cin.clear();
						cin.ignore(10000, '\n');
						cout << ">";
					}
					else {
						break;
					}
				}
				Swap(hand, deck, swapHandIndex-1, swapDeckIndex-1);
				break;
			}
			else {
				int keepNum = 0;
				int flag[5] = { 0 };
				if (input.find("5") != -1) {
					flag[4] = 1;
					keepNum++;
					input = input.replace(input.find("5"), 1, "");
				}
				if (input.find("4") != -1) {
					flag[3] = 1;
					keepNum++;
					input = input.replace(input.find("4"), 1, "");
				}
				if (input.find("3") != -1) {
					flag[2] = 1;
					keepNum++;
					input = input.replace(input.find("3"), 1, "");
				}
				if (input.find("2") != -1) {
					flag[1] = 1;
					keepNum++;
					input = input.replace(input.find("2"), 1, "");
				}
				if (input.find("1") != -1) {
					flag[0] = 1;
					keepNum++;
					input = input.replace(input.find("1"), 1, "");
				}
				if (input.empty()) {
					for (int i = 4; i >= 0; i--) {
						if (flag[i] == 0) {
							DiscardHand(hand, discard, i);
						}
					}
					cout << "You discarded " << 5 - keepNum << " cards." << endl;
					DrawHand(deck, hand, discard);
					SortList(hand);
					ShowList(hand);
					CalAward(hand);
					cout << "Your Money: $" << money << endl;
					return 0;
				}
				else {
					cout << "Invalid input, please try again.\n";
					cout << ">";
					continue;
				}
				
			}
		}
	}
	return 0;
}

int GameControl(linkedList* deck, linkedList* hand, linkedList* discard) {
	char input;
	InitDeck(deck);
	if (ShowMainMenu()) {
		return 0;
	}
	while (true) {
		
		if (GameFunc(deck, hand, discard)) {
			return 0;
		}
		DiscardAllHand(hand, discard);
		if (money <= 0) {
			cout << "The GAME end!\nYou ran out all of your money!\n";
			system("pause");
			return 0;
		}
		cout << "Want to play another round? Just pay the ante.(y/n)\n>";
		while (true) {
			cin >> input;
			if (cin.fail() || (input != 'y' && input != 'Y' && input != 'n' && input != 'N')) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			else if (input == 'y' || input == 'Y') {
				cin.clear();
				cin.ignore(1000, '\n');
				break;
			}
			else if (input == 'n' || input == 'N') {
				cin.clear();
				cin.ignore(1000, '\n');
				return 0;
			}
		}
	}
	return 0;
}

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
	srand((unsigned)time(NULL));
	ios::sync_with_stdio(true);
	system("chcp 437");
	linkedList* deck = CreateLinkedList();
	linkedList* discard = CreateLinkedList();
	linkedList* hand = CreateLinkedList();
	GameControl(deck, hand, discard);
	DelLinkedList(deck);
	DelLinkedList(hand);
	DelLinkedList(discard);
	system("cls");
	cout << "This is a simple game produced by Weiran Ye in UCSC GPM project GAME-230.\n"
		<< "Thank you so much for playing my game!\n";
	system("pause");
}
