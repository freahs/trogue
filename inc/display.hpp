#ifndef TROGUE_DISPLAY_H
#define TROGUE_DISPLAY_H

#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

namespace trogue {

    class Display {
        private:
            std::vector<std::vector<std::string>> m_arr;
            static size_t   s_rows;
            static size_t   s_cols;
            static Display* s_display;
            std::stringstream   m_render_stream;

            Display();
            static void resizeHandler(int);

        public:
            static Display* instance();
            ~Display();
            void put(size_t, size_t, std::string str);
            void put(size_t, size_t, uint8_t, uint8_t, std::string str);
            void put(size_t, size_t, std::string, std::string, std::string);
            void draw();
            void reset();
            size_t width() const;
            size_t height() const;
    };

}

#endif
