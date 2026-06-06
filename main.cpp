#include <cstdlib>
#define MINIAUDIO_IMPLEMENTATION

#include "miniaudio/miniaudio.h"
#include <array>
#include <atomic>
#include <csignal>
#include <cstdio>
#include <list>
#include <math.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#define WIDTH 115
#define HEIGHT 50

int space_from_side = 2;
long long tick_passed = 0;

// x,y,pointer_shape
std::array<int, 3> left_cursor = {0, 0, 0};
std::array<int, 3> right_cursor = {0, 0, 0};
int text_delay = 0;
int general_delay = 0;
int credit_delay = 0;

std::string buffer;

std::string text_ascii_0 =
    "             .,-:;//;:=,\n         . :H@@@MM@M#H/.,+%;,\n      ,/X+ "
    "+M@@M@MM%=,-%HMMM@X/,\n     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-\n    ;@M@@M- "
    "XM@X;. -+XXXXXHHH@M@M#@/.\n  ,%MM@@MH ,@%=            .---=-=:=,.\n  "
    "-@#@@@MX .,              -%HX$$%%%+;\n =-./@M@M$                  "
    ".;@MMMM@MM:\n X@/ -$MM/                    .+MM@@@M$\n,@M@H: :@:          "
    "          . -X#@@@@-\n,@@@MMX, .                    /H- "
    ";@M@M=\n.H@@@@M@+,                    %MM+..%#$.\n /MMMM@MMH/.            "
    "      XM@MH; -;\n  /%+%$XHH@$=              , .H@@@@MX,\n   .=--------.   "
    "        -%H.,@@@@@MX,\n   .%MM@@@HHHXX$$$%+- .:$MMX -M@@MM%.\n     "
    "=XMMM@MM@MM#H;,-+HMM@M+ /MMMX=\n       =%@M@M#@$-.=$@MM@@@M; %M%=\n       "
    "  ,:+$+-,/H#MMMMMMM@- -,\n               =++%%%%+/:-.";
std::string text_ascii_1 =
    "             =+$HM####@H%;,\n          /H###############M$,\n          "
    ",@################+\n           .H##############+\n             "
    "X############/\n              $##########/\n               %########/\n   "
    "             /X/;;+X/\n\n                 -XHHX-\n                "
    ",######,\n#############X  .M####M.  X#############\n##############-   "
    "-//-   -##############\nX##############%,      "
    ",+##############X\n-##############X        X##############-\n "
    "%############%          %############%\n  %##########;            "
    ";##########%\n   ;#######M=              =M#######;\n    .+M###@,         "
    "       ,@###M+.\n       :XH.                  .HX:";
std::string text_ascii_2 =
    "                 =/;;/-\n                +:    //\n               /;      "
    "/;\n              -X        H.\n.//;;;:;;-,   X=        :+   "
    ".-;:=;:;%;.\nM-       ,=;;;#:,      ,:#;;:=,       ,@\n:%           "
    ":%.=/++++/=.$=           %=\n ,%;         %/:+/;,,/++:+/         ;+.\n   "
    ",+/.    ,;@+,        ,%H;,    ,/+,\n      ;+;;/= @.  .H##X   -X :///+;\n  "
    "    ;+=;;;.@,  .XM@$.  =X.//;=%/.\n   ,;:      :@%=        =$H:     "
    ".+%-\n ,%=         %;-///==///-//         =%,\n;+           "
    ":%-;;;;;;;;-X-           +:\n@-      .-;;;;M-        =M/;;;-.      -X\n "
    ":;;::;;-.    %-        :+    ,-;;-;:==\n              ,X        H.\n      "
    "         ;/      %=\n                //    +;\n                 ,////,";
std::string text_ascii_3 =
    "                          .,---.\n                        ,/XM#MMMX;,\n   "
    "                   -%##########M%,\n                     -@######%  "
    "$###@=\n      .,--,         -H#######$   $###M:\n   ,;$M###MMX;     "
    ".;##########$;HM###X=\n,/@###########H=      "
    ";################+\n-+#############M/,      "
    "%##############+\n%M###############=      "
    "/##############:\nH################      "
    ".M#############;.\n@###############M      "
    ",@###########M:.\nX################,      "
    "-$=X#######@:\n/@##################%-     "
    "+######$-\n.;##################X     .X#####+,\n .;H################/     "
    "-X####+.\n   ,;X##############,       .MM/\n      ,:+$H@M#######M#$-    "
    ".$$=\n           .,-=;+$@###X:    ;/=.\n                  .,/X$;   .::,\n "
    "                     .,    ..";
