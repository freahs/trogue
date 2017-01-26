#ifndef TROGUE_XTERM_DISPLAY_H
#define TROGUE_XTERM_DISPLAY_H

#include <iostream>
#include <vector>

namespace trogue {

    class Display {
    private:
        static Display*    s_display;
        struct Data {
            std::string m_symbol;
            int         m_color;
            int         m_bg_color;
            bool        m_needs_update;
        };

        int                 m_rows;
        int                 m_cols;
        bool                m_was_resized;
        std::vector<Data>   m_data;
        std::vector<bool>   m_need_update;

        Display();

        static void resizeHandler(int);
        Data& data(int row, int col);

    public:
        ~Display();

        static Display* instance();
        void resize(int rows, int cols);

        void set(size_t row, size_t col, const std::string& symbol, int color, int bg_color);

        void draw(std::ostream& os);

        int width() const;
        int height() const;

    };


}

#endif
