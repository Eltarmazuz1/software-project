#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "symnmf.h"

static PyObject* sym_capi(PyObject* self, PyObject* args);

static PyObject* sym_capi(PyObject *self, PyObject  *args) {

    PyObject* py_datapoints;
    int numofcords;
    int numofdp;

    if (!PyArg_ParseTuple(args, "Oii", &py_datapoints, &numofcords, &numofdp)) {
        return NULL;
    }

    double **dpnums = (double **)malloc(numofdp * sizeof(double));
    if (dpnums == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return NULL;
    }

    for (int i = 0; i < numofdp; i++) {

        PyObject *dp_row = PyList_GetItem(py_datapoints, i);
        if (!PyList_Check(dp_row)) {
            PyErr_SetString(PyExc_TypeError, "All elements of py_datapoints must be lists.");
            return NULL;
        }

        dpnums[i] = malloc(sizeof(double) * numofcords);
        if (dpnums[i] == NULL) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory.");
            return NULL;
        }

        for (int j = 0; j < numofcords; j++) {
            PyObject *val = PyList_GetItem(dp_row, j);
            if (!PyFloat_Check(val) && !PyNumber_Check(val)) {
                PyErr_SetString(PyExc_TypeError, "All elements of dp_row must be floats or numbers.");
                return NULL;
            }
 
            dpnums[i][j] = PyFloat_AsDouble(val);
            
        }

    }
    
    double** A= sym(dpnums, numofcords, numofdp);

    PyObject* resultList = PyList_New(numofdp);

    for (int i = 0; i < numofdp; i++) {
        PyObject* rowList = PyList_New(numofdp);
        for (int j = 0; j < numofdp; j++) {
            PyObject* element = PyFloat_FromDouble(A[i][j]);
            PyList_SetItem(rowList, j, element);
        }
        PyList_SetItem(resultList, i, rowList);
    }

    return resultList;

}

static PyObject* ddg_capi(PyObject *self, PyObject  *args) {

    PyObject* py_datapoints;
    int numofcords;
    int numofdp;

    if (!PyArg_ParseTuple(args, "Oii", &py_datapoints, &numofcords, &numofdp)) {
        return NULL;
    }

    double **dpnums = (double **)malloc(numofdp * sizeof(double));
    if (dpnums == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return NULL;
    }

    for (int i = 0; i < numofdp; i++) {

        PyObject *dp_row = PyList_GetItem(py_datapoints, i);
        if (!PyList_Check(dp_row)) {
            PyErr_SetString(PyExc_TypeError, "All elements of py_datapoints must be lists.");
            return NULL;
        }

        dpnums[i] = malloc(sizeof(double) * numofcords);
        if (dpnums[i] == NULL) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory.");
            return NULL;
        }

        for (int j = 0; j < numofcords; j++) {
            PyObject *val = PyList_GetItem(dp_row, j);
            if (!PyFloat_Check(val) && !PyNumber_Check(val)) {
                PyErr_SetString(PyExc_TypeError, "All elements of dp_row must be floats or numbers.");
                return NULL;
            }
 
            dpnums[i][j] = PyFloat_AsDouble(val);
            
        }

    }
    
    double** D= ddg(dpnums, numofcords, numofdp);

    PyObject* resultList = PyList_New(numofdp);

    for (int i = 0; i < numofdp; i++) {
        PyObject* rowList = PyList_New(numofdp);
        for (int j = 0; j < numofdp; j++) {
            PyObject* element = PyFloat_FromDouble(D[i][j]);
            PyList_SetItem(rowList, j, element);
        }
        PyList_SetItem(resultList, i, rowList);
    }

    return resultList;

}

static PyObject* norm_capi(PyObject *self, PyObject  *args) {

    PyObject* py_datapoints;
    int numofcords;
    int numofdp;

    if (!PyArg_ParseTuple(args, "Oii", &py_datapoints, &numofcords, &numofdp)) {
        return NULL;
    }

    double **dpnums = (double **)malloc(numofdp * sizeof(double));
    if (dpnums == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return NULL;
    }

    for (int i = 0; i < numofdp; i++) {

        PyObject *dp_row = PyList_GetItem(py_datapoints, i);
        if (!PyList_Check(dp_row)) {
            PyErr_SetString(PyExc_TypeError, "All elements of py_datapoints must be lists.");
            return NULL;
        }

        dpnums[i] = malloc(sizeof(double) * numofcords);
        if (dpnums[i] == NULL) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory.");
            return NULL;
        }

        for (int j = 0; j < numofcords; j++) {
            PyObject *val = PyList_GetItem(dp_row, j);
            if (!PyFloat_Check(val) && !PyNumber_Check(val)) {
                PyErr_SetString(PyExc_TypeError, "All elements of dp_row must be floats or numbers.");
                return NULL;
            }
 
            dpnums[i][j] = PyFloat_AsDouble(val);
            
        }

    }
    
    double** W= norm(dpnums, numofcords, numofdp);

    PyObject* resultList = PyList_New(numofdp);

    for (int i = 0; i < numofdp; i++) {
        PyObject* rowList = PyList_New(numofdp);
        for (int j = 0; j < numofdp; j++) {
            PyObject* element = PyFloat_FromDouble(W[i][j]);
            PyList_SetItem(rowList, j, element);
        }
        PyList_SetItem(resultList, i, rowList);
    }

    return resultList;

}