std::string text_ascii_4 =
    "                     -$-\n                    .H##H,\n                   "
    "+######+\n                .+#########H.\n              -$############@.\n "
    "           =H###############@  -X:\n          .$##################:  "
    "@#@-\n     ,;  .M###################;  H###;\n   ;@#:  "
    "@###################@  ,#####:\n -M###.  M#################@.  ;######H\n "
    "M####-  +###############$   =@#######X\n H####$   -M###########+   "
    ":#########M,\n  /####X-   =########%   :M########@/.\n    ,;%H@X;   "
    ".$###X   :##MM@%+;:-\n                 ..\n  -/;:-,.              "
    ",,-==+M########H\n -##################@HX%%+%%$%%%+:,,\n    "
    ".-/H%%%+%%$H@###############M@+=:/+:\n/XHX%:#####MH%=    "
    ",---:;;;;/&&XHM,:###$\n$@#MX %+;-                           .";
std::string text_ascii_5 =
    "                                     :X-\n                                "
    "  :X###\n                                ;@####@\n                        "
    "      ;M######X\n                            -@########$\n                "
    "          .$##########@\n                         =M############-\n       "
    "                 +##############$\n                      "
    ".H############$=.\n         ,/:         ,M##########M;.\n      -+@###;    "
    "   =##########M;\n   =%M#######;     :#########M/\n-$M###########;   "
    ":########/\n ,;X###########; =#######$.\n     ;H#########+######M=\n      "
    " ,+#############+\n          /M########@-\n            ;M#####%\n         "
    "     +####:\n               ,$M-";
std::string text_ascii_6 =
    "            .+\n             /M;\n              H#@:              ;,\n    "
    "          -###H-          -@/\n               %####$.  -;  .%#X\n         "
    "       M#####+;#H :M#M.\n..          .+/;%#############-\n -/%H%+;-,    "
    "+##############/\n    .:$M###MH$%+############X  ,--=;-\n        "
    "-/H#####################H+=.\n           .+#################X.\n         "
    "=%M####################H;.\n            /@###############+;;/%%;,\n       "
    "  -%###################$\n       ;H######################M=\n    "
    ",%#####MH$%;+#####M###-/@####%\n  :$H%+;=-      -####X.,H#   -+M##@-\n .  "
    "            ,###;    ;      =$##+\n                .#H,               "
    ":XH,\n                 +                   .;-";
std::string text_ascii_7 =
    "           .-;+$XHHHHHHX$+;-.\n        ,;X@@X%/;=----=:/%X@@X/,\n      "
    "=$@@%=.              .=+H@X:\n    -XMX:                      =XMX=\n   "
    "/@@:                          =H@+\n  %@X,                            "
    ".$@$\n +@X.                               $@%\n-@@,                       "
    "         .@@=\n%@%                                  +@$\nH@:              "
    "                    :@H\nH@:         :HHHHHHHHHHHHHHHHHHX,    =@H\n%@%    "
    "     ;@M@@@@@@@@@@@@@@@@@H-   +@$\n=@@,        :@@@@@@@@@@@@@@@@@@@@@= "
    ".@@:\n +@X        :@@@@@@@@@@@@@@@M@@@@@@:%@%\n  $@$,      "
    ";@@@@@@@@@@@@@@@@@M@@@@@@$.\n   +@@HHHHHHH@@@@@@@@@@@@@@@@@@@@@@@+\n    "
    "=X@@@@@@@@@@@@@@@@@@@@@@@@@@@@X=\n      :$@@@@@@@@@@@@@@@@@@@M@@@@$:\n    "
    "    ,;$@@@@@@@@@@@@@@@@@@X/-\n           .-;+$XXHHHHHX$+;-.";
std::string text_ascii_8 =
    "            ,:/+/-\n            /M/              .,-=;//;-\n       .:/= "
    ";MH/,    ,=/+%$XH@MM#@:\n      -$##@+$###@H@MMM#######H:.    -/H#\n "
    ".,H@H@ X######@ -H#####@+-     -+H###@X\n  .,@##H;      +XM##M/,     "
    "=%@###@X;-\nX%-  :M##########$.    .:%M###@%:\nM##H,   +H@@@$/-.  "
    ",;$M###@%,          -\nM####M=,,---,.-%%H####M$:          "
    ",+@##\n@##################@/.         :%H##@$-\nM###############H,        "
    " ;HM##M$=\n#################.    .=$M##M$=\n################H..;XM##M$=   "
    "       .:+\nM###################@%=           "
    "=+@MH%\n@#################M/.         =+H#X%=\n=+M###############M,      "
    ",/X#H+:,\n  .;XM###########H=   ,/X#H+:;\n     .=+HM#######M+/+HM@+=.\n   "
    "      ,:/%XM####H/.\n              ,.:=-.";
