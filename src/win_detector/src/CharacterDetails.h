#ifndef CHARACTERDETAILS_H
#define CHARACTERDETAILS_H

#include <iostream>

class CharacterDetails {
public:
  CharacterDetails(std::string _name, std::string _template_filename): name(_name), template_filename(_template_filename) {}
  std::string name;
  std::string template_filename;
};

static const CharacterDetails FALCON("Falcon", "img/templates/cfalcon_template.jpg");
static const CharacterDetails DK("DK", "img/templates/dk_template.jpg");
static const CharacterDetails FOX("Fox", "img/templates/fox_template.jpg");
static const CharacterDetails JIGGLYPUFF("Jigglypuff", "img/templates/jigglypuff_template.jpg");
static const CharacterDetails KIRBY("Kirby", "img/templates/kirby_template.jpg");
static const CharacterDetails LINK("Link", "img/templates/link_template.jpg");
static const CharacterDetails MARIO("Mario", "img/templates/mario_template.jpg");
static const CharacterDetails NESS("Ness", "img/templates/ness_template.jpg");
static const CharacterDetails PIKACHU("Pikachu", "img/templates/pikachu_template.jpg");
static const CharacterDetails SAMUS("Samus", "img/templates/samus_template.jpg");
static const CharacterDetails YOSHI("Yoshi", "img/templates/yoshi_template.jpg");
static const CharacterDetails CHARACTERS[] = {
  FALCON,
  DK,
  FOX,
  JIGGLYPUFF,
  KIRBY,
  LINK,
  MARIO,
  NESS,
  PIKACHU,
  SAMUS,
  YOSHI,
};

#endif
