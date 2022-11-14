#ifndef TARGETGENERATOR_HPP
# define TARGETGENERATOR_HPP

# include <string>
# include <iostream>
# include <map>

class	ATarget;

class	TargetGenerator {

private:
	std::map<std::string, ATarget*> targets;
	TargetGenerator(const TargetGenerator& ref);
	TargetGenerator&	operator=(const TargetGenerator& ref);
	
public:
	TargetGenerator();
	~TargetGenerator();

	void				learnTargetType(ATarget* aTarget);
	void				forgetTargetType(const std::string& type);
	ATarget*				createTarget(const std::string& type);

};

#endif
