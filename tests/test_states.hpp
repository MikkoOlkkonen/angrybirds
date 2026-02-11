#pragma once

#include <iostream>
#include "menustate.hpp"

void testMenuButtonInit() {
    MenuState menu;
    std::vector<std::shared_ptr<Button>> buttons;
    buttons = menu.getButtons();
    if (buttons.size() == 5) {
        std::cout << "testButtonInit amount of buttons success!" << std::endl;
    } else { std::cout << "testButtonInit failed due to wrong button count" << std::endl; }
    if (buttons[0]->getAction() == std::pair<std::string,int>("open", 1)
    && buttons[4]->getAction() == std::pair<std::string,int>("sandbox", 0)) {
        std::cout << "testButtonInit buttons have right actions, success!" << std::endl;
    } else { std::cout << "testButtonInit buttons have right actions, success!" << std::endl; }
}

/*
void testMenuButtonClickRelease() {
    MenuState menu;
    sf::Event event;
    sf::Vector2i clickPosition(1366 / 2 - 175, 768 / 2 + 50);
    // event for clicking
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = sf::Mouse::Left;
    event.mouseButton.x = clickPosition.x;
    event.mouseButton.y = clickPosition.y;
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Test Window");
    sf::View view;
    window.setView(view);
    int zoom;
    // process click
    menu.processEvent(event, window, view, zoom);
    if (menu.getButtons()[1]->getShape().getFillColor() == sf::Color(220, 0, 0, 255)) {
        std::cout << "Menu Button click success!" << std::endl;
    } else { std::cout << "Menu button click changes no color, failure!" << std::endl; } 
    // event for release
    event.type = sf::Event::MouseButtonReleased;
    std::pair<std::string, int> action;
    // process release
    action = menu.processEvent(event, window, view, zoom);
    if (action.first == "open" && action.second == 2) {
        std::cout << "Menu button release success!" << std::endl;
    } else { std::cout << "Menu button returns wrong action, failure!" << std::endl; } 
}

void testMenuButtonHover() {
    MenuState menu;
    sf::Event event;
    sf::Vector2i position(1366 / 2 - 175, 768 / 2 + 50);
    event.type = sf::Event::MouseMoved;
    event.mouseButton.x = position.x;
    event.mouseButton.y = position.y;
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Test Window");
    sf::View view(sf::FloatRect(0, 0, 1366, 768));
    window.setView(view);
    int zoom;
    std::pair<std::string, int> action;
    action = menu.processEvent(event, window, view, zoom);
    if (menu.getButtons()[1]->getShape().getFillColor() == sf::Color::Red) {
        std::cout << "Menu button hover success!" << std::endl;
    } else { std::cout << "Menu button hover changes no color, failure!" << std::endl; } 
}
*/