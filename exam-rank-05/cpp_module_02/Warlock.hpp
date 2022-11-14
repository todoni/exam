#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <string>
# include <iostream>
# include <map>
# include "SpellBook.hpp"

class	ASpell;
class	ATarget;
class	SpellBook;

class	Warlock {

private:
	std::string	name;
	std::string	title;
	SpellBook spellBook;
	Warlock();
	Warlock(const Warlock& ref);
	Warlock&	operator=(const Warlock& ref);
	
public:
	Warlock(const std::string& name, const std::string& title);
	~Warlock();
	const std::string&	getName(void) const;
	const std::string&	getTitle(void) const;
	void				setTitle(const std::string& title);

	void				introduce(void) const;

	void				learnSpell(ASpell* spell);
	void				forgetSpell(const std::string& spellName);
	void				launchSpell(const std::string& spellName, const ATarget& aTarget);


};

#endif
