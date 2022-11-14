#include "TargetGenerator.hpp"
#include "ASpell.hpp"
#include "ATarget.hpp"

TargetGenerator::TargetGenerator() {}

TargetGenerator::~TargetGenerator()
{
}

void	TargetGenerator::learnTargetType(ATarget* aTarget)
{
	targets[aTarget->getType()] = aTarget;
}

void	TargetGenerator::forgetTargetType(const std::string& type)
{
	try
	{
		targets.erase(type);	
	}
	catch (std::exception& e)
	{

	}
}

ATarget*	TargetGenerator::createTarget(const std::string& type)
{
	return targets[type];
}