static PyObject* symnmf_capi(PyObject *self, PyObject  *args) {

    PyObject* H;
    PyObject* W;
    int k;
    int numofdp;

    if (!PyArg_ParseTuple(args, "OOii", &H, &W, &k, &numofdp)) {
        return NULL;
    }

    double **cH = (double **)malloc(numofdp * sizeof(double));
    if (cH == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return NULL;
    }

    for (int i = 0; i < numofdp; i++) {

        PyObject *cH_row = PyList_GetItem(H, i);
        if (!PyList_Check(cH_row)) {
            PyErr_SetString(PyExc_TypeError, "All elements of py_datapoints must be lists.");
            return NULL;
        }

        cH[i] = malloc(sizeof(double) * k);
        if (cH[i] == NULL) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory.");
            return NULL;
        }

        for (int j = 0; j < k; j++) {
            PyObject *val = PyList_GetItem(cH_row, j);
            if (!PyFloat_Check(val) && !PyNumber_Check(val)) {
                PyErr_SetString(PyExc_TypeError, "All elements of dp_row must be floats or numbers.");
                return NULL;
            }
 
            cH[i][j] = PyFloat_AsDouble(val);
        }

    }

    double **cW = (double **)malloc(numofdp * sizeof(double));
    if (cW == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return NULL;
    }

    for (int i = 0; i < numofdp; i++) {

        PyObject *cW_row = PyList_GetItem(W, i);
        if (!PyList_Check(cW_row)) {
            PyErr_SetString(PyExc_TypeError, "All elements of py_datapoints must be lists.");
            return NULL;
        }

        cW[i] = malloc(sizeof(double) * numofdp);
        if (cW[i] == NULL) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory.");
            return NULL;
        }

        for (int j = 0; j < numofdp; j++) {
            PyObject *val = PyList_GetItem(cW_row, j);
            if (!PyFloat_Check(val) && !PyNumber_Check(val)) {
                PyErr_SetString(PyExc_TypeError, "All elements of dp_row must be floats or numbers.");
                return NULL;
            }
 
            cW[i][j] = PyFloat_AsDouble(val);
            
        }

    }
    
    double** final_H= symnmf(cH, cW, k, numofdp);

    PyObject* resultList = PyList_New(numofdp);

    for (int i = 0; i < numofdp; i++) {
        PyObject* rowList = PyList_New(k);
        for (int j = 0; j < k; j++) {
            PyObject* element = PyFloat_FromDouble(final_H[i][j]);
            PyList_SetItem(rowList, j, element);
        }
        PyList_SetItem(resultList, i, rowList);
    }

    return resultList;

}



static PyMethodDef symnmf_methods[] = {
    {"sym_capi",
     (PyCFunction)sym_capi,
      METH_VARARGS,
      PyDoc_STR("calc. sym. matrix")
    },

    {"ddg_capi",
     (PyCFunction)ddg_capi,
      METH_VARARGS,
      PyDoc_STR("calc. ddg. matrix")
    },

    {"norm_capi",
     (PyCFunction)norm_capi,
      METH_VARARGS,
      PyDoc_STR("calc. graph Laplacian matrix")
    },

    {"symnmf_capi",
     (PyCFunction)symnmf_capi,
      METH_VARARGS,
      PyDoc_STR("calc. symnmf matrix")
    },

    {NULL, NULL, 0, NULL}  // Sentinel
};

static struct PyModuleDef symnmf_module = {
    PyModuleDef_HEAD_INIT,
    "mysymnmf",  // Module name
    "symnmf extension module.",
    -1,
    symnmf_methods  // Method definitions
};

PyMODINIT_FUNC PyInit_mysymnmf(void) {
    PyObject *m;
    m = PyModule_Create(&symnmf_module);
    if (!m) {
        return NULL;
    }
    return m;
}