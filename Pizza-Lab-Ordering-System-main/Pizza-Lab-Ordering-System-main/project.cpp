#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

const int infinity = INT_MAX;
using namespace std;

// Pizza Shop Ordering Management System

// Defining Data Types

struct customer
{
    int age;
    string name;
    string pizzaName;
    int quantity;
    double bill;

    customer() {}

    customer(int age, string name, int quantity, string pizzaName, double bill)
    {
        this->age = age;
        this->name = name;
        this->pizzaName = pizzaName;
        this->quantity = quantity;
        this->bill = bill;
    }
};

struct takeAwayCustomer
{
    customer cusotomer;
    takeAwayCustomer *next = NULL;

    takeAwayCustomer(int age, string name, int quantity, string pizzaName, double bill)
    {
        this->cusotomer = customer(age, name, quantity, pizzaName, bill);
    }
};

struct dineInCustomer
{
    customer cusotomer;
    dineInCustomer *next = NULL;

    dineInCustomer(int age, string name, int quantity, string pizzaName, double bill)
    {
        this->cusotomer = customer(age, name, quantity, pizzaName, bill);
    }
};

struct homeDeliveryCustomer
{

    customer cusotomer;
    string address;
    double deliveryCharges;
    int distanceDelivery;
    struct homeDeliveryCustomer *next = NULL;

    homeDeliveryCustomer(int age, string name, int quantity, string pizzaName, double bill, string address, double deliveryCharges, int distanceDelivery)
    {
        this->cusotomer = customer(age, name, quantity, pizzaName, bill);
        this->address = address;
        this->deliveryCharges = deliveryCharges;
        this->distanceDelivery = distanceDelivery;
    }
};

struct PizzaShop
{

    string shopName;
    string *menu;
    int *price;
    string address;
    takeAwayCustomer *nextTakeAwayCustomer = NULL;
    dineInCustomer *nextDineInCustomer = NULL;
    homeDeliveryCustomer *nextHomeDeliveryCustomer = NULL;
};

// Globally declaring the pizza Shop's pointer
PizzaShop *myPizzaShop = NULL;

// Globally Declaring the Current Customer's Pointers for all three Types
takeAwayCustomer *currentTakeAwayCustomer = NULL;
dineInCustomer *currentDineInCustomer = NULL;
homeDeliveryCustomer *currentHomeDeliveryCustomer = NULL;

// Globally declaring the variables for the total of all the orders in queue!
double total, takeAway, dineIn, homeDelivery;
// Globally declaring the variables for the total of all the orders served!
double servedTotal;

// In case of Serving , to keep the record of Customers Served, implementing AVL Tree for efficient Search
// to search the record of Customers by Name
// It can also Display all the customers Served

struct servedCustomer
{

    int age;
    string name;
    int quantity;
    string pizzaName;
    double bill;
    string customerType;
    servedCustomer *left;
    servedCustomer *right;

    // Constructor
    servedCustomer(int age, string name, int quantity, string pizzaName, double bill, string customerType)
    {
        // setting customers details

        this->age = age;
        this->name = name;
        this->quantity = quantity;
        this->pizzaName = pizzaName;
        this->bill = bill;
        this->customerType = customerType;

        // child to NULL
        this->left = NULL;
        this->right = NULL;
    }
};

servedCustomer *root = NULL; // Global pointer for the root of AVLTree

// isEmpty or not
int isEmpty(servedCustomer *root)
{
    return root == NULL;
}

// display Customers Details

void display(servedCustomer *root)
{
	cout << "\t\t\t\t\t\t\t|_______________________________|" << endl;
    cout << "\t\t\t\t\t\t\t| Name :" << root->name << endl;
    cout << "\t\t\t\t\t\t\t| Age  :" << root->age << endl;
    cout << "\t\t\t\t\t\t\t| Pizza :" << root->pizzaName << endl;
    cout << "\t\t\t\t\t\t\t| Quantity : " << root->quantity << endl;
    cout << "\t\t\t\t\t\t\t| Bill : " << root->bill << endl;
    cout << "\t\t\t\t\t\t\t| Customer Type: " << root->customerType << endl;
    cout << "\t\t\t\t\t\t\t|_______________________________|" << endl << endl;
}

// Traversal for the served Customers

servedCustomer *displayAllServedOrders(servedCustomer *root)
{

    if (root)
    {
        displayAllServedOrders(root->left);
        display(root); // will display all the served Customers
        displayAllServedOrders(root->right);
    }

    return root;
}

// Height of servedCustomer tree

int height(servedCustomer *root)
{
    if (!root)
        return 0;

    return max(height(root->left), height(root->right)) + 1;
}

// Balance Factor for each ServedCustomer node

int balanceFactor(servedCustomer *root)
{
    if (!root)
        return 0;

    return height(root->left) - height(root->right);
}

