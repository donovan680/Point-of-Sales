#define _CRT_SECURE_NO_WARNINGS  
// Lib includes
#include "iostream"
#include "cstring"
// inlcude Item and POS header files
#include "Item.h"
#include "POS.h"
#include "PosIO.h"
using namespace std;
namespace ict{
  // class Item implementaion
	Item::Item() {
		strcpy(m_sku, "\0");
		m_name = new char[1];
		m_name[0] = '\0';
		m_price = 0;
		m_quantity = 0;
	}

	Item::Item(const char barcode[],  const char* name, double price, bool taxed) {
		strncpy(m_sku, barcode, MAX_SKU_LEN - 1);
		m_sku[MAX_SKU_LEN] = '\0';
		m_name = nullptr;
		m_name = new char[strlen(name)+1];
		strcpy(m_name, name);
		m_quantity = 0;
		m_price = price;
		m_taxed = taxed;
	}

	Item::Item(const Item& item) {
		m_quantity = item.m_quantity;
		m_taxed = item.m_taxed;
		m_price = item.m_price;		
		strcpy(m_sku, item.m_sku);
		if (item.m_name != nullptr) {
			m_name = new char[strlen(item.m_sku)+1];
			strcpy(m_name, item.m_name);
			}
		else {
			m_name = nullptr;
		}
	}

	Item& Item::operator=(const Item& item) {
		if (m_sku != nullptr && strcmp(m_sku, "\0") == 0 &&
			m_name != nullptr && strcmp(m_name, "\0") == 0 &&
			m_quantity == 0 && m_price == 0) {
			if (this != &item) {
				m_quantity = item.m_quantity;
				m_taxed = item.m_taxed;
				m_price = item.m_price;
				strcpy(m_sku, item.m_sku);
				if (item.m_name != nullptr) {
					strcpy(m_name, item.m_name);
				}
				else {
					m_name = nullptr;
				}			
			}
			return *this;
		}
		else {
			return *this;
		}
	}

	void Item::sku(char* sku) {
		if (sku != nullptr) {
			strcpy(m_sku, sku);
		}
	}

	void Item::price(double price) {
		m_price = price;
	}

	void Item::name(char* name) {
		if (name != nullptr) {
			m_name = new char[strlen(name) + 1];
			strcpy(m_name, name);
		}
	}

	void Item::taxed(bool taxed) {
		m_taxed = taxed;
	}
	void Item::quantity(int quantity) {
		m_quantity = quantity;
	}

	char*  Item::sku()const {
		char* ch = new char[strlen(m_sku)+1];
		strcpy(ch, m_sku);
		return ch;
	}
	double Item::price()const {
		return m_price;
	}

	char* Item::name()const {
		char* ch = new char[strlen(m_name) + 1];
		strcpy(ch, m_name);
		return ch;
	}

	bool Item::taxed()const {
		return m_taxed;
	}

	int Item::quantity()const {
		return m_quantity;
	}

	double Item::cost()const {
		if (m_taxed) {
			return (1+TAX)*m_price;
		}
		else {
			return m_price;
		}
	}
	bool Item::isEmpty()const {
		return (m_sku != nullptr && strcmp(m_sku, "\0") == 0 &&
			m_name != nullptr && strcmp(m_name, "\0") == 0 &&
			m_quantity == 0 && m_price == 0);
	}

	Item::~Item() {
		
		m_name = nullptr;
	}

	bool Item::operator==( const char* I) {
		if ( strcmp(I, m_sku) == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	int Item::operator+=(int d) {
		return m_quantity +=  d;
	}

	int Item::operator-=(int d) {
		return m_quantity -=  d;
	}

	double operator+=(double& d, const Item& I) {
		d += I.cost()*I.quantity();
		return d;
	}

	ostream& operator<<(ostream& os, const Item& D) {
		D.write(os, true);
		return os;
	}
	istream& operator >> (istream& is, Item& D) {
		D.read(is);
		return is;
	}
}
