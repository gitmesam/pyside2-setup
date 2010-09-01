// Borrowed reference to QtGui module
extern PyObject* moduleQtGui;

static int QApplicationArgCount;
static char** QApplicationArgValues;
static const char QAPP_MACRO[] = "qApp";
static bool leavingPython = false;

void DeleteQApplicationAtExit()
{
    leavingPython = true;    
    PySide::SignalManager::instance().clear();
    QCoreApplication* cpp = QApplication::instance();
    if (cpp) {
        Shiboken::BindingManager &bmngr = Shiboken::BindingManager::instance();
        PyObject* pySelf = bmngr.retrieveWrapper(cpp);
        if (pySelf)
            bmngr.invalidateWrapper(pySelf);
        cpp->deleteLater();
    }
}

int SbkQApplication_Init(PyObject* self, PyObject* args, PyObject*)
{
    if (Shiboken::isUserType(self) && !Shiboken::canCallConstructor(self->ob_type, Shiboken::SbkType<QApplication >()))
        return -1;

    if (QApplication::instance()) {
        PyErr_SetString(PyExc_RuntimeError, "A QApplication instance already exists.");
        return -1;
    }

    int numArgs = PyTuple_GET_SIZE(args);
    if (numArgs != 1) {
        PyErr_BadArgument();
        return -1;
    }

    if (!PySequenceToArgcArgv(PyTuple_GET_ITEM(args, 0), &QApplicationArgCount, &QApplicationArgValues, "PySideApp")) {
        PyErr_BadArgument();
        return -1;
    }

    QApplicationWrapper* cptr = new QApplicationWrapper(QApplicationArgCount, QApplicationArgValues);
    Shiboken::setCppPointer(reinterpret_cast<SbkBaseWrapper*>(self),
                            Shiboken::SbkType<QApplication>(),
                            cptr);
    SbkBaseWrapper_setValidCppObject(self, 1);
    SbkBaseWrapper *sbkSelf = reinterpret_cast<SbkBaseWrapper*>(self);
    sbkSelf->containsCppWrapper = 1;
    sbkSelf->hasOwnership = 0;
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);
    PySide::signalUpdateSource(self);
    cptr->metaObject();

    // Verify if qApp is in main module
    PyObject* globalsDict = PyEval_GetGlobals();
    if (globalsDict) {
        PyObject* qAppObj = PyDict_GetItemString(globalsDict, QAPP_MACRO);
        if (qAppObj)
            PyDict_SetItemString(globalsDict, QAPP_MACRO, self);
    }

    PyObject_SetAttrString(moduleQtGui, QAPP_MACRO, self);
    Py_AtExit(DeleteQApplicationAtExit);
    Py_INCREF(self);
    return 1;
}