// Maximum of two integers as a helper function for height
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Searching in servedCustomer tree

servedCustomer *search(servedCustomer *root, string keyName)
{
    if (root == NULL)
    {
        return NULL;
    }
    else if (root->name == keyName)
    {
        return root;
    }
    else if (root->name < keyName)
    {
        return search(root->right, keyName);
    }
    else if (root->name > keyName)
    {
        return search(root->left, keyName);
    }

    return root;
}

// Finding Maximum Node of servedCustomer tree

servedCustomer *maxservedCustomer(servedCustomer *root)
{
    // Maximum Node is Present in the most Right Node  of the served Customer Tree

    servedCustomer *p = root;
    servedCustomer *temp = NULL;

    while (p != NULL)
    {
        temp = p;
        p = p->right;
    }

    return temp;
}

// Balancing the ServedCustomer's Tree thorugh AVL Rotations

// LL Rotation
servedCustomer *LLRotation(servedCustomer *root)
// rotate wese right per krna hai!
{
    // saving the new root and the lost element in case of rotation
    servedCustomer *x = root->left;
    servedCustomer *temp = x->right;

    // Performing rotation
    x->right = root;
    root->left = temp;

    // updating the root
    root = x;
    // returning the root
    return x;
}

// RR Rotation
servedCustomer *RRRotation(servedCustomer *root)
{
    // rotate wese left per krna hai!

    // saving the new root and the lost element in case of rotation

    servedCustomer *x = root->right;
    servedCustomer *temp = x->left;

    // Performing rotation
    x->left = root;
    root->right = temp;

    // updating the root
    root = x;

    // returning the root
    return x;
}

// LR Rotation
servedCustomer *LRRotation(servedCustomer *root)
{
    root->left = RRRotation(root->left);
    return LLRotation(root);
}

// RL Rotation
servedCustomer *RLRotation(servedCustomer *root)
{
    root->right = LLRotation(root->right);
    return RRRotation(root);
}

//  INSERTION in servedCustomer Tree

servedCustomer *insertion(int age, string name, int quantity, string pizzaName, double bill, string customerType, servedCustomer *root)
{
    servedCustomer *newNode = new servedCustomer(age, name, quantity, pizzaName, bill, customerType);

    if (root == NULL)
    {
        root = newNode;
    }

    else if (root->name > newNode->name)
    {
        root->left = insertion(age, name, quantity, pizzaName, bill, customerType, root->left);
    }
    else if (root->name < newNode->name)
    {
        root->right = insertion(age, name, quantity, pizzaName, bill, customerType, root->right);
    }

    else
    {
    	cout << "\t\t\t\t\t\t\t|__________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| No duplicates Values are Allowed |" << endl;
        cout << "\t\t\t\t\t\t\t|__________________________________|" << endl << endl;
        return root;
    }

    int bf = balanceFactor(root);

    if (bf == 2)
    {
        // LL
        if (root->left->name > newNode->name)
        {
            return LLRotation(root);
        }

        // LR
        if (root->left->name < newNode->name)
        {
            return LRRotation(root);
        }
    }
    else if (bf == -2)
    {
        // RR
        if (root->right->name < newNode->name)
        {
            return RRRotation(root);
        }

        // RL
        if (root->right->name > newNode->name)
        {
            return RLRotation(root);
        }
    }

    return root; // in case there is no need of rotation
}

servedCustomer *deleteNode(servedCustomer *root, string key)
{
    if (root == NULL)
        return root;
    else if (key < root->name)
        root->left = deleteNode(root->left, key);
    else if (key > root->name)
        root->right = deleteNode(root->right, key);
    else
    {
        // if deleteroot has one child or zero child
        if ((root->left == NULL) || (root->right == NULL))
        {
            servedCustomer *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            delete (temp);
        }
        else
        {
            // if deleteroot has two or more childs
            servedCustomer *temp = maxservedCustomer(root->right);
            root->name = temp->name;
            root->right = deleteNode(root->right, temp->name);
        }
    }

    if (root == NULL)
        return root;

    // getting the balance factor
    int balance = balanceFactor(root);

    // Rotation Cases
    // LEFT LEFT CASE
    if (balance > 1 && key < root->left->name)
        return LLRotation(root);

    // LEFT RIGHT CASE
    if (balance > 1 && key > root->left->name)
    {
        root->left = LLRotation(root->left);
        return LRRotation(root);
    }

    // RIHGT RIGHT CASE
    if (balance < -1 && key > root->right->name)
        return RRRotation(root);

    // RIGHT LEFT CASE
    if (balance < -1 && key < root->right->name)
    {
        return RLRotation(root);
    }

    return root;
}

