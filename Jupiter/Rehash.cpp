/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include "Rehash.h"
#include "DLList.h"

/**
* Considerations:
*	It may be worth-while making rehashables aware of which node they're in, or integrating them into their own node?
*/

/** Wrapper class to add rehashable functions. */
class RehashFunction : public Jupiter::Rehashable
{
public:
	int(*function)(void);
	int OnRehash() { return function(); };
	bool OnBadRehash(bool removed) { return removed; };
	RehashFunction(int(*func)(void)) { RehashFunction::function = func; };
};

/** List of Rehashable objects */
Jupiter::DLList<Jupiter::Rehashable> rehashables;

/** List of RehashFunction objects */
Jupiter::DLList<RehashFunction> rehashFunctions;

Jupiter::Rehashable::Rehashable()
{
	rehashables.add(this);
}

Jupiter::Rehashable::Rehashable(const Jupiter::Rehashable &)
{
	rehashables.add(this);
}

Jupiter::Rehashable::~Rehashable()
{
	if (rehashables.size() != 0)
	{
		for (Jupiter::DLList<Jupiter::Rehashable>::Node *n = rehashables.getNode(0); n != nullptr; n = n->next)
		{
			if (n->data == this)
			{
				rehashables.remove(n);
				break;
			}
		}
	}
}

unsigned int Jupiter::rehash()
{
	if (rehashables.size() == 0) return 0;
	unsigned int total = 0;
	int r;
	Jupiter::DLList<Jupiter::Rehashable>::Node *n = rehashables.getNode(0);
	Jupiter::DLList<Jupiter::Rehashable>::Node *d;
	while (n != nullptr)
	{
		r = n->data->OnRehash();
		if (r != 0)
		{
			total++;
			if (r < 0)
			{
				d = n;
				n = n->next;
				if (d->data->OnBadRehash(true))
					delete rehashables.remove(d);
				else rehashables.remove(d);
				continue;
			}
			rehashables.remove(n)->OnBadRehash(false);
		}
		n = n->next;
	}
	return total;
}

unsigned int Jupiter::getRehashableCount()
{
	return rehashables.size();
}

void Jupiter::addOnRehash(int(*function)(void))
{
	rehashFunctions.add(new RehashFunction(function));
}

bool Jupiter::removeOnRehash(int(*function)(void))
{
	if (rehashFunctions.size() == 0) return false;
	for (Jupiter::DLList<RehashFunction>::Node *n = rehashFunctions.getNode(0); n != nullptr; n = n->next)
	{
		if (n->data->function == function)
		{
			delete rehashFunctions.remove(n);
			return true;
		}
	}
	return false;
}

unsigned int Jupiter::removeAllOnRehash()
{
	unsigned int r = rehashFunctions.size();
	while (rehashFunctions.size() != 0) delete rehashFunctions.remove(unsigned int(0));
	return r;
}

// C forward interfaces

extern "C" unsigned int Jupiter_rehash()
{
	return Jupiter::rehash();
}

extern "C" unsigned int Jupiter_getRehashableCount()
{
	return Jupiter::getRehashableCount();
}

extern "C" void Jupiter_addOnRehash(int(*function)(void))
{
	return Jupiter::addOnRehash(function);
}

extern "C" bool Jupiter_removeOnRehash(int(*function)(void))
{
	return Jupiter::removeOnRehash(function);
}

extern "C" unsigned int Jupiter_removeAllOnRehash()
{
	return Jupiter::removeAllOnRehash();
}