#ifndef BUTTON_H
#define BUTTON_H

class Button {
 
 public:
  void Init(void);
  char IsPressed(void);
  char IsReleased(void);
  char TurnOffGlo(void);
  void WaitForResponse(void);
  void ChangeTier(void);

  unsigned int time;
  char tierChosen;
  char tier;
  char maxTier;

};

#endif
