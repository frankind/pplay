//
// Created by cpasjuste on 03/10/18.
//

#ifndef PPLAY_PLAYER_OSD_H
#define PPLAY_PLAYER_OSD_H

class PlayerOSD : public c2d::C2DRectangle {

public:
    PlayerOSD(Player *player);

    ~PlayerOSD();

    void setProgress(float duration, float position);

    bool isVisible();

    c2d::Visibility getVisibility() override;

    void setVisibility(c2d::Visibility visibility, bool tweenPlay = true) override;

private:
    Player *player = nullptr;
    c2d::TweenAlpha *tweenAlpha = nullptr;
    c2d::C2DRoundedRectangle *status = nullptr;
    c2d::Progress *progress = nullptr;
    c2d::Text *remaining = nullptr;
};

#endif //PPLAY_PLAYER_OSD_H
