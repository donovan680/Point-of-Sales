#define _CRT_SECURE_NO_WARNINGS
// lib includes
#include <iostream>
#include "Perishable.h"

using namespace std;

namespace ict {

	char Perishable::signature()const {
		return 'P';
	}

	Perishable::Perishable() {
		m_expiry.dateOnly(true);
	}


	fstream& Perishable::save(fstream& file)const {
		NonPerishable::save(file);
		file << "," << m_expiry;
		return file;
	}

	fstream& Perishable::load(fstream& file) {
		
		NonPerishable::load(file);
		file.ignore();
		file >> m_expiry;
		return file;
	}

	ostream& Perishable::write(ostream& os, bool linear)const {
		NonPerishable::write(os, linear);
		if (ok() && linear == false) {
			os << "Expiry date: " << m_expiry<< endl;
		}
		return os;
	}

	istream& Perishable::read(istream& is) {
		const char* p;
		cout << "Perishable ";
		NonPerishable::read(is);
		m_expiry.errCode(0);
		if (!is.fail()) {
			cout << "Expiry date (YYYY/MM/DD): ";
			is >> m_expiry;
			if (m_expiry.errCode() == CIN_FAILED) {
				p = "Invalid Date Entry";
				error(p);
				is.setstate(ios::failbit);
			}
			else if (m_expiry.errCode() == YEAR_ERROR) {
				p = "Invalid Year in Date Entry";
				error(p);
				is.setstate(ios::failbit);
			}
			else if (m_expiry.errCode() == MON_ERROR) {
				p = "Invalid Month in Date Entry";
				error(p);
				is.setstate(ios::failbit);
			}
			else if (m_expiry.errCode() == DAY_ERROR) {
				p = "Invalid Day in Date Entry";
				error(p);
				is.setstate(ios::failbit);
			}
		}
		return is;
	}

}
