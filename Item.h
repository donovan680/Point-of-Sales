#ifndef ICT_ITEM_H__
#define ICT_ITEM_H__
// inlcude PosIO and POS header files
#include "POS.h"
#include "PosIO.h"
namespace ict{
  // class Item
	class Item : public PosIO{
		char  m_sku[MAX_SKU_LEN + 1];
		char* m_name;
		double m_price;
		bool m_taxed;
		int m_quantity;
	public:
		Item();
		Item(const char barcode[],  const char* name, double price, bool taxed=true);
		Item(const Item&);
		Item& operator=(const Item& item);
		virtual ~Item();
		void sku(char* sku);
		void price(double);
		void name(char*);
		void taxed(bool);
		void quantity(int);
		char* sku()const;
		double price()const;
		char* name()const;
		bool taxed()const;
		int quantity()const;
		double cost()const;
		bool isEmpty()const;
		// operator += 
		int operator+=(int d);
		int operator-=(int d);
		bool operator==(const char* I);
	};
  // end class Item

	double operator+=(double& d, const Item& I);
  // operator << and operator >>
  std::ostream& operator<<(std::ostream& os, const Item& D);
  std::istream& operator >> (std::istream& is, Item& D);
}


#endif