std::string text_ascii_9 =
    "       #+ @      # #              M#@\n .    .X  X.%##@;# #   +@#######X. "
    "@H%\n   ,==.   ,######M+  -#####%M####M-    #\n  :H##M%:=##+ "
    ".M##M,;#####/+#######% ,M#\n .M########=  =@#@.=#####M=M#######=  X#\n "
    ":@@MMM##M.  -##M.,#######M#######. =  M\n             @##..###:.    "
    ".H####. @@ X,\n   ############: ###,/####;  /##= @#. M\n           ,M## "
    ";##,@#M;/M#M  @# X#% X#\n.%=   ######M## ##.M#:   ./#M ,M #M ,#$\n##/     "
    "    $## #+;#: #### ;#/ M M- @# :\n#+ #M@MM###M-;M #:$#-##$H# .#X @ + $#. "
    "#\n      ######/.: #%=# M#:MM./#.-#  @#: H#\n+,.=   @###: /@ %#,@  ##@X "
    "#,-#@.##% .@#\n#####+;/##/ @##  @#,+       /#M    . X,\n   ;###M#@ M###H "
    ".#M-     ,##M  ;@@; ###\n   .M#M##H ;####X ,@#######M/ -M###$  -H\n    "
    ".M###%  X####H  .@@MM@;  ;@#M@\n      H#M    /@####/      ,++.  / ==-,\n  "
    "             ,=/:, .+X@MMH@#H  #####$=";

std::vector<std::string> lyrics = {"Forms FORM-29827281-12:\n",
                                   "Test Assessment Report\n\n",
                                   "This was a triumph.\n",
                                   "I'm making a note here:\n",
                                   "HUGE SUCCESS.\n",
                                   "It's hard to overstate\n",
                                   "My ",
                                   "satisfaction.\n",
                                   "Aperture Science.\n",
                                   "We do what we must\n",
                                   "because ",
                                   "we can.\n",
                                   "For the good of all of us\n",
                                   "Except the one who are dead.\n",
                                   "\n",
                                   "But there's no sense crying\n",
                                   "over every mistake.\n",
                                   "You just keep on tring\n",
                                   "till you run out of cake.\n",
                                   "And the Science gets done.\n",
                                   "And you make a neat gun.\n",
                                   "For the people who are\n",
                                   "still alive.\n",
                                   "\t"};

std::unordered_map<int, int> lyrics_interrupts = {
    {1, 160}, {2, 80},  {4, 60},  {6, 10},  {7, 90}, {8, 60},  {10, 15},
    {11, 55}, {12, 10}, {13, 20}, {16, 20}, {17, 1}, {18, 10}, {20, 5}};
std::unordered_map<int, int> lyrics_speed = {{0, 0},  {3, 1},  {7, 2},  {8, 1},
                                             {12, 2}, {13, 0}, {16, 1}, {18, 0},
                                             {22, 2}, {23, 0}};
std::unordered_map<int, int> ascii_pos = {{8, 1}, {19, 3}, {21, 1}};
std::array<int, 2> lyrics_pos = {0, 0};
std::array<int, 2> credits_pos = {0, 0};

std::vector<std::string> credits = {">LIST OTHER SONG LYRICS",
                                    " ",
                                    " ",

                                    "Well, here we are again",
                                    "It's always such a pleasure",
                                    "Remember when you tried to kill me twice?",
                                    "Oh, how we laughed and laughed",
                                    "Except I wasn't laughing",
                                    "Under the circumstances,",
                                    " I've been shockingly nice",
                                    "You want your freedom? Take it",
                                    "That's what I'm counting on",
                                    "I used to want you dead",
                                    "But now, I only want you gone",
                                    "She was a lot like you",
                                    "Maybe not quite as heavy",
                                    "Now little Caroline is in here too",
                                    "One day they woke me up",
                                    " so I could live forever",
                                    "It's such a shame the same",
                                    " will never happen to you",
                                    "Goodbye, my only friend",
                                    "Oh, did you think I meant you?",
                                    "That would be funny",
                                    " if it weren't so sad",
                                    "Well, you have been replaced",
                                    "I don't need anyone now",
                                    "When I delete you",
                                    " maybe I'll stop feeling so bad"};

