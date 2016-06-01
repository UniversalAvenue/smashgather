#ifndef CHARACTERDETAILS_H
#define CHARACTERDETAILS_H

#include <iostream>

class CharacterDetails {
public:
  CharacterDetails(std::string _name): name(_name) {}
  std::string name;
};

static const CharacterDetails FALCON("Falcon");
static const CharacterDetails DK("DK");
static const CharacterDetails FOX("Fox");
static const CharacterDetails JIGGLYPUFF("Jigglypuff");
static const CharacterDetails KIRBY("Kirby");
static const CharacterDetails LINK("Link");
static const CharacterDetails LUIGI("Luigi");
static const CharacterDetails MARIO("Mario");
static const CharacterDetails NESS("Ness");
static const CharacterDetails PIKACHU("Pikachu");
static const CharacterDetails SAMUS("Samus");
static const CharacterDetails YOSHI("Yoshi");
static const CharacterDetails CHARACTERS[] = {
  FALCON,
  DK,
  FOX,
  JIGGLYPUFF,
  KIRBY,
  LINK,
  LUIGI,
  MARIO,
  NESS,
  PIKACHU,
  SAMUS,
  YOSHI,
};

#endif
