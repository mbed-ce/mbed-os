from builtins import super

class MockSerial(object):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._args = args
        self._kwargs = kwargs
        self._open = True
        self._rx_counter = 0
        self._tx_buffer = b""
        self._rx_buffer = b""
        self._upstream_write_cb = None

    def read(self, count):
        contents = self._rx_buffer[self._rx_counter:count]
        self._rx_counter += len(contents)
        return contents

    def write(self, data):
        self._tx_buffer += data
        if self._upstream_write_cb:
            self._upstream_write_cb(data)

    def close(self):
        self._open = False

    def downstream_write(self, data):
        self._rx_buffer += data.encode("utf-8")

    def downstream_write_bytes(self, data):
        self._rx_buffer += data

    def on_upstream_write(self, func):
        self._upstream_write_cb = func
