# Generator for the OLEDDashboard custom bitmap fonts.
# Produces C PROGMEM arrays. Run:  python gen_fonts.py
import io, os

def glyph_strings():
    """Large 8x16 bold glyphs. 16 rows of 8 chars, '#' = on, '.' = off."""
    G = {}
    G['0'] = [
        "..####..",
        ".######.",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        ".######.",
        "..####..",
    ]
    G['1'] = [
        "....##..",
        "...###..",
        "..####..",
        ".#####..",
        "...##...",
        "...##...",
        "...##...",
        "...##...",
        "...##...",
        "...##...",
        "...##...",
        "...##...",
        "...##...",
        "...##...",
        ".######.",
        ".######.",
    ]
    G['2'] = [
        "..#####.",
        "########",
        "#.....##",
        "......##",
        "......##",
        ".....##.",
        "....##..",
        "...##...",
        "..##....",
        ".##.....",
        "##......",
        "##......",
        "##......",
        "##......",
        "########",
        "########",
    ]
    G['3'] = [
        "..#####.",
        "########",
        "#.....##",
        "......##",
        "......##",
        ".....##.",
        "..####..",
        ".....##.",
        "......##",
        "......##",
        "......##",
        "......##",
        "#.....##",
        "#.....##",
        "########",
        "..#####.",
    ]
    G['4'] = [
        "....###.",
        "...####.",
        "..##.##.",
        "..##.##.",
        ".##..##.",
        "##...##.",
        "##...##.",
        "##...##.",
        "########",
        "########",
        ".....##.",
        ".....##.",
        ".....##.",
        ".....##.",
        ".....###",
        ".....###",
    ]
    G['5'] = [
        "########",
        "########",
        "##......",
        "##......",
        "##......",
        "##......",
        ".######.",
        "######.#",
        "......##",
        "......##",
        "......##",
        "......##",
        "#.....##",
        "#.....##",
        "########",
        "..#####.",
    ]
    G['6'] = [
        "..######",
        ".#######",
        "##......",
        "##......",
        "##......",
        "##......",
        ".######.",
        "########",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "########",
        ".######.",
    ]
    G['7'] = [
        "########",
        "########",
        "#.....##",
        "#.....##",
        "......##",
        ".....##.",
        "....##..",
        "...##...",
        "..##....",
        "..##....",
        "..##....",
        "..##....",
        "..##....",
        "..##....",
        "..####..",
        "..####..",
    ]
    G['8'] = [
        "..#####.",
        "########",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        ".######.",
        "########",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "########",
        "..#####.",
    ]
    G['9'] = [
        "..#####.",
        "########",
        "##....##",
        "##....##",
        "##....##",
        "##....##",
        "########",
        ".######.",
        "......##",
        "......##",
        "......##",
        "......##",
        "......##",
        "......##",
        "#######.",
        "######..",
    ]
    G['.'] = [
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        ".####...",
        "#######.",
        "#######.",
    ]
    G['-'] = [
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        ".######.",
        "########",
        "########",
        "........",
        "........",
        "........",
        "........",
        "........",
    ]
    G['%'] = [
        "..####..",
        ".##..##.",
        ".##..##.",
        ".##..##.",
        "..####..",
        "........",
        "........",
        "....##..",
        "...##...",
        "...##...",
        "...##...",
        "..##....",
        "..####..",
        ".##..##.",
        ".##..##.",
        "..####..",
    ]
    G[':'] = [
        "........",
        "........",
        "........",
        "..####..",
        "..####..",
        "..####..",
        "........",
        "........",
        "........",
        "........",
        "........",
        "..####..",
        "..####..",
        "..####..",
        "........",
        "........",
    ]
    G['C'] = [
        "..######",
        ".#######",
        "###.....",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "###.....",
        ".#######",
        "..######",
    ]
    G['DEG'] = [  # degree symbol
        ".####...",
        "##..##..",
        "##..##..",
        ".####...",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
    ]
    G['+'] = [
        "........",
        "........",
        "....##..",
        "....##..",
        "....##..",
        "....##..",
        ".#######",
        "########",
        "########",
        "....##..",
        "....##..",
        "....##..",
        "....##..",
        "........",
        "........",
        "........",
    ]
    G['F'] = [
        "########",
        "########",
        "##......",
        "##......",
        "##......",
        "##......",
        "#######.",
        "#######.",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
        "##......",
    ]
    return G

