#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <ctime>
#include "Date.h"
#include "POS.h"
using namespace std;

namespace ict {
  void Date::set(){
    time_t t = time(NULL);
    tm lt = *localtime(&t);
    m_day = lt.tm_mday;
    m_mon = lt.tm_mon + 1;
    m_year = lt.tm_year + 1900;
    if (dateOnly()){
      m_hour = m_min = 0;
    }
    else{
      m_hour = lt.tm_hour;
      m_min = lt.tm_min;
    }
  }

  int Date::value()const{
    return m_year * 535680 + m_mon * 44640 + m_day * 1440 + m_hour * 60 + m_min;
  }

  int Date::mdays()const{
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
    int mon = m_mon >= 1 && m_mon <= 12 ? m_mon : 13;
    mon--;
    return days[mon] + int((mon == 1)*((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0));
  }

  void Date::errCode(int errorCode) {
	  if (errorCode >= 0 && errorCode <= 6) {
		  m_readErrorCode = errorCode;
	  }
  }
  
  void Date::set(int year, int mon, int day, int hour, int min) {
	  m_year = year;
	  m_mon = mon;
	  m_day = day;
	  m_hour = hour;
	  m_min = min;
	  m_readErrorCode = NO_ERROR;
  }

	Date::Date() {
		m_dateOnly = false;
		set();
	}

	Date::Date(int year, int mon, int day) {
		m_dateOnly = true;
		m_year = year;
		m_mon = mon;
		m_day = day;
		m_readErrorCode = NO_ERROR;
	}

	Date::Date(int year, int mon, int day, int hour, int min) {
		m_dateOnly = false;
		m_year = year;
		m_mon = mon;
		m_day = day;
		m_hour = hour;
		m_min = min;
		m_readErrorCode = NO_ERROR;
	}

	bool Date::operator==(const Date& D)const {
		return (this->value() == D.value());
	}
	bool Date::operator!=(const Date& D)const {
		return (this->value() != D.value());
	}
	bool Date::operator<(const Date& D)const {
		return (this->value() < D.value());
	}
	bool Date::operator>(const Date& D)const {
		return (this->value() > D.value());
	}
	bool Date::operator<=(const Date& D)const {
		return (this->value() <= D.value());
	}
	bool Date::operator>=(const Date& D)const {
		return (this->value() >= D.value());
	}

	int Date::errCode()const {
		return m_readErrorCode;
	}

	bool Date::bad()const {
		return m_readErrorCode != 0;
	}

	bool Date::dateOnly()const {
		return m_dateOnly;
	}
	
	void Date::dateOnly(bool value) {
		m_dateOnly = value;
		if (value == true) {
			m_hour = 0;
			m_min = 0;
		}
	}

	std::istream& Date::read(std::istream & is ) {
		char temp;
		
		if (m_dateOnly == true) {
			is >> m_year >> temp >> m_mon >> temp >> m_day;
		}
		else {
			is >> m_year >> temp >> m_mon >> temp >> m_day >> temp >> m_hour >> temp >> m_min;
		}
		if (is.fail()) {
			m_readErrorCode = CIN_FAILED;
		}
		else if (m_year < MIN_YEAR || m_year > MAX_YEAR) {
			m_readErrorCode = YEAR_ERROR;
		}
		else if (m_mon > 12 || m_mon < 1) {
			m_readErrorCode = MON_ERROR;
		}
		else if (m_day >mdays() || m_day<=0) {
			m_readErrorCode = DAY_ERROR;
		}
		else if (m_hour < 0 || m_hour >23) {
			m_readErrorCode = HOUR_ERROR;
		}
		else if (m_min < 0 || m_min>59) {
			m_readErrorCode = MIN_ERROR;
		}
		return is;
	}
		ostream& Date::write(ostream& ostr )const {
			if (m_dateOnly && m_mon<10) {
				if (m_day > 10) {
					ostr << m_year << "/" <<0<< m_mon << "/" << m_day;
				}
				else {
					ostr << m_year << "/" << 0 << m_mon << "/" <<0<< m_day;
				}
			}
			else if (m_dateOnly && m_mon > 10) {
				if (m_day > 10) {
					ostr << m_year << "/" <<  m_mon << "/" << m_day;
				}
				else {
					ostr << m_year << "/" << m_mon << "/" << 0 << m_day;
				}
			}
			else {
				if (m_mon > 10) {
					if (m_day > 10) {
						ostr << m_year << "/" << m_mon << "/" << m_day << ", " << m_hour << ":" << m_min;
					}
					else {
						ostr << m_year << "/" << m_mon << "/" << 0 << m_day << ", " << m_hour << ":" << m_min;
					}
				}
				else {
					if (m_day > 10) {
						ostr << m_year << "/" << 0<<m_mon << "/" << m_day << ", " << m_hour << ":" << m_min;
					}
					else {
						ostr << m_year << "/" <<0<< m_mon << "/" << 0 << m_day << ", " << m_hour << ":" << m_min;
					}
				}
			}
			return ostr;
		}

		ostream& operator<<(ostream& os, const Date& D) {
			D.write(os);
			return os;
		}
		istream& operator >> (istream& is, Date& D) {
			D.read(is);
			return is;
		}
	}
