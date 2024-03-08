#include "TWTextCoder.h"

static constexpr QChar encodeTable[] {
        QChar{'\x00'},
        QChar{'\x01'},
        QChar{'\x02'},
        QChar{'\x03'},
        QChar{'\x04'},
        QChar{'\x05'},
        QChar{'\x06'},
        QChar{'\x07'},
        QChar{'\x08'},
        QChar{'\x09'},
        QChar{'\x0A'},
        QChar{'\x0B'},
        QChar{'\x0C'},
        QChar{'\x0D'},
        QChar{'\x0E'},
        QChar{'\x0F'},
        QChar{'\x10'},
        QChar{'\x11'},
        QChar{'\x12'},
        QChar{'\x13'},
        QChar{'\x14'},
        QChar{'\x15'},
        QChar{'\x16'},
        QChar{'\x17'},
        QChar{'\x18'},
        QChar{'\x19'},
        QChar{'\x1A'},
        QChar{'\x1B'},
        QChar{'\x1C'},
        QChar{'\x1D'},
        QChar{'\x1E'},
        QChar{'\x1F'}, //Control codes are all the same as in unicode
        QChar{' '},
        QChar{'!'},
        QChar{'"'},
        QChar{'#'},
        QChar{'$'},
        QChar{'%'},
        QChar{'&'},
        QChar{'\''},
        QChar{'('},
        QChar{')'},
        QChar{'*'},
        QChar{'+'},
        QChar{','},
        QChar{'-'},
        QChar{'.'},
        QChar{'/'},
        QChar{'0'},
        QChar{'1'},
        QChar{'2'},
        QChar{'3'},
        QChar{'4'},
        QChar{'5'},
        QChar{'6'},
        QChar{'7'},
        QChar{'8'},
        QChar{'9'},
        QChar{':'},
        QChar{';'},
        QChar{'<'},
        QChar{'='},
        QChar{'>'},
        QChar{'?'},
        QChar{'@'},
        QChar{'A'},
        QChar{'B'},
        QChar{'C'},
        QChar{'D'},
        QChar{'E'},
        QChar{'F'},
        QChar{'G'},
        QChar{'H'},
        QChar{'I'},
        QChar{'J'},
        QChar{'K'},
        QChar{'L'},
        QChar{'M'},
        QChar{'N'},
        QChar{'O'},
        QChar{'P'},
        QChar{'Q'},
        QChar{'R'},
        QChar{'S'},
        QChar{'T'},
        QChar{'U'},
        QChar{'V'},
        QChar{'W'},
        QChar{'X'},
        QChar{'Y'},
        QChar{'Z'},
        QChar{'['},
        QChar{'\\'},
        QChar{']'},
        QChar{'^'},
        QChar{'_'},
        QChar{'`'},
        QChar{'a'},
        QChar{'b'},
        QChar{'c'},
        QChar{'d'},
        QChar{'e'},
        QChar{'f'},
        QChar{'g'},
        QChar{'h'},
        QChar{'i'},
        QChar{'j'},
        QChar{'k'},
        QChar{'l'},
        QChar{'m'},
        QChar{'n'},
        QChar{'o'},
        QChar{'p'},
        QChar{'q'},
        QChar{'r'},
        QChar{'s'},
        QChar{'t'},
        QChar{'u'},
        QChar{'v'},
        QChar{'w'},
        QChar{'x'},
        QChar{'y'},
        QChar{'z'},
        QChar{'{'},
        QChar{'|'},
        QChar{'}'},
        QChar{'~'},
        QChar{'\x7F'}, //DEL char, also in unicode

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
