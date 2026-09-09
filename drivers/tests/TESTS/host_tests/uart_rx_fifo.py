"""Host-side support for the UART RX FIFO regression test."""

import time

from mbed_host_tests import BaseHostTest


class UartRxFifoTest(BaseHostTest):
    def setup(self):
        self.register_callback("r", self.send_burst)

    def send_burst(self, key, value, timestamp):
        time.sleep(0.02)
        self.send_kv("b", "01234567")
