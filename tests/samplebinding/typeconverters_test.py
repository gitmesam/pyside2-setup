#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# This file is part of the Shiboken Python Bindings Generator project.
#
# Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
#
# Contact: PySide team <contact@pyside.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# version 2.1 as published by the Free Software Foundation. Please
# review the following information to ensure the GNU Lesser General
# Public License version 2.1 requirements will be met:
# http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
# #
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301 USA

'''Tests various usages of the type converters.'''

import unittest
import sample

class GetPythonTypeByNameTest(unittest.TestCase):

    '''Uses an added function with inject code that uses the libshiboken
    function "Shiboken::Conversions::getPythonTypeObject(typeName)".'''

    def testGetObjectType(self):
        pyType1 = sample.getPythonType('ObjectType')
        self.assertEqual(pyType1, sample.ObjectType)
        pyType2 = sample.getPythonType('ObjectType*')
        self.assertEqual(pyType2, sample.ObjectType)
        self.assertEqual(pyType1, pyType2)

    def testGetValueType(self):
        pyType1 = sample.getPythonType('Point')
        self.assertEqual(pyType1, sample.Point)
        pyType2 = sample.getPythonType('Point*')
        self.assertEqual(pyType2, sample.Point)
        self.assertEqual(pyType1, pyType2)

    def testGetUsersPrimitiveType(self):
        pyType = sample.getPythonType('OddBool')
        self.assertEqual(pyType, bool)

    def testGetUsersPrimitiveTypeWithoutTargetLangApiName(self):
        '''If the primitive type attribute "target-lang-api-name" is not set
        there'll be no Python type associated with the C++ type.'''
        pyType = sample.getPythonType('PStr')
        self.assertEqual(pyType, None)

    def testPrimitiveTypeAndTypedef(self):
        pyType = sample.getPythonType('double')
        self.assertEqual(pyType, float)
        pyTypedef = sample.getPythonType('real')
        self.assertEqual(pyType, pyTypedef)


if __name__ == '__main__':
    unittest.main()