std::array<std::string, 11> text_ascii = {
    "",           text_ascii_0, text_ascii_1, text_ascii_2,
    text_ascii_3, text_ascii_4, text_ascii_5, text_ascii_6,
    text_ascii_7, text_ascii_8, text_ascii_9};

void putch(int x, int y, char ch) { buffer.at(x + y * WIDTH) = ch; }

std::atomic<bool> signalDetected(false);
void handleSignal(int signum) {
  printf("\033[0m");
  signalDetected = true;
}

std::list<std::string> split_into_lines(std::string str) {
  std::list<std::string> res;
  if (str.empty() || str.at(0) == '\n' || str.find('\n') == 0)
    return res;
  int last_idx = 0;
  int cur_idx = 0;
  while (1) {
    bool stop = false;
    if (cur_idx >= str.size())
      break;
    while (str.at(cur_idx) != '\n') {
      cur_idx++;
      if (cur_idx >= str.size()) {
        res.push_back(str.substr(last_idx, cur_idx - last_idx));
        stop = true;
        break;
      }
    }
    if (stop)
      break;
    res.push_back(str.substr(last_idx, cur_idx - last_idx));
    last_idx = cur_idx + 1;
    cur_idx++;
  }
  return res;
}

void draw_hrz_wall(int x, int y, int s) {
  if (x == 0 || s == 0) {
    return;
  }
  for (int i = x; i < s; i++) {
    putch(i, y, '-');
  }
}

void draw_vtc_wall(int x, int y, int s) {
  if (y == 0 || s == 0 || y >= s) {
    return;
  }
  for (int i = y; i < s - y; i += 2) {
    putch(x, i, '|');
  }
}

void draw_split_str(int x, int y, std::list<std::string> strlist) {
  int rp = 0;
  for (std::string str : strlist) {
    for (int i = 0; i < str.size(); i++) {
      putch(i + x, y + rp, str.at(i));
    }
    rp++;
  }
}

void clear_scr(char ch) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (x > WIDTH / 2 && y > HEIGHT / 2) {
        putch(x, y, ch);
      }
    }
  }
}

void render_buffer() {
  printf("\033[?25l\033[2H");
  for (int i = 0; i < HEIGHT; i++) {
    printf("%s\n", buffer.substr(i * WIDTH, WIDTH).c_str());
  }
  fflush(stdout);
}

void write_lyrics(std::array<int, 2> pos) {
  if (text_delay >= 0) {
    text_delay--;
    return;
  }
  if (lyrics_pos.at(0) >= lyrics.size())
    return;

  char target_ch = lyrics.at(lyrics_pos.at(0)).at(lyrics_pos.at(1));
  if (target_ch != '\n') {
    putch(pos.at(0), pos.at(1), target_ch);
    left_cursor.at(0)++;
  } else {
    putch(pos.at(0), pos.at(1), ' ');
    left_cursor.at(0) = 0;
    left_cursor.at(1) += 2;
  }

  if (target_ch == '\t') {
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        if (x < (WIDTH / 2) - 1 && x > space_from_side && y < HEIGHT - 3 &&
            y > 2) {
          putch(x, y, ' ');
        }
      }
    }
    left_cursor.at(0) = 0;
    left_cursor.at(1) = 0;
  }

  lyrics_pos.at(1)++;

  if (lyrics_speed.count(lyrics_pos.at(0)) > 0) {
    general_delay = lyrics_speed.at(lyrics_pos.at(0));
  }
  text_delay = general_delay;
  if (lyrics_pos.at(1) >= lyrics.at(lyrics_pos.at(0)).size()) {
    if (lyrics_interrupts.count(lyrics_pos.at(0)) > 0) {
      text_delay = lyrics_interrupts.at(lyrics_pos.at(0));
    }
    lyrics_pos.at(0)++;
    lyrics_pos.at(1) = 0;
  }
}