def emit_large(g):
    lines = []
    lines.append("// Auto-generated by gen_fonts.py -- DO NOT EDIT BY HAND.")
    lines.append("#include <Arduino.h>")
    lines.append("")
    glyphs = list(g.items())
    # digits first for fast access, then symbols
    order = [('d', '0'), ('d', '1'), ('d', '2'), ('d', '3'), ('d', '4'),
             ('d', '5'), ('d', '6'), ('d', '7'), ('d', '8'), ('d', '9'),
             ('c', '.'), ('c', '-'), ('c', '%'), ('c', ':'), ('c', 'C'),
             ('c', 'DEG'), ('c', '+'), ('c', 'F')]
    lines.append("/// 8x16 large bold glyph. Rows of 8 bits (bit 7 = leftmost).")
    lines.append("/// data points to the PROGMEM row bytes.")
    lines.append("typedef struct {")
    lines.append("    uint8_t code;")
    lines.append("    const uint8_t* data;")
    lines.append("} FontLargeGlyph;")
    lines.append("")
    # emit each glyph as static const to avoid large blobs inline
    for i, (kind, key) in enumerate(order):
        rows = g[key]
        name = "sLarge%02d" % i
        lines.append("static const uint8_t %s[%d] PROGMEM = {" % (name, len(rows)))
        for r in rows:
            v = 0
            for ch in r:
                v = (v << 1) | (1 if ch == '#' else 0)
            lines.append("    0x%02X," % v)
        lines.append("};")
        lines.append("")
    lines.append("/// Index table: ordered glyphs referenced by the renderer.")
    lines.append("static const FontLargeGlyph sLargeGlyphs[] PROGMEM = {")
    for i, (kind, key) in enumerate(order):
        if kind == 'd':
            code = str(ord(key))
        elif key == 'DEG':
            code = "0xB0"
        else:
            code = repr(key)
        lines.append("    {%s, %s}," % (code, "sLarge%02d" % i))
    lines.append("};")
    lines.append("")
    lines.append("/// Total number of large glyphs.")
    lines.append("static constexpr uint8_t kLargeGlyphCount = %d;" % len(order))
    lines.append("static constexpr int8_t kLargeGlyphH = %d;" % len(g['0']))
    lines.append("static constexpr int8_t kLargeGlyphW = 8;")
    lines.append("")
    return "\n".join(lines)

def emit_small():
    # Classic public-domain 5x7 font, full ASCII 0x20..0x7E, column-major,
    # 5 bytes per glyph, bit 0 = top row.
    rows = []
    rows.append("// Auto-generated by gen_fonts.py -- DO NOT EDIT BY HAND.")
    rows.append("#include <Arduino.h>")
    rows.append("")
    rows.append("/// 5x7 small font. 95 glyphs (0x20..0x7E), 5 bytes each.")
    rows.append("/// Each byte is one column; bit 0 is the top pixel.")
    rows.append("static const uint8_t sSmallFont[95][5] PROGMEM = {")
    font = small_font_table()
    for i, g in enumerate(font):
        if i % 8 == 0:
            rows.append("    // 0x%02X" % (i + 0x20))
        rows.append("    {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}," % tuple(g))
    rows.append("};")
    rows.append("")
    rows.append("/// Small font metrics.")
    rows.append("static constexpr int8_t kSmallGlyphW = 5;")
    rows.append("static constexpr int8_t kSmallGlyphH = 7;")
    rows.append("static constexpr uint8_t kSmallFirst = 0x20;")
    rows.append("static constexpr uint8_t kSmallLast = 0x7E;")
    rows.append("")
    return "\n".join(rows)

