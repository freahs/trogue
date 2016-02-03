#ifndef TROGUE_KEYBOARD_H
#define TROGUE_KEYBOARD_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

namespace trogue {

    class Input {
        private:
            static struct termios orig_termios;

            static void reset() {
                tcsetattr(0, TCSANOW, &Input::orig_termios);
            }

        public:
            static void init() {
                struct termios new_termios;

                /* take two copies - one for now, one for later */
                tcgetattr(0, &Input::orig_termios);
                memcpy(&new_termios, &orig_termios, sizeof(new_termios));

                /* register cleanup handler, and set the new terminal mode */
                //atexit(Input::reset);
                cfmakeraw(&new_termios);
                tcsetattr(0, TCSANOW, &new_termios);
            }

            static int kbhit() {
                struct timeval tv = { 0L, 0L };
                fd_set fds;
                FD_ZERO(&fds);
                FD_SET(0, &fds);
                return select(1, &fds, NULL, NULL, &tv);
            }

            static int getcha() {
                int r;
                unsigned char c;
                if ((r = read(0, &c, sizeof(c))) < 0) {
                    return r;
                } else {
                    return c;
                }
            }

    };

    termios Input::orig_termios;


}

#endif
