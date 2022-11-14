#include "Warlock.hpp"
#include "ASpell.hpp"
#include "ATarget.hpp"

Warlock::Warlock(const std::string& name, const std::string& title)
	:name(name), title(title)
{
	std::cout << name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock()
{
	std::cout << name << ": My job here is done!" << std::endl;
}

const std::string& Warlock::getName(void) const
{
	return (this->name);
}

const std::string& Warlock::getTitle(void) const
{
	return (this->title);
}

void	Warlock::setTitle(const std::string &title)
{
	this->title = title;
}

void	Warlock::introduce(void) const
{
	std::cout << this->name << ": I am " << this->name << ", " << this->title << "!" << std::endl;
}

void	Warlock::learnSpell(ASpell* spell)
{
	spellBook.learnSpell(spell);
}

void	Warlock::forgetSpell(const std::string& spellName)
{
	spellBook.forgetSpell(spellName);
}

void	Warlock::launchSpell(const std::string& spellName, const ATarget& aTarget)
{
	ASpell* spell = spellBook.createSpell(spellName);
	spell->launch(aTarget);
}