void deleteAllServedCustomers(servedCustomer *root)
{

    while (root)
    {
        root = deleteNode(root, root->name);
    }
	cout << "\t\t\t\t\t\t\t|_______________________________________|" << endl;
    cout << "\t\t\t\t\t\t\t| The Served Customer's List is Cleared |" << endl;
    cout << "\t\t\t\t\t\t\t|_______________________________________|" << endl << endl;
}

// Now defining Order Placing and Serving of Take Away Customer
// Based on : Older person will be served first (PRIORITY QUEUE)

void placeOrderTakeAwayCustomer(int age, string name, string pizzaName, int quantity, double bill)
{
    // making new Customer
    currentTakeAwayCustomer = new takeAwayCustomer(age, name, quantity, pizzaName, bill);

    if (myPizzaShop->nextTakeAwayCustomer == NULL)
    {
        // if first then insert in front
        myPizzaShop->nextTakeAwayCustomer = currentTakeAwayCustomer;
    }
    else
    {
        // finding the last Node
        takeAwayCustomer *temp = myPizzaShop->nextTakeAwayCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        if (temp->cusotomer.age < currentTakeAwayCustomer->cusotomer.age)
        {
            // insert at front
            takeAwayCustomer *firstCustomer = myPizzaShop->nextTakeAwayCustomer;
            myPizzaShop->nextTakeAwayCustomer = currentTakeAwayCustomer;
            currentTakeAwayCustomer->next = firstCustomer;
        }
        else
        {
            // insert at end
            temp->next = currentTakeAwayCustomer;
            currentTakeAwayCustomer->next = NULL;
        }
    }
    cout << "\t\t\t\t\t\t\t|________________________________________________________________________________________________________________|" << endl;
    cout << "\t\t\t\t\t\t\t| Your Order has been Placed MR/MRS " << name << " and your order is " << pizzaName << " with " << quantity << " quantity and total bill is " << bill << endl;
    cout << "\t\t\t\t\t\t\t|________________________________________________________________________________________________________________|" << endl << endl;

}
void serveOrderTakeAwayCustomer()
{
    if (myPizzaShop->nextTakeAwayCustomer == NULL)
    {
    	cout << "\t\t\t\t\t\t\t|________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| No Take Away Customer to Serve |" << endl;
        cout << "\t\t\t\t\t\t\t|________________________________|" << endl << endl;
    }
    else
    {
        // serving the first customer
        takeAwayCustomer *temp = myPizzaShop->nextTakeAwayCustomer;
        // if it has some next element
        if(temp->next != NULL){
            myPizzaShop->nextTakeAwayCustomer = temp->next;
        }
        else{
            myPizzaShop->nextTakeAwayCustomer = NULL;
        }
        cout << "\t\t\t\t\t\t\t|_____________________________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| Take Away Customer Served : " << temp->cusotomer.name << endl;
        cout << "\t\t\t\t\t\t\t|_____________________________________________________|" << endl << endl;

        string customerType = "Take-Away";
        // Now before deleting the node we need to update the servedCustomer Tree
        root = insertion(temp->cusotomer.age, temp->cusotomer.name, temp->cusotomer.quantity, temp->cusotomer.pizzaName, temp->cusotomer.bill, customerType, root);

        delete temp; // deleting the customer
    }
}

// Now defining Order Placing and Serving of Dine-In Customer
// Based on : First Come First Served (FIFO) (QUEUE)

void placeOrderDineInCustomer(int age, string name, string pizzaName, int quantity, double bill)
{
    // making new Customer
    currentDineInCustomer = new dineInCustomer(age, name, quantity, pizzaName, bill);

    if (myPizzaShop->nextDineInCustomer == NULL)
    {
        // if first insert in front
        myPizzaShop->nextDineInCustomer = currentDineInCustomer;
    }
    else
    {
        // finding the last Node
        dineInCustomer *temp = myPizzaShop->nextDineInCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentDineInCustomer;
        currentDineInCustomer->next = NULL;
    }
    
    cout << "\t\t\t\t\t\t\t|_____________________________________________________________________________________________________________|" << endl;
    cout << "\t\t\t\t\t\t\t| Your Order has been Placed MR/MRS " << name << " and your order is " << pizzaName << " with " << quantity << " quantity and total bill is " << bill << endl;
    cout << "\t\t\t\t\t\t\t|_____________________________________________________________________________________________________________|" << endl << endl;
}
void serveOrderDineInCustomer()
{
    if (myPizzaShop->nextDineInCustomer == NULL)
    {	
    	cout << "\t\t\t\t\t\t\t|______________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| No Dine-In Customer to Serve |" << endl;
        cout << "\t\t\t\t\t\t\t|______________________________|" << endl << endl;
    }
    else
    {
        // serving the first customer
        dineInCustomer *temp = myPizzaShop->nextDineInCustomer;
        if(temp->next != NULL){
            myPizzaShop->nextDineInCustomer = temp->next;
        }
        else{
            myPizzaShop->nextDineInCustomer = NULL;
        }
		cout << "\t\t\t\t\t\t\t|___________________________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| Dine-In Customer Served : " << temp->cusotomer.name << endl;
		cout << "\t\t\t\t\t\t\t|___________________________________________________|" << endl << endl;

        string customerType = "Dine-In";
        // Now before deleting the node we need to update the servedCustomer Tree
        root = insertion(temp->cusotomer.age, temp->cusotomer.name, temp->cusotomer.quantity, temp->cusotomer.pizzaName, temp->cusotomer.bill, customerType, root);

        delete temp; // deleting the customer
    }
}