def small_font_table():
    # 95 glyphs 0x20..0x7E. Classic public domain 5x7 font (Myst 5x7),
    # column-major, bit 0 = top. Source glyphs from the well known table.
    T = [
        [0,0,0,0,0],                     # 0x20 space
        [0,0,95,0,0],                    # 0x21 !
        [0,7,0,7,0],                     # 0x22 "
        [20,127,20,127,20],              # 0x23 #
        [36,42,127,42,18],               # 0x24 $
        [35,19,8,100,98],                # 0x25 %
        [54,73,85,34,80],                # 0x26 &
        [0,5,3,0,0],                     # 0x27 '
        [0,28,34,65,0],                  # 0x28 (
        [0,65,34,28,0],                  # 0x29 )
        [20,8,62,8,20],                  # 0x2A *
        [8,8,62,8,8],                    # 0x2B +
        [0,80,48,0,0],                   # 0x2C ,
        [8,8,8,8,8],                     # 0x2D -
        [0,96,96,0,0],                   # 0x2E .
        [32,16,8,4,2],                   # 0x2F /
        [62,81,73,69,62],                # 0x30 0
        [0,66,127,64,0],                 # 0x31 1
        [66,97,81,73,70],                # 0x32 2
        [33,65,69,75,49],                # 0x33 3
        [24,20,18,127,16],               # 0x34 4
        [39,69,69,69,57],                # 0x35 5
        [60,74,73,73,48],                # 0x36 6
        [1,113,9,5,3],                   # 0x37 7
        [54,73,73,73,54],                # 0x38 8
        [6,73,73,41,30],                 # 0x39 9
        [0,54,54,0,0],                   # 0x3A :
        [0,86,54,0,0],                   # 0x3B ;
        [8,20,34,65,0],                  # 0x3C <
        [20,20,20,20,20],                # 0x3D =
        [0,65,34,20,8],                  # 0x3E >
        [2,1,81,9,6],                    # 0x3F ?
        [50,73,121,65,62],               # 0x40 @
        [126,17,17,17,126],              # 0x41 A
        [127,73,73,73,54],               # 0x42 B
        [62,65,65,65,34],                # 0x43 C
        [127,65,65,34,28],               # 0x44 D
        [127,73,73,73,65],               # 0x45 E
        [127,9,9,9,1],                   # 0x46 F
        [62,65,73,73,122],               # 0x47 G
        [127,8,8,8,127],                 # 0x48 H
        [0,65,127,65,0],                 # 0x49 I
        [32,64,65,63,1],                 # 0x4A J
        [127,8,20,34,65],                # 0x4B K
        [127,64,64,64,64],               # 0x4C L
        [127,2,12,2,127],                # 0x4D M
        [127,4,8,16,127],                # 0x4E N
        [62,65,65,65,62],                # 0x4F O
        [127,9,9,9,6],                   # 0x50 P
        [62,65,81,33,94],                # 0x51 Q
        [127,9,25,41,70],                # 0x52 R
        [70,73,73,73,49],                # 0x53 S
        [1,1,127,1,1],                   # 0x54 T
        [63,64,64,64,63],                # 0x55 U
        [31,32,64,32,31],                # 0x56 V
        [63,64,56,64,63],                # 0x57 W
        [99,20,8,20,99],                 # 0x58 X
        [7,8,112,8,7],                   # 0x59 Y
        [97,81,73,69,67],                # 0x5A Z
        [0,127,65,65,0],                 # 0x5B [
        [2,4,8,16,32],                   # 0x5C backslash
        [0,65,65,127,0],                 # 0x5D ]
        [4,2,1,2,4],                     # 0x5E ^
        [64,64,64,64,64],                # 0x5F _
        [0,1,2,4,0],                     # 0x60 `
        [32,84,84,84,120],               # 0x61 a
        [127,72,68,68,56],               # 0x62 b
        [56,68,68,68,32],                # 0x63 c
        [56,68,68,72,127],               # 0x64 d
        [56,84,84,84,24],                # 0x65 e
        [8,126,9,1,2],                   # 0x66 f
        [12,82,82,82,62],                # 0x67 g
        [127,8,4,4,120],                 # 0x68 h
        [0,68,125,64,0],                 # 0x69 i
        [32,64,68,61,0],                 # 0x6A j
        [127,16,40,68,0],                # 0x6B k
        [0,65,127,64,0],                 # 0x6C l
        [124,4,24,4,120],                # 0x6D m
        [124,8,4,4,120],                 # 0x6E n
        [56,68,68,68,56],                # 0x6F o
        [124,20,20,20,8],                # 0x70 p
        [8,20,20,24,124],                # 0x71 q
        [124,8,4,4,8],                   # 0x72 r
        [72,84,84,84,32],                # 0x73 s
        [4,63,68,64,32],                 # 0x74 t
        [60,64,64,32,124],               # 0x75 u
        [28,32,64,32,28],                # 0x76 v
        [60,64,48,64,60],                # 0x77 w
        [68,40,16,40,68],                # 0x78 x
        [12,80,80,80,60],                # 0x79 y
        [68,100,84,76,68],               # 0x7A z
        [0,8,54,65,0],                   # 0x7B {
        [0,0,127,0,0],                   # 0x7C |
        [0,65,54,8,0],                   # 0x7D }
        [8,4,8,16,8],                    # 0x7E ~
    ]
    assert len(T) == 95, len(T)
    return T

