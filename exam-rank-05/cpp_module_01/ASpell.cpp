#include "ASpell.hpp"
#include "ATarget.hpp"

ASpell::ASpell(const std::string& name, const std::string& effects)
	:name(name), effects(effects) {}

ASpell::ASpell() {}

ASpell::~ASpell() {}

ASpell::ASpell(const ASpell& ref)
{
	*this = ref;	
}

ASpell&	ASpell::operator=(const ASpell& ref)
{
	if (this != &ref)
	{
		this->name = ref.getName();
		this->effects = ref.getEffects();
	}
	return (*this);
}

const std::string& ASpell::getName(void) const
{
	return (this->name);
}

const std::string& ASpell::getEffects(void) const
{
	return (this->effects);
}

void	ASpell::launch(const ATarget &aTarget)
{
	aTarget.getHitBySpell(*this);
}
