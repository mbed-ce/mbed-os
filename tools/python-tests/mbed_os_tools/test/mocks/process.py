from builtins import super
from threading import Thread

class MockProcess(Thread):
    def __init__(self, target=None, args=None):
        super().__init__(target=target, args=args)
        self._terminates = 0
        self.exitcode = 0

    def terminate(self):
        self._terminates += 1
