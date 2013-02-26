# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_tally', [dirname(__file__)])
        except ImportError:
            import _tally
            return _tally
        if fp is not None:
            try:
                _mod = imp.load_module('_tally', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _tally = swig_import_helper()
    del swig_import_helper
else:
    import _tally
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


EQUAL = _tally.EQUAL
LOGARITHMIC = _tally.LOGARITHMIC
OTHER = _tally.OTHER
MATERIAL = _tally.MATERIAL
ISOTOPE = _tally.ISOTOPE
REGION = _tally.REGION
COLLISION = _tally.COLLISION
FLUX = _tally.FLUX
ELASTIC = _tally.ELASTIC
ABSORPTION = _tally.ABSORPTION
CAPTURE = _tally.CAPTURE
FISSION = _tally.FISSION
TRANSPORT = _tally.TRANSPORT
DIFFUSION = _tally.DIFFUSION
LEAKAGE = _tally.LEAKAGE
class Tally(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Tally, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Tally, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _tally.new_Tally()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _tally.delete_Tally
    __del__ = lambda self : None;
    def getTallyName(self): return _tally.Tally_getTallyName(self)
    def getNumBins(self): return _tally.Tally_getNumBins(self)
    def getBinEdges(self): return _tally.Tally_getBinEdges(self)
    def getBinCenters(self): return _tally.Tally_getBinCenters(self)
    def getBinDelta(self, *args): return _tally.Tally_getBinDelta(self, *args)
    def getBinSpacingType(self): return _tally.Tally_getBinSpacingType(self)
    def getTallyDomainType(self): return _tally.Tally_getTallyDomainType(self)
    def getTallyType(self): return _tally.Tally_getTallyType(self)
    def getTallies(self): return _tally.Tally_getTallies(self)
    def getTally(self, *args): return _tally.Tally_getTally(self, *args)
    def getNumTallies(self, *args): return _tally.Tally_getNumTallies(self, *args)
    def getMaxTally(self): return _tally.Tally_getMaxTally(self)
    def getMinTally(self): return _tally.Tally_getMinTally(self)
    def getBinIndex(self, *args): return _tally.Tally_getBinIndex(self, *args)
    def getIsotopes(self): return _tally.Tally_getIsotopes(self)
    def setTallyName(self, *args): return _tally.Tally_setTallyName(self, *args)
    def setTallyDomainType(self, *args): return _tally.Tally_setTallyDomainType(self, *args)
    def setTallyType(self, *args): return _tally.Tally_setTallyType(self, *args)
    def setBinEdges(self, *args): return _tally.Tally_setBinEdges(self, *args)
    def setIsotopes(self, *args): return _tally.Tally_setIsotopes(self, *args)
    def generateBinEdges(self, *args): return _tally.Tally_generateBinEdges(self, *args)
    def generateBinCenters(self): return _tally.Tally_generateBinCenters(self)
    def tally(self, *args): return _tally.Tally_tally(self, *args)
    def weightedTally(self, *args): return _tally.Tally_weightedTally(self, *args)
    def normalizeTallies(self, *args): return _tally.Tally_normalizeTallies(self, *args)
Tally_swigregister = _tally.Tally_swigregister
Tally_swigregister(Tally)

# This file is compatible with both classic and new-style classes.


