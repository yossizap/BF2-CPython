/*
 * @brief: This module provides functions that are used by project
 *         reality's python modules and required an optimization.
 * @note: Make sure you test anything that you want to add and keep track of
 *        object reference counts. Refer to Doc/data/refcounts.dat.
 *        Refer to Misc/README.valgrind to compile python for valgrind and
 *        run the tests again with the following string:
 *        valgrind --log-file="logfile.out" --track-origins=yes --show-leak-kinds=all \
 *          --trace-children=yes --leak-check=full --tool=memcheck \
 *          --suppressions=Misc/valgrind-python.supp ./python {TEST_PATH}/test_realitycore.py
 *        Look for the string realitycore in the output file and fix whatever
 *        causes it to appear there.
 */

#include "Python.h"

#define _USE_MATH_DEFINES
#include "math.h"



static PyObject*
realitycore_calcDistance(PyObject *self, PyObject *args)
{
    PyObject *x1 = NULL, *x2 = NULL, *y1 = NULL, *y2 = NULL, *z1 = NULL, *z2 = NULL; 
    double dx, dy, dz, r;

    if (! PyArg_ParseTuple(args, "(OOO)(OOO):calcDistance", &x1, &y1, &z1, &x2, &y2, &z2))
    {
        return NULL;
    }

    dx = PyFloat_AsDouble(x1) - PyFloat_AsDouble(x2);
    dy = PyFloat_AsDouble(y1) - PyFloat_AsDouble(y2);
    dz = PyFloat_AsDouble(z1) - PyFloat_AsDouble(z2);

    r = sqrt((dx * dx) + (dy * dy) + (dz * dz));

    return PyFloat_FromDouble(r);
}

PyDoc_STRVAR(realitycore_calcDistance_doc,
"calcDistance(pos1, pos2)\n"
"pos1, pos2 - a tuple containing 3 floats\n"
"Return the distance between two points.");

static PyObject*
realitycore_calcDistanceSquared(PyObject *self, PyObject *args)
{
    PyObject *x1 = NULL, *x2 = NULL, *y1 = NULL, *y2 = NULL, *z1 = NULL, *z2 = NULL; 
    double dx, dy, dz, r;

    if (! PyArg_ParseTuple(args, "(OOO)(OOO):calcDistanceSquared", &x1, &y1, &z1, &x2, &y2, &z2))
    {
        return NULL;
    }

    dx = PyFloat_AsDouble(x1) - PyFloat_AsDouble(x2);
    dy = PyFloat_AsDouble(y1) - PyFloat_AsDouble(y2);
    dz = PyFloat_AsDouble(z1) - PyFloat_AsDouble(z2);

    r = (dx * dx) + (dy * dy) + (dz * dz);

    return PyFloat_FromDouble(r);
}

PyDoc_STRVAR(realitycore_calcDistanceSquared_doc,
"calcDistanceSquared(pos1, pos2)\n"
"pos1, pos2 - a tuple containing 3 floats\n"
"Returns the squared distance between two points.");

static PyObject*
realitycore_calcHorizDistance(PyObject *self, PyObject *args)
{
    PyObject *x1 = NULL, *x2 = NULL, *y1 = NULL, *y2 = NULL, *z1 = NULL, *z2 = NULL; 
    double dx, dz, r;

    if (! PyArg_ParseTuple(args, "(OOO)(OOO):calcHorizDistance", &x1, &y1, &z1, &x2, &y2, &z2))
    {
        return NULL;
    }

    dx = PyFloat_AsDouble(x1) - PyFloat_AsDouble(x2);
    dz = PyFloat_AsDouble(z1) - PyFloat_AsDouble(z2);

    r = sqrt((dx * dx) + (dz * dz));

    return PyFloat_FromDouble(r);
}

PyDoc_STRVAR(realitycore_calcHorizDistance_doc,
"calcHorizDistance(pos1, pos2)\n"
"pos1, pos2 - a tuple containing 3 floats\n"
"Returns the horizontal distance between two points.");

static PyObject*
realitycore_calcHorizDistanceSquared(PyObject *self, PyObject *args)
{
    PyObject *x1 = NULL, *x2 = NULL, *y1 = NULL, *y2 = NULL, *z1 = NULL, *z2 = NULL; 
    double dx, dz, r;

    if (! PyArg_ParseTuple(args, "(OOO)(OOO):calcHorizDistanceSquared", &x1, &y1, &z1, &x2, &y2, &z2))
    {
        return NULL;
    }

    dx = PyFloat_AsDouble(x1) - PyFloat_AsDouble(x2);
    dz = PyFloat_AsDouble(z1) - PyFloat_AsDouble(z2);

    r = (dx * dx) + (dz * dz);

    return PyFloat_FromDouble(r);
}

PyDoc_STRVAR(realitycore_calcHorizDistanceSquared_doc,
"calcHorizDistanceSquared(pos1, pos2)\n"
"pos1, pos2 - tuples containing 3 floats\n"
"Returns the squared horizontal distance between two points.");

static PyObject*
realitycore_calcPosFromPosRot(PyObject *self, PyObject *args)
{
    double posx, posy, posz, angle;
    PyObject *distance = NULL, *x = NULL, *y = NULL, *z = NULL, *r = NULL, *xrot = NULL, *yrot = NULL, *zrot = NULL;

    if (! PyArg_ParseTuple(args, "(OOO)(OOO)O:calcPosFromPosRot", &x, &y, &z, 
                                                                  &xrot, &yrot, &zrot,
                                                                  &distance))
    {
        return NULL;
    }

    angle = PyFloat_AsDouble(xrot) * M_PI/180.0;

    posx = PyFloat_AsDouble(x) + sin(angle) * PyFloat_AsDouble(distance);
    posz = PyFloat_AsDouble(z) + cos(angle) * PyFloat_AsDouble(distance);
    posy = PyFloat_AsDouble(y);

    r = PyTuple_New(3);
    if (NULL == r)
    {
        return NULL;
    }

    /* INCREF for the members isn't required since the tuple is packed with doubles */
    PyTuple_SetItem(r, 0, PyFloat_FromDouble(posx));
    PyTuple_SetItem(r, 1, PyFloat_FromDouble(posy));
    PyTuple_SetItem(r, 2, PyFloat_FromDouble(posz));

    return r;
}

