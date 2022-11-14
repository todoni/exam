#ifndef ASPELL_HPP
# define ASPELL_HPP

# include <string>
# include <iostream>

class	ATarget;

class	ASpell {

protected:
	std::string	name;
	std::string	effects;
	
public:
	ASpell(const std::string& name, const std::string& effect);
	ASpell();
	ASpell(const ASpell& ref);
	ASpell&	operator=(const ASpell& ref);
	virtual ~ASpell();
	const std::string&	getName(void) const;
	const std::string&	getEffects(void) const;
	virtual ASpell*		clone(void) = 0;
	void	launch(const ATarget& aTarget);

};

#endif