// Now defining Order Placing and Serving of Home Delivery Customer
// Based on : (when all orders are ready)(LIFO)(Stack)

void placeOrderHomeDeliveryCustomer(int age, string name, string pizzaName, int quantity, double bill, string address, int deliveryCharges, int distanceDelivery)
{
    // making new Customer
    currentHomeDeliveryCustomer = new homeDeliveryCustomer(age, name, quantity, pizzaName, bill, address, deliveryCharges, distanceDelivery);

    if (myPizzaShop->nextHomeDeliveryCustomer == NULL)
    {
        // if first insert in front
        myPizzaShop->nextHomeDeliveryCustomer = currentHomeDeliveryCustomer;
    }
    else
    {
        // finding the last Node
        homeDeliveryCustomer *temp = myPizzaShop->nextHomeDeliveryCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentHomeDeliveryCustomer;
        currentHomeDeliveryCustomer->next = NULL;
    }
    cout << "\t\t\t\t\t\t\t|__________________________________________________________________________________________________________________|" << endl;
    cout << "\t\t\t\t\t\t\t| Your Order has been Placed MR/MRS " << name << " and your order is " << pizzaName << " with " << quantity << " quantity and total bill is " << bill << endl;
    cout << "\t\t\t\t\t\t\t|__________________________________________________________________________________________________________________|" << endl << endl;

}

void serveOrderHomeDeliveryCustomer()
{
    if (myPizzaShop->nextHomeDeliveryCustomer == NULL)
    {
    	cout << "\t\t\t\t\t\t\t|____________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| No Home Delivery Customer to Serve |" << endl;
        cout << "\t\t\t\t\t\t\t|____________________________________|" << endl << endl;

    }
    else
    {

        // serving the last customer first
        homeDeliveryCustomer *first = myPizzaShop->nextHomeDeliveryCustomer;

        if (first->next == NULL)
        {
            // if it is the only customer

            myPizzaShop->nextHomeDeliveryCustomer = NULL;
			
			cout << "\t\t\t\t\t\t\t|_________________________________________________________|" << endl;
            cout << "\t\t\t\t\t\t\t| Home Delivery Customer Served : " << first->cusotomer.name << endl;
        	cout << "\t\t\t\t\t\t\t|_________________________________________________________|" << endl << endl;

            string customerType = "Home-Delivery Customer";
            root = insertion(first->cusotomer.age, first->cusotomer.name, first->cusotomer.quantity, first->cusotomer.pizzaName, first->cusotomer.bill, customerType, root);

            // now deleting the node
            delete (first);
        }
        else {
            homeDeliveryCustomer *s = first->next;
            while(s->next !=NULL){
                first = first->next;
                s = s->next;
            }

            first->next = NULL;
	        cout << "\t\t\t\t\t\t\t|___________________________________________________|" << endl;
            cout << "\t\t\t\t\t\t\t| Home Delivery Customer Served : " << s->cusotomer.name << endl;
            cout << "\t\t\t\t\t\t\t|___________________________________________________|" << endl << endl;

            string customerType = "Home-Delivery Customer";
            root = insertion(s->cusotomer.age, s->cusotomer.name, s->cusotomer.quantity, s->cusotomer.pizzaName, s->cusotomer.bill, customerType, root);

            // deleting the node

            delete (s);
        }

        
        // deleting the customer
    }
}

// It will serve all the waiting orders

void serveAllOrders()
{

    while (myPizzaShop->nextTakeAwayCustomer != NULL)
    {
        serveOrderTakeAwayCustomer();
    }
    while (myPizzaShop->nextDineInCustomer != NULL)
    {
        serveOrderDineInCustomer();
    }
    while (myPizzaShop->nextHomeDeliveryCustomer != NULL)
    {
        serveOrderHomeDeliveryCustomer();
    }
}