def compact_glyphs():
    """Compact 4x6 font (6 rows of 4 chars) for the footer statistics."""
    G = {}
    G[' '] = [
        "....",
        "....",
        "....",
        "....",
        "....",
        "....",
    ]
    G['0'] = [
        ".XX.",
        "X..X",
        "X..X",
        "X..X",
        "X..X",
        ".XX.",
    ]
    G['1'] = [
        "..X.",
        ".XX.",
        "..X.",
        "..X.",
        "..X.",
        ".XXX",
    ]
    G['2'] = [
        ".XX.",
        "X..X",
        "...X",
        "..X.",
        ".X..",
        "XXXX",
    ]
    G['3'] = [
        "XXX.",
        "...X",
        "..X.",
        "...X",
        "X..X",
        ".XX.",
    ]
    G['4'] = [
        "..X.",
        ".XX.",
        "X.X.",
        "XXXX",
        "..X.",
        "..X.",
    ]
    G['5'] = [
        "XXXX",
        "X...",
        "XXX.",
        "...X",
        "X..X",
        ".XX.",
    ]
    G['6'] = [
        ".XX.",
        "X...",
        "XXX.",
        "X..X",
        "X..X",
        ".XX.",
    ]
    G['7'] = [
        "XXXX",
        "...X",
        "..X.",
        ".X..",
        ".X..",
        ".X..",
    ]
    G['8'] = [
        ".XX.",
        "X..X",
        ".XX.",
        "X..X",
        "X..X",
        ".XX.",
    ]
    G['9'] = [
        ".XX.",
        "X..X",
        ".XXX",
        "...X",
        "X..X",
        ".XX.",
    ]
    G['.'] = [
        "....",
        "....",
        "....",
        "....",
        "..X.",
        "..X.",
    ]
    G[':'] = [
        "....",
        ".X..",
        ".X..",
        "....",
        ".X..",
        ".X..",
    ]
    G['-'] = [
        "....",
        "....",
        "XXXX",
        "....",
        "....",
        "....",
    ]
    G['M'] = [
        "X..X",
        "XX.X",
        "X.XX",
        "X..X",
        "X..X",
        "X..X",
    ]
    G['A'] = [
        ".XX.",
        "X..X",
        "X..X",
        "XXXX",
        "X..X",
        "X..X",
    ]
    G['i'] = [
        ".X..",
        "XX..",
        ".X..",
        ".X..",
        ".X..",
        "XXX.",
    ]
    G['n'] = [
        "....",
        "X.X.",
        "XX..",
        "X...",
        "X...",
        "X...",
    ]
    G['x'] = [
        "....",
        "X..X",
        ".XX.",
        ".XX.",
        "X..X",
        "....",
    ]
    G['a'] = [
        "....",
        "XXX.",
        "...X",
        ".XXX",
        "X..X",
        ".XXX",
    ]
    G['g'] = [
        "....",
        ".XXX",
        "X..X",
        ".XXX",
        "...X",
        ".XX.",
    ]
    G['v'] = [
        "....",
        "X..X",
        "X..X",
        "X..X",
        ".XX.",
        "....",
    ]
    G['C'] = [
        ".XXX",
        "X...",
        "X...",
        "X...",
        "X...",
        ".XXX",
    ]
    G['%'] = [
        "XX.X",
        "XX.X",
        "...X",
        "..X.",
        ".X..",
        "....",
    ]
    return G


