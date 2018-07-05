#pragma once
#include <functional>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "BintrsTools.h"
using namespace std;
using namespace sf;

extern float x100;
extern float y100;

class MenuObject {
public:
    MenuObject* neighborL = NULL;
    MenuObject* neighborR = NULL;
    MenuObject* neighborU = NULL;
    MenuObject* neighborD = NULL;
    bool pressed = false;
    function<void()> func = NULL;
    virtual ~MenuObject() {}
    virtual void animate() {}
    virtual void render() {}
};

class TextObject : public MenuObject {
    bool centered = false;
public:
    sf::Text text;
    sf::Font font;
    sf::Color color;
    int fontSize;
    string str;
    float x, y;
    float getW() { return text.getLocalBounds().width; }
    float getH() { return text.getLocalBounds().height; }
    TextObject() {}
    TextObject(string str, float x = 0, float y = 0,
               Color color = sf::Color::Green, int fontSize = 24) {
        this->str = str;
        this->x = x;
        this->y = y;
        this->color = color;
        this->fontSize = fontSize;
        font.loadFromFile("resources/fonts/expressway.ttf");
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(fontSize);
        text.setFillColor(color);
        text.setStyle(sf::Text::Bold);// | sf::Text::Underlined);
        text.setPosition(x, y);
    }
    void center() { centered = true; }
    void render() {
        if (centered) {
            text.setPosition(x - text.getLocalBounds().width / 2, y - text.getLocalBounds().height / 2);
            window->draw(text);
        }
        else {

            //text.setPosition(x,y);
            window->draw(text);
        }
    }
};

extern vec screenMouse;
extern vec globalMouse;
extern bool justPressedLeftClick;
extern bool justReleasedLeftClick;
extern bool justPressedRightClick;
extern bool justReleasedRightClick;
extern bool leftAction;
extern bool rightAction;
class Button;
extern MenuObject* activeMenuObj;


class Button : public MenuObject {
    RectangleShape disp_rect;
    bool clicked = false;
public:


    bool mouseOver = false;
    TextObject* textObject;
    sf::Color color1;
    sf::Color color2;
    int fontSize;
    float x, y, w, h;
    float activity = 0;
    ~Button() {
        delete textObject;
    }
    Button(string str,
           function<void()> func,
           float x, float y,
           float w, float h,
           Color color1 = Color(100, 0, 0),
           Color color2 = Color(200, 0, 0)) {
        this->func = func;
        this->x = x * x100;
        this->y = y * y100;
        this->w = w * x100;
        this->h = h * y100;
        this->color1 = color1;
        this->color2 = color2;
        float fontSize = 24;
        this->textObject = new TextObject(str, this->x, this->y, Color::White, fontSize);
        disp_rect.setPosition(this->x + this->w / 2, this->y + this->h / 2);
        disp_rect.setOutlineColor(Color(0, 0, 0));
        disp_rect.setSize(Vector2f(this->w, this->h));
        disp_rect.setOutlineThickness(3);
        float txtW = textObject->text.getLocalBounds().width;
        float txtH = textObject->text.getLocalBounds().height;
        textObject->text.setOrigin(txtW / 2, txtH / 2);
        textObject->text.setPosition(this->x + this->w / 2.f, this->y + this->h / 2.f);
    }
    void animate() {
        if (activeMenuObj == this) {
            if (pressed || clicked)
                activity *= (1 - .5);
            else
                activity += (1 - activity)*.15;
        }
        else activity *= (1 - .15);
    }
    void render() {
        // drawing
        disp_rect.setFillColor(interpColor(color2, color1, activity));
        disp_rect.setOutlineColor(interpColor(Color(255, 255, 255), Color(0, 0, 0), activity));
        textObject->text.setFillColor(interpColor(Color(255, 255, 255), Color(180, 180, 180), activity));
        float growth = .1;
        float size = 1 + (activity*growth);
        disp_rect.setPosition(x - activity*w*growth*.5, y - activity*h*growth*.5);
        disp_rect.setScale(size, size);
        window->draw(disp_rect);

        textObject->text.setScale(size, size);
        textObject->render();

        // logic
        if (pointRectCollision(screenMouse.x,
                               screenMouse.y,
                               x, y, w, h)) {
            activeMenuObj = this;
            mouseOver = true;
            if (justPressedLeftClick || justPressedRightClick) {
                justPressedLeftClick = false;
                justPressedRightClick = false;
                clicked = true;
            }
            if (clicked && (justReleasedLeftClick || justReleasedRightClick)) {
                clicked = false;
                if (func) {
                    func();
                }
            }
        }
        else {
            if (mouseOver) activeMenuObj = NULL;
            mouseOver = false;
            //pressed = false;
            clicked = false;
        }
    }
};

class MenuBox : public MenuObject {

public:
    RectangleShape disp_rect;
    MenuBox(int x, int y, int w, int h, Color color) {
        disp_rect.setPosition(x, y);
        disp_rect.setSize(Vector2f(w, h));
        disp_rect.setFillColor(color);
    }
    void render() {
        window->draw(disp_rect);
    }
};

