# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "rbuck"
__date__ = "$May 15, 2010 2:23:55 PM$"

class FileParser:

    def parse(self, filename):
        """Parses filename and returns a list of the lines contained in it"""
        lines = []

        f = open(filename)
        for line in f.readlines():
            lines.append(line.strip("\n"))

        f.close()
        return lines

if __name__ == "__main__":
    print "Hello World"


