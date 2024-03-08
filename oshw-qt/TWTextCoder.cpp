#include "TWTextCoder.h"

static constexpr QChar encodeTable[] {
        {'\x00'},
        {'\x01'},
        {'\x02'},
        {'\x03'},
        {'\x04'},
        {'\x05'},
        {'\x06'},
        {'\x07'},
        {'\x08'},
        {'\x09'},
        {'\x0A'},
        {'\x0B'},
        {'\x0C'},
        {'\x0D'},
        {'\x0E'},
        {'\x0F'},
        {'\x10'},
        {'\x11'},
        {'\x12'},
        {'\x13'},
        {'\x14'},
        {'\x15'},
        {'\x16'},
        {'\x17'},
        {'\x18'},
        {'\x19'},
        {'\x1A'},
        {'\x1B'},
        {'\x1C'},
        {'\x1D'},
        {'\x1E'},
        {'\x1F'}, //Control codes are all the same as in unicode
        {' '},
        {'!'},
        {'"'},
        {'#'},
        {'$'},
        {'%'},
        {'&'},
        {'\''},
        {'('},
        {')'},
        {'*'},
        {'+'},
        {','},
        {'-'},
        {'.'},
        {'/'},
        {'0'},
        {'1'},
        {'2'},
        {'3'},
        {'4'},
        {'5'},
        {'6'},
        {'7'},
        {'8'},
        {'9'},
        {':'},
        {';'},
        {'<'},
        {'='},
        {'>'},
        {'?'},
        {'@'},
        {'A'},
        {'B'},
        {'C'},
        {'D'},
        {'E'},
        {'F'},
        {'G'},
        {'H'},
        {'I'},
        {'J'},
        {'K'},
        {'L'},
        {'M'},
        {'N'},
        {'O'},
        {'P'},
        {'Q'},
        {'R'},
        {'S'},
        {'T'},
        {'U'},
        {'V'},
        {'W'},
        {'X'},
        {'Y'},
        {'Z'},
        {'['},
        {'\\'},
        {']'},
        {'^'},
        {'_'},
        {'`'},
        {'a'},
        {'b'},
        {'c'},
        {'d'},
        {'e'},
        {'f'},
        {'g'},
        {'h'},
        {'i'},
        {'j'},
        {'k'},
        {'l'},
        {'m'},
        {'n'},
        {'o'},
        {'p'},
        {'q'},
        {'r'},
        {'s'},
        {'t'},
        {'u'},
        {'v'},
        {'w'},
        {'x'},
        {'y'},
        {'z'},
        {'{'},
        {'|'},
        {'}'},
        {'~'},
        {'\x7F'}, //DEL char, also in unicode

        QChar{0x20AC},
        QChar{' '},
        QChar{0x20A1},
        QChar{0x0192},
        QChar{0x201E},
        QChar{0x2026},
        QChar{0x2020},
        QChar{0x2021},
        QChar{0x02C6},
        QChar{0x2030},
        QChar{0x0160},
        QChar{0x2039},
        QChar{0x0152},
        QChar{' '},
        QChar{0x017D},
        QChar{' '},
        QChar{' '},
        QChar{0x2018},
        QChar{0x2019},
        QChar{0x201C},
        QChar{0x201D},
        QChar{0x2022},
        QChar{0x2013},
        QChar{0x2014},
        QChar{0x02DC},
        QChar{0x2122},
        QChar{0x0161},
        QChar{0x203A},
        QChar{0x0153},
        QChar{' '},
        QChar{0x017E},
        QChar{0x0178},
        QChar{0x00A0}, //from here they all match Latin-1 exactly, which matches Unicode exactly
        QChar{0x00A1},
        QChar{0x00A2},
        QChar{0x00A3},
        QChar{0x00A4},
        QChar{0x00A5},
        QChar{0x00A6},
        QChar{0x00A7},
        QChar{0x00A8},
        QChar{0x00A9},
        QChar{0x00AA},
        QChar{0x00AB},
        QChar{0x00AC},
        QChar{0x00AD},
        QChar{0x00AE},
        QChar{0x00AF},
        QChar{0x00B0},
        QChar{0x00B1},
        QChar{0x00B2},
        QChar{0x00B3},
        QChar{0x00B4},
        QChar{0x00B5},
        QChar{0x00B6},
        QChar{0x00B7},
        QChar{0x00B8},
        QChar{0x00B9},
        QChar{0x00BA},
        QChar{0x00BB},
        QChar{0x00BC},
        QChar{0x00BD},
        QChar{0x00BE},
        QChar{0x00BF},
        QChar{0x00C0},
        QChar{0x00C1},
        QChar{0x00C2},
        QChar{0x00C3},
        QChar{0x00C4},
        QChar{0x00C5},
        QChar{0x00C6},
        QChar{0x00C7},
        QChar{0x00C8},
        QChar{0x00C9},
        QChar{0x00CA},
        QChar{0x00CB},
        QChar{0x00CC},
        QChar{0x00CD},
        QChar{0x00CE},
        QChar{0x00CF},
        QChar{0x00D0},
        QChar{0x00D1},
        QChar{0x00D2},
        QChar{0x00D3},
        QChar{0x00D4},
        QChar{0x00D5},
        QChar{0x00D6},
        QChar{0x00D7},
        QChar{0x00D8},
        QChar{0x00D9},
        QChar{0x00DA},
        QChar{0x00DB},
        QChar{0x00DC},
        QChar{0x00DD},
        QChar{0x00DE},
        QChar{0x00DF},
        QChar{0x00E0},
        QChar{0x00E1},
        QChar{0x00E2},
        QChar{0x00E3},
        QChar{0x00E4},
        QChar{0x00E5},
        QChar{0x00E6},
        QChar{0x00E7},
        QChar{0x00E8},
        QChar{0x00E9},
        QChar{0x00EA},
        QChar{0x00EB},
        QChar{0x00EC},
        QChar{0x00ED},
        QChar{0x00EE},
        QChar{0x00EF},
        QChar{0x00F0},
        QChar{0x00F1},
        QChar{0x00F2},
        QChar{0x00F3},
        QChar{0x00F4},
        QChar{0x00F5},
        QChar{0x00F6},
        QChar{0x00F7},
        QChar{0x00F8},
        QChar{0x00F9},
        QChar{0x00FA},
        QChar{0x00FB},
        QChar{0x00FC},
        QChar{0x00FD},
        QChar{0x00FE},
        QChar{0x00FF},
        };

