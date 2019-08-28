"""
Personal State Script
=====================
"""
import color
import os
import personal_state
import simple_menu

states = [
    ("No State", personal_state.NO_STATE),
    ("No Contact", personal_state.NO_CONTACT),
    ("Chaos", personal_state.CHAOS),
    ("Communication", personal_state.COMMUNICATION),
    ("Camp", personal_state.CAMP),
]


class StateMenu(simple_menu.Menu):
    color_sel = color.WHITE

    def on_scroll(self, item, index):
        personal_state.set(item[1], False)

    def on_select(self, item, index):
        personal_state.set(item[1], True)
        os.exit()

    def draw_entry(self, item, index, offset):
        if item[1] == personal_state.NO_CONTACT:
            bg = color.RED
            fg = color.WHITE
        elif item[1] == personal_state.CHAOS:
            bg = color.CHAOSBLUE
            fg = color.CHAOSBLUE_DARK
        elif item[1] == personal_state.COMMUNICATION:
            bg = color.COMMYELLOW
            fg = color.COMMYELLOW_DARK
        elif item[1] == personal_state.CAMP:
            bg = color.CAMPGREEN
            fg = color.CAMPGREEN_DARK
        else:
            bg = color.Color(100, 100, 100)
            fg = color.Color(200, 200, 200)

        self.disp.print(" " + str(item[0]) + " " * 9, posy=offset, fg=fg, bg=bg)


if __name__ == "__main__":
    StateMenu(states).run()
