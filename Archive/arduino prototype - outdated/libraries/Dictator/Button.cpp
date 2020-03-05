#include "Button.h"
#include "Arduino.h"
#include <MI0283QT2.h>

//-------------------- Constructor --------------------

Button::Button(void){
	m_Text = "";
	m_Width = 10;
	m_Height = 10;
	m_PosX = 0;
	m_PosY = 0;
	m_TextColor = RGB( 255, 255, 255);
	m_ButtonClickedColor = RGB( 255, 150, 255);
	m_ButtonHighlightedColor = RGB( 255, 0, 255);
	m_ButtonColor = RGB( 0, 0, 0);
	m_Clicked = false;
	m_Highlighted = false;
	m_ButtonRadius = 5;
}

Button::Button(String Text, int X, int Y, int width, int height) {
	m_Text = Text;
	m_Width = width;
	m_Height = height;
	m_PosX = X;
	m_PosY = Y;
	m_TextColor = RGB( 255, 255, 255);
	m_ButtonClickedColor = RGB( 255, 150, 255);
	m_ButtonHighlightedColor = RGB( 255, 0, 255);
	m_ButtonColor = RGB( 0, 0, 0);
	m_Clicked = false;
	m_Highlighted = false;
	m_ButtonRadius = 5;
}

//-------------------- Public --------------------

void Button::SetText(String Text) {
	m_Text = Text;
}

String Button::GetText() {
	return m_Text;
}

void Button::SetPosition(int x, int y) {
	m_PosX = x;
	m_PosY = y;
}

int Button::GetXPosition() {
	return m_PosX;
}

int Button::GetYPosition() {
	return m_PosX;
}

void Button::SetDimensions(int width, int height) {
	m_Width = width;
	m_Height = height;
}

int Button::GetWidth() {
	return m_Width;
}

int Button::GetHeight() {
	return m_Height;
}

void Button::SetClicked(boolean click) {
	m_Clicked = click;
}

boolean Button::IsClicked() {
	return m_Clicked;
}

void Button::SetHighlighted(boolean hl) {
	m_Highlighted = hl;
}

boolean Button::IsHighlighted() {
	return m_Highlighted;
}

void Button::Draw(MI0283QT2 lcd) {
	uint16_t drawColor = 0;
	if (m_Clicked) {
		drawColor = m_ButtonClickedColor;
	} else if (m_Highlighted) {
		drawColor = m_ButtonHighlightedColor;
	} else {
		drawColor = m_ButtonColor;
	}
	
	lcd.fillCircle(m_PosX+m_ButtonRadius, m_PosY+m_ButtonRadius, m_ButtonRadius, drawColor);
	lcd.fillCircle(m_PosX+m_Width-m_ButtonRadius, m_PosY+m_ButtonRadius, m_ButtonRadius, drawColor);
	lcd.fillCircle(m_PosX+m_ButtonRadius, m_PosY+m_Height-m_ButtonRadius, m_ButtonRadius, drawColor);
	lcd.fillCircle(m_PosX+m_Width-m_ButtonRadius, m_PosY+m_Height-m_ButtonRadius, m_ButtonRadius, drawColor);

	lcd.fillRect(m_PosX, m_PosY+m_ButtonRadius, m_PosX+m_ButtonRadius, m_PosY+m_Height-m_ButtonRadius, drawColor);
	lcd.fillRect(m_PosX+m_ButtonRadius, m_PosY, m_PosX+m_Width-m_ButtonRadius, m_PosY+m_Height, drawColor);
	lcd.fillRect(m_PosX+m_Width-m_ButtonRadius, m_PosY+m_ButtonRadius, m_PosX+m_Width, m_PosY+m_Height-m_ButtonRadius, drawColor);

	lcd.drawText(m_PosX+10, m_PosX+(m_Height/2)-5, m_Text, 1, m_TextColor, drawColor);// TODO: horizontal text centering
}
