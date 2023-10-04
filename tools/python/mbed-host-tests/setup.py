"""
This module defines the attributes of the
PyPI package for the mbed SDK test suite ecosystem tools
"""

"""
mbed SDK
Copyright (c) 2011-2019 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.Wirkus@arm.com>
"""

import os
from distutils.core import setup
from io import open
from setuptools import find_packages

DESCRIPTION = "mbed tools used to flash, reset and supervise test execution for mbed-enabled devices"
OWNER_NAMES = 'Qinghao Shi'
OWNER_EMAILS = 'qinghao.shi@arm.com'

repository_dir = os.path.dirname(__file__)


def read(fname):
    """
    Utility function to cat in a file (used for the README)
    @param fname: the name of the file to read,
    relative to the directory containing this file
    @return: The string content of the opened file
    """
    with open(os.path.join(repository_dir, fname), mode='r') as f:
        return f.read()


with open(os.path.join(repository_dir, 'requirements.txt')) as fh:
    requirements = fh.readlines()

with open(os.path.join(repository_dir, 'test_requirements.txt')) as fh:
    test_requirements = fh.readlines()

python_requires = '>=2.7.10, !=3.0.*, !=3.1.*, !=3.2.*, !=3.3.*, !=3.4.*, <4'
setup(name='mbed-host-tests',
      description=DESCRIPTION,
      long_description=read('README.md'),
      long_description_content_type='text/markdown',
      author=OWNER_NAMES,
      author_email=OWNER_EMAILS,
      maintainer=OWNER_NAMES,
      maintainer_email=OWNER_EMAILS,
      url='https://github.com/ARMmbed/mbed-os-tools',
      packages=find_packages(),
      license="Apache-2.0",
      test_suite='test',
      entry_points={
          "console_scripts":
              ["mbedhtrun=mbed_host_tests.mbedhtrun:main",
               "mbedflsh=mbed_host_tests.mbedflsh:main"],
      },
      classifiers=(
          'Development Status :: 5 - Production/Stable',
          'Intended Audience :: Developers',
          'License :: OSI Approved :: Apache Software License',
          'Programming Language :: Python :: 2',
          'Programming Language :: Python :: 2.7',
          'Programming Language :: Python :: 3',
          'Programming Language :: Python :: 3.5',
          'Programming Language :: Python :: 3.6',
          'Programming Language :: Python :: 3.7',
          'Programming Language :: Python',
          'Topic :: Software Development :: Build Tools',
          'Topic :: Software Development :: Embedded Systems',
          'Topic :: Software Development :: Testing',
      ),
      use_scm_version = {
          "root": "../..",
          "relative_to": __file__
      },
      python_requires=python_requires,
      install_requires=requirements,
      tests_require=test_requirements
)
