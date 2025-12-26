// config.h
#ifndef CONFIG_H
#define CONFIG_H

namespace Config {

    constexpr int WINDOW_WIDTH = 2000;
    constexpr int WINDOW_HEIGHT = 2000;
    
    //размеры dialog окна
    constexpr int MODAL_WIDTH = 400;
    constexpr int MODAL_HEIGHT = 320;
    
    //размеры элементов в модальном окне
    constexpr int FIELD_WIDTH = 150;
    constexpr int FIELD_HEIGHT = 30;
    
    //размеры кнопок
    constexpr int BUTTON_WIDTH = 80;
    constexpr int BUTTON_HEIGHT = 35;
    
    //размеры шрифтов
    constexpr int TITLE_FONT_SIZE = 24;
    constexpr int LABEL_FONT_SIZE = 20;
    constexpr int VALUE_FONT_SIZE = 20;
    constexpr int BUTTON_FONT_SIZE = 16;
    
    //позиционирование (относительные смещения)
    constexpr int TITLE_X = 30;
    constexpr int TITLE_Y = 25;
    constexpr int LABEL_X = 40;
    constexpr int LABEL_Y = 80;
    constexpr int FIELD_X = 200;
    constexpr int FIELD_Y = 75;
    constexpr int BUTTON_Y = 250;
    constexpr int BUTTON_SPACING = 100;



    ///относительный радиус шарика

    constexpr float RADIUS = 30.0f;
}

#endif