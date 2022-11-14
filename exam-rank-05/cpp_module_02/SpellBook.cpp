#include "SpellBook.hpp"
#include "ASpell.hpp"
#include "ATarget.hpp"

SpellBook::SpellBook() {}
SpellBook::~SpellBook()
{
}

void	SpellBook::learnSpell(ASpell* spell)
{
	spells[spell->getName()] = spell;
}

void	SpellBook::forgetSpell(const std::string& spellName)
{
	try
	{
		spells.erase(spellName);	
	}
	catch (std::exception& e)
	{

	}
}

ASpell*	SpellBook::createSpell(const std::string& spellName)
{
	return spells[spellName];
}
