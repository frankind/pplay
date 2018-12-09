//
// Created by cpasjuste on 20/10/18.
//

#include "cross2d/c2d.h"
#include "main.h"
#include "menu.h"

using namespace c2d;

#define ICON_SIZE 24
#define BUTTON_HEIGHT 64


MenuButton::MenuButton(Main *main, const MenuItem &item, const FloatRect &rect) : RectangleShape(rect) {

    this->item = item;
    setFillColor(Color::Transparent);
    setOrigin(Origin::Left);

    if (!item.icon.empty()) {
        icon = new C2DTexture(main->getIo()->getDataReadPath() + "skin/" + item.icon);
        icon->setOrigin(Origin::Left);
        icon->setPosition(16, getSize().y / 2);
        icon->setFillColor(COLOR_FONT);
        add(icon);
    }

    name = new Text(item.name, FONT_SIZE - 2, main->getFont());
    name->setOrigin(Origin::Left);
    name->setPosition(ICON_SIZE + 32, getSize().y / 2);
    name->setWidth(getSize().x - ICON_SIZE + 32);
    name->setFillColor(COLOR_FONT);
    add(name);
}


Menu::Menu(Main *m, const c2d::FloatRect &rect,
           const std::vector<MenuItem> &items, bool left) : RectangleShape(rect) {

    main = m;

    setFillColor(COLOR_BG);
    setAlpha(245);
    setOutlineColor(Color::GrayLight);
    setOutlineThickness(2);

#if 0
    // TODO: menu button belong to main
    // menu button
    menuButton = new RectangleShape({0, 0, 40, 40});
    menuButton->setFillColor(COLOR_BG);
    menuButton->setAlpha(245);
    menuButton->setOutlineColor(Color::GrayLight);
    menuButton->setOutlineThickness(2);
    auto *tex = new C2DTexture(main->getIo()->getDataReadPath() + "skin/menu.png");
    tex->setFillColor(COLOR_FONT);
    tex->setOrigin(Origin::Center);
    tex->setPosition(menuButton->getSize().x / 2, menuButton->getSize().y / 2);
    menuButton->add(tex);
    main->getMainRect()->add(menuButton);
#endif

    // highlight
    highlight = new Highlight({getSize().x, BUTTON_HEIGHT});
    highlight->setOrigin(Origin::Left);
    highlight->setPosition(0, 200);
    add(highlight);

    // title
    title = new Text("PPLAY______", 24, main->getFont());
    title->setStyle(Text::Underlined);
    title->setPosition(32, 32);
    title->setFillColor(COLOR_FONT);
    add(title);

    // options
    FloatRect top = {0, 200, getSize().x, BUTTON_HEIGHT};
    FloatRect bottom = {0, getSize().y - 32, getSize().x, BUTTON_HEIGHT};

    for (auto &item : items) {
        if (item.position == MenuItem::Position::Top) {
            auto *option = new MenuButton(main, item, top);
            add(option);
            buttons.push_back(option);
            top.top += 64;
        } else {
            auto *option = new MenuButton(main, item, bottom);
            add(option);
            buttons.push_back(option);
            bottom.top -= 64;
        }
    }

    // tween!
    if (left) {
        add(new TweenPosition({-rect.width, 0}, {0, 0}, 0.5f));
    } else {
        add(new TweenPosition({main->getSize().x, 0}, {main->getSize().x - getSize().x, 0}, 0.5f));
    }
}

bool Menu::onInput(c2d::Input::Player *players) {

    int keys = players[0].keys;

    if (keys & Input::Touch) {
        Vector2f touch = players[0].touch;
        if (!getGlobalBounds().contains(touch)) {
            setVisibility(Visibility::Hidden, true);
        } else {
            for (unsigned int i = 0; i < buttons.size(); i++) {
                if (buttons.at(i)->getGlobalBounds().contains(touch)) {
                    onOptionSelection(&buttons.at(i)->item);
                    index = i;
                    break;
                }
            }
        }
    } else {
        if (keys & Input::Up) {
            index--;
            if (index < 0) {
                index = (int) buttons.size() - 1;
            }
        } else if (keys & Input::Down) {
            index++;
            if (index == (int) buttons.size()) {
                index = 0;
            }
        } else if (keys & Input::Fire1) {
            onOptionSelection(&buttons[index]->item);
        }
    }

    highlight->setPosition(0, buttons[index]->getPosition().y);

    return true;
}

void Menu::setVisibility(c2d::Visibility visibility, bool tweenPlay) {
    C2DObject::setVisibility(visibility, true);
}

MenuItem *Menu::getSelection() {
    return &buttons[index]->item;
}
