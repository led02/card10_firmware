import leds, utime, math


def col_cor(colors, brightness=1, gamma=1):
    """
    Gamma and  brightness correction for the RGB channels
    """
    return [
        [int(255 * brightness * math.pow((y / 255.0), gamma)) for y in x]
        for x in colors
    ]


def halo(colors):
    """ 
    sets the four bottom/side LEDs to colors corresponding to the color spectrum on the outermost of the top 11 LEDs
    """
    used_leds = len(colors)
    #add additional RGB-Color-lists to the colors-list to fill up the top LEDs with emptiness
    colors += [[0, 0, 0]] * (11 - used_leds)
    #add four additional colors. the last one, the first one twice, the last one. 
    colors += [colors[used_leds - 1]] + [colors[0]] * 2 + [colors[used_leds - 1]]
    return colors


def kitt(
    #amount of cycles for the animation
    cycles=100,
    #time in microseconds until the animation moves on. (we could also call it framerate)
    delay=80,
    #the shape of your brightness curve. bigger values make a steeper curve, smaller values make the curve wider.
    power=10,
    #the minimal brightness
    minimum=0.3,
    #if you don't enter a spectrum this is the color we'll use
    rgb=[255, 0, 0],
    #a color spectrum consisting of up to 11 RGB-Value-Lists (e.g. [[255,255,255], [0,0,0], [255,255,255] and so on] - ). if you use less, the animation will be less wide.
    spectrum=[],
    
    halo=False,
):
    """
    LED Animation. Knight rider-Style. 
    """
    # create a basic table of values for a smooth increment of the LED brightness (if you don't understand this, don't worry, i don't either. just paste it into the python shell and see the output). Basically creates a negative cosinus curve. 
    kitt_table = [((-math.cos(math.pi * (x / 10.0))) + 1) / 2.0 for x in range(21)]
    #adjust the values to start with a minimum brightness and the width of the curve to the given power.
    kitt_table = [math.pow(x, power) * (1 - minimum) + minimum for x in kitt_table]

    #for the amount of specified cycles 
    for i in range(cycles):
        #repeat every 20 steps
        j = i % 20
        #and go backwards after 10 steps
        if j > 10:
            j = 20 - j
        #if a color spectrum wasn't given
        if spectrum == []:
            #set the amount of LEDs used to 11, because we're using the full width
            used_leds = 11
            #set the color values to the LEDs by multiplying the given color value with the corresponding brightness value in the kitt table
            output = [[int(x * y) for y in rgb] for x in kitt_table[j : (j + used_leds)]]
        else:
            #use the amount of leds specified in the spectrum
            used_leds = len(spectrum)
            #multiply the color values in the corresponding spectrum tuple with the brightness value in the kitt table
            output = [
                [int(y * kitt_table[j + x]) for y in spectrum[x]]
                for x in range(used_leds)
            ]
        #if a halo is True, also use the four bottom LEDs
        if halo:
            halo(output)
        #set the LEDs to the output defined above
        leds.set_all(output)
        #sleep for the amount of milliseconds specified in delay
        utime.sleep_ms(delay)
    #Switch off all LEDs.
    leds.clear()