PyDoc_STRVAR(realitycore_calcPosFromPosRot_doc,
"calcPosFromPosRot(pos1, rot1, distance)\n"
"pos1 tuple3\n"
"rot1 tuple3\n"
"distance float\n"
"return tuple3\n"
"Returns a new position based on postion, rotation and distance.\n"
"NOTE: C-api code calling this function must DECREF the returned\n"
"\tobject if it's the last one using it");

static PyObject*
realitycore_isClose(PyObject *self, PyObject *args)
{
    PyObject *pos1 = NULL, *pos2 = NULL, *distance = NULL, *pos_args = NULL, *calc_distance = NULL;
    //double ddistance, dcalc_distance;

    if (! PyArg_UnpackTuple(args, "isClose", 3, 3, &pos1, &pos2, &distance))
    {
        return NULL;
    }

    pos_args = PyTuple_New(2);
    if (NULL == pos_args)
    {
        return NULL;
    }

    /* We have to increase the reference count for pos1&2 since PyTuple_New      
     *  barrows their reference and they get removed with him by the GC */
    Py_INCREF(pos1);
    Py_INCREF(pos2);
    PyTuple_SetItem(pos_args, 0, pos1);
    PyTuple_SetItem(pos_args, 1, pos2);

    calc_distance = realitycore_calcDistanceSquared(self, pos_args);
    if (pow(PyFloat_AsDouble(distance), 2) >= PyFloat_AsDouble(calc_distance))
    {
        Py_DECREF(calc_distance);
        Py_DECREF(pos_args);
        Py_RETURN_TRUE;
    }
    else
    {
        Py_DECREF(calc_distance);
        Py_DECREF(pos_args);
        Py_RETURN_FALSE;
    }
}

PyDoc_STRVAR(realitycore_isClose_doc,
"isClose(pos1, pos2, distance)\n"
"pos1 tuple3\n"
"pos2 tuple3\n"
"distance float\n"
"Checks if pos2 is within the distance of pos1");

static PyObject*
realitycore_isCloseHoriz(PyObject *self, PyObject *args)
{
    PyObject *pos1 = NULL, *pos2 = NULL, *distance = NULL, *pos_args = NULL, *calc_distance = NULL;
    //double ddistance, dcalc_distance;

    if (! PyArg_UnpackTuple(args, "isCloseHoriz", 3, 3, &pos1, &pos2, &distance))
    {
        return NULL;
    }

    pos_args = PyTuple_New(2);
    if (NULL == pos_args)
    {
        return NULL;
    }

    /* We have to increase the reference count for pos1&2 since PyTuple_New      
     *  barrows their reference and they get removed with him by the GC */
    Py_INCREF(pos1);
    Py_INCREF(pos2);
    PyTuple_SetItem(pos_args, 0, pos1);
    PyTuple_SetItem(pos_args, 1, pos2);

    calc_distance = realitycore_calcHorizDistanceSquared(self, pos_args);
    if (pow(PyFloat_AsDouble(distance), 2) >= PyFloat_AsDouble(calc_distance))
    {
        Py_DECREF(calc_distance);
        Py_DECREF(pos_args);
        Py_RETURN_TRUE;
    }
    else
    {
        Py_DECREF(calc_distance);
        Py_DECREF(pos_args);
        Py_RETURN_FALSE;
    }
}

PyDoc_STRVAR(realitycore_isCloseHoriz_doc,
"isCloseHoriz(pos1, pos2, distance)\n"
"pos1 tuple3\n"
"pos2 tuple3\n"
"distance float\n"
"Checks if pos2 is within the distance of pos1 horizontally");


static PyMethodDef realitycore_methods[] = {
    {"calcDistance",             realitycore_calcDistance,             METH_VARARGS, realitycore_calcDistance_doc},
    {"calcDistanceSquared",      realitycore_calcDistanceSquared,      METH_VARARGS, realitycore_calcDistanceSquared_doc},
    {"calcHorizDistance",        realitycore_calcHorizDistance,        METH_VARARGS, realitycore_calcHorizDistance_doc},
    {"calcHorizDistanceSquared", realitycore_calcHorizDistanceSquared, METH_VARARGS, realitycore_calcHorizDistanceSquared_doc},
    {"isClose",                  realitycore_isClose,                  METH_VARARGS, realitycore_isClose_doc},
    {"isCloseHoriz",             realitycore_isCloseHoriz,             METH_VARARGS, realitycore_isCloseHoriz_doc},
    {"calcPosFromPosRot",        realitycore_calcPosFromPosRot,        METH_VARARGS, realitycore_calcPosFromPosRot_doc},
    {NULL,      NULL} /* Sentinel */
};

PyDoc_STRVAR(realitycore_module_doc,
"This module provides core functions used by project\n"
"reality's python modules that required optimizations.");

PyMODINIT_FUNC init_realitycore(void)
{
    PyObject *m;

    m = Py_InitModule3("_realitycore", realitycore_methods, realitycore_module_doc);
    if (m == NULL)
    {
        goto finally;
    }

finally:
    return;
}
