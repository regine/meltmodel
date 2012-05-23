#!/usr/bin/python

import numpy as np

def import_discharge():
    """Reads discharge data file from Hock's melt model
    
        Args: none now
            
        Returns: num_data: numpy array of data from the file
                column: dictionary of column names from the file

        TODO: ** make this method more general i.e. read any of
            the model's output text files and return a dictionary-index
            and numpy array
             
     """
    dis_file = open('discharge.txt','r')
    dis_data = [ii.split() for ii in dis_file.read().splitlines()]

# lets us find the columns we want w/o knowing which one it is a priori
    column = dict()
    current_line = dis_data.pop(0)
    for thing in current_line:
        column[thing] = current_line.index(thing)

# convert our data to something numpy can use
    num_data = np.array(dis_data,dtype=float)}
    return num_data, column
