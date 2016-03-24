#ifndef TROGUE_KEYBOARD_H
#define TROGUE_KEYBOARD_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <mutex>
#include <thread>
#include <chrono>
#include <map>

namespace trogue {

    enum class Key {
        NONE, UNKNOWN,

        A, B, C, D,
        E, F, G, H,
        I, J, K, L,
        M, N, O, P,
        Q, R, S, T,
        U, V, W, X,
        Y, Z,

        ONE, TWO, THREE, FOUR,
        FIVE, SIX, SEVEN, EIGHT,
        NINE, ZERO,

        F1, F2, F3, F4,
        F5, F6, F7, F8,
        F9, F10, F11, F12,

        UP, DOWN, LEFT, RIGHT,
    };

    class Input {
        private:
            std::map<char, Key> m_keymap;
            Key                 m_key;
            std::mutex          m_mutex;
            std::thread         m_thread;

            char readChar() {
                char buf = 0;
                struct termios old;
                if (tcgetattr(0, &old) < 0)
                    perror("tcsetattr()");
                old.c_lflag &= ~ICANON;
                old.c_lflag &= ~ECHO;
                old.c_cc[VMIN] = 1;
                old.c_cc[VTIME] = 0;
                if (tcsetattr(0, TCSANOW, &old) < 0)
                    perror("tcsetattr ICANON");
                if (read(0, &buf, 1) < 0)
                    perror ("read()");
                old.c_lflag |= ICANON;
                old.c_lflag |= ECHO;
                if (tcsetattr(0, TCSADRAIN, &old) < 0)
                    perror ("tcsetattr ~ICANON");
                return (buf);
            }

            void getInput() {
                char tmp = 0;
                int esc = 0;
                while(true) {
                    tmp = readChar();
                    if (esc == 0 && tmp == 27) {
                        esc = 1;
                    } else if (esc == 1 && tmp == 91) {
                        esc = 2;
                    } else if (tmp != 0) {
                        m_mutex.lock();
                        if (esc == 2 && tmp == 65) { m_key = Key::UP; }
                        else if (esc == 2 && tmp == 66) { m_key = Key::DOWN; }
                        else if (esc == 2 && tmp == 67) { m_key = Key::RIGHT; }
                        else if (esc == 2 && tmp == 68) { m_key = Key::LEFT; }
                        else if (m_keymap.find(tmp) != m_keymap.end()) { m_key = m_keymap[tmp]; }
                        else { m_key = Key::UNKNOWN; }
                        m_mutex.unlock();
                        esc = 0;
                    }
                }
            }



        public:
            Input() : m_key(Key::NONE) {
                m_keymap['a'] = Key::A;
                m_keymap['b'] = Key::B;
                m_keymap['c'] = Key::C;
                m_keymap['d'] = Key::D;
                m_keymap['e'] = Key::E;
                m_keymap['f'] = Key::F;
                m_keymap['g'] = Key::G;
                m_keymap['h'] = Key::H;
                m_keymap['i'] = Key::I;
                m_keymap['j'] = Key::J;
                m_keymap['k'] = Key::K;
                m_keymap['l'] = Key::L;
                m_keymap['m'] = Key::M;
                m_keymap['n'] = Key::N;
                m_keymap['o'] = Key::O;
                m_keymap['p'] = Key::P;
                m_keymap['q'] = Key::Q;
                m_keymap['r'] = Key::R;
                m_keymap['s'] = Key::S;
                m_keymap['t'] = Key::T;
                m_keymap['u'] = Key::U;
                m_keymap['v'] = Key::V;
                m_keymap['w'] = Key::W;
                m_keymap['x'] = Key::X;
                m_keymap['y'] = Key::Y;
                m_keymap['z'] = Key::Z;

                m_keymap['0'] = Key::ZERO;
                m_keymap['1'] = Key::ONE;
                m_keymap['2'] = Key::TWO;
                m_keymap['3'] = Key::THREE;
                m_keymap['4'] = Key::FOUR;
                m_keymap['5'] = Key::FIVE;
                m_keymap['6'] = Key::SIX;
                m_keymap['7'] = Key::SEVEN;
                m_keymap['8'] = Key::EIGHT;
                m_keymap['9'] = Key::NINE;

                m_thread = std::thread(&Input::getInput, this);
            }

            ~Input() {
                m_thread.detach();
            }

            bool has() {
                m_mutex.lock();
                bool res = m_key != Key::NONE && m_key != Key::UNKNOWN; 
                m_mutex.unlock();
                return res;
            }

            Key get() {
                m_mutex.lock();
                Key res = m_key;
                m_key = Key::NONE;
                m_mutex.unlock();
                return res;
            }
    };
}
#endif
