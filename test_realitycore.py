'''
A quick test made as a sanity check and a handy tool to find memory leaks within realitycore. This mostly useful on linux with valgrind.
Refer to Misc/README.valgrind to compile python for valgrind and run the tests again with the following string:
    valgrind --log-file="logfile.out" --track-origins=yes --show-leak-kinds=all \
        --trace-children=yes --leak-check=full --tool=memcheck \
        --suppressions=Misc/valgrind-python.supp ./python {TEST_PATH}/test_realitycore.py
Look for the string realitycore in the output file and fix whatever causes it to appear there.
This test can also be ran from the embedded version of this interpreter in the BF2 engine by adding "import test_realitycore; test_realitycore.runTests()"
to PR's init or BF2's init. The PR server can be ran in valgrind in the same way the interepter runs in it, just add LD_LIBRARY_PATH=./bin/{ARCH}/prbf2 at the start
'''

import math
import itertools
import random

import _realitycore

TEST_ITERATIONS=500
DISTANCE_CLOSE=50

g_coords = [(random.uniform(0, 2000), random.uniform(0, 2000), random.uniform(0, 2000)) for number in xrange(TEST_ITERATIONS)] 

g_close_distance = [DISTANCE_CLOSE for number in xrange(TEST_ITERATIONS)]

g_random_distances = [random.uniform(0, 2000) for number in xrange(TEST_ITERATIONS)] 

g_rotations = [(random.uniform(0, 360), random.uniform(0, 360), random.uniform(0, 360)) for number in xrange(TEST_ITERATIONS)] 

#def getVectorDistance( pos1, pos2 ):
def calcDistance(pos1, pos2):
    dx = pos1[0] - pos2[0]
    dy = pos1[1] - pos2[1]
    dz = pos1[2] - pos2[2]
    return math.sqrt((dx * dx) + (dy * dy) + (dz * dz))

#def getSquareVectorDistance(pos1, pos2):
def calcDistanceSquared(pos1, pos2):
    dx = pos1[0] - pos2[0]
    dy = pos1[1] - pos2[1]
    dz = pos1[2] - pos2[2]
    return dx * dx + dy * dy + dz * dz

#def getVectorHorizDistance(pos1, pos2):
def calcHorizDistance(pos1, pos2):
    dx = pos1[0] - pos2[0]
    dz = pos1[2] - pos2[2]
    return math.sqrt(dx * dx + dz * dz)

#def getSquareHorizDistance(pos1, pos2):
def calcHorizDistanceSquared(pos1, pos2):
    dx = pos1[0] - pos2[0]
    dz = pos1[2] - pos2[2]
    return dx * dx + dz * dz

#def getPositionFromPositionAndRotation(position, rotation, distance=0):
def calcPosFromPosRot(position, rotation, distance=0):
    if distance == 0:
        return position
    angle = math.radians(rotation[0])
    posX = position[0] + math.sin(angle) * distance
    posZ = position[2] + math.cos(angle) * distance
    posY = position[1]
    newPosition = (posX, posY, posZ)
    return newPosition

def isClose(mainPos, targetPos, distance=DISTANCE_CLOSE):
    if calcDistanceSquared(mainPos, targetPos) <= distance ** 2:
        return True
    return False

def isCloseHoriz(mainPos, targetPos, distance=DISTANCE_CLOSE):
    if calcHorizDistanceSquared(mainPos, targetPos) <= distance ** 2:
        return True
    return False

def runTests():
    global g_coords, g_close_distance, g_random_distances, g_rotations
    assertReturnValue(isClose, _realitycore.isClose, TEST_ITERATIONS, g_coords, g_coords[::-1], g_close_distance)
    assertReturnValue(isCloseHoriz, _realitycore.isClose, TEST_ITERATIONS, g_coords, g_coords[::-1], g_close_distance)
    assertReturnValue(calcHorizDistanceSquared, _realitycore.calcHorizDistanceSquared, TEST_ITERATIONS, g_coords, g_coords[::-1])
    assertReturnValue(calcHorizDistance, _realitycore.calcHorizDistance, TEST_ITERATIONS, g_coords, g_coords[::-1])
    assertReturnValue(calcDistance, _realitycore.calcDistance, TEST_ITERATIONS, g_coords, g_coords[::-1])
    assertReturnValue(calcDistanceSquared, _realitycore.calcDistanceSquared, TEST_ITERATIONS, g_coords, g_coords[::-1])
    assertReturnValue(calcPosFromPosRot, _realitycore.calcPosFromPosRot, TEST_ITERATIONS, g_coords, g_rotations, g_random_distances)

def assertReturnValue(native_function, builtin_function, iterations, *args):
    fileName =  './python_errors.log'
    errorFile = open(fileName, 'ab')
    errorFile.write("For %s iterations on %s:\n" % (str(iterations), native_function.__name__,))
    native_results = list(itertools.imap(native_function, *args))
    builtin_results = list(itertools.imap(builtin_function, *args))
    
    for i, results in enumerate(itertools.izip(native_results, builtin_results)):
        if results[0] != results[1]:
            errorFile.write("Result mismatch at iteration %d of function: %s.\n" % (i, builtin_function.__name__,))
            errorFile.write("  Builtin result: %s\n" % str(results[1]))
            errorFile.write("  Native result:  %s\n" % str(results[0]))
            errorFile.write("  Given args in iteration %d:\n" % i)
            for j, arg in enumerate(args):
                errorFile.write("    %d: %s\n" % (j, str(arg[i]),))

    errorFile.flush()
    errorFile.close()

runTests()