void displayAllOrdersTakeAwayCustomers()
{
    if (myPizzaShop->nextTakeAwayCustomer == NULL)
    {
    	cout << "\t\t\t\t\t\t\t|_________________________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| There is no Order for Walking Customer till yet |" << endl;
        cout << "\t\t\t\t\t\t\t|_________________________________________________|" << endl << endl;

    }
    else
    {
        takeAwayCustomer *traversal = myPizzaShop->nextTakeAwayCustomer;
        while (traversal != NULL)
        {

            cout << "\t\t\t\t\t\t\t|_________________________________________________|" << endl;
            cout << "\t\t\t\t\t\t\t| Take-Away Customer : " << traversal->cusotomer.name << endl;
            cout << "\t\t\t\t\t\t\t| Age : " << traversal->cusotomer.age << endl;
            cout << "\t\t\t\t\t\t\t| Pizza Name : " << traversal->cusotomer.pizzaName << endl;
            cout << "\t\t\t\t\t\t\t| Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "\t\t\t\t\t\t\t| Bill : " << traversal->cusotomer.bill << " RS/_" << endl;
            cout << "\t\t\t\t\t\t\t|__________________________________________________|" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrdersHomeDeliveryCustomers()
{
    if (myPizzaShop->nextHomeDeliveryCustomer == NULL)
    {	
    	cout << "\t\t\t\t\t\t\t|_______________________________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| There is no Order for Home Delivery Customer till yet |" << endl;
        cout << "\t\t\t\t\t\t\t|_______________________________________________________|" << endl << endl;

    }
    else
    {
        homeDeliveryCustomer *traversal = myPizzaShop->nextHomeDeliveryCustomer;
        while (traversal != NULL)
        {
            cout << "\t\t\t\t\t\t\t|___________________________________________________________|" << endl;
            cout << "\t\t\t\t\t\t\t| Home Delivery Customer : " << traversal->cusotomer.name << endl;
            cout << "\t\t\t\t\t\t\t| Age : " << traversal->cusotomer.age << endl;
            cout << "\t\t\t\t\t\t\t| Pizza Name : " << traversal->cusotomer.pizzaName << endl;
            cout << "\t\t\t\t\t\t\t| Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "\t\t\t\t\t\t\t| Delivery Distance : " << traversal->deliveryCharges << "KM"<<endl;
            cout << "\t\t\t\t\t\t\t| Delivery Charges : " << traversal->distanceDelivery << endl;
            cout << "\t\t\t\t\t\t\t| Bill : " << traversal->cusotomer.bill << " RS/_" << endl;
            cout << "\t\t\t\t\t\t\t| Delivery Address : " << traversal->address << endl;
            cout << "\t\t\t\t\t\t\t|____________________________________________________________|" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrdersDineInCustomers()
{
    if (myPizzaShop->nextDineInCustomer == NULL)
    {
    	cout << "\t\t\t\t\t\t\t|_________________________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t| There is no Order for Dine-In Customer till yet |" << endl;
        cout << "\t\t\t\t\t\t\t|_________________________________________________|" << endl << endl;

    }
    else
    {
        dineInCustomer *traversal = myPizzaShop->nextDineInCustomer;
        while (traversal != NULL)
        {
            cout << "\t\t\t\t\t\t\t|____________________________________________________|" << endl;
            cout << "\t\t\t\t\t\t\t| Walking Customer : " << traversal->cusotomer.name << endl;
            cout << "\t\t\t\t\t\t\t| Age : " << traversal->cusotomer.age << endl;
            cout << "\t\t\t\t\t\t\t| Pizza Name : " << traversal->cusotomer.pizzaName << endl;
            cout << "\t\t\t\t\t\t\t| Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "\t\t\t\t\t\t\t| Bill : " << traversal->cusotomer.bill << " RS/_" << endl;
            cout << "\t\t\t\t\t\t\t|____________________________________________________|" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrders()
{
	cout << "\t\t\t\t\t\t\t|________________________________________|" << endl;
    cout << "\t\t\t\t\t\t\t| The Take-Away Customers Are :" << endl;
    displayAllOrdersTakeAwayCustomers();

    cout << "\t\t\t\t\t\t\t| The Dine-IN Customers Are :" << endl;
    displayAllOrdersDineInCustomers();

    cout << "\t\t\t\t\t\t\t| The Home Delivery Customers Are :" << endl;
    displayAllOrdersHomeDeliveryCustomers();
	cout << "\t\t\t\t\t\t\t|_________________________________________|" << endl << endl;

}

void totalbillofPendingOrders()
{
    takeAwayCustomer *p = myPizzaShop->nextTakeAwayCustomer;
    while (p != NULL)
    {
        takeAway += p->cusotomer.bill;
        p = p->next;
    }
    dineInCustomer *q = myPizzaShop->nextDineInCustomer;
    while (q != NULL)
    {
        dineIn += q->cusotomer.bill;
        q = q->next;
    }
    homeDeliveryCustomer *r = myPizzaShop->nextHomeDeliveryCustomer;
    while (r != NULL)
    {
        homeDelivery += r->cusotomer.bill;
        r = r->next;
    }
    total = takeAway + dineIn + homeDelivery;

	cout << "\t\t\t\t\t\t\t|________________________________________________________________________________________|" << endl;
    cout << "\t\t\t\t\t\t\t| The total bill of pending orders for Take-Away customers are : " << takeAway << " RS/_" << endl;
    cout << "\t\t\t\t\t\t\t| The total bill of pending orders for Dine-In customers are : " << dineIn << " RS/_" << endl;
    cout << "\t\t\t\t\t\t\t| The total bill of pending orders for Delivery customers are : " << homeDelivery << " RS/_" << endl;
    cout << "\t\t\t\t\t\t\t| The Total orders pending are : " << total << " RS/_" << endl;
	cout << "\t\t\t\t\t\t\t|_________________________________________________________________________________________|" << endl << endl;

}

double totalEarnings(servedCustomer *root){

    if(root){
        totalEarnings(root->left);
        servedTotal += root->bill;
        totalEarnings(root->right);
    }

    return servedTotal;
}

// making a graph for the available delivery options

//                           0              1             2        3          4           5
string deliveryPoints[] = {"PizzaSHOP", "Nazimabad", "Gulshan", "Malir", "Clifton", "Shadman"};

// first value in the pair is vertex and second is the distance (weight) in KM

vector<vector<pair<int, int>>> deliveryMap = {

    // first value in the pair is vertex and second is the distance (weight) in KM
    {{1, 2}, {2, 3}, {3, 5}, {5, 4}}, //  0 
    {{0, 2}, {5, 1}},                 //  1 
    {{0, 3}, {3, 1}},                 //  2 
    {{0, 5}, {4, 2}, {5, 2}, {2, 1}}, //  3 
    {{3, 2}, {5, 2}},                 //  4 
    {{0, 4}, {1, 1},{3,2} ,{4, 2}}    //  5 

};

vector<int> dijkstra(int sourceNode)
{
    vector<int> distance(6, infinity);
    set<pair<int, int>> s;
    distance[sourceNode] = 0; // Pizza Shop
    s.insert(make_pair(0, sourceNode));

    while (!s.empty())
    {
        auto top = *(s.begin());
        int u = top.first;  //   current weight
        int v = top.second; //  current vertex

        s.erase(s.begin());

        // traversing the adjacency list of v
        for (auto child : deliveryMap[v])
        {
            int childVertex = child.first;
            int childWeight = child.second;

            if (u + childWeight < distance[childVertex])
            {
                distance[childVertex] = u + childWeight;
                s.insert(make_pair(distance[childVertex], childVertex));
            }
        }
    }

    return distance;
}

int main() {
    // making pizza shop
    myPizzaShop = new PizzaShop;
    myPizzaShop->shopName = "The Pizza Delight";
    myPizzaShop->address = "Bahadurabad, Karachi";

    // Setting the menu
    myPizzaShop->menu = new string[11]{"",
                                      "Chicken Tikka", "Arabic Ranch",
                                      "Chicken Fajita", "Cheese Lover",
                                      "Chicken Supreme", "All Veggie",
                                      "Garlic West", "Beef Bold",
                                      "Phantom", "Mexican Delight"};

    myPizzaShop->price = new int[11]{0, 2000, 2500, 2400, 2200, 2700, 2000, 2100, 3000, 3000, 2800};

    int option = -99;

    do {
        system("cls"); // Clear screen for cleaner display

        // Header
        cout << "\n\t\t\t\t\t\t\t|_____________________________________________________________|" << endl;
        cout << "\t\t\t\t\t\t\t" << RED <<"                Welcome to " << myPizzaShop->shopName << "       " <<RESET<< endl;
        cout << "\t\t\t\t\t\t\t|_____________________________________________________________|" << endl;
        cout << "\n\t\t\t\t\t\t\t           Located at: " << myPizzaShop->address << "\n" << endl;

        // Menu Section
        cout << GREEN <<"\t\t\t\t\t\t\t|_______________________ OUR MENU __________________________|" << endl;
        cout <<"\t\t\t\t\t\t\t|  Code  |        Pizza         |         Price (Rs)        |" << endl;
        cout << "\t\t\t\t\t\t\t|______ _|______________________|___________________________|" << RESET << endl;
        for (int i = 1; i <= 10; i++) {
            cout << "\t\t\t\t\t\t\t    " << i << "\t\t" << myPizzaShop->menu[i] << "\t\t" <<myPizzaShop->price[i] << endl;
        }
        cout << "\t\t\t\t\t\t\t|________|______________________|___________________________|\n" << endl;

        // Operations Menu
        cout << GREEN << "\t\t\t\t\t\t\t|________________________ OPERATIONS _______________________|" << RESET << endl;
        cout << "\t\t\t\t\t\t\t|                                                           |" << endl;
        cout << "\t\t\t\t\t\t\t|                       [Place Order]                       |" << endl;
        cout << "\t\t\t\t\t\t\t|   1. Take-Away Order                                      |" << endl;
        cout << "\t\t\t\t\t\t\t|   2. Home Delivery Order                                  |" << endl;
        cout << "\t\t\t\t\t\t\t|   3. Dine-In Order                                        |" << endl;
        cout << "\t\t\t\t\t\t\t|                                                           |" << endl;
        cout << "\t\t\t\t\t\t\t|                       [Manage Orders]                     |" << endl;
        cout << "\t\t\t\t\t\t\t|   4. Serve Take-Away Order                                |" << endl;
        cout << "\t\t\t\t\t\t\t|   5. Serve Home Delivery Order                            |" << endl;
        cout << "\t\t\t\t\t\t\t|   6. Serve Dine-In Order                                  |" << endl;
        cout << "\t\t\t\t\t\t\t|   7. Serve All Pending Orders                             |" << endl;
        cout << "\t\t\t\t\t\t\t|                                                           |" << endl;
        cout << "\t\t\t\t\t\t\t|                       [View Orders]                       |" << endl;
        cout << "\t\t\t\t\t\t\t|   8. View Take-Away Orders                                |" << endl;
        cout << "\t\t\t\t\t\t\t|   9. View Home Delivery Orders                            |" << endl;
        cout << "\t\t\t\t\t\t\t|   10. View Dine-In Orders                                 |" << endl;
        cout << "\t\t\t\t\t\t\t|   11. View All Orders                                     |" << endl;
        cout << "\t\t\t\t\t\t\t|                                                           |" << endl;
        cout << "\t\t\t\t\t\t\t|                       [Reports & Management]              |" << endl;
        cout << "\t\t\t\t\t\t\t|   12. View Served Orders History                          |" << endl;
        cout << "\t\t\t\t\t\t\t|   13. Search Order History                                |" << endl;
        cout << "\t\t\t\t\t\t\t|   14. Clear Order History                                 |" << endl;
        cout << "\t\t\t\t\t\t\t|   15. View Pending Orders Total                           |" << endl;
        cout << "\t\t\t\t\t\t\t|   16. View Total Earnings                                 |" << endl;
        cout << "\t\t\t\t\t\t\t|                                                           |" << endl;
        cout << "\t\t\t\t\t\t\t|   0. Exit System                                          |" << endl;
        cout << GREEN << "\t\t\t\t\t\t\t|___________________________________________________________|" << RESET << endl;
        
		cout <<"\t\t\t\t\t\t\t_______________________________________________________________" << endl;
        cout << BLUE << "\n\t\t\t\t\t\t\tPlease enter your choice (0-16): "<< RESET;
        cin >> option;

        // Input validation
        while (cin.fail() || option < 0 || option > 16) {
        	cout << "\t\t\t\t\t\t\t_____________________________________________" << endl;
            cout << "\n\t\t\t\t\t\t\tError: Please enter a valid option (0-16): ";
            cin.clear();
            cin.ignore(256, '\n');
            cin >> option;
        }

        int age, quantity, pizzaIndex;
        double bill;
        string address;
        string name;

        switch (option)
        {
        case 1:
        { // placing order for take away customer
        	cout << "\t\t\t\t\t\t\t|_________________________________|" << endl;
            cout << "\t\t\t\t\t\t\t| Enter the name of the customer: ";
            cin >> name;
            cout << "\t\t\t\t\t\t\t| Enter the age of the customer: ";
            cin >> age;
            cout << "\t\t\t\t\t\t\t| Enter the quantity of the pizza: ";
            cin >> quantity;
            cout << "\t\t\t\t\t\t\t| Enter the option for the pizza: ";
            cin >> pizzaIndex;
        	cout << "\t\t\t\t\t\t\t|_________________________________|" << endl;

            bill = quantity * myPizzaShop->price[pizzaIndex];
            placeOrderTakeAwayCustomer(age, name, myPizzaShop->menu[pizzaIndex], quantity, bill);
        }
        break;

        case 2:

        {                                               // placing order for Home Delivery customer
            vector<int> distanceFromShop = dijkstra(0); // As Zero is our Source Node

            int optionDelivery = -999;

            do
            {
            	cout << "\t\t\t\t\t\t\t________________________________________________"<< endl;
                cout << "\t\t\t\t\t\t\tThe delivery is available for following Areas : " << endl;
                for (int i = 1; i <= 5; i++)
                {
                    cout << "\t\t\t\t\t\t\t" << i << ". " << deliveryPoints[i] << endl;
                }
				
				cout << "\t\t\t\t\t\t\t___________________" << endl;
                cout << "\t\t\t\t\t\t\tEnter your option :";
                cin >> optionDelivery;

            } while (!(optionDelivery >= 0 && optionDelivery <= 5));

            // setting the delivery address of the Customer
            address = deliveryPoints[optionDelivery];
            
			cout << "\t\t\t\t\t\t\t|_____________________________|" << endl;
            cout << "\t\t\t\t\t\t\t| Enter the name of the customer: ";
            cin >> name;
            cout << "\t\t\t\t\t\t\t| Enter the age of the customer: ";
            cin >> age;
            cout << "\t\t\t\t\t\t\t| Enter the quantity of the pizza: ";
            cin >> quantity;
            cout << "\t\t\t\t\t\t\t| Enter the option for the pizza: ";
            cin >> pizzaIndex;
			cout << "\t\t\t\t\t\t\t|_____________________________|" << endl;

            int deliveryChargesPerKM = 50;
            int deliveryCharges = deliveryChargesPerKM * distanceFromShop[optionDelivery];
            bill = quantity * myPizzaShop->price[pizzaIndex] + deliveryCharges;

            // distance from the shop
            int distanceFromTheShop = distanceFromShop[optionDelivery];
            placeOrderHomeDeliveryCustomer(age, name, myPizzaShop->menu[pizzaIndex], quantity, bill, address, distanceFromTheShop, deliveryCharges);
        }
        break;

        case 3:
        { // placing order for Dine-in customer
			
			cout << "\t\t\t\t\t\t\t|_____________________________|" << endl;
            cout << "\t\t\t\t\t\t\t| Enter the name of the customer: ";
            cin >> name;
            cout << "\t\t\t\t\t\t\t| Enter the age of the customer: ";
            cin >> age;
            cout << "\t\t\t\t\t\t\t| Enter the quantity of the pizza: ";
            cin >> quantity;
            cout << "\t\t\t\t\t\t\t| Enter the option for the pizza: ";
            cin >> pizzaIndex;
			cout << "\t\t\t\t\t\t\t|_____________________________|" << endl;

            bill = quantity * myPizzaShop->price[pizzaIndex];
            placeOrderDineInCustomer(age, name, myPizzaShop->menu[pizzaIndex], quantity, bill);
        }
        break;

        case 4:
            // serving order for Take Away customer
            serveOrderTakeAwayCustomer();
            break;

        case 5:
            // serving order for Home Delivery customer
            serveOrderHomeDeliveryCustomer();
            break;

        case 6:
            // serving order for Dine-in customer
            serveOrderDineInCustomer();
            break;

        case 7:
            // serving all orders
            serveAllOrders();
            break;

        case 8:
            // displaying all orders of Take-away customers
            displayAllOrdersTakeAwayCustomers();
            break;

        case 9:
            // displaying all orders of Home Delivery customers
            displayAllOrdersHomeDeliveryCustomers();
            break;

        case 10:
            // displaying all orders of Dine-in customers
            displayAllOrdersDineInCustomers();
            break;

        case 11:
            // displaying all orders of all customers
            displayAllOrders();
            break;

        case 12:
        { // displaying all served orders
            servedCustomer *r = displayAllServedOrders(root);
            if (!r)
            	cout << "\t\t\t\t\t\t\t_____________________" << endl;
                cout << "\t\t\t\t\t\t\tNo Served Customer yet " << endl;
        }
        break;

        case 13:
        { // searching served orders
        cout << "\t\t\t\t\t\t\t_______________________________________________________" << endl;
            cout << "\t\t\t\t\t\t\tEnter the name of the customer you want to search: " << endl;
            cin >> name;
            servedCustomer *searchedCustomer = search(root, name);
            if (searchedCustomer == NULL){
            	cout << "\t\t\t\t\t\t\t_____________________________" << endl;
                cout << "\t\t\t\t\t\t\tNo such Customer was Served " << endl;
        	}
            else
                display(searchedCustomer);
        }
        break;

        case 14:
        { // clearing the served Orders

            deleteAllServedCustomers(root);
            root = NULL;
        }
        break;

        case 15:
        { // pending orders bill in queue for all!

            // resetting the state of total orders
            takeAway = 0, dineIn = 0, homeDelivery = 0, total = 0; 
            totalbillofPendingOrders();
        }
        break;

        case 16:
        { // total earnings from served orders!

        // resetting thr state of total served orders
        servedTotal = 0;
        double totalx = totalEarnings(root);
        cout << "\t\t\t\t\t\t\t_______________________________________" << endl;
        cout << "\t\t\t\t\t\t\tThe Total Earnings are : " << totalx << endl;
        }
        break;
        }
        if (option != 0) {
        	cout << "\t\t\t\t\t\t\t___________________________________________" << endl;
            cout << "\n\t\t\t\t\t\t\tPress Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while (option != 0);

    cout << "\n\t\t\t\t\t\t\t|_________________________________________________________|" << endl;
    cout << "\t\t\t\t\t\t\t|          Thank you for using The Pizza Delight!           |" << endl;
    cout << "\t\t\t\t\t\t\t|                    Have a great day!                      |" << endl;
    cout << "\t\t\t\t\t\t\t|___________________________________________________________|" << endl;

    return 0;
}