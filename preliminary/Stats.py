#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "rbuck"
__date__ = "$May 7, 2010 3:54:40 PM$"

from math import sqrt
from parser import *

class Stats:
    """
    Provides functions for calculating useful statistics on arrays.
    Array must be either float or int type.
    Statistics are usually returned as floats.
    """
    


    def standard_deviation(self, array):
        """
        See http://en.wikipedia.org/wiki/Standard_deviation

        array -- a one-dimensional array of float or int types.
        Returns the standard deviation of the values in array.
        """
         
        average = self.avg(array)
        temp = []

        for i in array:
            diff = i - average
            diff = diff * diff
            temp.append(diff)

        total = sum(temp)
        total = total / len(temp)
        total = sqrt(total)

        return total
     
    def avg(self, array):
        """
        array -- a one-dimensional array of float or int types.
        Returns the average of the values in array.
        """

        if len(array) is 0:
            print "ERROR: average of empty array called. Returning zero."
            return 0
        return float(1.0 * sum(array)) / float(len(array))

    def median(self, array):
        """
        array -- a one-dimensional array of float or int types.
        Returns the median value in array.
        """
        array.sort()
        length = len(array)
        
        if length is 1:
            return array[0]
        if length is 0:
            return None

        middle = int(length / 2)
        if length % 2 is 1:
            return float(array[middle])
        else:
            return float(self.avg([array[middle], array[middle - 1]]))


    def get_percentile(self, array, percentile):
        """
        Taken from http://en.wikipedia.org/wiki/Percentile but modified
        to account for 0 based indexing.  Namely, we don't add 0.5 to the index.
        
        array -- a one-dimensional array of float or int types.
        percentile -- number from 1 to 99 inclusive
        Returns the value at the rank specified by percentile.
        """
        array.sort()
        temp = percentile * len(array)
        index = int((temp / float(100)))
        return float(array[index])


    def get_75th_percentile(self, array):
        """
        array -- a one-dimensional array of float or int types.
        Returns the value at the 75th percentile.
        """
        return self.get_percentile(array, 75)

    def get_25th_percentile(self, array):
        """
        array -- a one-dimensional array of float or int types.
        Returns the value at the 25th percentile.
        """
        return self.get_percentile(array, 25)


if __name__ == "__main__":
    list = [2, 4, 4, 4, 5, 5, 7, 9]
    s = Stats()
    p = Parser()
    print "Average (should be 5): ", s.avg(list)

    print "Standard Deviation (should be 2): ", s.standard_deviation(list)

    print "Median of even length array (should be 4.5): ", s.median(list)
    list.pop()
    print "Median of odd length array (should be 4): ", s.median(list)

    print s.get_percentile(list, 25)
