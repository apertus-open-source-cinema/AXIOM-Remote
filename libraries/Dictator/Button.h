#ifndef Button_h
#define Button_h

#include "Arduino.h"
#include <MI0283QT2.h>

class Button
{
  public:
    Button();
	Button(String Text, int X, int Y, int width, int height);
	
	void SetDimensions(int width, int height);
	void SetPosition(int x, int y);
	int GetWidth();
	int GetHeight();
	int GetXPosition();
	int GetYPosition();
    void SetText(String Text);
	String GetText();
	void SetClicked(boolean click);
	boolean IsClicked();
	void SetHighlighted(boolean hl);
	boolean IsHighlighted();
    void Draw(MI0283QT2 lcd);
    
  private:
    uint16_t m_Width, m_Height, m_PosX, m_PosY;
	String m_Text;
	uint16_t m_ButtonColor, m_ButtonClickedColor, m_ButtonHighlightedColor, m_TextColor;
	boolean m_Highlighted, m_Clicked;
	int m_ButtonRadius;
};

#endif //Button_h
