import license_expression
import pytest
from unittest import mock

from mbed_license_checker import license_checker


def test_valid_license_expression():
    """
    Test that a valid license expression can be parsed
    """

    text = """
blablabla
text here
a
b
c
SPDX-License-Identifier: Apache-2.0
    """

    with mock.patch("mbed_license_checker.license_checker.open", mock.mock_open(read_data=text)) as mock_file:
        assert license_checker.find_license_in_file(mock_file) == license_expression.LicenseSymbol(key="Apache-2.0")


def test_invalid_license_expression():
    """
    Test that an invalid license name is caught and None is returned
    """

    text = """SPDX-License-Identifier: Blargh"""

    with mock.patch("mbed_license_checker.license_checker.open", mock.mock_open(read_data=text)) as mock_file:
        with pytest.raises(license_checker.LicenseCheckFailedError, match="Failed to parse license expression"):
            license_checker.find_license_in_file(mock_file)


def test_no_license_expression():
    """
    Test that an exception is thrown if there is no license data
    """

    text = """
blablabla
text here
    """

    with mock.patch("mbed_license_checker.license_checker.open", mock.mock_open(read_data=text)) as mock_file:
        with pytest.raises(license_checker.LicenseCheckFailedError, match="No SPDX-License-Identifier found"):
            license_checker.find_license_in_file(mock_file)


def test_single_permissive_license():
    """
    Test that a single permissive license is correctly detected
    """

    lic_symbol = license_checker.licensing.parse("Apache-2.0")
    assert license_checker.is_permissive_license(lic_symbol)


def test_single_nonpermissive_license():
    """
    Test that a single nonpermissive license is correctly detected
    """

    lic_symbol = license_checker.licensing.parse("GPL-2.0-or-later")
    assert not license_checker.is_permissive_license(lic_symbol)


def test_expression_permissive_license():
    """
    Test that an expression that allows a permissive license is correctly detected
    """

    lic_symbol = license_checker.licensing.parse("GPL-2.0-or-later OR (Apache-2.0 AND LicenseRef-Nordic-5-Clause)")
    assert license_checker.is_permissive_license(lic_symbol)


def test_expression_non_permissive_license():
    """
    Test that an expression that has no permissive license is correctly detected
    """

    lic_symbol = license_checker.licensing.parse("GPL-2.0-or-later OR (Apache-2.0 AND LicenseRef-scancode-esri)")
    assert not license_checker.is_permissive_license(lic_symbol)
