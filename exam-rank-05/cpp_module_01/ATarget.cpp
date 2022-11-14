#include "ATarget.hpp"
#include "ASpell.hpp"

ATarget::ATarget(const std::string& type)
	:type(type) {}

ATarget::ATarget() {}

ATarget::~ATarget() {}

ATarget::ATarget(const ATarget& ref)
{
	*this = ref;	
}

ATarget&	ATarget::operator=(const ATarget& ref)
{
	if (this != &ref)
	{
		this->type = ref.getType();
	}
	return (*this);
}

const std::string& ATarget::getType(void) const
{
	return (this->type);
}

void	ATarget::getHitBySpell(const ASpell& aSpell) const
{
	std::cout << type << " has been " << aSpell.getEffects() << "!" << std::endl;
}
