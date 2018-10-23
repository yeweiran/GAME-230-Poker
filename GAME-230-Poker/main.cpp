#include "iostream"
#include "time.h"   

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

int money = 5;
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

linkedList* InitDeck(linkedList* deck)
{
	if (deck->head != nullptr) {
		DelLinkedList(deck);
	}
	deck = CreateLinkedList();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			AddLast(deck, j, i);
		}
	}
	return deck;
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
	while (n != nullptr)
	{
		cout <<"\t"<< ++i << ": ";
		switch (n->suit)
		{
		case 0:
			cout << (char)3;
			//cout << "A";
			break;
		case 1:
			cout << (char)4;
			//cout << "B";
			break;
		case 2:
			cout << (char)5;
			//cout << "C";
			break;
		case 3:
			cout << (char)6;
			//cout << "D";
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
			cout << n->value << "\t";
			break;
		}
		cout << "\n";
		n = n->next;
	}
	return 0;
}

int ShowMainMenu() {
	int input = 0;
	system("cls");
	cout << "This is a Poker game.\n"
		<< "\tYou have: $"
		<< money
		<< " now.\n"
		<< "\tYou need to pay: $"
		<< ante
		<< " to play.\n"
		<< "\tEnter 1 to play the game.\n\tEnter 2 to quit the game.\n"
		<< "\t>";

	while (true) {
		cin >> input;
		if (cin.fail() || input > 2 || input < 1) {
			cout << "Invalid input, please try again.\n";
			cin.clear();
			cin.ignore(10000, '\n');
			cout << ">";
		}
		else {
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
	//TODO
	return 0;
}

int CalAward(linkedList* hand) {
	//TODO
	return 0;
}

int DiscardHand(linkedList* hand, linkedList* discard, int index) {
	node* n = hand->head;
	if (index >= CountItems(hand)) {
		return 1;
	}
	while (index > 0) {
		n = n->next;
	}
	hand->head = n->next;
	if (n->next != nullptr) {
		n->next->last = n->last;
	}
	n->next = discard->head;
	if (discard->head != nullptr) {
		discard->head->last = n;
	}
	discard->head = n;
	n->last = nullptr;
}

int GameFunc(linkedList* deck, linkedList* hand, linkedList* discard) {
	system("cls");
	DrawHand(deck, hand, discard);
	SortList(hand);
	cout << "Your hand contains: \n";
	ShowList(hand);
	cout << "The deck contains: " << CountItems(deck) << "card(s).\n"
		<< "Options:\n"
		<< "\t- Type the letters for the cards you wish to keep. (i.e., \"acd\")\n"
		<< "\t- Type \"deck\" to view the cards remaining in the deck.\n"
		<< "\t- Type \"none\" to discard all cards in your hand.\n"
		<< "\t- Type \"all\" to keep all cards in your hand.\n"
		<< "\t- Type \"exit\" to exit the game.\n"
		<< "\t- Type \"swap\" to CHEAT and swap a card in your hand with one in the deck.\n"
		<< "\t>";
	while (hand->head != nullptr)
	{
		DiscardHand(hand,discard, 0);
	}
	return 0;
}

int GameControl(linkedList* deck, linkedList* hand, linkedList* discard) {
	char input;
	deck = InitDeck(deck);
	while (true) {
		if (ShowMainMenu()) {
			return 0;
		}
		//deck = InitDeck(deck);
		GameFunc(deck, hand, discard);
		cout << "Want another Game?(y/n)\n>";
		while (true) {
			cin >> input;
			if (cin.fail() || (input != 'y' && input != 'Y' && input != 'n' && input != 'N')) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			else if (input == 'y' || input == 'Y') {
				break;
			}
			else if (input == 'n' || input == 'N') {
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
	system("chcp 437");
	linkedList* deck = CreateLinkedList();
	linkedList* discard = CreateLinkedList();
	linkedList* hand = CreateLinkedList();
	GameControl(deck, hand, discard);
}
