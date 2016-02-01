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

            struct FormatNode {
                    bool        m_bold;
                    bool        m_italic;
                    bool        m_underline;
                    int         m_fg_color;
                    int         m_bg_color;

                    std::string boldf() const;
                    std::string italicf() const;
                    std::string underlinef() const;
                    std::string fgcolorf() const;
                    std::string bgcolorf() const;

                    void bold(bool b) { m_bold = b; }
                    bool bold() const { return m_bold; }
                    void italic(bool b) { m_italic = b; }
                    bool italic() const { return m_italic; }
                    void underline(bool b) { m_underline = b; }
                    void fgcolor(int i) { m_fg_color = i; }
                    void bgcolor(int i) { m_bg_color = i; }
                    void color(int fg, int bg) { m_fg_color = fg; m_bg_color = bg; }

                    FormatNode();
                    FormatNode (bool bold, bool italic, bool underline, int fg_color, int bg_color);

                    void format(std::ostream&) const;
                    void format(std::ostream&, FormatNode&) const;
            };

            class TextNode : public FormatNode {
                private:
                    std::string m_text;

                public:
                    TextNode();
                    TextNode(const std::string& text);
                    TextNode(const FormatNode& format, const std::string& text);

                    const std::string& str() const { return m_text; }
            };

            std::vector<std::vector<TextNode>> m_arr;
            static size_t   s_rows;
            static size_t   s_cols;
            static Display* s_display;

            FormatNode      m_current_format;
            
            Display();
            static void resizeHandler(int);

        public:
            static const uint8_t NONE = 1;
            static const uint8_t BOLD = 2;
            static const uint8_t ITALIC = 4;
            static const uint8_t UNDERLINE = 8;

            ~Display();

            static Display* instance();

            void color(int, int);
            void format(uint8_t);
            void put(size_t, size_t, std::string str);

            void draw();
            void reset();

            size_t width() const;
            size_t height() const;
    };

}

#endif
