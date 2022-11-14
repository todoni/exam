#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

# include <string>
# include <iostream>
# include <map>

class	ASpell;

class	SpellBook {

private:
	std::map<std::string, ASpell*> spells;
	SpellBook(const SpellBook& ref);
	SpellBook&	operator=(const SpellBook& ref);
	
public:
	SpellBook();
	~SpellBook();

	void				learnSpell(ASpell* spell);
	void				forgetSpell(const std::string& spellName);
	ASpell*				createSpell(const std::string& spellName);

};

#endif
