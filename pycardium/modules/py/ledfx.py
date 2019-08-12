import leds, utime, math


def col_cor(colors, brightness=1, gamma=1):
    return [
        [int(255 * brightness * math.pow((y / 255.0), gamma)) for y in x]
        for x in colors
    ]


def halo(colors):
    used_leds = len(colors)
    colors += [[0, 0, 0]] * (11 - used_leds)
    colors += [colors[used_leds - 1]] + [colors[0]] * 2 + [colors[used_leds - 1]]
    return colors


def kitt(
    cycles=100,
    delay=80,
    power=10,
    minimum=0.3,
    rgb=[255, 0, 0],
    spectrum=[],
    halo=False,
):

    kitt_table = [((-math.cos(math.pi * (x / 10.0))) + 1) / 2.0 for x in range(21)]
    kitt_table = [math.pow(x, power) * (1 - minimum) + minimum for x in kitt_table]

    for i in range(cycles):
        j = i % 20
        if j > 10:
            j = 20 - j
        if spectrum == []:
            used_leds = 11
            output = [[int(x * y) for y in rgb] for x in kitt_table[j : (j + 11)]]
        else:
            used_leds = len(spectrum)
            output = [
                [int(y * kitt_table[j + x]) for y in spectrum[x]]
                for x in range(used_leds)
            ]
        if halo:
            halo(output)
        leds.set_all(output)
        utime.sleep_ms(delay)
    leds.clear()
