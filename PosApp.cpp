#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "PosApp.h"
#include <iomanip>
#include <cstring>
#include <ctime>
using namespace std;
namespace ict {
	int PosApp::menu() {
		int temp;
		cout << "The Sene-Store" << endl
			<< "1- List items" << endl
			<< "2- Add Perishable item" << endl
			<< "3- Add Non-Perishable item" << endl
			<< "4- Update item quantity" << endl
			<< "5- Show Item" << endl
			<< "6- POS" << endl
			<< "0- exit program" << endl
			<< "> ";

		cin >> temp;
		if (temp >= 0 && temp <= 6) {
			cout << endl ;
			return temp;
		}
		else {
			cout << endl ;
			return -1;
		}
		
	}


	PosApp::PosApp(const char* filename, const char* billname) {
		if (filename != nullptr) {
			strcpy(m_filename, filename);
		}
		if (billname != nullptr) {
			strcpy(m_billfname, billname);
		}
		m_items[0] = nullptr;
		m_noOfItems = 0;
	}
	void PosApp::deallocateItems() {
		for (int i = 0; i <= m_noOfItems; i++) {
			delete m_items[i];
		}
		m_noOfItems = 0;
	}

	
	void PosApp::loadRecs() {
		deallocateItems();
		fstream myfile;
		myfile.open(m_filename, ios::in);
		if (!myfile.is_open()) {
			myfile.clear();
			myfile.close();
			myfile.open(m_filename, ios::out);
			myfile.close();
		}
		else {
			char id;
			while (myfile>>id) {
				//cout << id << endl;//check id
				if (id=='P') {
					Perishable* p1;
					p1 = new Perishable;
					myfile.ignore();
					p1->load(myfile);
					m_items[m_noOfItems] = p1;
					m_items[m_noOfItems + 1] = '\0';
					m_noOfItems++;
				}
				else
				{
					if (id=='N') {
						NonPerishable* p1;
						p1 = new NonPerishable;
						myfile.ignore();
						p1->load(myfile);
						m_items[m_noOfItems] = p1;
						m_items[m_noOfItems + 1] = '\0';
						m_noOfItems++;
					}
				}
			}
		}
		myfile.close();
	}

	void PosApp::saveRecs() {
		fstream myfile;
		myfile.open(m_filename, ios::out);
		for (int i = 0; i < m_noOfItems; i++) {
			if (m_items[i]->quantity()>0) {			
				m_items[i]->save(myfile);
			}
		}
		myfile.close();
		loadRecs();
	}

	int PosApp::searchItems(const char* sku)const {
		bool valid = false;
		int i;
		for ( i = 0; i < m_noOfItems && valid==false; i++) {
			if (sku!=nullptr && m_items[i]->sku()!=nullptr && strcmp(sku,m_items[i]->sku())==0) {
				valid = true;
			}
		}
		if (valid) {
			return i-1;
		}
		else {
			return -1;
		}
	}

	void PosApp::unpdateQty() {
		 char* sku;
		int number;
		sku = new  char[MAX_SKU_LEN];
		cout << "Please enter the SKU: ";
		cin >> reinterpret_cast< char*>(sku);
		if (searchItems(sku) == -1) {
			cout << "Not found!" << endl;
		}
		else {
			m_items[searchItems(sku)]->write(cout, false);
			cout <<endl<< "Please enter the number of purchased items: ";
			cin >> number;
			(*m_items[searchItems(sku)]) += number;
			saveRecs();
			cout << "Updated!" << endl <<endl;
		}
	}

	void PosApp::addItem(bool isPerishable){
		
		if (isPerishable == true) {
			Perishable* p1;
			p1 = new Perishable;
			p1->read(cin);
			if (cin.fail()) {
				cin.clear();
				p1->write(cout,false);
			}
			else
			{
				m_items[m_noOfItems] = p1;//need hold it into m_items array!
				m_items[m_noOfItems + 1] = '\0';
				m_noOfItems++;
				saveRecs();
				cout << "Item added." << endl <<endl;
			}
		}
		else {
			NonPerishable* p1;
			p1 = new NonPerishable;
			p1->read(cin);
			if (cin.fail()) {
				cin.clear();
				p1->write(cout, false);
			}
			else
			{
				m_items[m_noOfItems] = p1;//need hold it into m_items array!
				m_items[m_noOfItems + 1] = '\0';
				m_noOfItems++;
				saveRecs();
				cout << "Item added." << endl <<endl;
			}
		}               
	}