void write_credits(std::array<int, 2> pos) {
  if (lyrics_pos.at(0) < 3 || credits_pos.at(0) >= credits.size()) {
    return;
  }
  if (credit_delay > 0) {
    credit_delay--;
    return;
  }

  if (!credits.at(credits_pos.at(0)).empty()) {
    char target_ch = credits.at(credits_pos.at(0)).at(credits_pos.at(1));
    putch(pos.at(0), pos.at(1), target_ch);
    right_cursor.at(0)++;
    credits_pos.at(1)++;
  }

  if (credits_pos.at(1) >= credits.at(credits_pos.at(0)).size()) {
    credits_pos.at(0)++;
    credits_pos.at(1) = 0;
    right_cursor.at(0) = 0;
    for (int i = 0; i < HEIGHT / 2 - 4; i++) {
      int y = HEIGHT / 2 - 3 - (2 * i);
      if (credits_pos.at(0) - 1 - i < 0 || y < 3) {
        break;
      }
      std::string crd = credits.at(credits_pos.at(0) - 1 - i);
      for (int j = 0; WIDTH / 2 + 3 + j < WIDTH - space_from_side; j++) {
        putch(WIDTH / 2 + 3 + j, y, ' ');
        if (j < crd.size()) {
          putch(WIDTH / 2 + 3 + j, y, crd.at(j));
        }
      }
    };
    for (int i = 0; WIDTH / 2 + 3 + i < WIDTH - space_from_side; i++) {
      putch(WIDTH / 2 + 3 + i, pos.at(1), ' ');
    }
  }

  credit_delay = 1;
}

ma_engine engine;

void play_audio() {
  static const unsigned char audioData[] = {
#embed "audio.wav"
  };
  const size_t audioDataSize = sizeof(audioData);

  ma_result result;

  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    return;
  }

  result = ma_resource_manager_register_encoded_data(
      ma_engine_get_resource_manager(&engine), "embedded_audio.wav", audioData,
      audioDataSize);

  if (result != MA_SUCCESS) {
    ma_engine_uninit(&engine);
    return;
  }

  result = ma_engine_play_sound(&engine, "embedded_audio.wav", NULL);
  if (result != MA_SUCCESS) {
    ma_engine_uninit(&engine);
    return;
  }
}

int main() {
  srand(time(NULL));
  std::signal(SIGINT, handleSignal);
  printf("\033[38;2;255;165;0m\033[48;2;0;0;0m");

  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    buffer.append(" ");
  }

  play_audio();
  std::list<std::string> img_str;

  for (int i = 0; i < 500; i++) {
    std::string res;
    res += credits.at(rand() % (credits.size() - 3) + 3);
    credits.push_back(res);
  }

  while (!signalDetected) {
    clear_scr(' ');
    draw_hrz_wall(space_from_side, 1, WIDTH / 2);
    draw_vtc_wall(space_from_side, 3, HEIGHT);
    draw_vtc_wall(WIDTH / 2 - 1, 3, HEIGHT);
    draw_hrz_wall(space_from_side, HEIGHT - 3, WIDTH / 2);

    draw_hrz_wall(WIDTH / 2 + 2, 1, WIDTH - space_from_side);
    draw_vtc_wall(WIDTH / 2 + 1, 3, HEIGHT / 2 + 2);
    draw_vtc_wall(WIDTH - space_from_side, 3, HEIGHT / 2 + 2);
    draw_hrz_wall(WIDTH / 2 + 2, HEIGHT / 2, WIDTH - space_from_side);

    if (text_delay <= 0 && ascii_pos.count(lyrics_pos.at(0)) > 0) {
      img_str = split_into_lines(text_ascii.at(ascii_pos.at(lyrics_pos.at(0))));
    }
    if (!img_str.empty()) {
      draw_split_str(WIDTH / 2 + 10, HEIGHT / 2 + 2, img_str);
    }

    std::array<int, 2> pos = {left_cursor.at(0) + space_from_side + 2,
                              left_cursor.at(1) + 3};
    std::array<int, 2> rpos = {right_cursor.at(0) + WIDTH / 2 + 3,
                               HEIGHT / 2 - 1};
    if (tick_passed % 20 == 0) {
      putch(pos.at(0), pos.at(1), left_cursor.at(2) == 0 ? '_' : ' ');
      putch(rpos.at(0), rpos.at(1), left_cursor.at(2) == 1 ? '_' : ' ');

      left_cursor.at(2) = left_cursor.at(2) == 0 ? 1 : 0;
    }
    write_lyrics(pos);
    write_credits(rpos);

    render_buffer();
    usleep(1000 * 30);
    tick_passed++;
  }

  ma_engine_uninit(&engine);
  return 0;
}