QString TWTextCoder::decode(QByteArray const& arr) {
    QString str{};
    for (char const& c : arr) {
        if (c == '\0') {
            break;
        }
        str += encodeTable[static_cast<unsigned char>(c)]; //note: may break on non-two's complement systems
    }
    return str;
}

QByteArray TWTextCoder::encode(QString const& str) {
    QByteArray arr{};
    for (QChar const& c : str) {
        if (c.unicode() <= 0xFF) {
            arr += static_cast<char>(c.unicode());
        } else {
            char byte;
            switch (c.unicode()) {
                case 0x20AC:
                    byte = '\x80';
                    break;
                case 0x20A1:
                    byte = '\x81';
                    break;
                case 0x0192:
                    byte = '\x82';
                    break;
                case 0x201E:
                    byte = '\x83';
                    break;
                case 0x2026:
                    byte = '\x84';
                    break;
                case 0x2020:
                    byte = '\x85';
                    break;
                case 0x2021:
                    byte = '\x86';
                    break;
                case 0x02C6:
                    byte = '\x87';
                    break;
                case 0x2030:
                    byte = '\x88';
                    break;
                case 0x0160:
                    byte = '\x89';
                    break;
                case 0x2039:
                    byte = '\x8A';
                    break;
                case 0x0152:
                    byte = '\x8B';
                    break;

                case 0x017D:
                    byte = '\x8E';
                    break;


                case 0x2018:
                    byte = '\x91';
                    break;
                case 0x2019:
                    byte = '\x92';
                    break;
                case 0x201C:
                    byte = '\x93';
                    break;
                case 0x201D:
                    byte = '\x94';
                    break;
                case 0x2022:
                    byte = '\x95';
                    break;
                case 0x2013:
                    byte = '\x96';
                    break;
                case 0x2014:
                    byte = '\x97';
                    break;
                case 0x02DC:
                    byte = '\x98';
                    break;
                case 0x2122:
                    byte = '\x99';
                    break;
                case 0x0161:
                    byte = '\x9A';
                    break;
                case 0x203A:
                    byte = '\x9B';
                    break;
                case 0x0153:
                    byte = '\x9C';
                    break;

                case 0x017E:
                    byte = '\x9E';
                    break;
                case 0x0178:
                    byte = '\x9F';
                    break;

                default:
                    byte = ' ';
                    break;
            }
            arr += byte;
        }
    }
    arr += '\0';
    return arr;
}
