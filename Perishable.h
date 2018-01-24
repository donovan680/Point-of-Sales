#ifndef ICT_PERISHABLE_H__
#define ICT_PERISHABLE_H__

#include "Date.h"
#include "Item.h"
#include "Error.h"
#include <fstream>
#include "PosIO.h"
#include "NonPerishable.h"

namespace ict {
	class Perishable : public NonPerishable {
		Date m_expiry;
	protected:
		char signature()const;
	public:
		 Perishable();
		 std::fstream& save(std::fstream& file)const;
		 std::fstream& load(std::fstream& file);
		 std::ostream& write(std::ostream& os, bool linear)const;
		 std::istream& read(std::istream& is);
	};

}



#endif
