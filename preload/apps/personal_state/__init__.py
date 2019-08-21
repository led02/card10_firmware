"""
Personal State Script
===========
With this script you can 
"""
import buttons
import color
import display
import os
import personal_state

states = [
    ("No State", personal_state.NO_STATE),
    ("No Contact", personal_state.NO_CONTACT),
    ("Chaos", personal_state.CHAOS),
    ("Communication", personal_state.COMMUNICATION),
    ("Camp", personal_state.CAMP),
]


def button_events():
    """Iterate over button presses (event-loop)."""
    yield 0
    button_pressed = False
    while True:
        v = buttons.read(buttons.BOTTOM_LEFT | buttons.BOTTOM_RIGHT | buttons.TOP_RIGHT)

        if v == 0:
            button_pressed = False

        if not button_pressed and v & buttons.BOTTOM_LEFT != 0:
            button_pressed = True
            yield buttons.BOTTOM_LEFT

        if not button_pressed and v & buttons.BOTTOM_RIGHT != 0:
            button_pressed = True
            yield buttons.BOTTOM_RIGHT

        if not button_pressed and v & buttons.TOP_RIGHT != 0:
            button_pressed = True
            yield buttons.TOP_RIGHT


COLOR1, COLOR2 = (color.CHAOSBLUE_DARK, color.CHAOSBLUE)


def draw_menu(disp, idx, offset):
    disp.clear()

    for y, i in enumerate(range(len(states) + idx - 3, len(states) + idx + 4)):
        selected = states[i % len(states)]
        disp.print(
            " " + selected[0] + " " * (11 - len(selected[0])),
            posy=offset + y * 20 - 40,
            bg=COLOR1 if i % 2 == 0 else COLOR2,
        )

    disp.print(">", posy=20, fg=color.COMMYELLOW, bg=COLOR2 if idx % 2 == 0 else COLOR1)
    disp.update()


def main():
    disp = display.open()
    numstates = len(states)

    current, _ = personal_state.get()
    for ev in button_events():
        if ev == buttons.BOTTOM_RIGHT:
            # Scroll down
            draw_menu(disp, current, -8)
            current = (current + 1) % numstates
            state = states[current]
            personal_state.set(state[1], False)
        elif ev == buttons.BOTTOM_LEFT:
            # Scroll up
            draw_menu(disp, current, 8)
            current = (current + numstates - 1) % numstates
            state = states[current]
            personal_state.set(state[1], False)
        elif ev == buttons.TOP_RIGHT:
            state = states[current]
            personal_state.set(state[1], True)
            # Select & start
            disp.clear().update()
            disp.close()
            os.exit(0)

        draw_menu(disp, current, 0)


if __name__ == "__main__":
    main()