def emit_compact(g):
    lines = []
    lines.append("// Auto-generated by gen_fonts.py -- DO NOT EDIT BY HAND.")
    lines.append("/// 4x6 compact glyph, 4 bytes (one per column), bit 0 = top.")
    lines.append("typedef struct {")
    lines.append("    uint8_t code;")
    lines.append("    const uint8_t* data;")
    lines.append("} FontCompactGlyph;")
    lines.append("")
    items = list(g.items())
    for i, (ch, rows) in enumerate(items):
        cols = [[0] * 6 for _ in range(4)]
        for r in range(6):
            for c in range(4):
                if rows[r][c] == 'X':
                    cols[c][r] = 1
        lines.append("static const uint8_t sCompact%02d[4] PROGMEM = {" % i)
        for c in range(4):
            v = 0
            for r in range(6):
                if cols[c][r]:
                    v |= (1 << r)
            lines.append("    0x%02X," % v)
        lines.append("};")
        lines.append("")
    lines.append("/// Indexed compact glyphs.")
    lines.append("static const FontCompactGlyph sCompactGlyphs[] PROGMEM = {")
    for i, (ch, _) in enumerate(items):
        lines.append("    {%s, sCompact%02d}," % (repr(ch), i))
    lines.append("};")
    lines.append("")
    lines.append("static constexpr uint8_t kCompactGlyphCount = %d;" % len(items))
    lines.append("static constexpr int8_t kCompactGlyphW = 4;")
    lines.append("static constexpr int8_t kCompactGlyphH = 6;")
    lines.append("")
    return "\n".join(lines)


def main():
    out = r"D:\CodingProject\OLEDDashBoardLib\src\Fonts.h"
    g = glyph_strings()
    parts = []
    parts.append("/*")
    parts.append(" * Fonts.h -- custom bitmap fonts for OLEDDashboard.")
    parts.append(" *")
    parts.append(" * Auto-generated font data: 5x7 small font (full printable")
    parts.append(" * ASCII) and an 8x16 large bold numeric font. Do not edit the")
    parts.append(" * generated arrays by hand; regenerate with gen_fonts.py.")
    parts.append(" *")
    parts.append(" * Large-font glyph codes: 0x20 ' ', '0'-'9', '.' '-' '%' ':' ")
    parts.append(" * '+' 'C' 'F' and 0xB0 for the degree symbol. The large font")
    parts.append(" * renderer falls back to the small font for unknown glyphs.")
    parts.append(" */")
    parts.append("#ifndef OLED_DASHBOARD_FONTS_H")
    parts.append("#define OLED_DASHBOARD_FONTS_H")
    parts.append("#include <Arduino.h>")
    parts.append("")
    parts.append(emit_small())
    parts.append("")
    parts.append(emit_large(g))
    parts.append("")
    parts.append(emit_compact(compact_glyphs()))
    parts.append("#endif  // OLED_DASHBOARD_FONTS_H")
    parts.append("")
    text = "\n".join(parts)
    with io.open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)
    print("Wrote", out, len(text), "bytes")

if __name__ == "__main__":
    main()