// horizontal menu slider object
class MenuSlider : public MenuObject {
    CircleShape disp_circ;
    RectangleShape disp_line;
    RectangleShape disp_slider;
    RectangleShape disp_popup;
    ConvexShape disp_popup_point;
public:
    float* linkedValue;
    float minValue, maxValue;
    string prompt;
    string units;
    float x, y, w, h;
    float activity = 0;
    bool clicked = false;
    bool mouseOver = false;
    float sliderW = 8;
    float sliderH = 16;
    function<void()> moveFunc;
    MenuSlider(string prompt, string units,
               float x, float y, float w, float h,
               float* linkedValue,
               float minValue, float maxValue,
               function<void()> releaseFunc = NULL,
               function<void()> moveFunc = NULL) {
        this->prompt = prompt;
        this->units = units;
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->linkedValue = linkedValue;
        this->minValue = minValue;
        this->maxValue = maxValue;
        this->func = releaseFunc;
        this->moveFunc = moveFunc;
        disp_circ.setRadius(4);
        disp_circ.setOrigin(4, 4);
        disp_circ.setFillColor(Color::Black);
        disp_line.setFillColor(Color::Black);
        disp_line.setPosition(x, y);
        disp_line.setSize(Vector2f(w, 2));
        disp_line.setOrigin(0, 1);
        disp_slider.setSize(Vector2f(sliderW, sliderH));
        //disp_slider.setOrigin(4, 8); 
        disp_slider.setOutlineThickness(2);

        disp_popup_point.setPointCount(3);
        disp_popup_point.setPoint(0, Vector2f(-4, 0));
        disp_popup_point.setPoint(1, Vector2f(4, 0));
        disp_popup_point.setPoint(2, Vector2f(0, 6));
        disp_popup_point.setOrigin(0, 30);
    }
    void animate() {
        if (activeMenuObj == this) {
            activity += (1 - activity)*.15;
        }
        else activity *= (1 - .15);
    }
    void render() {

        window->draw(disp_line);
        disp_circ.setPosition(x, y);
        window->draw(disp_circ);
        disp_circ.setPosition(x + w, y);
        window->draw(disp_circ);
        float sliderPos = 1 - ((maxValue - minValue) - (*linkedValue - minValue)) / float(maxValue - minValue);
        float sliderX = x + (sliderPos*w);
        float sliderY = y;
        float sliderTLX = sliderX - sliderW / 2;
        float sliderTLY = sliderY - sliderH / 2;
        TextObject promptTxt(prompt, x + w / 2, y - 24, Color::White, 16);
        promptTxt.center();
        //TextObject promptTxt(prompt, x - 10, y, Color::White, 16);
        //promptTxt.text.setPosition( promptTxt.x - promptTxt.getW(), promptTxt.y - promptTxt.getH()/2);
        promptTxt.render();
        TextObject textObj(to_string(int(*linkedValue)) + units, sliderX, sliderY - 45,
                           interpColor(Color(0, 0, 0), Color(0, 0, 0, 0), activity), 16);
        textObj.center();
        int txtW = textObj.getW() + 8;
        int txtH = textObj.getH();

        //float sliderW = ;
        //float sliderH = ;
        //say(sliderPos << " " << *linkedValue);
        disp_slider.setFillColor(interpColor(Color(255, 255, 255), Color(180, 180, 180), activity));
        disp_slider.setOutlineColor(interpColor(Color(255, 255, 255), Color(0, 0, 0), activity));
        disp_slider.setPosition(sliderTLX, sliderTLY);
        window->draw(disp_slider);

        disp_popup.setSize(Vector2f(txtW, 20));
        disp_popup.setOrigin(txtW / 2, 50);
        disp_popup.setPosition(sliderX, sliderY);
        disp_popup.setFillColor(interpColor(Color(255, 255, 255), Color(255, 255, 255, 0), activity));

        window->draw(disp_popup);
        disp_popup_point.setPosition(sliderX, sliderY);
        disp_popup_point.setFillColor(interpColor(Color(255, 255, 255), Color(255, 255, 255, 0), activity));
        window->draw(disp_popup_point);
        float mousePercentage = (screenMouse.x - x) / w;
        textObj.render();
        // logic
        if (justReleasedLeftClick && clicked) {
            if (func) func();
            clicked = false;
        }
        if (clicked) {
            *linkedValue = minValue + (mousePercentage*(maxValue - minValue));
            clamp(linkedValue, minValue, maxValue);
            if (moveFunc) moveFunc();
        }
        clamp(linkedValue, minValue, maxValue);
        if (pointRectCollision(screenMouse.x,
                               screenMouse.y,
                               x, y - 10, w, 20)) {
            activeMenuObj = this;
            mouseOver = true;

            if (justPressedLeftClick || justPressedRightClick) {
                justPressedLeftClick = false;
                justPressedRightClick = false;
                clicked = true;
            }
        }
        else if (!clicked) {
            if (mouseOver) activeMenuObj = NULL;
            mouseOver = false;
            //clicked = false;
        }
    }
};