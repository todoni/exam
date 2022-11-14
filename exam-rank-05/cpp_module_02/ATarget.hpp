#ifndef ATARGET_HPP
# define ATARGET_HPP

# include <string>
# include <iostream>

class	ASpell;

class	ATarget {

protected:
	std::string	type;
	
public:
	ATarget(const std::string& type);
	ATarget();
	ATarget(const ATarget& ref);
	ATarget&	operator=(const ATarget& ref);
	virtual ~ATarget();
	const std::string&	getType(void) const;
	virtual ATarget*		clone(void) = 0;
	void	getHitBySpell(const ASpell& aSpell) const;

};

#endif
