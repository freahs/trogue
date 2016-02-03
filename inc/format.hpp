#ifndef TROGUE_FORMAT_H
#define TROGUE_FORMAT_H

#include <string>

namespace trogue {

    class Format {
        private:
            bool        m_bold;
            bool        m_italic;
            bool        m_underline;
            int         m_fg_color;
            int         m_bg_color;

            std::string boldf() const {
                if (m_bold) return "\033[1m";
                return "\033[22m";
            }

            std::string italicf() const {
                if (m_italic) return "\033[3m";
                return "\033[23m";
            }

            std::string underlinef() const {
                if (m_underline) return "\033[4m";
                return "\033[24m";
            }

            std::string fgcolorf() const {
                if (m_fg_color == -1) return "\033[39m";
                return "\033[38;5;" + std::to_string(m_fg_color) + "m";
            }

            std::string bgcolorf() const {
                if (m_bg_color == -1) return "\033[49m";
                return "\033[48;5;" + std::to_string(m_bg_color) + "m";
            }

        public:
            const static Format empty;

            Format()
                : m_bold(false), m_italic(false), m_underline(false), m_fg_color(-1), m_bg_color(-1) { }

            Format(bool bold, bool italic, bool underline, int fg_color, int bg_color)
                : m_bold(bold), m_italic(italic), m_underline(underline), m_fg_color(fg_color), m_bg_color(bg_color) { }

            Format(const Format& format )
                : m_bold(format.m_bold), m_italic(format.m_italic), m_underline(format.m_underline), m_fg_color(format.m_fg_color), m_bg_color(format.m_bg_color)  { }

            Format& operator=(const Format& other) {
                this->m_bold = other.m_bold;
                this->m_italic = other.m_italic;
                this->m_underline = other.m_underline;
                this->m_fg_color = other.m_fg_color;
                this->m_bg_color = other.m_bg_color;
                return *this;
            }

            void bold(bool b)           { m_bold = b; }
            bool bold() const           { return m_bold; }
            void italic(bool b)         { m_italic = b; }
            bool italic() const         { return m_italic; }
            void underline(bool b)      { m_underline = b; }
            bool underline() const      { return m_underline; }
            void fgcolor(int i)         { m_fg_color = i; }
            int fgcolor() const         { return m_fg_color; }
            void bgcolor(int i)         { m_bg_color = i; }
            int bgcolor() const         { return m_bg_color; }
            void color(int fg, int bg)  { m_fg_color = fg; m_bg_color = bg; }

            void apply(std::ostream& os) const {
                os <<  boldf() << italicf() << underlinef() << fgcolorf() << bgcolorf();
            }

            void apply(std::ostream& os, const Format* prev_format) const {
                if (this->m_bold != prev_format->m_bold) os << boldf();
                if (this->m_italic != prev_format->m_italic) os << italicf();
                if (this->m_underline != prev_format->m_underline) os << underlinef();
                if (this->m_fg_color != prev_format->m_fg_color) os << fgcolorf();
                if (this->m_bg_color != prev_format->m_bg_color) os << bgcolorf();
                prev_format = this;
            }
    };

    const Format Format::empty = Format(false, false, false, 0, 0);

}

#endif
