#include <iostream>
using namespace std;

class Node
{
private:
    int accountNumber;
    int accountBalance;
    Node *nextNode;

public:
    Node(int acc)
    {
        accountNumber = acc;
        accountBalance = 1000;
        nextNode = nullptr;
    }

    friend class SinglyLinkedList;
};

class SinglyLinkedList
{
private:
    Node *head;
    Node *tail;
    int length;

public:
    SinglyLinkedList()
    {
        head = tail = nullptr;
        length = 0;
    }

    void addNode(int acc)
    {
        Node *newNode = new Node(acc);
        if (length == 0)
        {
            head = tail = newNode;
        }
        else
        {
            tail->nextNode = newNode;
            tail = newNode;
        }
        length++;
    }

    Node *findNode(int acc)
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            if (temp->accountNumber == acc)
            {
                return temp;
            }
            temp = temp->nextNode;
        }
        return nullptr;
    }

    void performTransaction(int acc, char type, int amount)
    {
        Node *targetNode = findNode(acc);
        if (targetNode != nullptr)
        {
            if (type == 'D')
            {
                targetNode->accountBalance += amount;
            }
            else if (type == 'W')
            {
                targetNode->accountBalance -= amount;
            }
        }
    }

    void printAccountsWithMinBalance(int x)
    {
        int count = 0;
        Node *temp = head;
        while (temp != nullptr)
        {
            if (temp->accountBalance >= x)
            {
                count++;
            }
            temp = temp->nextNode;
        }
        cout << count << endl;
    }

    void printAccountsWithMaxBalance()
    {
        int maxBalance = -1;
        Node *temp = head;

        while (temp != nullptr)
        {
            if (temp->accountBalance > maxBalance)
            {
                maxBalance = temp->accountBalance;
            }
            temp = temp->nextNode;
        }

        temp = head;
        while (temp != nullptr)
        {
            if (temp->accountBalance == maxBalance)
            {
                cout << temp->accountNumber << " ";
            }
            temp = temp->nextNode;
        }
        cout << endl;
    }

    void printBalance(int acc)
    {
        Node *temp = findNode(acc);
        if (temp != nullptr)
        {
            cout << temp->accountBalance << endl;
        }
    }
};

class TransactionNode
{
private:
    int accountNumber;
    char actionType;
    int amount;
    TransactionNode *nextNode;
    TransactionNode *prevNode;

public:
    TransactionNode(int acc = -1, char type = 'A', int amt = -1)
    {
        accountNumber = acc;
        actionType = type;
        amount = amt;
        nextNode = prevNode = nullptr;
    }

    friend class DoublyLinkedList;
};

class DoublyLinkedList
{
private:
    TransactionNode *head;
    TransactionNode *tail;
    TransactionNode *cursor;
    int length;
    int cursorIndex;

public:
    DoublyLinkedList()
    {
        head = new TransactionNode();
        tail = new TransactionNode();
        head->nextNode = tail;
        tail->prevNode = head;
        cursor = head;
        length = 0;
        cursorIndex = 0;
    }

    void addTransactionNode(int acc, char type, int amt)
    {
        TransactionNode *newNode = new TransactionNode(acc, type, amt);
        newNode->prevNode = tail->prevNode;
        tail->prevNode->nextNode = newNode;
        newNode->nextNode = tail;
        tail->prevNode = newNode;
        length++;
    }

    void processTransactions(int count, SinglyLinkedList &accountList)
    {
        while (cursor->nextNode != tail && count > 0)
        {
            cursor = cursor->nextNode;
            cursorIndex++;
            accountList.performTransaction(cursor->accountNumber, cursor->actionType, cursor->amount);
            count--;
        }
    }

    void undoTransactions(int count, SinglyLinkedList &accountList)
    {
        while (cursor != head && count > 0)
        {
            accountList.performTransaction(cursor->accountNumber, cursor->actionType, -(cursor->amount));
            cursor = cursor->prevNode;
            cursorIndex--;
            count--;
        }
    }

    void insertTransactionNode(int acc, char type, int amt, int pos, SinglyLinkedList &accountList)
    {
        if (pos >= 1 && pos <= length)
        {
            int i;
            TransactionNode *newNode = new TransactionNode(acc, type, amt);
            TransactionNode *temp = head;
            for (i = 0; i < pos; i++)
            {
                temp = temp->nextNode;
            }
            temp->nextNode->prevNode = newNode;
            newNode->nextNode = temp->nextNode;
            temp->nextNode = newNode;
            newNode->prevNode = temp;
            if (temp != cursor)
            {
                accountList.performTransaction(acc, type, amt);
            }
            length++;
        }
    }

    void deleteTransactions(int acc, int count, SinglyLinkedList &accountList)
    {
        TransactionNode *temp = cursor->nextNode;
        while (temp != tail && count != 0)
        {
            if (temp->accountNumber == acc)
            {
                TransactionNode *delNode = temp;
                temp->prevNode->nextNode = temp->nextNode;
                temp->nextNode->prevNode = temp->prevNode;
                temp = temp->nextNode;
                delete delNode;
                length--;
                count--;
            }
            else
            {
                temp = temp->nextNode;
            }
        }
        cursor->nextNode = temp;
    }

    void processAllTransactions(SinglyLinkedList &accountList)
    {
        processTransactions(length, accountList);
    }

    void printTransactions(int acc)
    {
        TransactionNode *temp = head->nextNode;
        while (temp != tail)
        {
            if (temp->accountNumber == acc)
            {
                cout << temp->accountNumber << " " << temp->actionType << " " << temp->amount << endl;
            }
            temp = temp->nextNode;
        }
    }
};

int main()
{
    int accountCount, transactionCount, accountNumber, amount, x, y, m, k;
    char command, transactionType;
    SinglyLinkedList accountList;
    DoublyLinkedList transactionList;

    cin >> accountCount;
    while (accountCount--)
    {
        cin >> accountNumber;
        accountList.addNode(accountNumber);
    }

    cin >> transactionCount;
    while (transactionCount--)
    {
        cin >> accountNumber >> transactionType >> amount;
        transactionList.addTransactionNode(accountNumber, transactionType, amount);
    }

    while (true)
    {
        cin >> command;
        switch (command)
        {
        case 'F':
            cin >> x;
            transactionList.processTransactions(x, accountList);
            break;

        case 'R':
            cin >> y;
            transactionList.undoTransactions(y, accountList);
            break;

        case 'I':
            cin >> accountNumber >> transactionType >> amount >> k;
            transactionList.insertTransactionNode(accountNumber, transactionType, amount, k, accountList);
            break;

        case 'D':
            cin >> accountNumber >> m;
            transactionList.deleteTransactions(accountNumber, m, accountList);
            break;

        case 'C':
            transactionList.processAllTransactions(accountList);
            break;

        case 'S':
            cin >> y;
            transactionList.printTransactions(y);
            break;

        case 'G':
            cin >> x;
            accountList.printAccountsWithMinBalance(x);
            break;

        case 'M':
            accountList.printAccountsWithMaxBalance();
            break;

        case 'V':
            cin >> x;
            accountList.printBalance(x);
            break;

        case 'E':
            return 0;
        }
    }

    return 0;
}