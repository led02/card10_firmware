flags = {}
flags["rainbow"] = [
    [255, 0, 24],
    [255, 165, 44],
    [255, 255, 65],
    [0, 128, 24],
    [0, 0, 249],
    [134, 0, 125],
]
flags["trans"] = [
    [85, 205, 252],
    [247, 168, 184],
    [255, 255, 255],
    [247, 168, 184],
    [85, 205, 252],
]
flags["bi"] = [[214, 2, 112], [155, 79, 150], [0, 56, 168]]
flags["ace"] = [[1, 1, 1], [164, 164, 164], [255, 255, 255], [150, 0, 150]]
flags["greyace"] = [
    [150, 0, 150],
    [164, 164, 164],
    [255, 255, 255],
    [164, 164, 164],
    [150, 0, 150],
]
flags["aro"] = [
    [61, 165, 66],
    [167, 211, 121],
    [255, 255, 255],
    [169, 169, 169],
    [1, 1, 1],
]
flags["greyaro"] = [
    [61, 165, 66],
    [164, 164, 164],
    [255, 255, 255],
    [164, 164, 164],
    [61, 165, 66],
]
flags["pan"] = [[255, 27, 141], [255, 218, 0], [27, 179, 255]]
flags["inter"] = [[255, 218, 0], [122, 0, 172]]
flags["genderqueer"] = [[201, 138, 255], [255, 255, 255], [80, 150, 85]]
flags["lesbian"] = [
    [139, 60, 105],
    [171, 99, 143],
    [187, 127, 179],
    [255, 255, 255],
    [228, 172, 207],
    [214, 113, 113],
    [134, 70, 70],
]
flags["nonbinary"] = [[255, 244, 51], [255, 255, 255], [155, 89, 208], [0, 0, 0]]
flags["genderfluid"] = [
    [254, 117, 161],
    [255, 255, 255],
    [189, 22, 213],
    [0, 0, 0],
    [50, 61, 187],
]
flags["agender"] = [
    [0, 0, 0],
    [150, 150, 150],
    [255, 255, 255],
    [182, 245, 131],
    [255, 255, 255],
    [150, 150, 150],
    [0, 0, 0],
]
flags["poly"] = [
    [0, 0, 255],
    [0, 0, 255],
    [0, 0, 255],
    [255, 0, 0],
    [255, 0, 0],
    [255, 255, 0],
    [255, 0, 0],
    [255, 0, 0],
    [0, 0, 0],
    [0, 0, 0],
    [0, 0, 0],
]
import leds, display, math, utime, ledfx


def expand(colors, cutoff=12):
    output = []
    if len(colors) != 7 or cutoff > 14:
        leds_per_color = int(cutoff / len(colors))
        for i in colors:
            output += [i] * leds_per_color
    else:
        for j, i in enumerate(colors):
            output += [i] * (1 + (j % 2 == 1))
    return output


def show_leds(flag="rainbow", brightness=0.5, gamma=1, cutoff=12, halo=True):
    colors = ledfx.col_cor(flags[flag], brightness, gamma)
    output = expand(colors, cutoff)[::-1][0:11]
    if halo:
        output = ledfx.halo(output)
    leds.clear()
    leds.set_all(output)


def show_display(flag="rainbow", brightness=1, gamma=1):
    colors = ledfx.col_cor(flags[flag], brightness, gamma)
    colors = expand(colors, 160)
    with display.open() as disp:
        disp.clear()
        for line, color in enumerate(colors):
            disp.line(line, 0, line, 80, col=color)
        disp.update()
        disp.close()


def get(flag="rainbow"):
    return flags[flag]


def demo(s_delay=2):
    for i in flags:
        print(i)
        show_leds(flag=i)
        show_display(flag=i)
        utime.sleep_ms(s_delay * 1000)
    leds.clear()
    with display.open() as disp:
        disp.clear().update()
        disp.close()
