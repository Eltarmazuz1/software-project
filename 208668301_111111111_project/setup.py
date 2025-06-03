from setuptools import setup, Extension
import numpy as np

module = Extension(
    'mysymnmf',  # Name of the C extension module
    sources=['symnmfmodule.c', 'symnmf.c'],  # List of C source files
    include_dirs=[np.get_include()],  # Include NumPy headers
)

setup(
    name='mysymnmf',  # Name of your package
    version='1.0',
    description='symnmf package',
    ext_modules=[module],  # List of C extension modules
)
