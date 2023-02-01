from anytree import NodeMixin, RenderTree

storyNames = ["THE TWO DROVERS", "MR. DEUCEACE", "THE BROTHERS", '"DR. MANETTE\'S MANUSCRIPT"', "THE CALDRON OF OIL",
              "THE BURIAL OF THE TITHE", "THE KNIGHTSBRIDGE MYSTERY", "THE COURTING OF DINAH SHADD", "THE SIRE DE MALETROIT'S DOOR",
              "THE SECRET OF GORESTHORPE GRANGE", "A CHANGE OF TREATMENT", "THE STICKIT MINISTER", "THE LAMMAS PREACHING",
              "AN UNDERGRADUATE'S AUNT", "THE SILHOUETTES", "MY BROTHER HENRY", "GILRAY'S FLOWER-POT", "MR. O'LEARY'S SECOND LOVE",
              "THE INDIFFERENCE OF THE MILLER OF HOFBAU", "THE STOLEN BODY", 'THE LAZARETTE OF THE "HUNTRESS"',
              "THE GREAT TRIANGULAR DUEL", "THREE THIMBLES AND A PEA"]

stories = []
chapters = []
specialName = []
roman = ["I", "I.", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X"]

f = open("/home/manh/Working_Space/DSAA/EX2/pg32846.txt", "r")
content = f.read()
f.close()
rows = content.split("\n")


def addStoryNames():
    i = -1
    count = 0
    for idx in range(134, len(rows)):
        if (rows[idx] == ''):
            count += 1
        else:
            count = 0
        if (count == 4):
            stories[i].rEnd = idx + 1
        if (i != 22):
            if (rows[idx].find(storyNames[i + 1]) != -1):
                i += 1
                storyName = storyNames[i]
                stories.append(MyClass(storyName, idx + 1, 0, parent=Stories))
def addChapterNames():
    for idx1 in range(0, len(stories) - 1):
        chapters.append([])
        for idx2 in range(stories[idx1].rBegin - 1, stories[idx1].rEnd - 1):
            if (rows[idx2].find("CHAPTER") != -1 or rows[idx2][:2] in roman or rows[idx2][:3] in roman) and len(rows[idx2]) <= 70:
                chapters[idx1].append(MyClass(rows[idx2], idx2 + 1, stories[idx1].rEnd, parent=stories[idx1]))

    for chapter in chapters:
        for idx in range(0, len(chapter) - 1):
            if (idx != len(chapter) - 1):
                chapter[idx].rEnd = chapter[idx + 1].rBegin - 1 

def isValid(word):
    if len(word) == 0:
        return 0
    if (ord(word[0]) < 65 or ord(word[0]) > 90):
        return 0
    return 1


def addSpecialName(p):
    for idx in range(p.rBegin - 1, p.rEnd - 1):
        words = rows[idx].strip().replace("  ", " ").split(" ")
        for idx1 in range(0, len(words) - 1):
            if (isValid(words[idx1]) and idx1 != 0):
                if (len(words[idx1 - 1])):
                    c = ord(words[idx1 - 1][len(words[idx1 - 1]) - 1])
                    if ((c >= 48 and c <= 57) or (c >= 65 and c <= 122)):
                        specialName.append(MyClass(words[idx1], idx, idx, parent=p))
    

def ProcessAddSpecialName():
    for story in stories:
        if (story.children == None):
            addSpecialName(story)
        else:
            for child in story.children:
                addSpecialName(child)

class MyBaseClass(object):
    x = 7

class MyClass(MyBaseClass, NodeMixin):
    def __init__(self, name, rBegin, rEnd, parent = None, children = None):
        super(MyClass, self).__init__()
        self.name = name
        self.rBegin = rBegin
        self.rEnd = rEnd
        self.parent = parent
        if children:
            self.children = children

# my1 = MyClass('my1', 1, 0, parent=my0)
# my2 = MyClass('my2', 0, 2, parent=my0)

if __name__ == '__main__':
    Stories = MyClass('stories', 1, len(rows) + 1)
    addStoryNames()
    addChapterNames()
    ProcessAddSpecialName()
    # print(len(rows[1588]))
    f = open("ex2output.txt", "w+")
    for pre, fill, node in RenderTree(Stories):
        print("%s%s" % (pre, node.name), node.rBegin, node.rEnd)
        # s = pre + node.name + str(node.rBegin) + str(node.rEnd)
        # f.write(s)
    f.close()