	void PosApp::listItems()const {
		double total=0;
		int j = 1;
		
		cout << " Row | SKU    | Item Name          | Price |TX |Qty |   Total |" << endl
			<< "-----|--------|--------------------|-------|---|----|---------|" << endl;
		for (int i = 0; i < m_noOfItems; i++) {
			if (j < 10) {
				cout << "   " << j << " | ";
			}
			else {
				cout << "  " << j << " | ";
			}
			m_items[i]->write(cout, true);
			total += m_items[i]->cost()*m_items[i]->quantity();
			j++;
		}
		cout << "-----^--------^--------------------^-------^---^----^---------^" << endl
			<< "                               Total Asset: $  |       " << right << setw(7) << fixed << setprecision(2) << total << "|" << endl
			<< "-----------------------------------------------^--------------^"<<endl<<endl;
	}

	void PosApp::truncateBillFile() {
		fstream myfile;
		myfile.open(m_billfname, ios::out);
		myfile.open(m_billfname, ios::trunc);
		myfile.close();
	}

	void PosApp::showBill() {
		fstream myfile;
		myfile.open(m_billfname, ios::in);
		if (!myfile.is_open()) {
			myfile.clear();
			myfile.close();
			myfile.open(m_filename, ios::out);
			myfile.close();
		}
		else {
			char id;
			double total = 0;
			time_t now;
			struct tm nowLocal;
			now = time(nullptr);
			nowLocal = *localtime(&now);
			cout << "v--------------------------------------------------------v" << endl;
			//insert time here, in-text citation //https://www.youtube.com/watch?v=qXB5icpo0qY //
			if (nowLocal.tm_hour < 10 && nowLocal.tm_min>10) {
				cout << "| " << nowLocal.tm_year + 1900 << "/0" << nowLocal.tm_mon << "/" << nowLocal.tm_mday << ", 0" << nowLocal.tm_hour
					<< ":" << nowLocal.tm_min << "                                      |" << endl;
			}
			else if (nowLocal.tm_hour < 10 && nowLocal.tm_min<10)
			{
				cout << "| " << nowLocal.tm_year + 1900 << "/0" << nowLocal.tm_mon << " " << nowLocal.tm_mday << ", 0" << nowLocal.tm_hour
					<< ":0" << nowLocal.tm_min << "                                      |" << endl;
			}
			else if (nowLocal.tm_hour > 10 && nowLocal.tm_min > 10) {
				cout << "| " << nowLocal.tm_year + 1900 << "/0" << nowLocal.tm_mon << "/" << nowLocal.tm_mday << ", " << nowLocal.tm_hour
					<< ":" << nowLocal.tm_min << "                                      |" << endl;
			}
			else if (nowLocal.tm_hour > 10 && nowLocal.tm_min < 10) {
				cout << "| " << nowLocal.tm_year + 1900 << "/0" << nowLocal.tm_mon << "/" << nowLocal.tm_mday << ", " << nowLocal.tm_hour
					<< ":0" << nowLocal.tm_min << "                                      |" << endl;
			}
			cout << "| SKU    | Item Name          | Price |TX |Qty |   Total |" << endl;
			cout << "|--------|--------------------|-------|---|----|---------|" << endl;
			while (myfile>>id) {	
				if (id =='P') {
					Perishable* p1;
					p1 = new Perishable;
					myfile.ignore();
					(*p1).load(myfile);
					cout << "| ";
					p1->write(cout, true);
					total += p1->cost()*p1->quantity();
					delete p1;
				}
				else
				{
					if (id =='N') {
						NonPerishable* p1;
						p1 = new NonPerishable;
						myfile.ignore();
						p1->load(myfile);
						cout << "| ";
						p1->write(cout, true);
						total += p1->cost()*p1->quantity();
						delete p1;
					}
				}
			}
			cout << "^--------^--------------------^-------^---^----^---------^" << endl
				<< "|                               Total: $  |       " << right << setw(7) << fixed << setprecision(2) << total << "|" << endl
				<< "^-----------------------------------------^--------------^" << endl<<endl;
		}
		myfile.close(); 
		truncateBillFile();
	}

	void PosApp::addToBill(Item& I) {
		int i;
		fstream myfile;
		myfile.open(m_billfname, ios::out | ios::app);
		if (myfile.is_open()) {
			i = I.quantity();		
			I.quantity(1);
			I.save(myfile);
			//cout << I.price() << " " << I.sku() << endl;//check value
			I.quantity(i - 1);
			myfile.close();
			saveRecs();
		}
	}

	void PosApp::POS() {
		bool valid = false;
		 char* sku;
		sku = new char[MAX_SKU_LEN];
		cin.ignore();
		while (!valid)
		{
			cout << "Sku: ";
			cin.getline(sku, MAX_SKU_LEN);
			//cout << searchItems(sku)<<endl;//check this

			if (searchItems(sku) != -1 ) {
				cout << "v------------------->" << endl;
				cout << "| "<<m_items[searchItems(sku)]->name()<<endl;
				cout << "^------------------->" << endl;
				addToBill((*m_items[searchItems(sku)]));// add to bill list
				
			}
			else if (strcmp(sku, "") != 0)
			 {
				cout << "Not found!" << endl;
			}
			else  {
				showBill();
				valid = true;
			}
		}
	}

