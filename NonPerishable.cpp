#define _CRT_SECURE_NO_WARNINGS
// lib includes
#include "NonPerishable.h"
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;



namespace ict {


	bool NonPerishable::ok() const {
		return m_err.isClear();
	}

	void NonPerishable::error(const char* message) {
		 m_err.message(message);
	}

	char NonPerishable::signature()const {
		return 'N';
	}

	fstream& NonPerishable::save(fstream& file)const {
		if (file.is_open()) {	
			file << signature() << "," << sku() << "," << name() << "," << price() << "," << taxed() << "," << quantity();
		}
		
		return file;
	}

	fstream& NonPerishable::load(fstream& file) {
		double dbuf;
		int ibuff;
		char buf[2000];
		if (file.is_open()) {
			file.getline(buf, 7, ',');	
			sku(buf);
			file.getline(buf, 21, ',');
			name(buf);
			file >> dbuf;
			price(dbuf);
			file.ignore(1, ',');
			file >> ibuff;
			if (ibuff == 1) {
				taxed(true);
			}
			else {
				taxed(false);
			}
			file.ignore(1, ',');
			file >> ibuff;
			quantity(ibuff);
		}
		return file;
	}

	ostream& NonPerishable::write(ostream& os, bool linear)const {
		if (ok()) {
			if (linear) {
				if (taxed()) {
					os << left << setw(MAX_SKU_LEN) << sku() << "|"
						<< left << setw(20) << name() << "|";
						os<< right << setw(7)<<fixed << setprecision(2) << price()<<"|"
						<<  " T" <<signature() << "|"
						<< right << setw(4) << quantity() << "|"
						<< right << setw(9) << fixed << setprecision(2) << quantity()*cost() << "|"<<endl;
				}
				else {
					os << left << setw(MAX_SKU_LEN) << sku() << "|"
						<< left << setw(20) << name() << "|";
					os << right << setw(7) << fixed << setprecision(2) << price() << "|"
						<< "  " << signature() << "|";
					os << right << setw(4) << quantity() << "|";
						os<< right << setw(9) << fixed << setprecision(2) << quantity()*cost() << "|" << endl;
				}
			}
			else {
				if (taxed()) {
					os << "Name:" << endl
						<< left  << name() << endl
						<< "Sku: " << sku() << endl
						<< "Price: " << fixed << setprecision(2) << price() << endl
						<< "Price after tax: "  << cost() << endl
						<< "Quantity: " << quantity() << endl
						<< "Total Cost: " << fixed << setprecision(2) << quantity()*cost() << endl;
				}
				else {
					os << "Name:"  << endl
						<<left<<name()<<endl
						<< "Sku: " << sku() << endl
						<< "Price: " << fixed << setprecision(2) << price() << endl
						<< "Price after tax: " << "N/A" << endl
						<< "Quantity: " << quantity() << endl
						<< "Total Cost: " << fixed << setprecision(2) << quantity()*cost() << endl;
				}
			}
			return os;
		}
		else {
			os << m_err;
			return os;
		}
	}

	istream& NonPerishable::read(istream& is) {
		char buf[2000];
		double dbuf;
		int ibuf;
		const char* p;
		m_err.clear();
		cout << "Item Entry:" << endl;
			cout << "Sku: ";
			is >> buf;
			sku(buf);
			is.ignore();
			cout << "Name:"<<endl;
			is.getline(buf, 2000);
			name(buf);
			cout << "Price: ";
			is >> dbuf;
			if (is.fail()) {
				 p="Invalid Price Entry";
				 error(p);
			}
			else {
				price(dbuf);
				cout << "Taxed: ";
				is >> buf;
				if (buf[0] != 'y' && buf[0] != 'n') {
					p="Invalid Taxed Entry, (y)es or (n)o";
					error(p);
					is.setstate(ios::failbit);
				}
				else {
					bool temp = true;
					if (buf[0] == 'y') {
						taxed(temp);
					}
					else {
					 temp = false;
						taxed(temp);
					}
					cout << "Quantity: ";
					is >>ibuf;
					if (is.fail()) {
						p= "Invalid Quantity Entry";
						error(p);
						return is;
					}
					else {
						quantity(ibuf);
					}
				}
			}
			return is;
		
	}
}
