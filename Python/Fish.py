# -*- coding: utf-8 -*-
"""
Created on Fri Aug 14 07:28:11 2015

@author: Benjamin Woodward
"""

class Fish(object):
    def __init__(self,fishType=0,subTypeLabels=[]):
        self.__fishLabels = dict([(0,'Unknown'),(1,'Flounder'),(2,'Skate')])
        self._subTypeLabels = subTypeLabels
        self.fishType = self.fishLabels[fishType]
    
    def setSubType(self,subType=0):
        self.subType = self._subTypeLabels[subType]
        
class Flounder(Fish):
    def __init__(self,subtype=0):
        self.__subTypeLabels = dict([(0,'Unknown'),(1,'Type1')])
        self.subType = self.subTypeLabels[subtype]
        super(Flounder,self).__init__(0,self.__subTypeLabels)
        
class Skate(Fish):
    def __init__(self,subtype=0):
        self.__subTypeLabels = dict([(0,'Unknown'),(1,'Type1')])
        self.subType = self.subTypeLabels[subtype]
        super(Skate,self).__init__(1,self.__subTypeLabels)
        
        
