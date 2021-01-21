

powerline_separators = {"Left": "\ue0b0", "Right": "\ue0b2",
                        "AltLeft": "\ue0b1", "AltRight": "\ue0b3"}

colors = {"Black": "\u001b[30m",
          "Red": "\u001b[31m",
          "Green": "\u001b[32m",
          "Yellow": "\u001b[33m",
          "Blue": "\u001b[34m",
          "Magenta": "\u001b[35m",
          "Cyan": "\u001b[36m",
          "Grey": "\u001b[37m",
          "DarkGrey": "\u001b[90m",
          "BrightRed": "\u001b[91m",
          "BrightGreen": "\u001b[92m",
          "BrightYellow": "\u001b[93m",
          "BrightBlue": "\u001b[94m",
          "BrightMagenta": "\u001b[95m",
          "BrightCyan": "\u001b[96m",
          "White": "\u001b[97m"}
reset = "\u001b[0m"
background_colors = {"Black": "\u001b[40m",
                     "Red": "\u001b[41m",
                     "Green": "\u001b[42m",
                     "Yellow": "\u001b[43m",
                     "Blue": "\u001b[44m",
                     "Magenta": "\u001b[45m",
                     "Cyan": "\u001b[46m",
                     "Grey": "\u001b[47m",
                     "DarkGrey": "\u001b[100m",
                     "BrightRed": "\u001b[101m",
                     "BrightGreen": "\u001b[102m",
                     "BrightYellow": "\u001b[103m",
                     "BrightBlue": "\u001b[104m",
                     "BrightMagenta": "\u001b[105m",
                     "BrightCyan": "\u001b[106m",
                     "White": "\u001b[107m"}

# for v in background_colors.values():
#    print(v+" ")
# print(reset)


class PowerlineString:

    def __init__(self, color: str, background: str, string: str, priority=0):
        self.color = color
        self.background = background
        self.string = string
        self.priority = priority

    def render(self) -> str:
        return colors[self.color]+background_colors[self.background]+self.string+reset


class PowerlineSequence:
    def __init__(self, lst: list, from_raw=False):
        if from_raw:
            raw_lst = lst
            lst = []
            for s in raw_lst:
                if len(s) == 3:
                    if isinstance(s[0], str) and isinstance(s[1], str) and isinstance(s[2], str):
                        lst.append(PowerlineString(s[0], s[1], s[2]))
                elif len(s) == 4:
                    if isinstance(s[0], str) and isinstance(s[1], str) and isinstance(s[2], str) and isinstance(s[3], int):
                        lst.append(PowerlineString(s[0], s[1], s[2], s[3]))
        self.lst = lst
        self.leftBackground = lst[0].background
        self.rightBackground = lst[-1].background

    def render(self, decorate=[]) -> str:
        if len(self.lst) == 0:
            return ""

        rendered_string = ""
        if "Left" in decorate:
            rendered_string += colors[self.leftBackground] + \
                powerline_separators["Right"] + reset

        if len(self.lst) == 1:
            rendered_string += self.lst[0].render
        else:
            for i in range(len(self.lst)-1):
                lst = self.lst
                rendered_string += lst[i].render()
                separator = ""
                color = ""
                background = ""
                if lst[i].background == lst[i+1].background:
                    separator += "Alt"
                    background = lst[i].background
                    if lst[i].priority >= lst[i+1].priority:
                        color = lst[i].color
                    else:
                        color = lst[i+1].color
                if lst[i].priority >= lst[i+1].priority:
                    separator += "Left"
                    if background == "":
                        color = lst[i].background
                        background = lst[i+1].background
                else:
                    separator += "Right"
                    if background == "":
                        color = lst[i+1].background
                        background = lst[i].background
                rendered_string += colors[color]+background_colors[background] + \
                    powerline_separators[separator] + \
                    reset
            rendered_string += lst[-1].render()
        if "Right" in decorate:
            rendered_string += colors[self.rightBackground] + \
                powerline_separators["Left"] + reset
        return rendered_string


def tasker(taskname, status, info="", tasklen=20):
    line = []
    if taskname != "":
        line.append(["Grey", "Black", " "+taskname+" "])
    stcol = "White"
    stback = "Blue"
    infcol = "Grey"
    if status == "OK":
        stback = "Green"
    elif status == "ERROR":
        stback = "Red"
        infcol = "Red"
    elif status == "...":
        stcol = "Black"
        stback = "Grey"
    elif status == "IGNORED":
        stcol = "Black"
        stback = "BrightBlue"
    line.append([stcol, stback, " \u001b[1m"+status +
                 "\u001b[22m "])  # Bold on, then bold off
    if info != "":
        line.append([infcol, "Black", " "+info+" "])
    return line