	void PosApp::run() {
		while (true)
		{
			int i;
			i=menu();
			if (i == 1) {
				loadRecs();
				listItems();
			}
			else if (i == 2) {
				addItem(true);
			}
			else if (i==3)
			{
				addItem(false);
			}
			else if (i == 4) {
				unpdateQty();
			}
			else if (i == 5) {
				char* sku;
				sku = new char[MAX_SKU_LEN];
				cout << "Please enter the SKU: ";
				cin >> reinterpret_cast< char*>(sku);
				if (searchItems(sku) != -1) {
					cout << "v-----------------------------------v" << endl;
					m_items[searchItems(sku)]->write(cout, false);
					cout << "^-----------------------------------^" << endl<<endl;
				}
				else {
					cout << "Not found!"<<endl;
				}
			}
			else if (i == 6) {
				POS();
			}
			else if (i == 0) {
				cout << "Goodbye!"<<endl;
				break;
			}
			else
			{
				cout << " == = Invalid Selection, try again == = "<<endl;
			}
		}
	}
}

/*
ouputs:
-------------------------------------
update:
Please enter the SKU: 1313
Name:
Paper Tissue
Sku: 1313
Price: 1.22
Price after tax: 1.38
Quantity: 194
Total Cost: 267.45

Please enter the number of purchased items: 12
Updated!

------------------------------
Please enter the SKU: 3434
Not found!

-------------------------------------
add item:
Perishable Item Entry:
Sku: 5656
Name:
Honey
Price: 12.99
Taxed: y
Quantity: 12
Expiry date (YYYY/MM/DD): 1200/10/12
Invalid Year in Date Entry

Perishable Item Entry:
Sku: 5656
Name:
Honey
Price: 12.99
Taxed: y
Quantity: 12
Expiry date (YYYY/MM/DD): 2017/5/15
Item added.

--------------------------------------------
list item:
 Row | SKU    | Item Name          | Price |TX |Qty |   Total |
-----|--------|--------------------|-------|---|----|---------|
   1 | 1234   |Milk                |   3.99|  P|   2|     7.98|
   2 | 3456   |Paper Cups          |   5.99| TN|  38|   257.21|
   3 | 4567   |Butter              |   4.56| TP|   9|    46.38|
   4 | 1212   |Salted Butter       |   5.99|  P| 111|   664.89|
   5 | 1313   |Paper Tissue        |   1.22| TN| 206|   283.99|
   6 | 5656   |Honey               |  12.99| TP|  12|   176.14|
-----^--------^--------------------^-------^---^----^---------^
			       Total Asset: $  |       1436.59|
-----------------------------------------------^--------------^


--------------------------------------
printbill:
v--------------------------------------------------------v
| 2017/04/02, 12:42                                      |
| SKU    | Item Name          | Price |TX |Qty |   Total |
|--------|--------------------|-------|---|----|---------|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1313   |Paper Tissue        |   1.22| TN|   1|     1.38|
| 5656   |Honey               |  12.99| TP|   1|    14.68|
^--------^--------------------^-------^---^----^---------^
|                               Total: $  |         22.05|
^-----------------------------------------^--------------^

-------------------------------------------------------
POS:
Sku: 1212
v------------------->
| Salted Butter
^------------------->
Sku: 1212
v------------------->
| Salted Butter
^------------------->
Sku: 1313
v------------------->
| Paper Tissue
^------------------->
Sku: 1234
v------------------->
| Milk
^------------------->
Sku: 7654
Not found!
Sku: 5656
v------------------->
| Honey
^------------------->
Sku:
v--------------------------------------------------------v
| 2017/04/02, 12:58                                      |
| SKU    | Item Name          | Price |TX |Qty |   Total |
|--------|--------------------|-------|---|----|---------|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1313   |Paper Tissue        |   1.22| TN|   1|     1.38|
| 1234   |Milk                |   3.99|  P|   1|     3.99|
| 5656   |Honey               |  12.99| TP|   1|    14.68|
^--------^--------------------^-------^---^----^---------^
|                               Total: $  |         32.03|
^-----------------------------------------^--------------^

------------------------------------------------------
run:
The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 5

Please enter the SKU: 5656
v-----------------------------------v
Name:
Honey
Sku: 5656
Price: 12.99
Price after tax: 14.68
Quantity: 10
Total Cost: 146.79
Expiry date: 2017/05/15
^-----------------------------------^

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 5

Please enter the SKU: 12345
Not found!

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> five

===Invalid Selection, try again===

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 0

Goodbye!

*/
