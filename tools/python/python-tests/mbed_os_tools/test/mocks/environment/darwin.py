from builtins import super
from mock import MagicMock

from .posix import MockTestEnvironmentPosix

class MockTestEnvironmentDarwin(MockTestEnvironmentPosix):

    def __init__(self, test_case, platform_info, image_path):
        super().__init__(test_case, platform_info, image_path)

        self.patch(
            'os.uname',
            new=MagicMock(return_value=('Darwin',)),
            create=True
        )

    def __exit__(self, type, value, traceback):
        super().__exit__(type, value, traceback)

        if value:
            return False

        # Assert for proper image copy
        mocked_call = self.patches[
            'mbed_os_tools.test.host_tests_plugins.host_test_plugins.call'
        ]

        second_call_args = mocked_call.call_args_list[1][0][0]
        self._test_case.assertEqual(second_call_args, ["sync"])

        # Ensure only two subprocesses were started
        self._test_case.assertEqual(len(mocked_call.call_args_list), 2)
