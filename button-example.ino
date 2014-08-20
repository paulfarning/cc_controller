class Button {
private:
  byte down_at;
  byte down;

public:
  Button();
  byte downFor();
  void resetDown();
  bool isDown();
  void turnOn();
  void turnOff();
};

Button::Button()
{
  this->down_at = 0;
  this->down = 0;
}

// Find the number of beats the button has been down for. Function can compute max. 8 sec down before rolling over.
byte Button::downFor()
{
  byte time;
  if (this->down_at > sc_beat)
  {
   time = (sc_beat + 256) - this->down_at;
  }
  else
  {
    time = sc_beat - this->down_at;
  }
  return time;
}

bool Button::isDown()
{
  return (bool)this->down;
}

void Button::resetDown()
{
  this->down_at = sc_beat;
  this->down = 1;
}

void Button::turnOn()
{
  if (!this->down)
  {
    this->down_at = sc_beat;
    this->down = 1;
  }
  return;
}

void Button::turnOff()
{
  this->down = 0;
  return;
}
